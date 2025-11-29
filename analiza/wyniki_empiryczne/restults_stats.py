
#!/usr/bin/env python3
"""Create statistics and plots for testing times produced by the C++ runner.

This script expects the CSV written by `helpers/file_logger.cpp` (default:
`programy/testing_times.csv`). It will:
- parse meta columns (Num States, Alphabet Size, Missing Edges, Num Samples,
  Sample Length, Length Variance, Automaton Type),
- detect algorithm runtime columns and corresponding ERROR_ columns,
- produce summary CSVs and several plots saved to an output directory.

Usage:
	python3 restults_stats.py --input /path/to/testing_times.csv --outdir ./wykresy

Requirements:
	pandas, matplotlib, seaborn, numpy

"""

import argparse
import os
import sys
from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm.auto import tqdm


META_COLUMNS = [
	"Num States",
	"Alphabet Size",
	"Missing Edges",
	"Num Samples",
	"Sample Length",
	"Length Variance",
	"Automaton Type",
]


def read_and_clean(csv_path: str) -> pd.DataFrame:
	# Fast path: read header first to infer which columns are runtime/error columns
	header = pd.read_csv(csv_path, nrows=0).columns.tolist()
	header = [c.strip() for c in header]

	# detect algorithm/runtime and error columns from header
	algs = [c for c in header if c not in META_COLUMNS and not c.startswith("ERROR_")]
	error_cols = [c for c in header if c.startswith("ERROR_")]

	# Build a dtype map to speed reading large files: runtime columns as float, errors as object
	dtype = {}
	for a in algs:
		dtype[a] = "float64"
	for e in error_cols:
		dtype[e] = "object"

	# Prefer numeric types for some meta columns when present
	for m in ["Num States", "Alphabet Size", "Missing Edges", "Num Samples", "Sample Length"]:
		if m in header:
			# use pandas nullable integer to allow missing values
			dtype[m] = "Int64"
	if "Length Variance" in header:
		dtype["Length Variance"] = "float64"
	if "Automaton Type" in header:
		dtype["Automaton Type"] = "object"

	# Read CSV using the dtype map; low_memory=False avoids mixed-type inference overhead
	df = pd.read_csv(csv_path, dtype=dtype, skipinitialspace=True, engine="c", low_memory=False)
	df.columns = [c.strip() for c in df.columns]
	return df


def detect_algorithms(df: pd.DataFrame):
	cols = list(df.columns)
	# Algorithms are columns that are not in META_COLUMNS and don't start with ERROR_
	algs = [c for c in cols if c not in META_COLUMNS and not c.startswith("ERROR_")]
	# Identify error columns
	error_cols = [c for c in cols if c.startswith("ERROR_")]
	# Map error column to algorithm name by removing ERROR_ prefix
	err_map = {c[len("ERROR_"):]: c for c in error_cols}
	# Filter algs to those that have corresponding runtime data (numerical)
	# Keep order as in file
	return algs, err_map


def make_long(df: pd.DataFrame, algs: list) -> pd.DataFrame:
	# Convert runtime columns to numeric in a vectorized way
	if algs:
		df.loc[:, algs] = df.loc[:, algs].apply(pd.to_numeric, errors="coerce")

	id_vars = [c for c in META_COLUMNS if c in df.columns]

	# Use pandas melt which is efficient; result is a long-form table suitable for groupby/plot
	long = df.melt(id_vars=id_vars, value_vars=algs, var_name="Algorithm", value_name="Runtime_ms")
	return long


def compute_and_save_stats(df_long: pd.DataFrame, outdir: Path):
	stats = df_long.groupby("Algorithm")["Runtime_ms"].agg(["count", "mean", "median", "std", "min", "max"]).reset_index()
	stats = stats.sort_values("mean")
	stats.to_csv(outdir / "algo_overall_stats.csv", index=False)

	# Per-Num States
	if "Num States" in df_long.columns:
		by_states = df_long.groupby(["Algorithm", "Num States"])["Runtime_ms"].agg(["count", "mean", "median", "std", "min", "max"]).reset_index()
		by_states.to_csv(outdir / "algo_by_num_states.csv", index=False)

	# Per-Alphabet Size
	if "Alphabet Size" in df_long.columns:
		by_alpha = df_long.groupby(["Algorithm", "Alphabet Size"])["Runtime_ms"].agg(["count", "mean", "median", "std", "min", "max"]).reset_index()
		by_alpha.to_csv(outdir / "algo_by_alphabet_size.csv", index=False)

	return stats


def count_errors(df: pd.DataFrame, algs: list, err_map: dict) -> pd.DataFrame:
	# Vectorized count: for each algorithm, check corresponding ERROR_ column and count non-empty strings
	rows = []
	for alg in algs:
		err_col = err_map.get(alg)
		if err_col and err_col in df.columns:
			# fillna with empty string, strip and count non-empty
			cnt = df[err_col].fillna("").astype(str).str.strip().ne("").sum()
		else:
			cnt = 0
		rows.append({"Algorithm": alg, "Errors": int(cnt)})
	return pd.DataFrame(rows)


def _sanitize_name(s: str) -> str:
	# Replace spaces and slashes, limit length
	return (
		s.strip()
		.replace(" ", "-")
		.replace("/", "_")
		.replace("\\", "_")
		.replace(",", "")
		.replace("\"", "")
	)[:64]


def make_output_subdir(base_outdir: Path, algs: list) -> Path:
	import hashlib

	base_outdir = Path(base_outdir)
	alg_part = "__".join([_sanitize_name(a) for a in algs])
	# short hash of algorithm list + timestamp to avoid collisions
	h = hashlib.md5((",".join(algs)).encode()).hexdigest()[:8]
	folder_name = f"results_{alg_part[:200]}_{h}"
	out = base_outdir / folder_name
	out.mkdir(parents=True, exist_ok=True)
	return out


def make_plots(df_long: pd.DataFrame, error_df: pd.DataFrame, outdir: Path, show_samples: bool = True):
	"""Create a reduced set of plot files:
	- combined_summary.png: boxplot (top) + mean vs num states and mean vs alphabet size (bottom two)
	- runtime_histograms.png: faceted histograms
	- error_counts.png: error bar chart
	"""
	sns.set(style="whitegrid")

	# Prepare aggregated data
	order = df_long.groupby("Algorithm")["Runtime_ms"].median().sort_values().index

	# Combined figure
	import matplotlib.gridspec as gridspec

	fig = plt.figure(constrained_layout=True, figsize=(14, 10))
	gs = gridspec.GridSpec(nrows=2, ncols=2, figure=fig, height_ratios=[1.0, 0.9])

	ax_box = fig.add_subplot(gs[0, :])
	sns.boxplot(x="Algorithm", y="Runtime_ms", data=df_long, order=order, ax=ax_box)
	ax_box.set_title("Runtime (ms) distribution by algorithm")
	for label in ax_box.get_xticklabels():
		label.set_rotation(45)
		label.set_ha("right")

	ax_left = fig.add_subplot(gs[1, 0])
	if "Num States" in df_long.columns:
		mean_by_state = df_long.groupby(["Num States", "Algorithm"])["Runtime_ms"].mean().reset_index()
		sns.lineplot(data=mean_by_state, x="Num States", y="Runtime_ms", hue="Algorithm", marker="o", ax=ax_left)
		ax_left.set_title("Mean runtime vs Num States")
	else:
		ax_left.axis("off")

	ax_right = fig.add_subplot(gs[1, 1])
	if "Alphabet Size" in df_long.columns:
		mean_by_alpha = df_long.groupby(["Alphabet Size", "Algorithm"])["Runtime_ms"].mean().reset_index()
		sns.lineplot(data=mean_by_alpha, x="Alphabet Size", y="Runtime_ms", hue="Algorithm", marker="o", ax=ax_right)
		ax_right.set_title("Mean runtime vs Alphabet Size")
	else:
		ax_right.axis("off")

	combined_path = outdir / "combined_summary.png"
	fig.savefig(str(combined_path))
	plt.close(fig)

	# Histogram per algorithm (faceted)
	g = sns.FacetGrid(df_long, col="Algorithm", col_wrap=3, sharex=False, sharey=False, height=3.5)
	g.map(sns.histplot, "Runtime_ms", bins=30)
	plt.subplots_adjust(top=0.9)
	g.fig.suptitle("Runtime histograms per algorithm")
	hist_path = outdir / "runtime_histograms.png"
	g.savefig(str(hist_path))
	plt.close()

	# Error counts bar chart
	plt.figure(figsize=(10, 5))
	sns.barplot(data=error_df.sort_values("Errors", ascending=False), x="Algorithm", y="Errors")
	plt.xticks(rotation=45, ha="right")
	plt.title("Error counts per algorithm")
	plt.tight_layout()
	err_path = outdir / "error_counts.png"
	plt.savefig(str(err_path))
	plt.close()

	# Variable comparison plots (runtime vs each variable)
	comp_paths = make_variable_comparison_plots(df_long, outdir, show_samples=show_samples)

	return {
		"combined": combined_path,
		"histograms": hist_path,
		"errors": err_path,
		"comparisons": comp_paths,
	}


def _sanitize_varname(s: str) -> str:
	return s.replace(" ", "_").replace("/", "_").replace("\\", "_")


def make_variable_comparison_plots(df_long: pd.DataFrame, outdir: Path, show_samples: bool = True):
	"""Create one plot per relevant variable comparing algorithms.

	Numeric variables will be plotted as mean runtime vs variable (lineplot).
	Categorical variables (Automaton Type) will use grouped bar charts.
	Returns list of created file Paths.
	"""
	created = []
	# Candidate numeric variables
	numeric_vars = [
		"Num States",
		"Alphabet Size",
		"Missing Edges",
		"Num Samples",
		"Sample Length",
		"Length Variance",
	]

	for var in tqdm(numeric_vars, desc="Creating var comparison plots"):
		if var in df_long.columns:
			# drop NaN runtimes
			tmp = df_long.dropna(subset=[var, "Runtime_ms"]).copy()
			if tmp.empty:
				continue
			# compute mean runtime per variable value using groupby (vectorized)
			mean_df = tmp.groupby([var, "Algorithm"], sort=True)["Runtime_ms"].mean().reset_index()
			plt.figure(figsize=(10, 6))
			sns.lineplot(data=mean_df, x=var, y="Runtime_ms", hue="Algorithm", marker="o")
			# optionally overlay a light scatter of raw points for context (use sample if very large)
			if show_samples:
				if len(tmp) > 20000:
					sample = tmp.sample(n=20000, random_state=1)
				else:
					sample = tmp
				sns.scatterplot(data=sample, x=var, y="Runtime_ms", hue="Algorithm", legend=False, alpha=0.08)
			plt.title(f"Mean runtime vs {var}")
			plt.tight_layout()
			fname = outdir / f"Time_vs_{_sanitize_varname(var)}.png"
			plt.savefig(str(fname))
			plt.close()
			created.append(fname)

	# Categorical: Automaton Type
	cat_var = "Automaton Type"
	if cat_var in df_long.columns:
		tmp = df_long.dropna(subset=[cat_var, "Runtime_ms"]).copy()
		if not tmp.empty:
			mean_df = tmp.groupby([cat_var, "Algorithm"], sort=True)["Runtime_ms"].mean().reset_index()
			plt.figure(figsize=(12, 6))
			sns.barplot(data=mean_df, x=cat_var, y="Runtime_ms", hue="Algorithm")
			plt.title("Mean runtime by Automaton Type and Algorithm")
			plt.xticks(rotation=45)
			plt.tight_layout()
			fname = outdir / "Time_vs_Automaton_Type.png"
			plt.savefig(str(fname))
			plt.close()
			created.append(fname)

	return created


def main():
	parser = argparse.ArgumentParser(description="Produce stats and plots from testing_times CSV")
	parser.add_argument("--input", "-i", default="../programy/testing_times.csv", help="Path to testing_times.csv")
	parser.add_argument("--outdir", "-o", default="./wykresy", help="Output directory for CSV summaries and plots")
	parser.add_argument("--no-sample-overlay", dest="no_sample_overlay", action="store_true", help="Do not plot raw-sample scatter overlays on variable comparison plots")
	args = parser.parse_args()

	csv_path = Path(args.input).expanduser().resolve()
	outdir = Path(args.outdir).expanduser()
	outdir.mkdir(parents=True, exist_ok=True)

	if not csv_path.exists():
		print(f"Input CSV not found: {csv_path}")
		sys.exit(2)

	print(f"Reading data from: {csv_path}")
	df = read_and_clean(str(csv_path))

	algs, err_map = detect_algorithms(df)
	if not algs:
		print("No algorithm runtime columns detected. Check CSV header and META_COLUMNS list.")
		sys.exit(2)

	print(f"Detected algorithms ({len(algs)}): {', '.join(algs)}")
	# Create a results subdirectory derived from the algorithm names
	results_dir = make_output_subdir(outdir, algs)

	df_long = make_long(df.copy(), algs)

	print("Computing summary statistics...")
	overall_stats = compute_and_save_stats(df_long, results_dir)

	print("Counting errors...")
	error_df = count_errors(df, algs, err_map)
	error_df.to_csv(results_dir / "error_counts.csv", index=False)

	print("Generating plots (this may take a moment)...")
	created = make_plots(df_long, error_df, results_dir, show_samples=not args.no_sample_overlay)

	print("Done. Outputs saved to:")
	for p in sorted(results_dir.glob("*.png")):
		print(" -", p)
	for p in sorted(results_dir.glob("*.csv")):
		print(" -", p)


if __name__ == "__main__":
	main()


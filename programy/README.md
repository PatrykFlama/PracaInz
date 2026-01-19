# Program do testowania algorytmów

W tym katalogu znajduje się program do testowania algorytmów naprawiania wybrakowanego DFA, wraz z ich implementacjami.

## Struktura projektu

- **algorithms/** - Implementacje algorytmów naprawiania DFA
  - Bruteforce, heurystyki, optymalizacje z jump tables
- **generators/** - Generatory automatów i przypadków testowych
- **structures/** - Struktury danych (automaton, samples, edge stats)
- **helpers/** - Funkcje pomocnicze (walidacja, wizualizacja, logging, tablica czasów)

## Kompilacja i uruchomienie

```bash
make          # Kompilacja z flagami debug
make release  # Kompilacja optymalizowana
make run      # Uruchomienie programu
make clean    # Czyszczenie artefaktów
```

## Wykorzystywane biblioteki
- [TQDM file](https://gitlab.com/miguelraggi/tqdm-cpp) - Pasek postępu w terminalu

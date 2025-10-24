# Analiza problemu uzupełniania brakujących krawędzi w automacie skończonym

## Problemset
### Zmienne problemu
- `S+` - zbiór słów pozytywnych
- `S-` - zbiór słów negatywnych
- `S` - zbiór wszystkich słów ($S = S_+ \cup S_-$)
- `E` - zbiór krawędzi w oryginalnym automacie
- `V` - zbiór wierzchołków automatu
- `n` - liczba stanów w oryginalnym automacie
- `k` - liczba brakujących krawędzi
- `m` - maksymalna długość słowa w zbiorze S
- $\Sigma$ - alfabet

### Złożoności
#### Dowolna liczba brakujących krawędzi
NP-zupełny

#### Ograniczona liczba brakujących krawędzi
| Podejście | Czas | Pamięć | Uwagi |
|-----------|------|--------|-------|
| Bruteforce | $O(n^k * size(S) * m)$ |  | Sprawdzamy wszystkie możliwe miejsca połączenia k krawędzi |


### Podproblemy
```mermaid
graph TD;
    NO-EDGES[W automacie brakuje k krawędzi] 
    
    NO-EDGES --> NO-EDGES_LIMIT-V[Ograniczony zbór wierzchołków z brakującymi krawędziami V'];
    NO-EDGES_LIMIT-V --> NO-EDGES_LIMIT-V_M[Zbiór V' ma rozmiar M, np M=1];

    NO-EDGES --> NO-EDGES_CONSTRAINT-SIZE[Dodatkowe założenia o zbiorze S+ i S-];
    NO-EDGES_CONSTRAINT-SIZE --> NO-EDGES_CONSTRAINT-SIZE_WORD-N[Długość słów w S ograniczona przez n];
    NO-EDGES_CONSTRAINT-SIZE --> NO-EDGES_CONSTRAINT-SIZE_WORDS-M[Liczba słów w S ograniczona przez M];
    NO-EDGES_CONSTRAINT-SIZE --> NO-EDGES_CONSTRAINT-SIZE_WORD-END-KNOWN[Wiemy w których stanach kończą się słowa z S];
    NO-EDGES_CONSTRAINT-SIZE --> NO-EDGES_CONSTRAINT-SIZE_WORD-LIMITED-UNKNOWN[Wiemy, że słowa z S przechodzą przez ograniczoną liczbę nieznanych krawędzi];

    NO-EDGES --> NO-EDGES_ALPHABET-SIZE[Ograniczony alfabet Σ, np do 2 liter];

    NO-EDGES --> NO-EDGES_ONLY-LABELS[Nie znamy tylko etykiet k krawędzi];
    NO-EDGES_ONLY-LABELS --> NO-EDGES_ONLY-LABELS_ONE_VERTEX[Nie znamy etykiet tylko krawędzi z tylko jednego wierzchołka, np wszystkich];

    NO-EDGES --> NO-EDGES_CONSTRAINT-AUTOMATA[Ograniczony typ automatu];
    NO-EDGES_CONSTRAINT-AUTOMATA --> NO-EDGES_CONSTRAINT-AUTOMATA_NO-CYCLES[?Automat acykliczny];
    NO-EDGES_CONSTRAINT-AUTOMATA --> NO-EDGES_CONSTRAINT-AUTOMATA_NO-LOOP[?Automat bez pętli];

    NO-EDGES --> NO-EDGES_LABELS-AND-UNDIRECTED[Nie znamy etykiet k krawędzi i ich kierunku];
```

### Ciekawe idee
- preprocessing 
  - prosty: pozbywanie się zbędnych próbek, etc
  - złożony: grupowanie stanów, etc
- caching
- drzewo prefiksowe dla S

### Pomysły do rozważenia
#### Preprocessing dla S
- Dla każdego słowa, dla każdej pozycji oraz długości podsłowa pamiętać gdzie najdalej możemy skoczyć - aby nie łazić po jego cyklu.  
Da to nakład pamięciowy $O(|S| * m^2)$ (co może można też sprytnie poprawić).  
*Wtedy przechodząc słowem $s$ po automacie, mamy gwarancję, że każdy stan odwiedzimy maksymalnie $O(1)$ razy - w pp musielibyśmy chodzić po jakimś cyklu w grafie, co moglibyśmy wykryć i korzystając z policzonych skoków ominąć.*  - **no nie wiem**  
Wtedy złożoność bruteforce to $O(n^k * |S| * n + |S| * m^2) = O(|S|(n^{k} + m^2))$.  

- Inny, oparty na tym pomysł: gdyby policzyć dla każdego sufiksu oraz stanu, gdzie najdalej możemy skoczyć?  
Wtedy efektywnie chodzimy w automacie tylko między nieznanymi krawędziami.  
  - Co gdy się pętlimy ciągle między tymi krawędziami?
  - Czy da się szybko policzyć takie skoki? - tak mi się wydaje, zwykła rekurencja z memoizacją już nie przechodzi?
Jeżeli tak, to mamy $O(n * |S| * m)$ pamięci i preprocessingu + trzeba policzyć co to wgl daje przy wykonaniu?  
Otrzymujemy swego rodzaju przekształcony/skompresowany automat, gdzie od razu połączone są stany posiadające nieznane krawędzie.  
Wtedy złożoność bruteforce to $O(n^k * |S| * k + n * |S| * m) = O(|S|(n^{k} * k + n*m))$.  
**tych funkcji w środku jest $n^n$**  
**może nie warto zapisywać dla każdego stanu** - to coś daje?  


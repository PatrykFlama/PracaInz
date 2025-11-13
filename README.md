# Naprawianie automatów z brakującymi stanami
## [Analiza problemu](./analiza/)
## [Programy](./programy/)
[(Link do dysku na wyniki eksperymentów)](https://drive.google.com/drive/folders/1PSMrtszgpOd5jcioOSFZGcdl_dgbD6ZP?usp=sharing)

Mamy dane dwa zbiory próbek: próbki pozytywne S+ i negatywne S- (to są bardzo duże zbiory słów), a dodatkowo deterministyczny automat, w którym usunięto/uszkodzono niektóre stany (lub niektóre przejścia). Naszym celem jest odbudowanie całego automatu tak, aby akceptował wszystkie słowa ze zbioru S+ i odrzucał wszystkie słowa ze zbioru S-.

Warianty problemu obejmują sytuację, gdy liczba brakujących stanów/krawędzi jest dowolna (dana na wejściu) oraz taką, gdy liczba ta jest ograniczona (np. w definicji problemu zakładamy, że rozważamy tylko automaty, którym brakuje co najwyżej 10 stanów).

Zgaduję, że przypadku, gdy liczba brakujących krawędzi jest dowolna, trzeba będzie pokazać NP-zupełność, a gdy ograniczona, problem jest w P (być może tylko dla ustalonego alfabetu). W takim razie praca obejmowałaby dowód takiej NP-zupełności, a także analizę wariantu ograniczonego, wraz z implementacją (być może z kilkoma heurystykami) i analizą wydajności.

Możliwe stopnie trudności obejmowałyby przeanalizowanie tego dla deterministycznych automatów skończonych, automatów z sumą (każda krawędź ma jakąś liczbę, a wartość słowa to suma wartości krawędzi) i ewentualnie automatów niedeterministycznych (ale to już chyba za trudne).


# References
- [TQDM file](https://gitlab.com/miguelraggi/tqdm-cpp)

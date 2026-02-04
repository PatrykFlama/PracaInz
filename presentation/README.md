# Final presentation

## Concept
- co jest tematem pracy
- definicje niezbędne do zrozumienia tematu
  - DFA
  - definicja naszego problemu
- część teoretyczna (dlaczego ona istnieje aka 'chcieliśmy przeanalizować ten problem')
- NP - mówimy że nasz problem jest NP-zupełny, bo wykazaliśmy NP-trudność (redukcją z problemu *najmniejszego zgodnego automatu*) oraz przynależność do NP
- złożoność parametryczna - tldr o złożoności parametrycznej + dlaczego to analizowaliśmy (problem był trudny, zadaliśmy sobie pytanie jak bardzo, 'ułatwiając' go)
  - rysunek z hierarchią klas złożoności parametrycznej, szybkie omówienie klas
  - wykazaliśmy że problem jest W[2]-trudny względem liczby stanów automatu
    - rysunki/idee redukcji
    - analogicznie dla redukcji z alfabetem 2
  - wykazaliśmy że problem należy do W[P]
  - rezultat - nasz problem jest międzdy W[2] a W[P] + dać intuicję co to oznacza
    - nie jest FPT chyba że FPT = W[2] (problem milenialny)
    - nie jest też XP
- mimo trudności problemu, chcieliśmy znaleźć podejście praktyczne, które umożliwiłoby rozwiązywanie rzeczywistych instancji
  - podejście brute-force traktowaliśmy jako punkt odniesienia
  - podejście z przeskakiwaniem ciągów znanych krawędzi
  - podejście z przeszukiwaniem przestrzeni rozwiązań z heurystyką (czemu nie działało to tak dobrze - lokalne minima)
  - podejście z pruningiem 'drzewa rekurencji'
- wyniki eksperymentów (eksperymentu)


## Sidenotes
### Definicje

Problem:  
Naprawianie uszkodzonych deterministycznych automatów skończonych (DFA) na podstawie próbek pozytywnych i negatywnych.

- Automat może mieć brakujące krawędzie (przejścia niezdefiniowane)
- Automat może mieć brakujące stany
- Musimy uzupełnić automat, aby akceptował wszystkie słowa z S+ i odrzucał wszystkie słowa z S−


### Dlaczego to jest ciekawe/ważne?

> Praktyczne zastosowania:
> - Odtwarzanie stanu systemu z logów
> - Znamy część jakiegoś oprogramowania i chcemy je uzupełnić
> - Klasyczne uczenie pasywne, ale z automatem wejściowym - np gdy mamy częściową wiedzę o rozwiązaniu


### NP-zupełność
dopowiedzieć że redukcja do NP-trudności jest z problemu najmniejszego zgodnego automatu

### Złożoność parametryczna

Obserwacja:  
Jeśli automat ma tylko **1 brakującą krawędź**, to wystarczy sprawdzić |Q| opcji.   
Jeśli ma **k brakujących krawędzi**, to wystarczy sprawdzić |Q|^k opcji.  
Pytanie: Czy możemy **rozdzielić** trudność problemu od rozmiaru automatu?  

**omówić klasy**


### W[2]-trudność
k-set cover:
<!-- - uniwersum U, rodzina zbiorów F, liczba k
- czy istnieje podzbiór F' który pokrywa U i jest mniejszy od k
- W[2]-zupełny względem parametru k -->


Alfabet ma rozmiar zależny od |U| — to słaby punkt redukcji.

**Idea**: zamiast jednej litery ej dla każdego elementu, używamy bin. reprezentacji indeksu

### W[P] przynależność

### Podejście praktyczne
Dlaczego sie tym zajmujemy?
-> Mimo trudności tego problemu, chcieliśmy znaleźć empirycznie szybszy algorytm. 

Brute force jako punkt odniesienia. Dla kazdej wygenerowanej kombinacji przejsc, symulujemy próbki i sprawdzamy czy sa poprawne z danymi wejsciowymi. Jesli tak, to konczymy algorytm.

### Algorytm ze skokami

Na początku budujemy Jump Table, w której zapamiętujemy gdzie od danego stanu mozemy najdalej przeskoczyc.

Te strukture wykorzytujemy przy walidacji probek dla wygenerowanych juz przejsc.

Najbardziej zyskujemy przy dlugich ciagach, ktore mozemy w ten sposob przeskoczyc.

Nasza struktura jest tablica, ale  podejrzewamy ze da sie zoptymalizowac pamiec.

### Heurystyka

Zauwazylismy, ze jest to przeszukiwanie przestrzeni co zainspirowalo nas do zinterpretowania problemu w kontekscie AI dla gier (wiadomo o co chodzi, albo nie).

Myslelismy o roznych mozliwosciach funkcji celu. Powstanowilismy oprzec sie o probki.

opowiedziec o algorytmie

Co osiągamy?
-> algorytm wykazuje poprawe w stosunku do naiwnego dla malych danych, niestety przy wiekszych mamy duza czestotliwosc timeoutow

Dlaczego tak jest?
-> Poprawa liczby błędnych probek wcale nie musi przyblizac nas do rozwiazania: utykamy na minimach lokalnych.

### Pruning 
Idea: chcemy wiedziec szybciej o sensie eksploracji

Robimy filtrowanie automatow za pomoca zestawu probek. Wiemy ile dana próbka Chcemy wiedziec czy dana konfiguracja krawedzi ma jeszcze jakis zestaw poprawnych autmatow.

### Wyniki

Sposob testowania:
-> parametry, poza badanym, mialy stale wartosci
-> automat generowany jako pelny, na tej podstawie probki, potem losowe usuwanie przejsc (ustalona liczba i te ktore korzystaja z probek)
-> rolling mean: chcielismy uwidocznic trend, dlatego nie ma slupkowych




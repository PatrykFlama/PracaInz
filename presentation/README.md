# Final presentation
<!-- 
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
- wyniki eksperymentów (eksperymentu) -->


## Sidenotes
### Definicje

Problem:  

- **brakujące krawędzie** - niezdefiniowane przejścia
- **brakujące stany**
- **nieznane wartościowanie stanów** - nie wiemy w którym zbiorze F jest stan
- Musimy **uzupełnić automat**, aby **akceptował** wszystkie słowa z **S+** i **odrzucał** wszystkie słowa z **S−**


### Dlaczego to jest ciekawe/ważne?

Praktyczne zastosowania:
- Klasyczne uczenie pasywne, ale z automatem wejściowym - np gdy mamy częściową wiedzę o rozwiązaniu
- Odtwarzanie stanu systemu z logów
- Znamy część jakiegoś oprogramowania i chcemy je uzupełnić


### NP-zupełność
- redukcja do **NP-trudności** jest z problemu **najmniejszego zgodnego automatu**  
- wykazaliśmy też **przynależność** do **NP**  
- więc problem jest **NP-zupełny**

### Złożoność parametryczna

Obserwacja:  
- Jeśli automat ma tylko **1 brakującą krawędź**, to wystarczy sprawdzić |liczba stanów| opcji.   
- Jeśli ma **k brakujących krawędzi**, to wystarczy sprawdzić |liczba stanów|^|liczba brakujących krawędzi| opcji.  
- Pytanie: Czy możemy **rozdzielić** zależność brakujących krawędzi od rozmiaru automatu?  

**omówić klasy** + od razu widać że **jest w XP**


### W[2]-trudność
> k-set cover:
> - uniwersum U, rodzina zbiorów F, liczba k
> - czy istnieje podzbiór F' który pokrywa U i jest mniejszy od k
> - W[2]-zupełny względem parametru k

omówić redukcję

**Alfabet** ma rozmiar **zależny** od |**liczba stanów**| - to słaby punkt redukcji.

**Idea binarnego**: zamiast jednej litery ej dla każdego elementu, używamy bin. reprezentacji indeksu

### W[P] przynależność

jak wykazaliśmy

### Rezultat
- problem jest między **W[2]** a **W[P]**
- nie jest **FPT** chyba że **FPT = W[2]**
- jest też prostszy niż **XP-trudne**

-----------


### Podejście praktyczne
Dlaczego sie tym zajmujemy?
- Mimo trudności tego problemu, chcieliśmy znaleźć empirycznie szybszy algorytm. 

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
- algorytm wykazuje poprawe w stosunku do naiwnego dla malych danych, niestety przy wiekszych mamy duza czestotliwosc timeoutow

Dlaczego tak jest?
- Poprawa liczby błędnych probek wcale nie musi przyblizac nas do rozwiazania: utykamy na minimach lokalnych.

### Pruning 
Idea: chcemy wiedziec szybciej o sensie eksploracji

Robimy filtrowanie automatow za pomoca zestawu probek. Chcemy wiedziec czy dana konfiguracja krawedzi ma jeszcze jakis zestaw poprawnych autmatow.

(Optional*: aka Nie mow dopoki nie zapytaja)
Jak to robimy?
- dla ulatwienia bierzemy losowo wybrany zestaw próbek i sprawdzamy na tych wybranych probkach sprawdzanie i dopiero poziej na reszcie.

### Wyniki

Sposob testowania:
- parametry, poza badanym, mialy stale wartosci
- automat generowany jako pelny, na tej podstawie probki, potem losowe usuwanie przejsc (ustalona liczba i te ktore korzystaja z probek)
- rolling mean: chcielismy uwidocznic trend, dlatego nie ma slupkowych

Rolling mean:
- srednia (wazona) z okna o stalej dlugosci, dlatego mamy niecalkowite liczby przy krawedziach




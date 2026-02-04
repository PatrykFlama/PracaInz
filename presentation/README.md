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
### Czym zajmujemy się w pracy?
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



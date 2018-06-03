Karolina Deszcz,
277284

# AAL

Projekt realizowany w ramach przedmiotu Analiza algorytmów.

## 1. Opis problemu
Danych jest n ponumerowanych kolejno pojemników. W pojemnikach znajdują się
klocki w k kolorach, przy czym łącznie we wszystkich pojemnikach jest nie więcej niż
n klocków danego koloru. Pojemność i-tego pojemnika wynosi p i klocków. Należy
przełożyć klocki w taki sposób, żeby w każdym pojemniku pozostał co najwyżej jeden
klocek każdego koloru. W jednym ruchu można przełożyć jeden klocek z pojemnika,
w którym się znajduje do pudełka sąsiedniego (dozwolone jest przenoszenie pomiędzy
pudełkiem pierwszym i n-tym).

## 2. Zaimplementowane algorytmy
2.1 Algorytm naiwny

Zaczynając od pierwszego kubka sprawdzamy czy w zbiorze klocków do
przeniesienia znajduje się jakiś klocek. Jeśli tak, wyszukujemy najbliższy
pojemnik do którego może zostać przeniesiony. Wybieramy kierunek
przenoszenia tak, aby liczba pojemników pośrednich była jak najmniejsza. Jeśli,
któryś z pośrednich pojemników jest pełny i nie może przyjąć nowego klocka,
wybierany jest przeciwny kierunek przenoszenia. Jeśli i w tą stronę
przeniesienie nie jest możliwe wybierany jest kolejny klocek do przeniesienia.
Następnie rozpatrujemy w ten sam sposób drugi i każdy kolejny pojemnik.
W momencie gdy po przejściu przez wszystkie pojemniki nie zostało wykonane
żadne przeniesienie problem zostaje uznany za niemożliwy do rozwiązania.

2.2 BFS – przeszukiwanie wszerz 
Problem reprezentowany jest w formie drzewa, którego wierzchołkami są stany
reprezentujące obecne rozmieszczenie klocków w pojemnikach. Wierzchołki
potomne powstają poprzez przeniesienie jednego klocka do pojemnika po
prawej, a następnie do pojemnika po lewej. Każdy nadmiarowy klocek w
którymkolwiek pojemniku, generuje dwa stany potomne.

2.3 IDA* - iterative deeping A*
Tak jak w przypadku algorytmu BFS problem reprezentowany jest w formie
drzewa. Jednak stanom przyporządkowane są wartości funkcji celu, która jest
zdefiniowana jako suma kosztu dotarcia do danego węzła (g(n)) i wartości
funkcji heurystycznej w danym węźle (h(n)). H(n) wyrażana jest przez liczbę
klocków, nie spełniających wymagań zadania w danym stanie. W każdej iteracji algorytmu graf przeszukiwany
jest w głąb, jednak rozwijane są tylko te węzły, których funkcja celu nie przekracza limitu, który jest 
ustalany na początku każdej iteracji. W pierwszej, jest to wartość funkcji celu w stanie początkowym, 
w każdej kolejnej jest to najmniejsza wartość funkcji celu, która przekroczyła limit w poprzedniej iteracji.
Algorytm działa tak długo, aż nie zostanie wygenerowany węzeł, dla którego h(n) = 0.

## 3. Struktury danych
3.1 Bucket  <br />
pojemnik, zawiera wektor klocków, których kolor reprezowany jest przez liczbę całkowitą.
Dodatkowo bucket przechowuje zbiór zaakceptowanych klocków (set) oraz zbiór klocków, które muszą zostać przeniesione do 
innego pojemnika (multiset)
3.2 State <br />
struktura używana w algorytmach grafowych. Reprezentuje stan wszystkich pojemników. Przechowuje wektor pojemników.
Dla bardziej optymalnego przechowywania stanów odwiedzonych została posiada funkcję hashującą.

## 4.Przewodnik po plikach

generator.h/generator.cpp => implementacja klasy Bucket, implementacja klasy Test, generatora losowych danych <br />
Naive.h/Naive.cpp => implementacja algorytmu naiwnego <br />
BSF.h/BSF.cpp => implementacja algorytmu przeszukującego wszerz <br />
IDAStar.h/IDAStar.cpp => implementacja algorytmu Iterative Deeping A* <br />
Application.h/Application.cpp => interfejs użytkownika, wywołuje wykonanie algorytmów <br />
Main.cpp => uruchamia aplikację <br />

## 5. Sposób uruchomienia programu

Użytkownik ma możliwość uruchomienia programu w trzech trybach (m1/m2/m3). <br /><br />
Tryb m1: przypadek testowy zapisany jest w pliku, którego ścieżkę podaje się jako argument programu. Na standardowe wyjście zostaną wyświetlone czas i ilość iteracji w jakich problem został rozwiązany przez dany algorytm. Użytkownik może dodać parametr --a file.txt, wtedy dla każdego algorytmu zostaną wygenerowane pliki zawieracjące szczegółowe informacje odnośnie przebiegu działania algorytmów (pliki odpowiednio Naive_file.txt, BSF_file.txt, IDA_file.txt) <br />
przykład wywołania: ./App m1 -f input.txt -a out.txt <br /> <br />
Tryb m2: przypadek testowy generowany jest automatycznie, jednak użytkownik określa parametry generatora. Podaje liczbę pojemników (-n), liczbę kolorów (-k). Pozostałe parametry są opcjonalne: maksymalna pojemność pojemnika (--max_p), minimalna pojemność pojemnika(--min_p), maksymalna ilość wolnego miejsca jaka może pozostać w pojemniku(--max_free_space), minimalna ilość wolnego miejsca(--min_free_space). Parametry, które nie zostaną podane przez użytkownika przybierają wartości domyslne. Jeśli użytkownik doda parametr --a, jak w poprzednim trybie zostaną wygenerowane pliki z zapisem przebiegu algorytmów.  Na standardowe wyjście zostaną wyświetlone czas i ilość iteracji w jakich problem został rozwiązany przez dany algorytm. <br />
przykład wywołania: ./App m2 -n 10 -k 5 --max_p 5 -a out.txt <br /> <br />
Tryb m3 : wygenerowanie zestawu przypadków testowych. Użytkownik podaje liczbę testów (-t), liczbę pojemników (-n) oraz liczbę kolorów (-k). Dla każdego algorytmu wyświetlana jest średnia ilość wykonanych iteracji oraz średni czas w jakim algorytm rozwiązał problem. </br>
przykład wywołania: ./App m3 -t 10 -n 10 -k 5 

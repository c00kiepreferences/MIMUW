## crosswords

W ramach zadania należy zaimplementować moduł crosswords pozwalający na
tworzenie krzyżówek. W ramach tego modułu powinny być udostępnione następujące
klasy:
 * Word       – reprezentująca pojedyncze słowo umieszczane w krzyżówce;
 * RectArea   – reprezentująca prostokątny obszar, będący wycinkiem przestrzeni,
                w której umieszczać można słowa w krzyżówce;
 * Crossword  – reprezentująca krzyżówkę.

W klasie Word powinna być możliwość:

 - przechowywania pozycji początku słowa w krzyżówce, orientacji słowa (poziomej
   albo pionowej) oraz napisu zawierającego kolejne znaki słowa.
   Zakładamy, że wspomniana pozycja należy do dyskretnej przestrzeni
   (1)    {(x, y): x i y są liczbami typu size_t}.
   Punkt (0, 0) leży w lewym górnym rogu tej przestrzeni. Współrzędna x jest
   numerem kolumny, zaś y jest numerem wiersza.
   Orientacja słowa może mieć jedynie wartość H lub V typu wyliczeniowego
   opartego na typie bool.

 - utworzenia obiektu w oparciu o podane parametry.
   Nie powinno być możliwości tworzenia obiektu bez podania żadnych parametrów.
   Dodatkowo, przekazywany napis nie może być pusty. Dlatego w takim przypadku
   należy go zastąpić domyślnym napisem "?".

 - utworzenia obiektu za pomocą domyślnego kopiowania oraz przenoszenia.

 - realizacji domyślnych operacji przypisania w wersji kopiującej i przenoszącej.

 - pozyskania informacji o pozycji początkowej i pozycji końcowej słowa,
   o orientacji słowa, o znaku znajdującym się pod danym indeksem w napisie
   oraz o długości napisu.

 - porównywania obiektów za pomocą operatora <=>, zgodnie z porządkiem
   leksykograficznym względem (x, y, orientacja), przy czym H < V.

 - porównywania operatorami == i != sprawdzającymi jedynie równość lub różność
   trójek (x, y, orientacja).

 - pozyskania obiektu RectArea obejmującego współrzędne, na których leży słowo.

W klasie RectArea powinna być możliwość:

 - przechowywania pozycji lewego górnego i prawego dolnego rogu prostokątnego
   wycinka przestrzeni (1).
   Zakładamy, że niepoprawne położenie wspomnianych rogów względem siebie jest
   możliwe i zawsze oznacza pusty obszar.

 - utworzenia obiektu w oparciu o punkty podane poprzez parametry.
   Nie powinno być możliwości tworzenia obiektu bez podania współrzędnych obu
   rogów.

 - utworzenia obiektu za pomocą domyślnego kopiowania oraz przenoszenia.

 - realizacji domyślnych operacji przypisania w wersji kopiującej i przenoszącej.

 - odczytania i zmiany pozycji rogów obszaru.

 - znajdowania części wspólnej dwóch obszarów, zarówno za pomocą operatora *,
   jak i *=;

 - pozyskania rozmiaru obszaru, tzn. pary składającej się z jego szerokości
   i wysokości.
   Zakładamy, że w przypadku pustego obszaru jego rozmiary są zawsze równe (0, 0).

 - sprawdzenia, czy obszar jest pusty.

 - powiększenia obszaru, tak żeby objął sobą podany punkt z przestrzeni (1).
   Powiększenie to powinno być możliwie najmniejsze.

W klasie Crossword powinna być możliwość:

 - przechowywania obiektów typu Word (słów znajdujących się w krzyżówce) oraz
   obiektu typu RectArea, będącego najmniejszym wycinkiem przestrzeni (1)
   obejmującym wszystkie słowa zawarte w krzyżówce.

 - utworzenia obiektu krzyżówki w oparciu o słowa przekazane poprzez parametry.
   Słowa mają być wstawiane do krzyżówki w kolejności ich podania, przy czym
   jeśli wstawiane słowo koliduje ze słowami już znajdującymi się w krzyżówce, to
   należy je pominąć. Nie powinno być możliwości utworzenia krzyżówki bez podania
   jakiegokolwiek słowa. Każda tworzona krzyżówka musi mieć zatem co najmniej
   jedno słowo.

 - utworzenia krzyżówki za pomocą kopiowania oraz przenoszenia oraz realizacji
   operacji przypisania w wersji kopiującej i przenoszącej.
   Może się okazać, że przeniesiona krzyżówka będzie pusta. W takiej sytuacji
   należy zapewnić, żeby przechowywany obszar RectArea też był pusty.

 - wstawienia słowa do krzyżówki, o ile nie koliduje ono ze słowami znajdującymi
   się już w krzyżówce.

 - pozyskania rozmiaru krzyżówki, czyli pary liczb znaków zajmowanych przez
   krzyżówkę w poziomie i w pionie;

 - pozyskania pary liczb słów zorientowanych horyzontalnie i wertykalnie.

 - dodawania dwóch krzyżówek za pomocą operatorów + i +=.
   Podczas tej operacji słowa z drugiej krzyżówki wstawiane są po kolei
   do pierwszej krzyżówki, pomijając ewentualne kolizje.

 - drukowania krzyżówki na standardowym wyjściu z tłem wypełnianym znakiem
   podanym poprzez odpowiednią zmienną (początkowo ma to być kropka).
   Rysowany ma być tylko wycinek zawierający słowa krzyżówki plus obramowanie
   wielkości jednego znaku, a nie cała przestrzeń (1).
   Pisane mogą być tylko wielkie litery (konwertowane ewentualnie z małych) oraz
   '?', gdy znak w słowie nie jest literą. Na wydruku, odstęp pomiędzy
   sąsiadującymi poziomo znakami krzyżówki (znakami słowa, znakami tła) powinien
   być zrobiony pojedynczą spacją.

 - sprawdzenia (prywatnego) kolizji podanego poprzez parametr słowa ze słowami
   w krzyżówce.
   Odstęp pomiędzy słowami musi mieć wielkość co najmniej jednego znaku.
   Krzyżujące się słowa muszą w miejscu przecięcia zgadzać się z dokładnością do
   wyświetlanego znaku.

Oprócz wspomnianych klas należy zdefiniować stałe lub zmienne inicjowane tylko
podczas kompilacji, które określają:
 - domyślny znak, który nie jest literą ('?');
 - domyślny napis przechowywany w słowie Word ("?");
 - domyślny pusty obszar RectArea;
 - znak tła krzyżówki ('.');

Do reprezentacji pozycji w przestrzeni (1) i rozmiarów występujących w RectArea
oraz Crossword należy wykorzystać typy nazwane odpowiednio pos_t i dim_t,
oba będące parami elementów typu size_t.

Przykład użycia znajduje się w pliku crosswords_example.cc. Informacje
wypisywane przez ten przykład znajdują się w pliku crosswords_example.out.

Rozwiązanie będzie kompilowane poleceniem

g++ -Wall -Wextra -O2 -std=c++20 *.cc

Rozwiązanie powinno zawierać plik crosswords.h oraz opcjonalnie plik
crosswords.cc. Pliki te należy umieścić w repozytorium w katalogu

grupaN/zadanie3/ab123456+cd123456

lub

grupaN/zadanie3/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami
członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie
powinien zawierać innych plików. Nie wolno umieszczać w repozytorium plików
dużych, binarnych, tymczasowych (np. *.o) ani innych zbędnych.



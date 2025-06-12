# Autorzy
- Adam **Marzec**
- Marta **Załuska**

# Opis
Nasz program - **system biletów Polskich Kolei Politechnicznych (PKP)** - umożliwia **rezerwowanie miejsc** w pociągach dodanych do bazy danych. Stworzona przez nas przykładowa baza danych zawiera jedno połączenie **Przemyśl Główny - Ustka** (czyli dwie relacje, bo w obie strony). Dodatkowo mamy możliwość **wyświetlenia informacji** o pociągu, pasażerze lub połączeniu. Program jest stworzony dla kasjerów, dlatego też nie ma faktycznego systemu poboru opłat, jedynie wyświetla znalezione miejsca wraz z ceną.

# Instrukcja użytkownika
Użytkownik może poruszać się po programie za pomocą klawiszy **strzałek**, zatwierdzać opcje za pomocą klawisza **Enter** oraz cofać się lub anulować transację za pomocą klawisza **Escape**.  

Najpierw użytkownik znajduje się w głównym menu i wybiera, czy chce **zarezerwować bilet**, czy **wyświetlić informacje** o danym obiekcie. Po wejściu w opcję wyświetlania informacji ma do wyboru o czym wyświetlić informacje - o **trasie** (wyświetla wszystkie stacje na danej trasie), **pociągu** (wyświetla skład pociągu - numery i modele wagonów), czy o **pasażerze** (wyświetla imię, nazwisko oraz informacje o bilecie).  

Po wejściu w opcję rezerwacji biletów użytkownik wybiera **relację**, **stacje** pomiędzy którymi odbędzie się podróż i **datę przejazdu**, a następnie wybiera z listy interesujący go **przejazd** (jednego dnia może się odbyć kilka przejazdów między danymi stacjami). Następnie podaje **ilość pasażerów**, **imię oraz nazwisko** rezerwującego, **zniżki** dla każdego pasażera oraz **klasę**, którą chce pojechać. Jeśli rezerwacja odbywa się dla jednej osoby, użytkownik jest pytany o **preferencje siedzenia** (przodem/tyłem do kierunku jazdy, okno/korytarz/środek, czy przy stoliku). Program postara się dopasować miejsce zgodnie z preferencjami, jeśli jednak nie ma takiego wolnego miejsca, spyta użytkownika, czy chce kontynuować rezerwację bez preferencji. Jeśli rezerwacja jest grupowa, nie można podać preferencji dla każdego pasażera z osobna, zamiast tego program dopasowuje miejsca **w tym samym przedziale** dla całej grupy (jeśli zmieści się ona w jednym przedziale, inaczej rozdziela ją na kilka przedziałów).  

Jeśli program nie znajdzie wystarczającej ilości miejsc w danym pociągu, poinformuje użytkownika, że rezerwacja **nie jest możliwa**. W przeciwnym wypadku **wyświetli dane** (numer biletu, numer wagonu, numer siedzenia, obliczona cena) dla każdego pasażera oraz spyta, czy użytkownik ostatecznie zatwierdza rezerwację. Jeśli użytkownik się zgodzi, rezerwacja jest **zapisywana do bazy danych**.

# Porównanie z systemem PKP IC
Nasz system bazuje w swojej strukturze na systemie **PKP IC** - odwzorowaliśmy i wprowadziliśmy do własnej bazy danych wybrane połączenie, w szczególności: stacje na trasie, dokładne odległości między stacjami, czasy przyjazdów i odjazdów, kody pociągów, składy wagonów, ich numery i modele, a także ilości, numery i położenie siedzeń.  
**Ciekawostka:** miejsca w PKP IC są numerowane tak, aby można było łatwiej je obsługiwać, dlatego nawet jeśli miejsca fizycznie nie znajdują się w przedziale, to i tak mają przydzielony przedział, a ich numer składa się z numeru przedziału i numeru miejsca w tym przedziale - dlatego nigdy nie spotkamy miejsca 10, 20, czy też 39.  

Naszym celem było **zniwelować niektóre problemy**, które istnieją obecnie w systemie PKP IC:
- możliwość rezerwacji tylko dla grup, w których ilość osób jest mniejsza od 6 - **skutecznie**, nasz program wspiera rezerwacje **bez określonego limitu osób**, był testowany nawet dla absurdalnie dużych liczb takich jak 140 osób
- przy rezerwacji grupowej, jeśli nie ma tylu wolnych miejsc obok siebie, system nie potrafi znaleźć miejsc - **skutecznie**, nasz system w takim wypadku proponuje użytkownikowi **rozdzielenie grup** tak, aby każdy miał jakieś miejsce, nawet jeśli grupa nie będzie siedzieć razem
- rezerwacja z konkretnymi preferencjami zwraca brak miejsc, gdy nie ma miejsc o danych preferencjach, nawet jeżeli są dostępne inne wolne miejsca - **skutecznie**, gdy nasz program nie znajdzie żadnych miejsc zgodnych z preferencjami, pyta użytkownika czy może **usunąć preferencje**, aby znaleźć inne miejsce (robi to tylko wtedy, gdy jest wystarczająca ogólna ilość miejsc w pociągu)
- nie da się określić preferencji kierunku siedzenia (przodem/tyłem do kierunku jazdy) - **skutecznie**, nasz system **pozwala na sprecyzowanie** takiej preferencji

# Wykorzystane rozwiązania
Nasz system łączy się z lokalną bazą danych **SQLite** - przechowuje całą bazę danych w jednym pliku. Do jej obsługi użyliśmy biblioteki [SQLiteCpp](https://github.com/SRombauts/SQLiteCpp) (autor: [SRombauts](https://github.com/SRombauts)) - to obiektowa biblioteka C++, umożliwia ona wykonywanie zapytań SQL, obsługę parametrów, transakcji i błędów w sposób zgodny praktykami C++. Dzięki tej bibliotece kod dostępu do bazy danych w projekcie jest **czytelniejszy i łatwiejszy w utrzymaniu**. 

# Podsumowanie
Zrealizowaliśmy **wszystkie założenia wstępne**, czyli rezerwację biletów zgodnie z preferencjami oraz sprawdzanie informacji o wybranych pociągach, czy trasach. Dodatkowo udało nam się **rozwiązać niektóre problemy**, które ciągle istnieją w faktycznym systemie PKP IC.

# Rozbudowa programu
Nasz system można by oczywiście rozszerzyć o więcej stacji, relacji, pociągów i dat przejazdów, jednak byłoby to jedynie dodanie rekordów do bazy danych, ponieważ nasz kod jest **w pełni skalowalny**. Jedyną rzeczą, którą należałoby zmienić byłoby ulepszenie wsparcia dla pociągów, które w pewnym momencie swojej trasy przejeżdżają przez północ (zmienia się data).  
W samym kodzie moglibyśmy jeszcze dodać **obsługę miejsc ze specjalnymi udogodnieniami**, takimi jak miejsca dla niepełnosprawnych na wózku inwalidzkim, miejsca na rowery, strefa ciszy, czy miejsca dla dzieci do lat 6. Przygotowaliśmy w kodzie oraz w bazie miejsce na takie preferencje, jednak nie zdążyliśmy dokończyć obsługi ich rezerwacji.  
Moglibyśmy również automatycznie **usuwać bilety** na przejazdy, które już się odbyły, co zaoszczędziłoby miejsca w bazie danych. 

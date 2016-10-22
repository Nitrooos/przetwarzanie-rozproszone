Robot Battle Arena
======

System zawiera k klientów połączonych w sieć w topologii kliki. Klienci uzgadniają wspólną logiczną planszę P w dwóch wymiarach. 
Rozgrywka prowadzona jest w rundach. W każdej rundzie dowolny klient uruchamia n robotów na planszy P,
gdzie każdy z robotów ri jest zaprogramowamy przez użytkownika klienta za pomocą algorytmu A(ri). Po rozpoczęciu rundy 
roboty mogą w czasie rzeczywistym poruszać się po planszy, obracać się, prowadzić ogień. Ostatni robot który żyje na P wygrywa rundę.
System jest zdecentralizowany: tj. nie posiada wyszczególnionych klientów którzy pełnią funkcję koordynatora systemu.
System utrzymuje spójny stan przez consensus (za pomocą rozgłoszeniowego algorytmu konsensusu podstawowego).

####**Użyte technologie:**
    - Scala
    - Akka

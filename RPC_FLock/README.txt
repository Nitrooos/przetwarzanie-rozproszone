Projekt wykorzystuje bibliotekę Sun RPC do implementacji zdalnej usługi 
zakładania blokad plikowych (w trybie wyłącznym - do zapisu i współdzielonym -
do odczytu).

Zasada działania:
Klient zgłasza się do serwera w celu otrzymania unikalnego numeru (id) żądania,
które serwer mu przydziela. Następnie wykonuje na zdalną procedurę blokady
pliku na serwerze (parametry takie jak: nazwa pliku, tryb blokady, bajt startowy,
długość blokady są parametrami wykonania programu klienta). Serwer odpowiada
albo komunikatem o sukcesie operacji albo informacją o niemożliwości założenia
żądanej blokady w danym momencie (inni klienci mogli założyć blokady 
konfliktujące z naszą). Wówczas klient uruchamia wątek WŁASNEGO serwera RPC z
1 procedurą - informującą o założeniu żądanej wcześniej blokady. Sam wątek
klienta jest usypiany. Jeśli serwerowi uda założyć się blokadę żądaną wcześniej
przez klienta (np w wyniku zwolnienia konfliktujących blokad), to serwer wywoła
u klienta (w jego wątku serwerowym) procedurę, która poinformuje o tym klienta.

Projekt wykorzystuje wiele cech standardu C++11, dlatego wymaga w miarę nowego
kompilatora (testowane na gcc w wersji 5.2.1, ale powinien wystarczyć 4.8.2
i późniejsze).

Proces serwera wizualizuje blokady założone na pliku public/file.txt na 
standardowym wyjściu, przy użyciu kolorów dla rozróżnienia typów blokad - 
czerwone blokady są wyłączne, a zielone - współdzielone.

Cały kod znajduje się w katalogu RPC_FLock/src.

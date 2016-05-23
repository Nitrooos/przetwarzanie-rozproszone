***System monitorowania parametrów sieci komputerowej w systemie operacyjnym GNU/Linux z użyciem gniazd sieciowych PF_NETLINK***

Opis projektu
---
Projekt napisany został w architekturze klient-serwer. Klient został zaimplementowany w C++ a serwer
w Ruby. Klient ma za zadanie łączyć się z jądrem systemu operacyjnego i nasłuchiwać nadchodzących
wiadomości a następnie wypisywać je na bieżąco na wyjściu, podając przy tym typ, poszczególne flagi, atrybuty
i inne dane wiadomości. Opcjonalnie informacje te mogą być wysyłane do serwera, który je agreguje i
zapisuje do jednego pliku.

Zawartość plików źródłowych i zasada kompilacji
---
src/client                      pliki źródłowe programu klienta
          application.*         klasa główna aplikacji
          /arguments            pliki odpowiedzialne za parsowanie argumentów wywołania programu
          /connection           pliki klas odpowiedzialnych za połączenie klienta z serwerem 
                                poprzez protokół TCP oraz za połączenie klienta z jądrem systemu
                                operacyjnego za pomocą gniazd PF_NETLINK
          /exceptions           plik definujący możliwe do wystąpienia wyjątki w programie
          /message              poszczególne katalogi wewnątrz opisują różne typy wiaomości z jądra
                  /base.*       abstrakcyjny typ wiadomości z jądra systemu operacyjnego
                  /addr         pliki obsługi wiadomości typu RTM_NEWADDR i RTM_DELADDR
                  /link         pliki obsługi wiadomości typu RTM_NEWLINK i RTM_DELLINK
                  /neigh        pliki obsługi wiadomości typu RTM_NEWNEIGH i RTM_DELNEIGH
                  /route        pliki obsługi wiadomości typu RTM_NEWROUTE i RTM_DELROUTE
                  /rule         pliki obsługi wiadomości typu RTM_NEWRULE i RTM_DELRULE 

Program kliencki należy kompilować komendą make w katalogu projekt-zaliczeniowy.
Program serwera to skrypt w Ruby, nie wymaga więc kompilacji.

Sposób uruchomienia programu
---
Klienta uruchomić można z katalogu projekt-zaliczeniowy komendą
    debug/netmon [SERVER_IP_ADDRESS [PORT]]
np. debug/netmon 127.0.0.1 1234

Serwer uruchamiany jest poleceniem
    ruby src/server/application.rb
Dodanie przełącznika -h lub --help wyświetli listę dostępnych opcji.

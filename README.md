Programowanie sieciowe
======

Repozytorium zawiera projekt zaliczeniowy "Netmon" (Network Monitor) z przedmiotu Programowanie sieciowe.
Netmon działa w oparciu o technologię gniazd sieciowych PF_NETLINK, poprzez które następuje komunikacja
z jądrem systemu operacyjnego. Program otrzymuje z jądra wiadomości za każdym razem, gdy zostanie utworzony nowy/usunięty
intefejs sieciowy, utworzona/usunięta zostanie trasa w tablicy routingu, dodany/usunięty zostanie adres IP z interfejsu
lub rozgłoszony zostanie fakt dodania nowego węzła w bezpośrednim otoczeniu sieciowym przez protokół ARP.

####**Użyte technologie:**
    - klient
        * C++ 11
        * gniazda sieciowe PF_INET oraz PF_NETLINK
    - serwer
        * Ruby
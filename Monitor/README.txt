Projekt wykorzystuje bibliotekę MPI w implementacji MPICH w wersji 3.2.
Do poprawnej kompilacji konieczne jest używanie MPI wspierającego wielowątkowość
tworzonych aplikacji (np w przypadku MPICH konieczna jest kompilacja biblioteki
z flagą --enable-threads).

Projekt wykorzystuje wiele cech standardu C++11, dlatego wymaga w miarę nowego
kompilatora (testowane na gcc w wersji 5.2.1, ale powinien wystarczyć 4.8.2
i późniejsze).

Cały kod znajduje się w katalogu Monitor/src.
W nim, w podkatalogu app/ znajdują się źródła przykładu napisanego za pomocą 
stworzonego przeze mnie monitora - jest to rozwiązanie rozproszonego problemu
producenta-konsumenta.
Z kolei w podkatalogu monitor/ znajdują się źródła samego rozproszonego
monitora, algorytmu dostępu do sekcji krytycznej, synchronizacji za pomocą XDR
itp.

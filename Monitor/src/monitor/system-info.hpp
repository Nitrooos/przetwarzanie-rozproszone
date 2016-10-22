#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <mpi.h>

struct SystemInfo {
    int _mpi_tid,                               // tid procesu
        _monitors_count;                        // liczba procesów niorących udział w przetwarzaniu
    char _host_name[MPI_MAX_PROCESSOR_NAME];    // nazwa komputera, na którym wykonuje się bieżący proces    
};

#endif /* SYSTEM_INFO_H */

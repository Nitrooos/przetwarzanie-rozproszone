#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include <rpc/xdr.h>

namespace Monitor {

    struct SharedVariables {
        virtual bool transform(XDR *xdr) = 0;                   // klasa pochodna musi implementować metodę serializującą własne współdzielone dane
        virtual string to_string() = 0;                         // wypisz na ekran stan zmiennych współdzielonych
        virtual int size_of() = 0;                              // rozmiar struktury
    };
        
}

#endif

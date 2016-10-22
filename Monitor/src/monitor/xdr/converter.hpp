#ifndef CONVERTER_H
#define CONVERTER_H

#include "shared-variables.hpp"

#include <cstring>

namespace eXDR {
    
    class Filter {
        protected:
            int _buffer_size;
            XDR _stream;
            char *_buffer;
        public:
            Filter(int buffer_size)
                : _buffer_size(buffer_size) 
            {
                this->_buffer = new char[this->_buffer_size];
            }
            
            virtual ~Filter() {
                delete [] this->_buffer;
            }
    };
    
    class Encoder : public Filter {
        public:
            Encoder(int buffer_size) : Filter(buffer_size) { }
            
            // zwraca zserializowaną strukturę vars
            char* serialize(Monitor::SharedVariables *vars) {
                xdrmem_create(&this->_stream, this->_buffer, this->_buffer_size, XDR_ENCODE);
                vars->transform(&this->_stream);
                xdr_destroy(&this->_stream);
                
                return this->_buffer;
            }
    };
    
    class Decoder : public Filter {
        public:
            Decoder(int buffer_size) : Filter(buffer_size) { }
            
            // update_buffer jest parametrem wejściowym, vars wyjściowym
            void deserialize(char *update_buffer, Monitor::SharedVariables *vars) {
                xdrmem_create(&this->_stream, this->_buffer, this->_buffer_size, XDR_DECODE);
                memcpy(this->_buffer, update_buffer, this->_buffer_size);
                vars->transform(&this->_stream);
                xdr_destroy(&this->_stream);
            }
    };
    
    struct Converter {
        unique_ptr<Encoder> _encoder;
        unique_ptr<Decoder> _decoder;
        
        Converter(int buffer_size) : _encoder(new Encoder(buffer_size)), _decoder(new Decoder(buffer_size)) { }
    };

}

#endif

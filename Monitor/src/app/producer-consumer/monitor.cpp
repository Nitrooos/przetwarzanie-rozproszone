#include "monitor.hpp"

#include <iomanip>
#include <sstream>

/*
 * Funkcja wygenerowana dzięki uprzejmości rpcgen'a
 */
bool SharedBuffer::transform(XDR *xdrs) {
    register int32_t *buf;
    int i;

    if (xdrs->x_op == XDR_ENCODE) {
        buf = XDR_INLINE (xdrs, (4 +  BUFFER_SIZE )* BYTES_PER_XDR_UNIT);
        if (buf == NULL) {
             if (!xdr_vector (xdrs, (char *)this->_buffer, BUFFER_SIZE,
                sizeof (int), (xdrproc_t) xdr_int))
                 return false;
             if (!xdr_int (xdrs, &this->_products_count))
                 return false;
             if (!xdr_int (xdrs, &this->_next_product_no))
                 return false;
             if (!xdr_int (xdrs, &this->_index_producer))
                 return false;
             if (!xdr_int (xdrs, &this->_index_consumer))
                 return false;
        } else {
            {
                register int *genp;

                for (i = 0, genp = this->_buffer;
                    i < BUFFER_SIZE; ++i) {
                    IXDR_PUT_LONG(buf, *genp++);
                }
            }
            IXDR_PUT_LONG(buf, this->_products_count);
            IXDR_PUT_LONG(buf, this->_next_product_no);
            IXDR_PUT_LONG(buf, this->_index_producer);
            IXDR_PUT_LONG(buf, this->_index_consumer);
        }
        return true;
    } else if (xdrs->x_op == XDR_DECODE) {
        buf = XDR_INLINE (xdrs, (4 +  BUFFER_SIZE )* BYTES_PER_XDR_UNIT);
        if (buf == NULL) {
             if (!xdr_vector (xdrs, (char *)this->_buffer, BUFFER_SIZE,
                sizeof (int), (xdrproc_t) xdr_int))
                 return false;
             if (!xdr_int (xdrs, &this->_products_count))
                 return false;
             if (!xdr_int (xdrs, &this->_next_product_no))
                 return false;
             if (!xdr_int (xdrs, &this->_index_producer))
                 return false;
             if (!xdr_int (xdrs, &this->_index_consumer))
                 return false;
        } else {
            {
                register int *genp;

                for (i = 0, genp = this->_buffer;
                    i < BUFFER_SIZE; ++i) {
                    *genp++ = IXDR_GET_LONG(buf);
                }
            }
            this->_products_count = IXDR_GET_LONG(buf);
            this->_next_product_no = IXDR_GET_LONG(buf);
            this->_index_producer = IXDR_GET_LONG(buf);
            this->_index_consumer = IXDR_GET_LONG(buf);
        }
     return true;
    }

     if (!xdr_vector (xdrs, (char *)this->_buffer, BUFFER_SIZE,
        sizeof (int), (xdrproc_t) xdr_int))
         return false;
     if (!xdr_int (xdrs, &this->_products_count))
         return false;
     if (!xdr_int (xdrs, &this->_next_product_no))
         return false;
     if (!xdr_int (xdrs, &this->_index_producer))
         return false;
     if (!xdr_int (xdrs, &this->_index_consumer))
         return false;
    return true;
}

/*
 * Ładne wyświetlenie zawartości bufora (do testów)
 */
string SharedBuffer::to_string() {
    ostringstream F;
    F << setw(4) << right << this->_buffer[0];
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        F << "," << setw(4) << right << this->_buffer[i];
    }

    return string("[" + F.str() + "]");
}

/*
 * Zwróc rozmiar struktury
 */
int SharedBuffer::size_of() {
    return sizeof(*this);
}

ProducerConsumerMonitor::ProducerConsumerMonitor() : Base(_shared_buffer = new SharedBuffer()) { }

void ProducerConsumerMonitor::produce() {
    this->lock();
    
    SharedBuffer *s = this->_shared_buffer;
    while (s->_products_count >= BUFFER_SIZE) {                  // jeśli bufor jest pełny
        this->wait(CondVariable::CAN_PRODUCE);                   // czekaj aż będzie można produkować
    }
    
    int produced_no = s->_next_product_no;                       // numer produktu
    s->_buffer[s->_index_producer % BUFFER_SIZE] = produced_no;  // umieść nowy element w buforze
    s->_products_count++;                                        // zwiększ liczbę produktów w buforze
    s->_next_product_no++;                                       // zwiększ numer następnego elementu
    s->_index_producer++;                                        // zwiększ indeks do produkcji elementów    
    
    this->log("Producing: " + std::to_string(produced_no) + " " + this->_shared_buffer->to_string() + "\n");
    
    this->unlock();
    this->pulse(CondVariable::CAN_CONSUME);                     // powiadom czekających o możliwości konsumpcji
}

void ProducerConsumerMonitor::consume() {
    this->lock();
    
    SharedBuffer *s = this->_shared_buffer;
    while (s->_products_count <= 0) {                            // jeśli bufor jest pusty
        this->wait(CondVariable::CAN_CONSUME);                   // czekaj aż będzie można konsumować
    }
    
    int taken_product_no = s->_buffer[s->_index_consumer % BUFFER_SIZE];
    s->_buffer[s->_index_consumer % BUFFER_SIZE] = 0;            // w miejsce elementu pobranego wstaw 0
    s->_products_count--;                                        // zmniejsz liczbę elementów w buforze
    s->_index_consumer++;                                        // zwiększ indeks do pobierania elementów
    
    this->log("Consuming: " + std::to_string(taken_product_no) + " " + this->_shared_buffer->to_string() + "\n");
    
    this->unlock();
    this->pulse(CondVariable::CAN_PRODUCE);                     // powiadom czekających o możliwości produkcji
}

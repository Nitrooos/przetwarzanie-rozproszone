#include "../shared/RPC_FLock.hpp"

bool_t xdr_FILE_LOCK (XDR *xdrs, FILE_LOCK *objp) {
    register int32_t *buf;
    int i;

    if (xdrs->x_op == XDR_ENCODE) {
         if (!xdr_vector (xdrs, (char *)objp->_filename, MAX_NAME_LENGTH, sizeof (char), (xdrproc_t) xdr_char))
             return FALSE;
        buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
        if (buf == NULL) {
             if (!xdr_int (xdrs, &objp->_request_no))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_type))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_start))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_length))
                 return FALSE;
        } else {
            IXDR_PUT_LONG(buf, objp->_request_no);
            IXDR_PUT_LONG(buf, objp->_type);
            IXDR_PUT_LONG(buf, objp->_start);
            IXDR_PUT_LONG(buf, objp->_length);
        }
        return TRUE;
    } else if (xdrs->x_op == XDR_DECODE) {
         if (!xdr_vector (xdrs, (char *)objp->_filename, MAX_NAME_LENGTH, sizeof (char), (xdrproc_t) xdr_char))
             return FALSE;
        buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
        if (buf == NULL) {
             if (!xdr_int (xdrs, &objp->_request_no))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_type))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_start))
                 return FALSE;
             if (!xdr_int (xdrs, &objp->_length))
                 return FALSE;
        } else {
            objp->_request_no = IXDR_GET_LONG(buf);
            objp->_type = IXDR_GET_LONG(buf);
            objp->_start = IXDR_GET_LONG(buf);
            objp->_length = IXDR_GET_LONG(buf);
        }
     return TRUE;
    }

     if (!xdr_vector (xdrs, (char *)objp->_filename, MAX_NAME_LENGTH, sizeof (char), (xdrproc_t) xdr_char))
         return FALSE;
     if (!xdr_int (xdrs, &objp->_request_no))
         return FALSE;
     if (!xdr_int (xdrs, &objp->_type))
         return FALSE;
     if (!xdr_int (xdrs, &objp->_start))
         return FALSE;
     if (!xdr_int (xdrs, &objp->_length))
         return FALSE;
    return TRUE;
}

bool_t xdr_LOCK_RESULT (XDR *xdrs, LOCK_RESULT *objp) {
    register int32_t *buf;

     if (!xdr_int (xdrs, &objp->_status))
         return FALSE;
     if (!xdr_FILE_LOCK (xdrs, &objp->_operation))
         return FALSE;
    return TRUE;
}

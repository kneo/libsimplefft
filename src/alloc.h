#ifndef ALLOC_H
#define ALLOC_H



#include "libsimplefft.h"

//allocating functions

/**allocates a buffer of complex numbers of type where type is one of the CPLX_TYPE_ defines above
*/
CPLX_SAMPLES* lsfft_alloc_complex_buffer(uint32_t samples, uint8_t type);

/**frees a complex number buffer
*/
void lsfft_free_complex_buffer(CPLX_SAMPLES* buffer);

#endif

#ifndef PRNGE_UFDISTR_H
#define PRNGE_UFDISTR_H

#include <float.h>

/* Uniform float distribution
 * Return floats/doubles in range [0, 1)
 * Randint in range [0, 2^w), where w is 32 or 64
 * P.S. use higher 24/53 bits
 */

#if FLT_MANT_DIG == 24
#define prnge_ufdistr_zeroone(randint32) (0x1.p-24f * ((randint32) >> 8))
#else
#error "This code require IEEE-754 32-bit float"
#endif

#if DBL_MANT_DIG == 53
#define prnge_uddistr_zeroone(randint64) (0x1.p-53 * ((randint64) >> 11))
#else
#error "This code require IEEE-754 64-bit float"
#endif

#endif // PRNGE_UFDISTR_H
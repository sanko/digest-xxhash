#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

/* define int64_t and uint64_t when using MinGW compiler */
#ifdef __MINGW32__
#include <stdint.h>
#endif

/* define int64_t and uint64_t when using MS compiler */
#ifdef _MSC_VER
#include <stdlib.h>
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

/* uncomment the following line to use native IVs if they are 64bits long */
/* #define MATH_INT64_NATIVE_IF_AVAILABLE */

#include "perl_math_int64.h"

#include "xxhash.h"

MODULE = Digest::xxHash  PACKAGE = Digest::xxHash 

PROTOTYPES: DISABLE


TYPEMAP: <<HERE
int64_t         T_INT64
uint64_t        T_UINT64

INPUT
T_INT64
        $var = SvI64($arg);

T_UINT64
        $var = SvU64($arg);

OUTPUT
T_INT64
        $arg = newSVi64($var);

T_UINT64
        $arg = newSVu64($var);
HERE


uint64_t
xxhash64_hex(u64, int length(input), UV seed )
    uint64_t u64
CODE:
    //RETVAL = u64 * u64;
    RETVAL = randU64();
OUTPUT:
    RETVAL

U32
xxhash32( const char *input, int length(input), UV seed )
    CODE:
        RETVAL = XXH32(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

uint64_t
xxhash64( const char *input, int length(input), UV seed )
    CODE:
        PERL_MATH_INT64_LOAD;
        RETVAL = XXH64(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

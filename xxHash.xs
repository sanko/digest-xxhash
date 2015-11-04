#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#if(IVSIZE < 8)
#include "perl_math_int64.h"
#else
#define SvI64 SvIV
#define newSVi64 newSViv
#endif

#include "xxhash.h"
 
/* #include "ppport.h" */

MODULE = Digest::xxHash  PACKAGE = Digest::xxHash 

TYPEMAP: <<HERE

unsigned long long         T_UV

HERE

U32
xxhash32( const char *input, int length(input), UV seed )
    CODE:
        RETVAL = XXH32(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

unsigned long long
xxhash64( const char *input, int length(input), UV seed )
    CODE:
        RETVAL = XXH64(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

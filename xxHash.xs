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

#define MATH_INT64_NATIVE_IF_AVAILABLE

#include "perl_math_int64.h"

#define XXH_STATIC_LINKING_ONLY
#include "xxhash.h"

MODULE = Digest::xxHash  PACKAGE = Digest::xxHash

PROTOTYPES: DISABLE

U32
xxhash32( const char *input, int length(input), UV seed )
    CODE:
        RETVAL = XXH32(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

uint64_t
xxhash64( const char *input, int length(input), UV seed )
    CODE:
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH64(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

uint64_t
xxh3_64( const char *input, int length(input), UV seed )
    CODE:
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH3_64bits_withSeed(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

void
xxh3_128( const char *input, int length(input), UV seed )
    PPCODE:
    {
        XXH128_hash_t h = XXH3_128bits_withSeed(input, STRLEN_length_of_input, seed);
        XPUSHs(sv_2mortal(newSVu64(h.low64)));
        XPUSHs(sv_2mortal(newSVu64(h.high64)));
    }

void
xxh3_generate_secret_from_seed( UV seed )
    PPCODE:
    {
        unsigned char secret[XXH3_SECRET_DEFAULT_SIZE];
        XXH3_generateSecret_fromSeed(secret, seed);
        XPUSHs(sv_2mortal(newSVpvn((const char*)secret, XXH3_SECRET_DEFAULT_SIZE)));
    }

IV
_xxxh32_create()
    CODE:
        RETVAL = PTR2IV(XXH32_createState());
    OUTPUT:
        RETVAL

void
_xxxh32_free( IV ctx )
    CODE:
        XXH32_freeState(INT2PTR(XXH32_state_t*, ctx));

void
_xxxh32_copy( IV dst, IV src )
    CODE:
        XXH32_copyState(INT2PTR(XXH32_state_t*, dst), INT2PTR(const XXH32_state_t*, src));

int
_xxxh32_reset( IV ctx, UV seed )
    CODE:
        RETVAL = XXH32_reset(INT2PTR(XXH32_state_t*, ctx), (XXH32_hash_t)seed);
    OUTPUT:
        RETVAL

int
_xxxh32_update( IV ctx, const char *input, int length(input) )
    CODE:
        RETVAL = XXH32_update(INT2PTR(XXH32_state_t*, ctx), input, STRLEN_length_of_input);
    OUTPUT:
        RETVAL

U32
_xxxh32_digest( IV ctx )
    CODE:
        RETVAL = XXH32_digest(INT2PTR(const XXH32_state_t*, ctx));
    OUTPUT:
        RETVAL

IV
_xxxh64_create()
    CODE:
        RETVAL = PTR2IV(XXH64_createState());
    OUTPUT:
        RETVAL

void
_xxxh64_free( IV ctx )
    CODE:
        XXH64_freeState(INT2PTR(XXH64_state_t*, ctx));

void
_xxxh64_copy( IV dst, IV src )
    CODE:
        XXH64_copyState(INT2PTR(XXH64_state_t*, dst), INT2PTR(const XXH64_state_t*, src));

int
_xxxh64_reset( IV ctx, UV seed )
    CODE:
        RETVAL = XXH64_reset(INT2PTR(XXH64_state_t*, ctx), seed);
    OUTPUT:
        RETVAL

int
_xxxh64_update( IV ctx, const char *input, int length(input) )
    CODE:
        RETVAL = XXH64_update(INT2PTR(XXH64_state_t*, ctx), input, STRLEN_length_of_input);
    OUTPUT:
        RETVAL

uint64_t
_xxxh64_digest( IV ctx )
    CODE:
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH64_digest(INT2PTR(const XXH64_state_t*, ctx));
    OUTPUT:
        RETVAL

IV
_xxxh3_create()
    CODE:
        RETVAL = PTR2IV(XXH3_createState());
    OUTPUT:
        RETVAL

void
_xxxh3_free( IV ctx )
    CODE:
        XXH3_freeState(INT2PTR(XXH3_state_t*, ctx));

void
_xxxh3_copy( IV dst, IV src )
    CODE:
        XXH3_copyState(INT2PTR(XXH3_state_t*, dst), INT2PTR(const XXH3_state_t*, src));

int
_xxxh3_64bits_reset( IV ctx )
    CODE:
        RETVAL = XXH3_64bits_reset(INT2PTR(XXH3_state_t*, ctx));
    OUTPUT:
        RETVAL

int
_xxxh3_64bits_reset_withSeed( IV ctx, UV seed )
    CODE:
        RETVAL = XXH3_64bits_reset_withSeed(INT2PTR(XXH3_state_t*, ctx), seed);
    OUTPUT:
        RETVAL

int
_xxxh3_64bits_reset_withSecret( IV ctx, const char *secret, int length(secret) )
    CODE:
        RETVAL = XXH3_64bits_reset_withSecret(INT2PTR(XXH3_state_t*, ctx), secret, STRLEN_length_of_secret);
    OUTPUT:
        RETVAL

int
_xxxh3_64bits_reset_withSecretandSeed( IV ctx, const char *secret, int length(secret), UV seed )
    CODE:
        RETVAL = XXH3_64bits_reset_withSecretandSeed(INT2PTR(XXH3_state_t*, ctx), secret, STRLEN_length_of_secret, seed);
    OUTPUT:
        RETVAL

int
_xxxh3_64bits_update( IV ctx, const char *input, int length(input) )
    CODE:
        RETVAL = XXH3_64bits_update(INT2PTR(XXH3_state_t*, ctx), input, STRLEN_length_of_input);
    OUTPUT:
        RETVAL

uint64_t
_xxxh3_64bits_digest( IV ctx )
    CODE:
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH3_64bits_digest(INT2PTR(const XXH3_state_t*, ctx));
    OUTPUT:
        RETVAL

void
_xxxh3_128bits_reset( IV ctx )
    CODE:
        XXH3_128bits_reset(INT2PTR(XXH3_state_t*, ctx));

void
_xxxh3_128bits_reset_withSeed( IV ctx, UV seed )
    CODE:
        XXH3_128bits_reset_withSeed(INT2PTR(XXH3_state_t*, ctx), seed);

void
_xxxh3_128bits_reset_withSecret( IV ctx, const char *secret, int length(secret) )
    CODE:
        XXH3_128bits_reset_withSecret(INT2PTR(XXH3_state_t*, ctx), secret, STRLEN_length_of_secret);

void
_xxxh3_128bits_reset_withSecretandSeed( IV ctx, const char *secret, int length(secret), UV seed )
    CODE:
        XXH3_128bits_reset_withSecretandSeed(INT2PTR(XXH3_state_t*, ctx), secret, STRLEN_length_of_secret, seed);

int
_xxxh3_128bits_update( IV ctx, const char *input, int length(input) )
    CODE:
        RETVAL = XXH3_128bits_update(INT2PTR(XXH3_state_t*, ctx), input, STRLEN_length_of_input);
    OUTPUT:
        RETVAL

void
_xxxh3_128bits_digest( IV ctx )
    PPCODE:
    {
        XXH128_hash_t h = XXH3_128bits_digest(INT2PTR(const XXH3_state_t*, ctx));
        XPUSHs(sv_2mortal(newSVu64(h.low64)));
        XPUSHs(sv_2mortal(newSVu64(h.high64)));
    }

// Disables the implicit 'pTHX_' context pointer argument, which is good practice for
// modern Perl XS code that uses the 'aTHX_' macro explicitly.
#define PERL_NO_GET_CONTEXT 1
#include <EXTERN.h>
#include <perl.h>
// Disables Perl's internal locking mechanisms for certain structures.
// This is often used when the XS module manages its own thread safety.
#define NO_XSLOCKS
#include <XSUB.h>

#include <inttypes.h>

#ifdef __MINGW32__
#include <stdint.h>
#endif

#ifdef _MSC_VER
#include <stdlib.h>
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

#define MATH_INT64_NATIVE_IF_AVAILABLE

#include "perl_math_int64.h"

#define XXH_STATIC_LINKING_ONLY
#include "xxhash.h"

static const char hex_chars[16] = "0123456789abcdef";

#if defined(__GNUC__)
#define FORCE_INLINE static inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define FORCE_INLINE static inline __forceinline
#else
#define FORCE_INLINE static inline
#endif

FORCE_INLINE void write_hex32(char * buf, uint32_t val) {
    buf[0] = hex_chars[(val >> 28) & 0xf];
    buf[1] = hex_chars[(val >> 24) & 0xf];
    buf[2] = hex_chars[(val >> 20) & 0xf];
    buf[3] = hex_chars[(val >> 16) & 0xf];
    buf[4] = hex_chars[(val >> 12) & 0xf];
    buf[5] = hex_chars[(val >> 8) & 0xf];
    buf[6] = hex_chars[(val >> 4) & 0xf];
    buf[7] = hex_chars[(val) & 0xf];
}

FORCE_INLINE void write_hex64(char * buf, uint64_t val) {
    buf[0] = hex_chars[(val >> 60) & 0xf];
    buf[1] = hex_chars[(val >> 56) & 0xf];
    buf[2] = hex_chars[(val >> 52) & 0xf];
    buf[3] = hex_chars[(val >> 48) & 0xf];
    buf[4] = hex_chars[(val >> 44) & 0xf];
    buf[5] = hex_chars[(val >> 40) & 0xf];
    buf[6] = hex_chars[(val >> 36) & 0xf];
    buf[7] = hex_chars[(val >> 32) & 0xf];
    buf[8] = hex_chars[(val >> 28) & 0xf];
    buf[9] = hex_chars[(val >> 24) & 0xf];
    buf[10] = hex_chars[(val >> 20) & 0xf];
    buf[11] = hex_chars[(val >> 16) & 0xf];
    buf[12] = hex_chars[(val >> 12) & 0xf];
    buf[13] = hex_chars[(val >> 8) & 0xf];
    buf[14] = hex_chars[(val >> 4) & 0xf];
    buf[15] = hex_chars[(val) & 0xf];
}

#ifndef PERL_UNUSED_VAR
#define PERL_UNUSED_VAR(var) \
    if (0)                   \
    var = var
#endif

#ifndef dVAR
#define dVAR dNOOP
#endif

#undef XS_INTERNAL
#if defined(PERL_EUPXS_ALWAYS_EXPORT)
#define XS_INTERNAL(name) XS_EXTERNAL(name)
#else
#define XS_INTERNAL(name) XS(name)
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE \
    assert(cv);                         \
    assert(params)

STATIC void S_croak_xs_usage(const CV * const cv, const char * const params);

STATIC void S_croak_xs_usage(const CV * const cv, const char * const params) {
    const GV * const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char * const gvname = GvNAME(gv);
        const HV * const stash = GvSTASH(gv);
        const char * const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak_nocontext("Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak_nocontext("Usage: %s(%s)", gvname, params);
    }
    else {
        Perl_croak_nocontext("Usage: CODE(0x%" UVxf ")(%s)", PTR2UV(cv), params);
    }
}
#undef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define croak_xs_usage S_croak_xs_usage

#endif

#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file) newXS_flags(name, c_impl, file, NULL, 0)
#else
#define newXSproto_portable(name, c_impl, file) (PL_Sv = (SV *)newXS(name, c_impl, file), (CV *)PL_Sv)
#endif

XS_INTERNAL(Digest_xxHash_xxhash32) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        UV RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        RETVAL = (UV)XXH32(input, STRLEN_length_of_input, seed);
        TARGu((UV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxhash64) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        UV RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        RETVAL = (UV)XXH64(input, STRLEN_length_of_input, seed);
        TARGu((UV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxh3_64) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        UV RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        RETVAL = (UV)XXH3_64bits_withSeed(input, STRLEN_length_of_input, seed);
        TARGu((UV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxhash32_hex) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        char * RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        static char hexbuf[9];
        write_hex32(hexbuf, (uint32_t)XXH32(input, STRLEN_length_of_input, seed));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxhash64_hex) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        char * RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        static char hexbuf[17];
        write_hex64(hexbuf, (uint64_t)XXH64(input, STRLEN_length_of_input, seed));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxh3_64_hex) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        char * RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        static char hexbuf[17];
        write_hex64(hexbuf, (uint64_t)XXH3_64bits_withSeed(input, STRLEN_length_of_input, seed));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxh3_128) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        {
            XXH128_hash_t h = XXH3_128bits_withSeed(input, STRLEN_length_of_input, seed);
            XPUSHs(sv_2mortal(newSVu64(h.low64)));
            XPUSHs(sv_2mortal(newSVu64(h.high64)));
        }
        PUTBACK;
        return;
    }
}

XS_INTERNAL(Digest_xxHash_xxh3_128_hex) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "input, seed");
    {
        char * RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        const char * input = (const char *)SvPV(ST(0), STRLEN_length_of_input);
        UV seed = (UV)SvUV(ST(1));

        static char hexbuf[33];
        XXH128_hash_t h = XXH3_128bits_withSeed(input, STRLEN_length_of_input, seed);
        write_hex64(hexbuf, (uint64_t)h.high64);
        write_hex64(hexbuf + 16, (uint64_t)h.low64);
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxh3_generate_secret_from_seed) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "seed");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
        UV seed = (UV)SvUV(ST(0));
        {
            unsigned char secret[XXH3_SECRET_DEFAULT_SIZE];
            XXH3_generateSecret_fromSeed(secret, seed);
            XPUSHs(sv_2mortal(newSVpvn((const char *)secret, XXH3_SECRET_DEFAULT_SIZE)));
        }
        PUTBACK;
        return;
    }
}

XS_INTERNAL(Digest_xxHash_xxxh32_create) {
    dXSARGS;
    if (items != 0)
        croak_xs_usage(cv, "");
    {
        IV RETVAL;
        dXSTARG;
        RETVAL = PTR2IV(XXH32_createState());
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh32_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH32_freeState(INT2PTR(XXH32_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh32_copy) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "dst, src");
    {
        IV dst = (IV)SvIV(ST(0));
        IV src = (IV)SvIV(ST(1));
        XXH32_copyState(INT2PTR(XXH32_state_t *, dst), INT2PTR(const XXH32_state_t *, src));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh32_reset) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, seed");
    {
        int RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        UV seed = (UV)SvUV(ST(1));
        RETVAL = XXH32_reset(INT2PTR(XXH32_state_t *, ctx), (XXH32_hash_t)seed);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh32_update) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, input");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        IV ctx = (IV)SvIV(ST(0));
        const char * input = (const char *)SvPV(ST(1), STRLEN_length_of_input);

        RETVAL = XXH32_update(INT2PTR(XXH32_state_t *, ctx), input, STRLEN_length_of_input);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh32_digest) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        U32 RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        RETVAL = XXH32_digest(INT2PTR(const XXH32_state_t *, ctx));
        TARGu((UV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh64_create) {
    dXSARGS;
    if (items != 0)
        croak_xs_usage(cv, "");
    {
        IV RETVAL;
        dXSTARG;
        RETVAL = PTR2IV(XXH64_createState());
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh64_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH64_freeState(INT2PTR(XXH64_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh64_copy) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "dst, src");
    {
        IV dst = (IV)SvIV(ST(0));
        IV src = (IV)SvIV(ST(1));
        XXH64_copyState(INT2PTR(XXH64_state_t *, dst), INT2PTR(const XXH64_state_t *, src));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh64_reset) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, seed");
    {
        int RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        UV seed = (UV)SvUV(ST(1));
        RETVAL = XXH64_reset(INT2PTR(XXH64_state_t *, ctx), seed);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh64_update) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, input");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        IV ctx = (IV)SvIV(ST(0));
        const char * input = (const char *)SvPV(ST(1), STRLEN_length_of_input);

        RETVAL = XXH64_update(INT2PTR(XXH64_state_t *, ctx), input, STRLEN_length_of_input);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh64_digest) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        uint64_t RETVAL;
        IV ctx = (IV)SvIV(ST(0));
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH64_digest(INT2PTR(const XXH64_state_t *, ctx));
        {
            SV * RETVALSV;
            RETVALSV = newSVu64(RETVAL);
            RETVALSV = sv_2mortal(RETVALSV);
            ST(0) = RETVALSV;
        }
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_create) {
    dXSARGS;
    if (items != 0)
        croak_xs_usage(cv, "");
    {
        IV RETVAL;
        dXSTARG;
        RETVAL = PTR2IV(XXH3_createState());
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH3_freeState(INT2PTR(XXH3_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_copy) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "dst, src");
    {
        IV dst = (IV)SvIV(ST(0));
        IV src = (IV)SvIV(ST(1));
        XXH3_copyState(INT2PTR(XXH3_state_t *, dst), INT2PTR(const XXH3_state_t *, src));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_reset) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        int RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        RETVAL = XXH3_64bits_reset(INT2PTR(XXH3_state_t *, ctx));
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_reset_withSeed) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, seed");
    {
        int RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        UV seed = (UV)SvUV(ST(1));
        RETVAL = XXH3_64bits_reset_withSeed(INT2PTR(XXH3_state_t *, ctx), seed);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_reset_withSecret) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, secret");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_secret;
        IV ctx = (IV)SvIV(ST(0));
        const char * secret = (const char *)SvPV(ST(1), STRLEN_length_of_secret);

        RETVAL = XXH3_64bits_reset_withSecret(INT2PTR(XXH3_state_t *, ctx), secret, STRLEN_length_of_secret);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_reset_withSecretandSeed) {
    dXSARGS;
    if (items != 3)
        croak_xs_usage(cv, "ctx, secret, seed");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_secret;
        IV ctx = (IV)SvIV(ST(0));
        const char * secret = (const char *)SvPV(ST(1), STRLEN_length_of_secret);
        UV seed = (UV)SvUV(ST(2));

        RETVAL =
            XXH3_64bits_reset_withSecretandSeed(INT2PTR(XXH3_state_t *, ctx), secret, STRLEN_length_of_secret, seed);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_update) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, input");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        IV ctx = (IV)SvIV(ST(0));
        const char * input = (const char *)SvPV(ST(1), STRLEN_length_of_input);

        RETVAL = XXH3_64bits_update(INT2PTR(XXH3_state_t *, ctx), input, STRLEN_length_of_input);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_digest) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        uint64_t RETVAL;
        IV ctx = (IV)SvIV(ST(0));
#if !MATH_INT64_NATIVE
        PERL_MATH_INT64_LOAD;
#endif
        RETVAL = XXH3_64bits_digest(INT2PTR(const XXH3_state_t *, ctx));
        {
            SV * RETVALSV;
            RETVALSV = newSVu64(RETVAL);
            RETVALSV = sv_2mortal(RETVALSV);
            ST(0) = RETVALSV;
        }
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_reset) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH3_128bits_reset(INT2PTR(XXH3_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_reset_withSeed) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, seed");
    {
        IV ctx = (IV)SvIV(ST(0));
        UV seed = (UV)SvUV(ST(1));
        XXH3_128bits_reset_withSeed(INT2PTR(XXH3_state_t *, ctx), seed);
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_reset_withSecret) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, secret");
    {
        STRLEN STRLEN_length_of_secret;
        IV ctx = (IV)SvIV(ST(0));
        const char * secret = (const char *)SvPV(ST(1), STRLEN_length_of_secret);

        XXH3_128bits_reset_withSecret(INT2PTR(XXH3_state_t *, ctx), secret, STRLEN_length_of_secret);
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_reset_withSecretandSeed) {
    dXSARGS;
    if (items != 3)
        croak_xs_usage(cv, "ctx, secret, seed");
    {
        STRLEN STRLEN_length_of_secret;
        IV ctx = (IV)SvIV(ST(0));
        const char * secret = (const char *)SvPV(ST(1), STRLEN_length_of_secret);
        UV seed = (UV)SvUV(ST(2));

        XXH3_128bits_reset_withSecretandSeed(INT2PTR(XXH3_state_t *, ctx), secret, STRLEN_length_of_secret, seed);
    }
    XSRETURN_EMPTY;
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_update) {
    dXSARGS;
    if (items != 2)
        croak_xs_usage(cv, "ctx, input");
    {
        int RETVAL;
        dXSTARG;
        STRLEN STRLEN_length_of_input;
        IV ctx = (IV)SvIV(ST(0));
        const char * input = (const char *)SvPV(ST(1), STRLEN_length_of_input);

        RETVAL = XXH3_128bits_update(INT2PTR(XXH3_state_t *, ctx), input, STRLEN_length_of_input);
        TARGi((IV)RETVAL, 1);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_digest) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
        IV ctx = (IV)SvIV(ST(0));
        {
            XXH128_hash_t h = XXH3_128bits_digest(INT2PTR(const XXH3_state_t *, ctx));
            XPUSHs(sv_2mortal(newSVu64(h.low64)));
            XPUSHs(sv_2mortal(newSVu64(h.high64)));
        }
        PUTBACK;
        return;
    }
}

XS_INTERNAL(Digest_xxHash_xxxh32_hex) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        char * RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        static char hexbuf[9];
        write_hex32(hexbuf, (uint32_t)XXH32_digest(INT2PTR(const XXH32_state_t *, ctx)));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh64_hex) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        char * RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        static char hexbuf[17];
        write_hex64(hexbuf, (uint64_t)XXH64_digest(INT2PTR(const XXH64_state_t *, ctx)));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_64bits_hex) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        char * RETVAL;
        dXSTARG;
        IV ctx = (IV)SvIV(ST(0));
        static char hexbuf[17];
        write_hex64(hexbuf, (uint64_t)XXH3_64bits_digest(INT2PTR(const XXH3_state_t *, ctx)));
        RETVAL = hexbuf;
        sv_setpv((SV *)TARG, RETVAL);
        ST(0) = TARG;
    }
    XSRETURN(1);
}

XS_INTERNAL(Digest_xxHash_xxxh3_128bits_hex) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
        IV ctx = (IV)SvIV(ST(0));
        {
            static char hexbuf[33];
            XXH128_hash_t h = XXH3_128bits_digest(INT2PTR(const XXH3_state_t *, ctx));
            write_hex64(hexbuf, (uint64_t)h.high64);
            write_hex64(hexbuf + 16, (uint64_t)h.low64);
            XPUSHs(sv_2mortal(newSVpvn(hexbuf, 32)));
        }
        PUTBACK;
        return;
    }
}

void boot_Digest__xxHash(pTHX_ CV * cv) {
    dVAR;
    dXSBOOTARGSXSAPIVERCHK;

    PERL_UNUSED_VAR(cv);    /* -W */
    PERL_UNUSED_VAR(items); /* -W */

    (void)newXSproto_portable("Digest::xxHash::xxhash32", Digest_xxHash_xxhash32, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxhash64", Digest_xxHash_xxhash64, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxh3_64", Digest_xxHash_xxh3_64, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxhash32_hex", Digest_xxHash_xxhash32_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxhash64_hex", Digest_xxHash_xxhash64_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxh3_64_hex", Digest_xxHash_xxh3_64_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxh3_128", Digest_xxHash_xxh3_128, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::xxh3_128_hex", Digest_xxHash_xxh3_128_hex, __FILE__);
    (void)newXSproto_portable(
        "Digest::xxHash::xxh3_generate_secret_from_seed", Digest_xxHash_xxh3_generate_secret_from_seed, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_create", Digest_xxHash_xxxh32_create, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_free", Digest_xxHash_xxxh32_free, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_copy", Digest_xxHash_xxxh32_copy, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_reset", Digest_xxHash_xxxh32_reset, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_update", Digest_xxHash_xxxh32_update, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_digest", Digest_xxHash_xxxh32_digest, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_create", Digest_xxHash_xxxh64_create, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_free", Digest_xxHash_xxxh64_free, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_copy", Digest_xxHash_xxxh64_copy, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_reset", Digest_xxHash_xxxh64_reset, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_update", Digest_xxHash_xxxh64_update, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_digest", Digest_xxHash_xxxh64_digest, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_create", Digest_xxHash_xxxh3_create, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_free", Digest_xxHash_xxxh3_free, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_copy", Digest_xxHash_xxxh3_copy, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_64bits_reset", Digest_xxHash_xxxh3_64bits_reset, __FILE__);
    (void)newXSproto_portable(
        "Digest::xxHash::_xxxh3_64bits_reset_withSeed", Digest_xxHash_xxxh3_64bits_reset_withSeed, __FILE__);
    (void)newXSproto_portable(
        "Digest::xxHash::_xxxh3_64bits_reset_withSecret", Digest_xxHash_xxxh3_64bits_reset_withSecret, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_64bits_reset_withSecretandSeed",
                              Digest_xxHash_xxxh3_64bits_reset_withSecretandSeed,
                              __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_64bits_update", Digest_xxHash_xxxh3_64bits_update, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_64bits_digest", Digest_xxHash_xxxh3_64bits_digest, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_128bits_reset", Digest_xxHash_xxxh3_128bits_reset, __FILE__);
    (void)newXSproto_portable(
        "Digest::xxHash::_xxxh3_128bits_reset_withSeed", Digest_xxHash_xxxh3_128bits_reset_withSeed, __FILE__);
    (void)newXSproto_portable(
        "Digest::xxHash::_xxxh3_128bits_reset_withSecret", Digest_xxHash_xxxh3_128bits_reset_withSecret, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_128bits_reset_withSecretandSeed",
                              Digest_xxHash_xxxh3_128bits_reset_withSecretandSeed,
                              __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_128bits_update", Digest_xxHash_xxxh3_128bits_update, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_128bits_digest", Digest_xxHash_xxxh3_128bits_digest, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh32_hex", Digest_xxHash_xxxh32_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh64_hex", Digest_xxHash_xxxh64_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_64bits_hex", Digest_xxHash_xxxh3_64bits_hex, __FILE__);
    (void)newXSproto_portable("Digest::xxHash::_xxxh3_128bits_hex", Digest_xxHash_xxxh3_128bits_hex, __FILE__);
    Perl_xs_boot_epilog(aTHX_ ax);
}

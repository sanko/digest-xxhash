/*
 * Generated from xxHash.xs via xsubpp, then hand-optimized.
 * - Removed XSauto_length dead code
 * - Removed dVAR (no-op since 5.10)
 * - Removed #line debug directives
 * - Force-inlined hex helpers with unrolled loops
 */

/*
 * Pre-generated from xxHash.xs via xsubpp, then hand-optimized.
 * - Removed XSauto_length dead code
 * - Removed dVAR (no-op since 5.10)
 * - Force-inlined hex helpers with unrolled loops
 * - Eliminated sprintf for fixed-width hex output
 * - Removed ppport.h (not needed without .xs)
 */

#define PERL_NO_GET_CONTEXT 1
#include <EXTERN.h>
#include <perl.h>
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

/* ---- Fast hex formatting (no sprintf) ---- */

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

/* ---- XS glue ---- */

#ifndef PERL_UNUSED_VAR
#define PERL_UNUSED_VAR(var) \
    if (0)                   \
    var = var
#endif

#ifndef dVAR
#define dVAR dNOOP
#endif

/* This stuff is not part of the API! You have been warned. */
#ifndef PERL_VERSION_DECIMAL
#define PERL_VERSION_DECIMAL(r, v, s) (r * 1000000 + v * 1000 + s)
#endif
#ifndef PERL_DECIMAL_VERSION
#define PERL_DECIMAL_VERSION PERL_VERSION_DECIMAL(PERL_REVISION, PERL_VERSION, PERL_SUBVERSION)
#endif
#ifndef PERL_VERSION_GE
#define PERL_VERSION_GE(r, v, s) (PERL_DECIMAL_VERSION >= PERL_VERSION_DECIMAL(r, v, s))
#endif
#ifndef PERL_VERSION_LE
#define PERL_VERSION_LE(r, v, s) (PERL_DECIMAL_VERSION <= PERL_VERSION_DECIMAL(r, v, s))
#endif

/* XS_INTERNAL is the explicit static-linkage variant of the default
 * XS macro.
 *
 * XS_EXTERNAL is the same as XS_INTERNAL except it does not include
 * "STATIC", ie. it exports XSUB symbols. You probably don't want that
 * for anything but the BOOT XSUB.
 *
 * See XSUB.h in core!
 */

/* TODO: This might be compatible further back than 5.10.0. */
#if PERL_VERSION_GE(5, 10, 0) && PERL_VERSION_LE(5, 15, 1)
#undef XS_EXTERNAL
#undef XS_INTERNAL
#if defined(__CYGWIN__) && defined(USE_DYNAMIC_LOADING)
#define XS_EXTERNAL(name) __declspec(dllexport) XSPROTO(name)
#define XS_INTERNAL(name) STATIC XSPROTO(name)
#endif
#if defined(__SYMBIAN32__)
#define XS_EXTERNAL(name) EXPORT_C XSPROTO(name)
#define XS_INTERNAL(name) EXPORT_C STATIC XSPROTO(name)
#endif
#ifndef XS_EXTERNAL
#if defined(HASATTRIBUTE_UNUSED) && !defined(__cplusplus)
#define XS_EXTERNAL(name) void name(pTHX_ CV * cv __attribute__unused__)
#define XS_INTERNAL(name) STATIC void name(pTHX_ CV * cv __attribute__unused__)
#else
#ifdef __cplusplus
#define XS_EXTERNAL(name) extern "C" XSPROTO(name)
#define XS_INTERNAL(name) static XSPROTO(name)
#else
#define XS_EXTERNAL(name) XSPROTO(name)
#define XS_INTERNAL(name) STATIC XSPROTO(name)
#endif
#endif
#endif
#endif

/* perl >= 5.10.0 && perl <= 5.15.1 */

/* The XS_EXTERNAL macro is used for functions that must not be static
 * like the boot XSUB of a module. If perl didn't have an XS_EXTERNAL
 * macro defined, the best we can do is assume XS is the same.
 * Dito for XS_INTERNAL.
 */
#ifndef XS_EXTERNAL
#define XS_EXTERNAL(name) XS(name)
#endif
#ifndef XS_INTERNAL
#define XS_INTERNAL(name) XS(name)
#endif

/* Now, finally, after all this mess, we want an ExtUtils::ParseXS
 * internal macro that we're free to redefine for varying linkage due
 * to the EXPORT_XSUB_SYMBOLS XS keyword. This is internal, use
 * XS_EXTERNAL(name) or XS_INTERNAL(name) in your code if you need to!
 */

#undef XS_EUPXS
#if defined(PERL_EUPXS_ALWAYS_EXPORT)
#define XS_EUPXS(name) XS_EXTERNAL(name)
#else
/* default to internal */
#define XS_EUPXS(name) XS_INTERNAL(name)
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE \
    assert(cv);                         \
    assert(params)

/* prototype to pass -Wmissing-prototypes */
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
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak_nocontext("Usage: CODE(0x%" UVxf ")(%s)", PTR2UV(cv), params);
    }
}
#undef PERL_ARGS_ASSERT_CROAK_XS_USAGE

#define croak_xs_usage S_croak_xs_usage

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) \
    (PL_Sv = (SV *)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV *)PL_Sv)
#endif /* !defined(newXS_flags) */

#if PERL_VERSION_LE(5, 21, 5)
#define newXS_deffile(a, b) Perl_newXS(aTHX_ a, b, file)
#else
#define newXS_deffile(a, b) Perl_newXS_deffile(aTHX_ a, b)
#endif

/* simple backcompat versions of the TARGx() macros with no optimisation */
#ifndef TARGi
#define TARGi(iv, do_taint) sv_setiv_mg(TARG, iv)
#define TARGu(uv, do_taint) sv_setuv_mg(TARG, uv)
#define TARGn(nv, do_taint) sv_setnv_mg(TARG, nv)
#endif

XS_EUPXS(XS_Digest__xxHash_xxhash32); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxhash32) {
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

XS_EUPXS(XS_Digest__xxHash_xxhash64); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxhash64) {
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

XS_EUPXS(XS_Digest__xxHash_xxh3_64); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxh3_64) {
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

XS_EUPXS(XS_Digest__xxHash_xxhash32_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxhash32_hex) {
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

XS_EUPXS(XS_Digest__xxHash_xxhash64_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxhash64_hex) {
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

XS_EUPXS(XS_Digest__xxHash_xxh3_64_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxh3_64_hex) {
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

XS_EUPXS(XS_Digest__xxHash_xxh3_128); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxh3_128) {
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

XS_EUPXS(XS_Digest__xxHash_xxh3_128_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxh3_128_hex) {
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

XS_EUPXS(XS_Digest__xxHash_xxh3_generate_secret_from_seed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash_xxh3_generate_secret_from_seed) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_create); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_create) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_free); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH32_freeState(INT2PTR(XXH32_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_EUPXS(XS_Digest__xxHash__xxxh32_copy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_copy) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_reset); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_reset) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_update); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_update) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_digest); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_digest) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_create); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_create) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_free); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH64_freeState(INT2PTR(XXH64_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_EUPXS(XS_Digest__xxHash__xxxh64_copy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_copy) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_reset); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_reset) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_update); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_update) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_digest); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_digest) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_create); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_create) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_free); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_free) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH3_freeState(INT2PTR(XXH3_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_EUPXS(XS_Digest__xxHash__xxxh3_copy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_copy) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSeed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSeed) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSecret); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSecret) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSecretandSeed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_reset_withSecretandSeed) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_update); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_update) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_digest); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_digest) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset) {
    dXSARGS;
    if (items != 1)
        croak_xs_usage(cv, "ctx");
    {
        IV ctx = (IV)SvIV(ST(0));
        XXH3_128bits_reset(INT2PTR(XXH3_state_t *, ctx));
    }
    XSRETURN_EMPTY;
}

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSeed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSeed) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSecret); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSecret) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSecretandSeed); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_reset_withSecretandSeed) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_update); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_update) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_digest); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_digest) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh32_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh32_hex) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh64_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh64_hex) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_64bits_hex) {
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

XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_hex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Digest__xxHash__xxxh3_128bits_hex) {
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

#ifdef __cplusplus
extern "C" {
#endif
XS_EXTERNAL(boot_Digest__xxHash); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Digest__xxHash) {
#if PERL_VERSION_LE(5, 21, 5)
    dXSARGS;
#else
    dVAR;
    dXSBOOTARGSXSAPIVERCHK;
#endif
#if PERL_VERSION_LE(5, 8, 999) /* PERL_VERSION_LT is 5.33+ */
    char * file = __FILE__;
#else
    const char * file = __FILE__;
#endif

    PERL_UNUSED_VAR(file);

    PERL_UNUSED_VAR(cv);    /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#if PERL_VERSION_LE(5, 21, 5)
    XS_VERSION_BOOTCHECK;
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
#endif

    newXS_deffile("Digest::xxHash::xxhash32", XS_Digest__xxHash_xxhash32);
    newXS_deffile("Digest::xxHash::xxhash64", XS_Digest__xxHash_xxhash64);
    newXS_deffile("Digest::xxHash::xxh3_64", XS_Digest__xxHash_xxh3_64);
    newXS_deffile("Digest::xxHash::xxhash32_hex", XS_Digest__xxHash_xxhash32_hex);
    newXS_deffile("Digest::xxHash::xxhash64_hex", XS_Digest__xxHash_xxhash64_hex);
    newXS_deffile("Digest::xxHash::xxh3_64_hex", XS_Digest__xxHash_xxh3_64_hex);
    newXS_deffile("Digest::xxHash::xxh3_128", XS_Digest__xxHash_xxh3_128);
    newXS_deffile("Digest::xxHash::xxh3_128_hex", XS_Digest__xxHash_xxh3_128_hex);
    newXS_deffile("Digest::xxHash::xxh3_generate_secret_from_seed", XS_Digest__xxHash_xxh3_generate_secret_from_seed);
    newXS_deffile("Digest::xxHash::_xxxh32_create", XS_Digest__xxHash__xxxh32_create);
    newXS_deffile("Digest::xxHash::_xxxh32_free", XS_Digest__xxHash__xxxh32_free);
    newXS_deffile("Digest::xxHash::_xxxh32_copy", XS_Digest__xxHash__xxxh32_copy);
    newXS_deffile("Digest::xxHash::_xxxh32_reset", XS_Digest__xxHash__xxxh32_reset);
    newXS_deffile("Digest::xxHash::_xxxh32_update", XS_Digest__xxHash__xxxh32_update);
    newXS_deffile("Digest::xxHash::_xxxh32_digest", XS_Digest__xxHash__xxxh32_digest);
    newXS_deffile("Digest::xxHash::_xxxh64_create", XS_Digest__xxHash__xxxh64_create);
    newXS_deffile("Digest::xxHash::_xxxh64_free", XS_Digest__xxHash__xxxh64_free);
    newXS_deffile("Digest::xxHash::_xxxh64_copy", XS_Digest__xxHash__xxxh64_copy);
    newXS_deffile("Digest::xxHash::_xxxh64_reset", XS_Digest__xxHash__xxxh64_reset);
    newXS_deffile("Digest::xxHash::_xxxh64_update", XS_Digest__xxHash__xxxh64_update);
    newXS_deffile("Digest::xxHash::_xxxh64_digest", XS_Digest__xxHash__xxxh64_digest);
    newXS_deffile("Digest::xxHash::_xxxh3_create", XS_Digest__xxHash__xxxh3_create);
    newXS_deffile("Digest::xxHash::_xxxh3_free", XS_Digest__xxHash__xxxh3_free);
    newXS_deffile("Digest::xxHash::_xxxh3_copy", XS_Digest__xxHash__xxxh3_copy);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_reset", XS_Digest__xxHash__xxxh3_64bits_reset);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_reset_withSeed", XS_Digest__xxHash__xxxh3_64bits_reset_withSeed);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_reset_withSecret", XS_Digest__xxHash__xxxh3_64bits_reset_withSecret);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_reset_withSecretandSeed",
                  XS_Digest__xxHash__xxxh3_64bits_reset_withSecretandSeed);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_update", XS_Digest__xxHash__xxxh3_64bits_update);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_digest", XS_Digest__xxHash__xxxh3_64bits_digest);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_reset", XS_Digest__xxHash__xxxh3_128bits_reset);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_reset_withSeed", XS_Digest__xxHash__xxxh3_128bits_reset_withSeed);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_reset_withSecret", XS_Digest__xxHash__xxxh3_128bits_reset_withSecret);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_reset_withSecretandSeed",
                  XS_Digest__xxHash__xxxh3_128bits_reset_withSecretandSeed);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_update", XS_Digest__xxHash__xxxh3_128bits_update);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_digest", XS_Digest__xxHash__xxxh3_128bits_digest);
    newXS_deffile("Digest::xxHash::_xxxh32_hex", XS_Digest__xxHash__xxxh32_hex);
    newXS_deffile("Digest::xxHash::_xxxh64_hex", XS_Digest__xxHash__xxxh64_hex);
    newXS_deffile("Digest::xxHash::_xxxh3_64bits_hex", XS_Digest__xxHash__xxxh3_64bits_hex);
    newXS_deffile("Digest::xxHash::_xxxh3_128bits_hex", XS_Digest__xxHash__xxxh3_128bits_hex);
#if PERL_VERSION_LE(5, 21, 5)
#if PERL_VERSION_GE(5, 9, 0)
    if (PL_unitcheckav)
        call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
#else
    Perl_xs_boot_epilog(aTHX_ ax);
#endif
}

#ifdef __cplusplus
}
#endif

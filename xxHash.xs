#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#else
#define SvI64 SvIV
#define newSVi64 newSViv
#endif

#include "xxhash.h"
 
/* #include "ppport.h" */

MODULE = Digest::xxHash  PACKAGE = Digest::xxHash 

TYPEMAP: <<HERE

U64           T_UV

unsigned long long         T_UV

HERE

 
U32
xxhash( const char *input, int length(input), UV seed )
    CODE:
        RETVAL = XXH32(input, STRLEN_length_of_input, seed);
    OUTPUT:
        RETVAL

void *
xxHash::new( UV seed )
    PPCODE:
        RETVAL = XXH32_init(seed);
        ST(0) = sv_newmortal();
        sv_setref_pv(ST(0), CLASS, (void*)RETVAL);
        XSRETURN(1);

void
add( self, const char *input, int length(input) )
    PREINIT:
        void * state;
    PPCODE:
        if (SvROK(ST(0)) && sv_derived_from(ST(0), "Digest::xxHash")) {
            IV tmp = SvIV((SV*)SvRV(ST(0)));
            state = INT2PTR(void *,tmp);
        }
        else
            Perl_croak(aTHX_ "%s: %s is not of type %s", "Digest::xxHash::add", "self", "Digest::xxHash");
        XXH32_update(state, input, STRLEN_length_of_input );

U32
digest( self, ... )
    PREINIT:
        void * state;
    PPCODE:
        if (SvROK(ST(0)) && sv_derived_from(ST(0), "Digest::xxHash")) {
            IV tmp = SvIV((SV*)SvRV(ST(0)));
            state = INT2PTR(void *,tmp);
        }
        else
            Perl_croak(aTHX_ "%s: %s is not of type %s", "Digest::xxHash::add", "self", "Digest::xxHash");
         RETVAL = XXH32_intermediateDigest(state);
         XSprePUSH; PUSHu((UV)RETVAL);
         XSRETURN(1);

void
reset( self )
    PREINIT:
        void * state;
    PPCODE:
        if (SvROK(ST(0)) && sv_derived_from(ST(0), "Digest::xxHash")) {
            IV tmp = SvIV((SV*)SvRV(ST(0)));
            state = INT2PTR(void *,tmp);
        }
        else
            Perl_croak(aTHX_ "%s: %s is not of type %s", "Digest::xxHash::add", "self", "Digest::xxHash");
        struct XXH_state32_t * _state = (struct XXH_state32_t *) state;
        XXH32_resetState(state, _state->seed);
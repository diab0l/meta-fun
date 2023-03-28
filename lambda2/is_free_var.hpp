#ifndef __LAMBDA2_IS_FREE_VAR_HPP__
#define __LAMBDA2_IS_FREE_VAR_HPP__

#include "types.hpp"

namespace lambda2 {
    /**
     * Is the variable `var_x` element of the free variables of lambda term `M`
     * @param var_x The candidate variable
     * @param M The target term
     */
    template<typename var_x, typename M, typename Bool>
    struct is_free_var {
        using type = typename Bool::False;
    };

    // The free variables of `x` are just `x`
    template<typename x, typename Bool>
    struct is_free_var<var<x>, var<x>, Bool> {
        using type = typename Bool::True;
    };

    // The set of free variables of `λx.t` is the set of free variables of `t`, but with `x` removed
    // This pattern removes `x`
    template<typename x, typename t, typename Bool>
    struct is_free_var<var<x>, abs<var<x>, t>, Bool> {
        using type = typename Bool::False;
    };

    // The set of free variables of `λx.t` is the set of free variables of `t`, but with `x` removed
    // This pattern recurses into `t`
    template<typename x, typename y, typename t, typename Bool>
    struct is_free_var<var<x>, abs<var<y>, t>, Bool> {
        private:
        using var_x = var<x>;
        
        public:
        using type = typename is_free_var<var_x, t, Bool>::type;
    };

    // The set of free variables of `t s` is the union of the set of free variables of `t` and the set of free variables of `s`.
    template<typename x, typename t, typename s, typename Bool>
    struct is_free_var<var<x>, app<t, s>, Bool> {
        private:
        using var_x = var<x>;
        using is_free_in_t = typename is_free_var<var_x, t, Bool>::type;
        using is_free_in_s = typename is_free_var<var_x, s, Bool>::type;
        
        template<typename x1, typename x2>
        struct bool_or {
            using type = typename Bool::False;
        };
        
        template<typename x1>
        struct bool_or<x1, typename Bool::True> {
            using type = typename Bool::True;
        };

        template<typename x2>
        struct bool_or<typename Bool::True, x2> {
            using type = typename Bool::True;
        };

        public:
        using type = typename bool_or<is_free_in_s, is_free_in_t>::type;
    };
}

#endif
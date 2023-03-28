#ifndef __LAMBDA2_SUBST_HPP__
#define __LAMBDA2_SUBST_HPP__

#include "types.hpp"

namespace lambda2 {
    /**
     * Replaces all free occurrences of the variable V in the expression M with expression N
     * M[V := N]
     * @param M The expression in which to replace
     * @param var_x The variable to be replaced
     * @param N The replacement term
     */
    template<typename M, typename var_x, typename N>
    struct subst {};

    /* x[x := N] = N */
    // substituting variable x for N in variable x
    // should result in N 
    template<typename x, typename N>
    struct subst<var<x>, var<x>, N> {
        using type = N;
    };

    /* y[x := N] = y, if x ≠ y */
    // substituting variable x for N in variable y
    // should result in y
    // condition x ≠ y fulfilled by previous pattern
    template<typename y, typename x, typename N>
    struct subst<var<y>, var<x>, N> {
        using type = y;
    };
    
    /* (M1 M2)[x := N] = (M1[x := N]) (M2[x := N]) */
    // substituting variable x for N in the subterms M1 and M2
    // should result recurse into M1 and M2
    template<typename M1, typename M2, typename x, typename N>
    struct subst<app<M1, M2>, var<x>, N> {
        using M1_ = subst<M1, var<x>, N>::type;
        using M2_ = subst<M2, var<x>, N>::type;
        using type = app<M1_, M2_>;
    };

    /* (λx.M)[x := N] = λx.M */
    // substituting variable x for N in the abstraction \x.M
    // should result in \x.M unchanged
    template<typename x, typename M, typename N>
    struct subst<abs<var<x>, M>, var<x>, N> {
        using type = abs<var<x>, M>;
    };

    // (λy.M)[x := N] = λy.(M[x := N]), if x ≠ y and y ∉ FV(N)
    // substitution variable x for N in the abstraction \y.M
    // condition x ≠ y fulfilled by previous pattern
    // condition y ∉ FV(N) not established yet
    template<typename y, typename M, typename x, typename N>
    struct subst<abs<var<y>,M>, var<x>, N> {
        // TODO
    };
}

#endif
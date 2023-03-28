#ifndef __LAMBDA_SUBST_HPP__
#define __LAMBDA_SUBST_HPP__

#include "types.hpp"

namespace lambda {
    // M[V := N]

    template<typename M, typename V, typename N>
    struct subst {};

    // x[x := N] = N
    template<typename x, typename N>
    struct subst<var<x>, var<x>, N> {
        using type = N;
    };

    // y[x := N] = y        if x != y
    template<typename y, typename x, typename N>
    struct subst<var<y>, var<x>, N> {
        using type = var<y>;
    };
    
    // (M1 M2)[x := N] = (M1[x := N]) (M2[x := N])
    template<typename M1, typename M2, typename x, typename N>
    struct subst<app<M1, M2>, var<x>, N> {
        using M1_ = typename subst<M1, var<x>, N>::type;
        using M2_ = typename subst<M2, var<x>, N>::type;

        using type = app<M1_, M2_>;
    };

    // \x.M[x := N] = \x.M
    template<typename x, typename M, typename N>
    struct subst<abs<var<x>, M>, var<x>, N> {
        using type = abs<var<x>, M>;
    };

    // \y.M[x := N] = \y.(M[x := N])        if x != y and y not in FV(N)
    template<typename y, typename M, typename x, typename N>
    struct subst<abs<var<y>, M>, var<x>, N> {
        /*
            M1 = abs<y, M>
            M2 = if y in FV(N) then alpha<M1, y, y_> else M1

            where y_ = fresh<max_fresh<N>>;

            result = abs<var<y>, subst<M2, var<x>, N>::type>;
        */
    };
}

#endif
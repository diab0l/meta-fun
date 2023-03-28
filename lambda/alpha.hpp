#ifndef __LAMBDA_ALPHA_HPP__
#define __LAMBDA_ALPHA_HPP__

#include "types.hpp"

namespace lambda {
    template<typename, typename, typename pattern>
    struct _if_eq {
        using type = typename pattern::False;
    };

    template<typename x, typename pattern>
    struct _if_eq<x, x, pattern> {
        using type = typename pattern::True;
    };

    template<typename x, typename y, typename pattern>
    using _if_eq_t = typename _if_eq<x, y, pattern>::type;

    template<typename, typename, typename>
    struct alpha {};
    
    template<typename x, typename y, typename r>
    struct alpha<var<x>, var<y>, var<r>> {
        using type = var<x>;
    };

    template<typename x, typename M, typename y, typename r>
    struct alpha<abs<var<x>, M>, var<y>, var<r>> {
        private:
        template<typename M_>
        struct alpha_rec {};
        
        template<typename M_>
        using alpha_rec_t = typename alpha_rec<M_>::type;

        template<>
        struct alpha_rec<var<y>> {
            using type = var<r>;
        };

        template<typename x_>
        struct alpha_rec<var<x_>> {
            using type = var<x_>;
        };

        template<typename x_, typename M_>
        struct alpha_rec<abs<var<x_>, M_>> {
            struct x__is_y {
                using True = abs<var<x_>, M_>;
                using False = abs<var<x_>, typename alpha_rec<M_>::type>;
            };

            using type = typename _if_eq_t<x_, y, x__is_y>::type;
        };

        template<typename M_, typename N_>
        struct alpha_rec<app<M_, N_>> {
            using M__ = typename alpha_rec<M_>::type;
            using N__ = typename alpha_rec<N_>::type;

            using type = app<M__, N__>;
        };

        struct x_is_y {
            using True = abs<var<r>, typename alpha_rec<M>::type>;
            using False = abs<var<x>, M>;
        };

        public:
        using type = _if_eq_t<x, y, x_is_y>;
    };

    template<typename M, typename N, typename y, typename r>
    struct alpha<app<M, N>, var<y>, var<r>> {
        private:
        using M_ = typename alpha<M, var<y>, var<r>>::type;
        using N_ = typename alpha<N, var<y>, var<r>>::type;

        public:
        using type = app<M_, N_>;
    };
}

#endif
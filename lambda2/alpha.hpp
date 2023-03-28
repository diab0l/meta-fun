#ifndef __LAMBDA2_ALPHA_HPP__
#define __LAMBDA2_ALPHA_HPP__

#include "types.hpp"

namespace lambda2 {
    /**
     * α-conversion
     * (λx.M[x]) → (λy.M[y])
     * @param var_x The variable to be substituted
     * @param M The term in which to substitute
     * @param var_y The substitution variable
     */
    template<typename var_x, typename M, typename var_y>
    struct α {};
}

#endif
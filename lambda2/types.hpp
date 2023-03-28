#ifndef __LAMBDA_TYPES_HPP__
#define __LAMBDA_TYPES_HPP__

namespace lambda2 {
    /**
     * Variable
     * @param x Name of the variable
     */
    template<typename x>
    struct var {};

    /**
     * Abstraction
     * @param var_x Variable
     * @param M Term of the abstraction
     */ 
    template<typename var_x, typename M>
    struct abs {};

    /**
     * Application
     * @param M Function term
     * @param N Argument term
     */
    template<typename M, typename N>
    struct app {};
}

#endif 
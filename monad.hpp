#pragma once

#include <iostream>
#include <thread>
#include "range/v3/all.hpp"

#include "metafunction.hpp"




namespace monadstate{



 //S requires to be variant with monostates, variant of states
template<typename S,typename T>
S mreturn(const T &a)
{
    return S{a};
}



template<typename F,typename S > //S requires to be variant of states with monostates
bool guard(const S &v)
{
    return std::holds_alternative<F>(v);
}


//S requires to be variant with monostates
template<typename S,typename F>
S mbind(const S &v, F  &&k)
{
    return std::visit([k=k](auto &&arg)->S
    {
        using TypeVariant = std::decay_t<decltype(arg)>;
        using TypeArg=typename decltype(k)::type;

        if constexpr(std::is_same_v<TypeVariant, TypeArg>)
        {
            return k(arg);
        }
    },v);
}


template <typename S,typename T>
struct nextState{
    using type=T;

    S operator () (const T & s) const
    {
        std::cout <<"operator () parameter: "<< typeid(decltype(s)).name()<<std::endl;
        return transition (s);
    }

};

template<typename S,typename T>
using to_next_state_tuple = decltype(compoundParamPacks<std::tuple,std::tuple,nextState,S>(std::declval<T>()));

template<typename S> // S is states
using tupleStates=to_tuple_from_variant_with_zero<S>;

template<typename S> // S is states
using tupleNextState=to_next_state_tuple<S,tupleStates<S>>;

template<typename S> // S is states
inline size_t findIndexInTuple(const tupleNextState<S> &tpl, const S &s)
{
    size_t ret=std::numeric_limits<size_t>::max();
    size_t i=0;
    for_each(tpl,[s=s,&i,&ret](auto &&arg)
    {
        using TypeVariant = std::decay_t<decltype(arg)>;
        using TypeArg=typename TypeVariant::type;
        if (std::holds_alternative<TypeArg>(s))
        {
            ret=i;
        }
        i++;
    });
    return ret;
}


}

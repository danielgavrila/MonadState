#pragma once
#include<tuple>
#include<variant>
#include<vector>
#include<array>
#include<utility>
#include<type_traits>
#include<functional>
namespace monadstate{
template< template  <typename ... > typename Target ,
          template  <typename ... > typename Source ,
          typename... T>
Target<T...>   moveParamPacks( Source <T ... >);

template< typename FirstParam,
          template  <typename ... > typename Target ,
          template  <typename ... > typename Source ,
          typename... T>
Target<FirstParam,T...>   moveParamPacks( Source <T ... >);


template< typename FirstParam,
          template  <typename ... > typename Target ,
          template  <typename ... > typename Source ,
          typename... T>
Target<T...>   moveParamPacksInv( Source <FirstParam,T ... >);

template< template  <typename ... > typename Target ,
          template  <typename ... > typename Source ,
          template <typename , typename> typename F,
          typename S,
          typename... T>
Target<F<S,T>...>   compoundParamPacks( Source <T ... >);



//T requires to be a tuple
template<typename T>
using to_variant_with_zero = decltype(moveParamPacks<std::monostate,std::variant,std::tuple>(std::declval<T>()));

template<typename T>
using to_tuple_from_variant_with_zero = decltype(moveParamPacksInv<std::monostate,std::tuple,std::variant>(std::declval<T>()));



template<std::size_t N, class Tuple, class Functor>
void apply_one(Tuple&& p, Functor&& func)
{
    std::forward<Functor>(func)(std::get<N>(std::forward<Tuple>(p)));
}

template<class Tuple, class Functor, std::size_t... Is>
void applyIndex(Tuple&& p, int index, Functor &&func, std::index_sequence< Is...>)
{
    using FT = std::function<void(Tuple&&, Functor&&)>;
    //FT arr[] = { [](Tuple &&p,Functor &&func)
    //{
    // std::forward<Functor>(func)(std::get<Is>(std::forward<Tuple>(p)));
    //}
    //... };

    constexpr std::size_t N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    std::array<FT,N> arr = { apply_one<Is,Tuple,Functor>... };

    arr[index](std::forward<Tuple>(p), std::forward<Functor>(func));
}

template<typename Tuple, typename Functor>
void applyIndex(Tuple&& p, int index, Functor&& func)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    applyIndex(std::forward<Tuple>(p), index, std::forward<Functor>(func), std::make_index_sequence<N>{});
}




//for_each for tuples
template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1,
        (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
    };
}



template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}
}

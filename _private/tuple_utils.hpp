/** @file tuple_utils.h
 *  @brief Expand tuple utils
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <functional>
#include <memory>

namespace tuple_utils {

    template<int...> struct index_tuple{}; 

    template<int I, typename IndexTuple, typename... Types> 
    struct make_indexes_impl; 

    template<int I, int... Indexes, typename T, typename ... Types> 
    struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...> 
    { 
        typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type; 
    }; 

    template<int I, int... Indexes> 
    struct make_indexes_impl<I, index_tuple<Indexes...> > 
    { 
        typedef index_tuple<Indexes...> type; 
    }; 

    template<typename ... Types> 
    struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...> 
    {}; 





    template<class Ret, class... Args, int... Indexes > 
    Ret apply_helper(const std::function<Ret(Args...)>& pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        return pf( std::forward<Args>( std::get<Indexes>(tup))... ); 
    } 

    template<class Ret, class ... Args> 
    Ret apply(const std::function<Ret(Args...)>& pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<class Ret, class ... Args> 
    Ret apply(const std::function<Ret(Args...)>& pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }







    template<typename Context, class... Args, int... Indexes > 
    void apply_helper(std::shared_ptr<Context> context, std::_Mem_fn<void (Context::*)(Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        pf(context, std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, class ... Args> 
    void apply(std::shared_ptr<Context> context, std::_Mem_fn<void (Context::*)(Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, class ... Args> 
    void apply(std::shared_ptr<Context> context, std::_Mem_fn<void (Context::*)(Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }






    template<typename Context, typename Ret, class... Args, int... Indexes > 
    Ret apply_helper(std::shared_ptr<Context> context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        return pf(context, std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, typename Ret, class ... Args> 
    Ret apply(std::shared_ptr<Context> context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, typename Ret, class ... Args> 
    Ret apply(std::shared_ptr<Context> context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }





    template<std::size_t...> struct seq{};
        
    template<std::size_t N, std::size_t... Is>
    struct gen_seq : gen_seq<N-1, N-1, Is...>{};
    
    template<std::size_t... Is>
    struct gen_seq<0, Is...> : seq<Is...>{};
    
    template<class Ch, class Tr, class Tuple, std::size_t... Is>
    void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
        using swallow = int[];
        (void)swallow{0, (void(os << (Is == 0? "" : ", ") << std::get<Is>(t)), 0)...};
    }
 
}

template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) -> std::basic_ostream<Ch, Tr>&
{
    os << "(";
    tuple_utils::print_tuple(os, t, tuple_utils::gen_seq<sizeof...(Args)>());
    return os << ")";
}
/** @file tuple_utils.h
 *  @brief Expand tuple utils
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <functional>

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
    void apply_helper(Context* context, std::_Mem_fn<void (Context::*)(Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        pf(context, std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, class ... Args> 
    void apply(Context* context, std::_Mem_fn<void (Context::*)(Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, class ... Args> 
    void apply(Context* context, std::_Mem_fn<void (Context::*)(Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }






    template<typename Context, typename Ret, class... Args, int... Indexes > 
    Ret apply_helper(Context* context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        return pf(context, std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, typename Ret, class ... Args> 
    Ret apply(Context* context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, typename Ret, class ... Args> 
    Ret apply(Context* context, std::_Mem_fn<Ret (Context::*)(Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }

}
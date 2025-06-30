#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <utility>

namespace serp
{

    template <typename Value>
    class Response;

    // Helper structs for index sequences (used to unpack tuples)
    template <int...>
    struct index_tuple
    {
    };

    template <int I, typename IndexTuple, typename... Types>
    struct make_indexes_impl;

    template <int I, int... Indexes, typename T, typename... Types>
    struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
    {
        using type = typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type;
    };

    template <int I, int... Indexes>
    struct make_indexes_impl<I, index_tuple<Indexes...>>
    {
        using type = index_tuple<Indexes...>;
    };

    template <typename... Types>
    struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>
    {
    };

    // Invoke a normal function with unpacked tuple arguments
    template <class... Args, int... Indexes>
    inline void invokeAsyncCallFunc(const std::function<void(Args...)> &func,
                                    const index_tuple<Indexes...> /*i_tup*/,
                                    const std::tuple<Args...> &&tup)
    {
        func(std::forward<const Args>(std::get<Indexes>(tup))...);
    }

    // Invoke a member function with context pointer as first argument
    template <typename Context, typename Ret, class... Args, int... Indexes>
    inline void invokeAsyncCall(Context *const context,
                                const std::function<Ret(Context *, Args...)> func,
                                const index_tuple<Indexes...> /*i_tup*/,
                                const std::tuple<Args...> &&tup)
    {
        func(context, std::forward<const Args>(std::get<Indexes>(tup))...);
    }

    // Invoke a PRequest function with reply callback and unpacked arguments
    template <typename Ret, class... Args, int... Indexes>
    inline void invokePRequestFunc(
        const std::function<void(const std::function<void(Ret)>, Args...)> func,
        const std::function<void(Ret)> reply,
        index_tuple<Indexes...> /*i_tup*/,
        const std::tuple<Args...> &&tup)
    {
        func(reply, std::forward<const Args>(std::get<Indexes>(tup))...);
    }

    // Invoke a PRequest function with context, reply callback, and unpacked arguments
    template <typename Context, typename Ret, class... Args, int... Indexes>
    inline void invokePRequest(
        Context *const context,
        const std::function<void(Context *, const std::function<void(Ret)>, Args...)> func,
        const std::function<void(Ret)> reply,
        const index_tuple<Indexes...> /*i_tup*/,
        const std::tuple<Args...> &&tup)
    {
        func(context, reply, std::forward<const Args>(std::get<Indexes>(tup))...);
    }

    template <typename Ret, class... Args, int... Indexes>
    inline void invokePRequestFunc(
        const std::function<void(std::shared_ptr<Response<Ret>>, Args...)> func,
        std::shared_ptr<Response<Ret>> reply,
        index_tuple<Indexes...> /*i_tup*/,
        std::tuple<Args...> &&tup)
    {
        func(std::move(reply), std::forward<Args>(std::get<Indexes>(tup))...);
    }

    // Invoke PRequest function with context, reply (shared_ptr) by value and unpacked arguments
    template <typename Context, typename Ret, class... Args, int... Indexes>
    inline void invokeRequest(
        Context *const context,
        const std::function<void(Context *, std::shared_ptr<Response<Ret>>, Args...)> func,
        std::shared_ptr<Response<Ret>> response,
        const index_tuple<Indexes...> /*i_tup*/,
        std::tuple<Args...> &&tup)
    {
        func(context, std::move(response), std::forward<Args>(std::get<Indexes>(tup))...);
    }

} // namespace serp

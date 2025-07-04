/** @file Call.cpp
 *  @brief Call classes declaration
 *
 *  Calls class contain information about funciton which should be called and all attributes
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <iostream>

#include "../itc.hpp"
#include "tuple_utils.hpp"
#include "ICallable.hpp"
#include "CallBinder.hpp"
#include "Logger.hpp"

namespace itc {

template<typename... Args>
    class CallStatic: public _private::ICallable
    {
    public:
        CallStatic(const std::function<void(Args...)>& func, std::tuple<Args...> params = std::make_tuple<>())
            : mFunc(func) 
            , mParams(params)
            {
            } 

        void call() const override {
            tuple_utils::apply(mFunc, mParams);
        }
    private:
        const std::function<void(Args...)> mFunc;
        std::tuple<Args...> mParams;
    };

    const std::string& currentThreadName(); //To resolve name currentThreadName on first phase of ADL
    template<typename Context, typename... Args>
    class Call : public _private::ICallable
    {
    public:
        Call(std::shared_ptr<Context> context, std::_Mem_fn<void (Context::*)(Args...)> func, std::tuple<Args...> params = std::make_tuple<>())
            : mContext(context)
            , mFunc(func)
            , mParams(params)
        {
        }

        void call() const override {
            //std::cout << "???" << " -R-> " << currentThreadName() << ": " << typeid(mContext).name() << "::" << typeid(mFunc).name() << mParams << std::endl;
            tuple_utils::apply(mContext, mFunc, mParams);
        }
    private:
        std::shared_ptr<Context> mContext;
        std::_Mem_fn<void (Context::*)(Args...)> mFunc;
        std::tuple<Args...> mParams;
    };
    
    bool invoke(const itc::_private::CallBinder& callBinder); //To resolve name currentThreadName on first phase of ADL
    template<typename ContextRequest, typename ContextResponse,  typename Ret, typename... Args>
    class Request: public _private::ICallable
    {
    public:
        Request(std::shared_ptr<ContextRequest> contextRequest, 
            std::shared_ptr<ContextResponse> contextResponse, 
            std::_Mem_fn<Ret (ContextRequest::*)(Args...)> request,            
            std::_Mem_fn<void (ContextResponse::*)(Ret)> response, 
            std::tuple<Args...> params)
            : mContextRequest(contextRequest)
            , mContextResponse(contextResponse)
            , mRequest(request)
            , mResponse(response) 
            , mParams(params)
            , mResponseThread(currentThreadName())
            {
            }
        
        void call() const override {
            Ret result = tuple_utils::apply(mContextRequest, mRequest, mParams);
            invoke(_private::CallBinder(mResponseThread, std::make_shared<itc::Call<ContextResponse, Ret>>(mContextResponse, mResponse, std::make_tuple<Ret>(std::forward<Ret>(result)))));
        }

    private:
        std::shared_ptr<ContextRequest>mContextRequest;
        std::shared_ptr<ContextResponse> mContextResponse;
        std::_Mem_fn<Ret (ContextRequest::*)(Args...)> mRequest;
        std::_Mem_fn<void (ContextResponse::*)(Ret)> mResponse;
        std::tuple<Args...> mParams;
        std::string mResponseThread;
    };

    template<typename... Args> 
    class InlineEvent : public itc::_private::CallBinder 
    { 
        public: 
        InlineEvent<Args...>(const std::function<void(Args...)>& func, Args... args) 
        : CallBinder(itc::currentThreadName() 
            , std::make_shared<itc::CallStatic<Args...>>(func, std::make_tuple(args...))) 
        { 
            std::cout << " ---EVENT---> " << itc::currentThreadName() << ": " << "InlineEvent"; 
            itc::_private::logArgs(std::cout, std::forward<Args>(args)...); 
            std::cout << std::endl; 
        } 
    };
}
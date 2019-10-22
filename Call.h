/** @file Call.cpp
 *  @brief Call classes declaration
 *
 *  Calls class contain information about funciton which should be called and all attributes
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "itc.h"
#include "_private/tuple_utils.h"
#include "_private/ICallable.h"
#include "iostream"


namespace itc {

template<typename... Args>
    class CallStatic: public _private::ICallable
    {
    public:
        CallStatic(const std::function<void(Args...)>& func, Args... params)
            : mFunc(func) 
            , mParams(std::tuple<Args...>(std::forward<Args>(params)...))
            {
            } 

        void call() const override {
            tuple_utils::apply(mFunc, mParams);
        }
    private:
        const std::function<void(Args...)> mFunc;
        std::tuple<Args...> mParams;
    };

    const std::string& currentThreadName(); //To resove name currentThreadName on first phase of ADL
    template<typename Context, typename... Args>
    class Call : public _private::ICallable
    {
    public:
        Call(Context* context, std::_Mem_fn<void (Context::*)(Args...)> func, std::tuple<Args...> params = std::make_tuple<Args...>())
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
        Context* mContext;
        std::_Mem_fn<void (Context::*)(Args...)> mFunc;
        std::tuple<Args...> mParams;
    };
    
    template<typename ContextRequest, typename ContextResponse,  typename Ret, typename... Args>
    class Request: public _private::ICallable
    {
    public:
        Request(ContextRequest* contextRequest, 
            ContextResponse* contextResponse, 
            std::_Mem_fn<Ret (ContextRequest::*)(Args...)> request,            
            std::_Mem_fn<void (ContextResponse::*)(Ret)> response, 
            std::tuple<Args...> params)
            : mContextRequest(contextRequest)
            , mContextResponse(contextResponse)
            , mRequest(request)
            , mResponse(response) 
            , mParams(params)//std::tuple<Args...>(std::forward<Args>(params)...))
            , mResponseThread(currentThreadName())
            {
            }
        
        void call() const override {
            Ret result = tuple_utils::apply(mContextRequest, mRequest, mParams);
            invoke(mResponseThread, new itc::Call<ContextResponse, Ret>(mContextResponse, mResponse, std::make_tuple<Ret>(std::forward<Ret>(result))));
        }

    private:
        ContextRequest* mContextRequest;
        ContextResponse* mContextResponse;
        std::_Mem_fn<Ret (ContextRequest::*)(Args...)> mRequest;
        std::_Mem_fn<void (ContextResponse::*)(Ret)> mResponse;
        std::tuple<Args...> mParams;
        std::string mResponseThread;
    };
}
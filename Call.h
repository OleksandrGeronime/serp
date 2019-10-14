/** @file Call.cpp
 *  @brief Call classes declaration
 *
 *  Calls class contain information about funciton which should be called and all attributes
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "_private/tuple_utils.h"
#include "_private/ICallable.h"

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

template<typename Context, typename... Args>
class Call: public _private::ICallable
{
public:
	Call(Context* context, std::_Mem_fn<void (Context::*)(Args...)> func, Args... params)
		: mContext(context)
		, mFunc(func) 
		, mParams(std::tuple<Args...>(std::forward<Args>(params)...))
		{
		} 
	
	void call() const override {
		tuple_utils::apply(mContext, mFunc, mParams);
	}
private:
	Context* mContext;
	std::_Mem_fn<void (Context::*)(Args...)> mFunc;
	std::tuple<Args...> mParams;
};
}
#include <iostream>
#include "itc.h"

#include <thread>
#include <chrono>

struct A{
    std::string str;
    int i;
};

//-------------------------
// Thread bgThread2 START
//-------------------------
struct Service1;

struct Service2 {
    Service1* mpService1;
    int m;
    void bar(int i);
	void bar2(int i);
};

//-------------------------
// Thread bgThread2 END
//-------------------------




//-------------------------
// Thread bgThread1 START
//-------------------------


struct Service1 {
    void foo(int i, std::string msg, Service2* pService2) {
		std::cout << itc::currentThreadName() << ": " << "Service1::foo(" << i << ", " << msg << ")" << std::endl;

		auto s = *pService2;
		itc::invoke("bgThread2", new itc::Call<decltype(s), int>(&s, std::mem_fn(&Service2::bar), i));

        //itc::invoke("bgThread2", new itc::TRequest<Service2, int>(pService2, std::mem_fn(&Service2::bar), i));
    }
};

//-------------------------
// Thread bgThread1 END
//-------------------------


//-------------------------
// Thread bgThread2 START
//-------------------------

void func(A a) {
    std::cout << "bgThread2: " << "func2: " << a.str << " " << a.i  << std::endl;
}

void Service2::bar(int i) {
    m = m + 1;
	std::cout << itc::currentThreadName() << ": " << "Service2::bar(" << i << "), m=" << m << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

    itc::invoke("bgThread1", new itc::Call<Service1, int, std::string,  Service2*>(mpService1, std::mem_fn(&Service1::foo), m, "Hello from Service2", this));
}

void Service2::bar2(int i) {
	std::cout << itc::currentThreadName() << ": " << "Service2::bar(" << i << ")" << std::endl;
}

//-------------------------
// Thread bgThread2 END
//-------------------------


int main()
{	
	Service1 service1;
    Service2 service2;
    service2.m = 1000;
    service2.mpService1 = &service1;


    itc::createEventLoop("bgThread1");
    itc::createEventLoop("bgThread2");

    //itc::invoke("bgThread1", new itc::Call<Service1, int, std::string, Service2*>(&service1, std::mem_fn(&Service1::foo), 0, "Hello", &service2));

    itc::invoke("bgThread1", new itc::Call<Service1, int, std::string, Service2*>(&service1, std::mem_fn(&Service1::foo), 0, "Hello from main", &service2));



	itc::invoke("bgThread2", new itc::Call<Service2, int>(&service2, std::mem_fn(&Service2::bar2), 444));

	using namespace std::chrono_literals;
	itc::createTimer("bgThread2", new itc::Call<Service2, int>(&service2, std::mem_fn(&Service2::bar2), 111), 1s, true).start();


    int i = 333;
    std::string msg = "HELLO WP1";
    int j = 0;
	A a;
	a.str = msg;
	a.i = i;
    while(1) {
        i++;
		

        //itc::invoke("bgThread1", new itc::CallStatic<A>(func, a));

        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

	
    return 0;
}
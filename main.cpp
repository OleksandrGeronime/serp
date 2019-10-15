#include <iostream>
#include "itc.h"

#include <thread>
#include <chrono>

static std::chrono::system_clock::time_point gAppStartTime = std::chrono::system_clock::now();

long long getTimeFromStart() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - gAppStartTime).count();
}

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
	void testRequest();

    void timerEvent() {
        static int i = 0;
        std::cout << itc::currentThreadName() << ": " << "timerEvent i = " << i << std::endl;
        i++;
    }

	void onResponseSum(int sum) {
		std::cout << itc::currentThreadName() << "Service2::onResponce: " << sum << std::endl;
	} 
};

//-------------------------
// Thread bgThread2 END
//-------------------------




//-------------------------
// Thread bgThread1 START
//-------------------------


struct Service1 {
    void foo(int i, std::string msg, Service2* pService2) {
		if (!(i % 10000)) std::cout << itc::currentThreadName() << ": " << "Service1::foo(" << i << ", " << msg << ")" << std::endl;

		itc::invoke("bgThread2", new itc::Call<Service2, int>(pService2, std::mem_fn(&Service2::bar), i));
    }

    void startTimers() {        
        itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(10), true).start();
        itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(4), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(5), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(3), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(2), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(6), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(200), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(500), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(300), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(250), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(350), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(400), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(550), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(330), true).start();
		itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(220), true).start();

		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(10), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(4), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(5), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(3), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(2), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(6), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(200), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(500), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(300), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(250), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(350), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(400), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(550), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(330), true).start();
		itc::createTimer("bgThread2", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::milliseconds(220), true).start();
    }

    void timerEvent() {
		static int counter = 0;
		counter++;
		if (!(counter % 100)) std::cout << itc::currentThreadName() << ": " << "timer " << itc::getLastTimerId() << " " << getTimeFromStart() << std::endl;
    }

	int onRequestSum(int a, int b) {
		std::cout << itc::currentThreadName() << "Service1::onRequest: a = " << a << ", b = " << b << std::endl;
		return a + b;
	} 
};

//-------------------------
// Thread bgThread1 END
//-------------------------


//-------------------------
// Thread bgThread2 START
//-------------------------

void func(int i) {
    std::cout << "func: " << i << std::endl;
}

void Service2::bar(int i) {
    m = m + 1;
	if (!(i % 10000)) std::cout << itc::currentThreadName() << ": " << "Service2::bar(" << i << "), m=" << m << std::endl;

	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

    itc::invoke("bgThread1", new itc::Call<Service1, int, std::string,  Service2*>(mpService1, std::mem_fn(&Service1::foo), m, "Hello from Service2", this));
}

void Service2::testRequest() {
	std::cout << itc::currentThreadName() << ": " << "Service2::testRequest" << std::endl;

	itc::invoke("bgThread1", new itc::Request<Service1, Service2, int, int, int>
		(mpService1, this, std::mem_fn(&Service1::onRequestSum), std::mem_fn(&Service2::onResponseSum), 2, 3));

}

//-------------------------
// Thread bgThread2 END
//-------------------------


void loop(int i) {

	std::string s = itc::currentThreadName();
	if (itc::currentThreadName() == "bgt9") {
		s = "bgt0";
	} else {
		s = s.substr(0, 3).append(std::to_string(std::stoi(s.substr(3, 1)) + 1));
	}

	if (!(i % 10000)) std::cout << itc::currentThreadName() << ": " << "loop " << i << std::endl;
	itc::invoke(s, new itc::CallStatic<int>(loop, ++i));
}

int main()
{	
	Service1 service1;
    Service2 service2;
    service2.m = 1000;
    service2.mpService1 = &service1;
	
    itc::createEventLoop("bgThread1");
    itc::createEventLoop("bgThread2");


	itc::invoke("bgThread2", new itc::Call<Service2>(&service2, std::mem_fn(&Service2::testRequest)));



	itc::invoke("bgThread1", new itc::Call<Service1, int, std::string, Service2*>(&service1, std::mem_fn(&Service1::foo), 0, "Hello from main", &service2));
	
    itc::invoke("bgThread1", new itc::Call<Service1>(&service1, std::mem_fn(&Service1::startTimers)));
	
    itc::invoke("bgThread2", new itc::CallStatic<int>(func, 5));


	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	itc::createEventLoop("bgt0");
	itc::createEventLoop("bgt1");
	itc::createEventLoop("bgt2");
	itc::createEventLoop("bgt3");
	itc::createEventLoop("bgt4");
	itc::createEventLoop("bgt5");
	itc::createEventLoop("bgt6");
	itc::createEventLoop("bgt7");
	itc::createEventLoop("bgt8");
	itc::createEventLoop("bgt9");
	itc::invoke("bgt0", new itc::CallStatic<int>(loop, 0));



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
#include <iostream>
#include "itc.h"

#include <thread>
#include <chrono>
#include <string>
#include <tuple>

#include "demo/CallStaticDemo.h"
#include "demo/CallDemo.h"
#include "demo/RequestDemo.h"
#include "demo/TimerDemo.h"

static const std::string THREAD_1 = "bgThread1";
static const std::string THREAD_2 = "bgThread2";

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

    void onResponseSum(int sum);
};

//-------------------------
// Thread bgThread2 END
//-------------------------




//-------------------------
// Thread bgThread1 START
//-------------------------


struct Service1 {
    void foo(int i, std::string msg, Service2* pService2) {
        if (!(i % 1)) std::cout << itc::currentThreadName() << ": " << "Service1::foo(" << i << ", " << msg << ")" << std::endl;

        //itc::invoke("bgThread2", new itc::Call<Service2, int>(pService2, std::mem_fn(&Service2::bar), i));
    }

    void startTimers();

    void timerEvent() {
        static int counter = 0;
        counter++;
        //if (!(counter % 100)) 
        std::cout << itc::currentThreadName() << ": " << "timer " << itc::getLastTimerId() << " " << itc::getTimeFromStart() << std::endl;
    }

    int onRequestSum(int a, int b) {
        std::cout << itc::currentThreadName() << ": " << "Service1::onRequest: a = " << a << ", b = " << b << std::endl;
        return a + b;
    } 
};

//-------------------------
// Thread bgThread1 END
//-------------------------

DECLARE_CALL(Service2_testRequest, THREAD_2, Service2, testRequest)
DECLARE_REQUEST(Service1_requestSum, THREAD_1, Service2, onResponseSum, int, Service1, onRequestSum, int, int)

DECLARE_CALL(Service1_timerEvent, THREAD_1, Service1, timerEvent)

//-------------------------
// Thread bgThread2 START
//-------------------------

void func(int i) {
    std::cout << itc::currentThreadName() << ": " << "func: " << i << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << itc::currentThreadName() << ": " << "func invoke"<< std::endl;
    itc::invoke(THREAD_1, new itc::CallStatic<int>(func, 5));
}

void Service2::bar(int i) {
    m = m + 1;
    if (!(i % 1)) std::cout << itc::currentThreadName() << ": " << "Service2::bar(" << i << "), m=" << m << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //itc::invoke(THREAD_1, new itc::Call<Service1, int, std::string, Service2*>(mpService1, std::mem_fn(&Service1::foo), m, "Hello from Service2", this));
}


void Service2::testRequest() {
    std::cout << itc::currentThreadName() << ": " << "Service2::testRequest" << std::endl;

    //itc::invoke(THREAD_1, new SR1::Request(mpService1, this, SR1::onRequestSum, SR1::onResponseSum, std::make_tuple<int, int>(6, 7) ));

    static int a = 1;
    static int b = 1;
    itc::invoke(Service1_requestSum::Request(mpService1, std::make_tuple<int&, int&>(a, b), this));
    a++;
    b+=2;
}

void Service2::onResponseSum(int sum) {
    std::cout << itc::currentThreadName() << ": " << "Service2::onResponce: " << sum << std::endl;

    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    itc::invoke(Service2_testRequest::Call(this, Service2_testRequest::Params()));
} 



//-------------------------
// Thread bgThread2 END
//-------------------------



void Service1::startTimers() {
    std::cout << itc::currentThreadName() << ": " << "Service1::startTimers" << std::endl;

    auto& t = itc::timer(Service1_timerEvent::Call(this), std::chrono::seconds(1), true);
    t.start();
    //itc::timer(Service1_timerEvent::Call(this), std::chrono::seconds(1), true).start();

    //itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(10), true).start();
    /*itc::createTimer("bgThread1", new itc::Call<Service1>(this, std::mem_fn(&Service1::timerEvent)), std::chrono::seconds(4), true).start();
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
*/}




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

namespace AAA {
    class B {
    public: 
        void foo(int i, float f) {
            std::cout << itc::currentThreadName() << ": " << "foo(int, float) " << i << " " << f << std::endl;
        }

        void foo(int i) {
            std::cout << itc::currentThreadName() << ": " << "foo(int) " << i << std::endl;
        }
    };
}




//DECLARE_CALL(B_Call1, AAA::B, foo, int, float)
//DECLARE_CALL(B_Call2, AAA::B, foo, int)




DECLARE_CALL(B_foo2, THREAD_1, AAA::B, foo, int, float)
DECLARE_CALL(B_foo1, THREAD_1, AAA::B, foo, int)

//DECLARE_CALL(Service1_Call1, Service1, foo, int, std::string, Service2*)

DECLARE_CALL(Service1_startTimers, THREAD_1, Service1, startTimers)


int main()
{    
    Service1 service1;
    Service2 service2;
    service2.m = 1000;
    service2.mpService1 = &service1;
    
    itc::createEventLoop(THREAD_1);
    itc::createEventLoop(THREAD_2);

    AAA::B b;

    CallStaticDemo callStaticDemo;
    callStaticDemo.run();

    CallDemo callDemo;
    //callDemo.run();

    RequestDemo requestDemo;
    //requestDemo.run();

    TimerDemo timerDemo;
    //timerDemo.run();

//    itc::invoke(THREAD_1, new B_Call1::Call(&b, B_Call1::foo, 6, 3.4f));
//    itc::invoke(THREAD_1, new B_Call2::Call(&b, B_Call2::foo, 4));

 //   itc::invoke(B_foo2::Call(&b, B_foo2::Params(8, 3.5f)));
 //   itc::invoke(B_foo1::Call(&b, B_foo1::Params(3)));

 //   itc::invoke(Service1_startTimers::Call(&service1));

/*    itc::invoke(THREAD_1, new itc::CallStatic<int>(func, 5));

    
    itc::invoke(THREAD_1, new Service1_Call1::Call(&service1, Service1_Call1::foo, 0, "Hello from main", &service2));

    

*/

    //itc::invoke(Service2_testRequest::Call(&service2, Service2_testRequest::Params()));





/*    itc::invoke(THREAD_1, new itc::Call<Service1, int, std::string, Service2*>(&service1, std::mem_fn(&Service1::foo), 0, "Hello from main", &service2));

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

*/


    int i = 333;
    std::string msg = "HELLO WP1";
    int j = 0;
    A a;
    a.str = msg;
    a.i = i;
    while(1) {
        i++;
        //itc::invoke(THREAD_1, new itc::CallStatic<A>(func, a));        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    
    return 0;
}
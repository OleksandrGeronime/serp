#include <thread>
#include <chrono>
#include <sstream>

#include "Server/private/Server.hpp"
#include "Client/IClient.hpp"
#include "Calc/ICalc.hpp"

const std::string IClient::THREAD_NAME = "SERVER_TEST";
const std::string ICalc::THREAD_NAME = "SERVER_TEST";

template <typename Head, typename... Tail>
void args(std::ostream& out, Head&& head, Tail&&... tail)
{    
    out << "(" << std::forward<Head>(head);
    using expander = int[];
    (void)expander{0, (void(out << ", " << std::forward<Tail>(tail)), 0)...};
    out << ")";
}

class Validator
{
public:
    void waitFor(std::string expected) 
    {
        std::cout << "Validator111 expected:\"" + expected + "\" and received \"" << mReceived << "\"" << std::endl;
        while (!mbReady) {
            std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        mbReady = false;

        std::cout << "Validator expected:\"" + expected + "\" and received \"" << mReceived << "\"" << std::endl;

        if (expected != mReceived) {
            throw "VALIDATION FAILED";
        }
    }

    void validate(std::string result) 
    {
        mReceived = result;
        mbReady = true;
    }

    bool mbReady = false;
    std::string mReceived;
};

class CalcStub: public ICalc
{
public:
    CalcStub(Validator* pValidator): mpValidator(pValidator){}

    void setConsumer(ICalcConsumer* pConsumer) override{};
    void sum(int a, int b) override{};
    void multiply(int a, int b) override
    {
        std::ostringstream stream;
        stream << "ICalc::multiply(" << a << ", " << b << ")";
        mpValidator->validate(stream.str()); 
    };

private:
    Validator* mpValidator;
};

class ClientStub: public IClient
{
public:
    ClientStub(Validator* pValidator): mpValidator(pValidator){}

    void responseSum(int response) override {}
    void responseFactorial(int response) override 
    {
        std::ostringstream stream;
        stream << "IClient::responseFactorial(" << response << ")";
        mpValidator->validate(stream.str()); 
    };
    void responseConvert(int response) override {}

    void run() override {}

private:
    Validator* mpValidator;
};



void testCase1(Server* pServer, Validator* pValidator)
{
    itc::invoke(CALL_requestFactorial::Call(pServer, 5));
    pValidator->waitFor("ICalc::multiply(5, 4)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 20));
    pValidator->waitFor("ICalc::multiply(20, 3)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 60));
    pValidator->waitFor("ICalc::multiply(60, 2)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 120));
    pValidator->waitFor("IClient::responseFactorial(120)");
}

void testCase2(Server* pServer, Validator* pValidator)
{
    itc::invoke(CALL_requestFactorial::Call(pServer, 10));
    pValidator->waitFor("ICalc::multiply(10, 9)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 90));
    pValidator->waitFor("ICalc::multiply(90, 8)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 720));
    pValidator->waitFor("ICalc::multiply(720, 7)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 5040));
    pValidator->waitFor("ICalc::multiply(5040, 6)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 30240));
    pValidator->waitFor("ICalc::multiply(30240, 5)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 151200));
    pValidator->waitFor("ICalc::multiply(151200, 4)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 604800));
    pValidator->waitFor("ICalc::multiply(604800, 3)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 1814400));
    pValidator->waitFor("ICalc::multiply(1814400, 2)");
    itc::invoke(CALL_multiplyResult::Call(pServer, 3628800));
    pValidator->waitFor("IClient::responseFactorial(3628800)");
}

DECLARE_STATIC_CALL (CALL_testCase1, IClient::THREAD_NAME, testCase1, IServer*)

int main()
{
    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);
    
    Validator* pValidator = new Validator();
    ICalc* pCalc = new CalcStub(pValidator);
    IClient* pClient = new ClientStub(pValidator);
    Server server(pCalc);
    server.setClient(pClient);

    try 
    {
        testCase1(&server, pValidator);
        std::cout << "Test case 1 SUCCESS" << std::endl;

        testCase2(&server, pValidator);
        std::cout << "Test case 2 SUCCESS" << std::endl;
    }
    catch(char const* exeption) {
        std::cerr << exeption << std::endl;
    }

    return 0;
}

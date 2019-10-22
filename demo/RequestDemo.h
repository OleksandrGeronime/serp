/** @file RequestDemo.h
 *  @brief Demo request functionality
 * 
 *  There are two threads. In one thread works PBUP. In another - DB.
 *  From RequestDemo::run() made call to PBUP to start process.
 *  Next PBUM makes request to DB and provides this as consumer.
 *  DB receive request, process it and when result will be ready it will be returned in PBUP.
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include "../itc.h"

namespace ns_RequestDemo {
    static const std::string THREAD_PBUP = "thread_PBUP";
    static const std::string THREAD_DB = "thread_DB";

    class DB 
    {
    public:
        int execute(std::string statement){
            std::cout << itc::currentThreadName() << ": " << "DB::execute(" << statement << ")" << std::endl;
            
            // do some work
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            return 404;
        };
    };

    class PBUP
    {
    public:
        void request();

        void onExecuteResponse(int result) {
            std::cout << itc::currentThreadName() << ": " << "PBUP::onExecuteResponse(" << result << ")" << std::endl;
        }

        DB db;
    };
}

DECLARE_CALL(CALL_request, ns_RequestDemo::THREAD_PBUP, ns_RequestDemo::PBUP, request)
DECLARE_REQUEST(REQUEST_execute, ns_RequestDemo::THREAD_DB, ns_RequestDemo::PBUP, onExecuteResponse, int, ns_RequestDemo::DB, execute, std::string)

void ns_RequestDemo::PBUP::request()
{
    itc::invoke(REQUEST_execute::Request(&db, REQUEST_execute::Params("SELECT * FROM DUAL;"), this));
}

class RequestDemo
{
public:
    void run();
};

void RequestDemo::run() 
{
    itc::createEventLoop(ns_RequestDemo::THREAD_DB);
    itc::createEventLoop(ns_RequestDemo::THREAD_PBUP);

    ns_RequestDemo::PBUP pbup;

    itc::invoke(CALL_request::Call(&pbup));
}
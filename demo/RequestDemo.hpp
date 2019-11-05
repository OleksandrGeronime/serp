/** @file RequestDemo.h
 *  @brief Demo request functionality
 *
 *  Reuqest could be used to call member funciton in another thread and receive function result value in another member function in invoker thread. 
 *  There are two threads. In one thread works PBUP. In another - DB.
 *  From RequestDemo::run() made call to PBUP to start process.
 *  Next PBUM makes request to DB and provides "this" as consumer.
 *  DB receive request, process it and when result will be ready it will be returned in PBUP.
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include "../itc.hpp"

namespace ns_RequestDemo {
    static const std::string THREAD_PBAP = "RequestDemo_thread_PBAP";
    static const std::string THREAD_DB = "RequestDemo_thread_DB";

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

    class PBAP
    {
    public:
        void request();

        void onExecuteResponse(int result) {
            std::cout << itc::currentThreadName() << ": " << "PBAP::onExecuteResponse(" << result << ")" << std::endl;
        }

        DB db;
    };
}

DECLARE_CALL(CALL_request, ns_RequestDemo::THREAD_PBAP, ns_RequestDemo::PBAP, request)
DECLARE_REQUEST(REQUEST_execute, ns_RequestDemo::THREAD_DB, ns_RequestDemo::PBAP, onExecuteResponse, int, ns_RequestDemo::DB, execute, std::string)

void ns_RequestDemo::PBAP::request()
{
    itc::invoke(REQUEST_execute::Request(&db, this, "SELECT * FROM DUAL;"));
}

class RequestDemo
{
public:
    void run();
};

void RequestDemo::run() 
{
    itc::createEventLoop(ns_RequestDemo::THREAD_DB);
    itc::createEventLoop(ns_RequestDemo::THREAD_PBAP);

    ns_RequestDemo::PBAP pbap;

    itc::invoke(CALL_request::Call(&pbap));
}
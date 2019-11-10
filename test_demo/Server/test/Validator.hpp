#pragma once

#include <string>
#include <iostream>
#include <thread>

class Validator
{
public:
    static const int MAX_WAIT = 1000;

    void waitFor(std::string expected) 
    {
        int counter = 0;
        while (!mbReady) {
            counter++;
            if (counter >= MAX_WAIT) {
                throw "RESPONSE TIMEOUT";
            }
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
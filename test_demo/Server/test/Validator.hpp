#pragma once

#include <string>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

class Validator
{
public:
    Validator(int timeout = 1000);
    ~Validator() = default;

    void waitFor(std::string expected);
    void validate(std::string result);

private:
    std::condition_variable mCV;
    std::mutex mMutex;
    std::queue<std::string> mToValidate;
    int mTimeout;
    bool mbReady = false;
};
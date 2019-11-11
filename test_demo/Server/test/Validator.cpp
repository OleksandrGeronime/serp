#include "Validator.hpp"

Validator::Validator(int timeout /* = 1000 */)
: mCV()
, mMutex()
, mToValidate()
, mTimeout(timeout)
{}

void Validator::waitFor(std::string expected) 
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto start = std::chrono::system_clock::now();
    while (!mbReady && std::chrono::system_clock::now() < start + std::chrono::milliseconds(mTimeout)) {
        mCV.wait_for(lock, std::chrono::milliseconds(mTimeout));
    }
    if (!mToValidate.empty()) {
        std::cout << "Validator expected:\"" + expected + "\" and received \"" << mToValidate.front() << "\"" << std::endl;
        if (mToValidate.front() != expected) {
            throw std::string("VALIDATION FAILED");
        }
        mToValidate.pop();
    } else {
        throw std::string("RESPONSE TIMEOUT Expected:" + expected);;
    }
    if (mToValidate.empty()) {
        mbReady = false;
    }
}

void Validator::validate(std::string result) 
{
    std::unique_lock<std::mutex> lock(mMutex);
    mToValidate.push(result);
    mbReady = true;
    mCV.notify_one();
}
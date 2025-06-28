/** @file Validator.cpp
 *  @brief Definition validator for tests 
 * 
 *  User for validation responces from component under test using Google Test
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "Validator.hpp"

#include <gtest/gtest.h>

namespace itc {
    namespace test {

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
                EXPECT_EQ(mToValidate.front(), expected);
                if (mToValidate.front() != expected) {
                    std::cout << "\033[1;31m" << "VALIDATION FAILED" << "\033[0m" << std::endl;
                    // throw std::string("VALIDATION FAILED");
                }
                mToValidate.pop();
            } else {
                std::cerr << "\033[1;31m" << "RESPONSE TIMEOUT Expected:" << expected << "\033[0m" << std::endl;
                EXPECT_TRUE(false);
                //throw std::string("RESPONSE TIMEOUT Expected:" + expected);
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

    }
}
/** @file Logger.cpp
 *  @brief Logger utilities
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include "Logger.hpp"

namespace itc {
	namespace _private {

        void logArgs(std::ostream& out)
        {    
            out << "()" << std::endl;
        }

    }
}
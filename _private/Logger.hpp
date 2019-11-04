/** @file Logger.hpp
 *  @brief Logger utilities
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <utility>

namespace itc {
	namespace _private {

        

        template <typename Head, typename... Tail>
        void logArgs(std::ostream& out, Head&& head, Tail&&... tail)
        {    
            out << "(" << std::forward<Head>(head);
            using expander = int[];
            (void)expander{0, (void(out << ", " << std::forward<Tail>(tail)), 0)...};
            out << ")" << std::endl;
        }

        void logArgs(std::ostream& out);

    }
}
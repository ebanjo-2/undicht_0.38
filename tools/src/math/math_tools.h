#ifndef math_tools
#define math_tools

#include <algorithm>


namespace undicht {

    namespace tools {

        template<typename number>
        bool overlappingRanges(number range_start_1, number range_end_1, number range_start_2, number range_end_2) {
			/** "touching" ranges dont overlap */

            // making sure the start of the range is before the end
            number s1 = std::min(range_start_1, range_end_1);
            number e1 = std::max(range_start_1, range_end_1);

            number s2 = std::min(range_start_2, range_end_2);
            number e2 = std::max(range_start_2, range_end_2);

            // cases in which the ranges do not overlap (is it really that easy ?)
            if((e1 <= s2) || (e2 <= s1)) {

                return false;
            }

            return true;
        }


    }

}


#endif // math_tools

#ifndef BUFFER_LAYOUT_H_INCLUDED
#define BUFFER_LAYOUT_H_INCLUDED

#include <core/types.h>
#include <vector>

namespace undicht {

    namespace core {

        /// a class that describes the data type stored in a buffer
        class BufferLayout {
            public:

                std::vector<int> m_types; /// the primitive types that the stored struct is made out of

                unsigned int getTotalSize() const; /// returns the sum of the size of all the primtive types in m_types

                BufferLayout() {}
                BufferLayout(const std::vector<int>& types) : m_types(types) {}
                virtual ~BufferLayout() {}

        };

    }

}



#endif // BUFFER_LAYOUT_H_INCLUDED

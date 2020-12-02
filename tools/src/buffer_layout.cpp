#include <core/buffer_layout.h>


namespace undicht {

    namespace core {

        unsigned BufferLayout::getTotalSize() const{


            unsigned int total_size = 0;

            for(const int& type : m_types ) {

                total_size += getSizeOfType(type);

            }

            return total_size;
        }


    } // core

} // undicht

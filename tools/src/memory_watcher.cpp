#include <core/memory_watcher.h>
#include <iostream>


namespace undicht {

    namespace core {

        std::vector<int> MemoryWatcher::s_user_count;
        std::vector<int> MemoryWatcher::s_deleted_user_counts;


        /////////////////////////////////////////// constructor / destructor /////////////////////////////////////////


        MemoryWatcher::MemoryWatcher() {
            //ctor
        }

        MemoryWatcher::~MemoryWatcher() {
            //dtor
        }


        ///////////////////////////////////// automating the user count on copy ////////////////////////////////////////


        void MemoryWatcher::operator= (const MemoryWatcher& c) {



            if(s_user_count.size() > (unsigned int) c.m_user_id) {

                s_user_count.at(c.m_user_id) += 1;
                m_user_id = c.m_user_id;

                //std::cout << "copying data of user " << m_user_id << "\n";
                //std::cout << "user count: " << s_user_count.at(c.m_user_id) << "\n";
            } else {

                if(c.m_user_id != -1) {

                    std::cout << "failed to add user to " << c.m_user_id << "\n";
                    std::cout << "user count does not exist " << "\n";

                }

            }

        }

        MemoryWatcher::MemoryWatcher(const MemoryWatcher& c) {

            *this = c;

        }


        /////////////////////////////////////// using the user count ///////////////////////////////////////////////////

        void MemoryWatcher::createNewUserCount() {
            /** reserves a new user count & updates m_id
            * should be called when the data to be watched over is initialized or changed
            * when there was old data stored in the object it should also be checked if this data
            * has any users left */


            // looking for old user counts that can be reused
            if(s_deleted_user_counts.size()) {
                m_user_id = s_deleted_user_counts.back();
                s_user_count.at(m_user_id) = 1;
                s_deleted_user_counts.pop_back();
                return;
            }

            m_user_id = s_user_count.size();
            s_user_count.push_back(1);

        }


        int MemoryWatcher::deleteUser() {
            /** deletes the user & returns the number of users left over
            * if no user (return: 0) is left, the data should be deleted */

            if(m_user_id == -1) {
                // the data was not initialized
                // so it should not get deleted
                return 1;
            }

            if(s_user_count.size() > (unsigned int) m_user_id) {
                s_user_count.at(m_user_id) -= 1;

                if(!s_user_count.at(m_user_id)) // user count reached 0, can be reused
                    s_deleted_user_counts.push_back(m_user_id);

            } else {
                std::cout << "failed to delete user " << m_user_id << "\n";
                std::cout << "user count does not exist " << "\n";
            }

            int old_user_id = m_user_id;
            m_user_id = -1; // object is deleted

            return s_user_count.at(old_user_id);
        }


    } // core

} // undicht

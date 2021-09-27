#ifndef UNDICHT_THREAD_H
#define UNDICHT_THREAD_H

#include <thread>
#include <chrono>

namespace undicht {

    namespace tools {

        class Thread {

            private:

                std::thread* m_thread = 0;

            public:

                /** what this class does:
                *
                * pass the thread your function is running onto that function
                * then call detach() once the function is finished.
                * the thread then will no longer be joinable(), thats how the class knows it has finished
                * this classes destructor will be waiting until the thread has finished */

                // i dont even know half of what i am doing here

                template<typename Callable, typename... Args>
                Thread(Callable&& c, Args&&... args) {

                    m_thread = new std::thread([&] {
                        c(args...);
                        m_thread->detach();
                    });

                }


                virtual ~Thread() {

                    using namespace std::chrono_literals;

                    while(m_thread->joinable()) {
                        // waiting for the thread to finish
                        std::this_thread::sleep_for(10ms);
                    }

                    delete m_thread;
                }

                void join() {
                    // waits until the thread has finished executing its function

                    using namespace std::chrono_literals;

                    if(!m_thread) return;

                    while(m_thread->joinable()) {
                        // waiting for the thread to finish
                        std::this_thread::sleep_for(10ms);
                    }

                }

                bool hasFinished() {
                    if(m_thread)
                        return !m_thread->joinable();

                    return false;
                }


        };

    } // tools

} // undicht

#endif // UNDICHT_THREAD_H

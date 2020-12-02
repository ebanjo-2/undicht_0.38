#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>
#include <core/memory_watcher.h>
#include <sstream>
#include <iostream>

namespace undicht {

    namespace core {

        // can be used to request only certain types of events from the event EventLoggerger
        enum NOTE_TYPE {
            UND_WARNING,
            UND_ERROR,
            UND_MESSAGE
        };



        class Note{
                /** a note object can transport information about an error, warning, message
                * common error codes (in form of an id) returned by external libraries can be automatically translated by this class
                * if they are set as static messages associated with the id */

            private:

                static std::vector<std::string> s_notes;

                int m_message_id = -1; // a note can either store the message directly or as an id which can be used to retrieve the message string from s_notes
                NOTE_TYPE m_type = UND_MESSAGE;
                std::string m_message;
                std::string m_origin; // the file & line from where the event originated


            public:
                // setting static messages

                /** a note object can transport information about an error, warning, message
                * common error codes (in form of an id) returned by external libraries can be automatically translated by this class
                * if they are set as static messages associated with the id */
                static void setStaticMessage(const std::string& message, int id);

                /** a note object can transport information about an error, warning, message
                * common error codes (in form of an id) returned by external libraries can be automatically translated by this class
                * @return the id with which the message is associated */
                static int setStaticMessage(const std::string& message);

            public:

                /** the message transported will be in the s_notes vector at the id */
                void setMessage(int id);
                void setMessage(const char* message);
                void setMessage(char* message);
                void setMessage(const std::string& message); // to transport temporary messages

                /** the file & line from where the event originated */
                void setOrigin(const std::string& origin);
                void setType(NOTE_TYPE type);

                /** @return the message transported by the event object*/
                const std::string& getMessage() const;

                int getMessageId();
                NOTE_TYPE getType() const;

                const std::string& getOrigin() const;


                Note();
                Note(NOTE_TYPE type, const std::string& message, const std::string& origin);
                Note(NOTE_TYPE type, int message_id, const std::string& origin);
                virtual ~Note();


        };


    } // core

} // undicht

#endif // EVENT_H

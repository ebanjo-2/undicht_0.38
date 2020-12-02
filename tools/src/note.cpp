#include "note.h"
#include <core/string_tools.h>

namespace undicht {

    namespace core {

        std::vector<std::string> Note::s_notes;

        Note::Note() {
            setMessage(0);
        }

        Note::Note(NOTE_TYPE type, const std::string& message, const std::string& origin) {

            setType(type);
            setMessage(message);
            setOrigin(origin);

        }

        Note::Note(NOTE_TYPE type, int message_id, const std::string& origin) {
            setType(type);
            setMessage(message_id);
            setOrigin(origin);
        }

        Note::~Note() {

        }


        //////////////////////////////////////////// managing the static storage of the Note messages ///////////////////////////////////////




        void Note::setStaticMessage(const std::string& message, int id) {
            /** a note object can transport information about an error, warning, message
            * common error codes (in form of an id) returned by external libraries can be automatically translated by this class
            * if they are set as static messages associated with the id */

            if(s_notes.size() <= id ) {
                s_notes.resize(id + 1);
            }

            s_notes.at(id) = message;

        }

        int Note::setStaticMessage(const std::string& message) {
            /** a note object can transport information about an error, warning, message
            * common error codes (in form of an id) returned by external libraries can be automatically translated by this class
            * @return the id with which the message is associated */

            s_notes.push_back(message);
            return s_notes.size() - 1;
        }

        ///////////////////////////////////////////////////// changing and accessing the Note objects message ////////////////////////////////

        void Note::setMessage(int id) {
            /** the message transported will be in the s_notes vector at the id */

            m_message_id = id;
        }


        void Note::setMessage(const std::string& message) {
            // to transport temporary messages
            m_message = message;
        }

        void Note::setMessage(const char* message) {
            // to transport temporary messages
            if(message)
                m_message = toStr(message);
        }

        void Note::setMessage(char* message) {
            // to transport temporary messages
            if(message)
                m_message = toStr(message);
        }

        void Note::setOrigin(const std::string& origin) {
            m_origin = origin;
        }

        void Note::setType(NOTE_TYPE type) {
            m_type = type;
        }


        const std::string& Note::getMessage() const {
            /** @return the message transported by the Note object*/

            if(m_message_id && (m_message_id < s_notes.size())) {

                return s_notes.at(m_message_id);
            }

            return m_message; // should be something like "Note:ERROR: seems like there is no message with this Note"
        }

        int Note::getMessageId() {

            return m_message_id;
        }

        const std::string& Note::getOrigin() const {

            return m_origin;
        }

        NOTE_TYPE Note::getType() const {

            return m_type;
        }



    } // core

} // undicht

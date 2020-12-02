#ifndef EventLogger_H
#define EventLogger_H

#include <vector>
#include "note.h"
#include "string_tools.h"

namespace undicht {

    namespace tools {


        class EventLogger {
            /** the class through which undicht is able to communicate with the user application
            * to provide a general EventLoggerging of the Notes in the engine, error messages, warnings, ... */

        private:
                // all Notes of the undicht engine
                static std::vector<Note> s_error_Notes;
                static std::vector<Note> s_warning_Notes;
                static std::vector<Note> s_message_Notes;

        public:
            // storing Notes on the Note stacks

                static void storeNote(const Note& Note);

        public:
            // getting Notes

                /** returns all Notes with the Note type */
                static const std::vector<Note>& getNotes(NOTE_TYPE type);

                /** clears the stack of Notes with this type */
                static void clearNotes(NOTE_TYPE type);

                /** returns the last Note with the Note type
                * will then clear this Note from the stack of Notes with this type */
                static Note getLastNote(NOTE_TYPE type);



        };

    } // tools

} // undicht

#endif // EventLogger_H

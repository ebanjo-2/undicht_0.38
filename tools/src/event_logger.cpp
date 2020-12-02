#include "event_logger.h"

namespace undicht {

    namespace tools {

        std::vector<Note> EventLogger::s_error_Notes;
        std::vector<Note> EventLogger::s_warning_Notes;
        std::vector<Note> EventLogger::s_message_Notes;


        ///////////////////////////////// storing Notes on the Note stacks/////////////////////////////////////

        void EventLogger::storeNote(const Note& Note) {

            std::cout << "Debug message: " << Note.getMessage() << "\n";
            std::cout << "from here " << Note.getOrigin() << "\n";

            if(Note.getType() == UND_ERROR) {
                s_error_Notes.push_back(Note);
            } else if (Note.getType() == UND_MESSAGE) {
                s_message_Notes.push_back(Note);
            } else if (Note.getType() == UND_WARNING) {
                s_warning_Notes.push_back(Note);
            }

        }


        /////////////////////////////////////////////// getting Notes ///////////////////////////////////////////


        const std::vector<Note>& EventLogger::getNotes(NOTE_TYPE type) {
            /** returns all Notes with the Note type */

            if(type == UND_ERROR) {
                return s_error_Notes;
            } else if (type == UND_MESSAGE) {
                return s_message_Notes;
            } else if (type == UND_WARNING) {
                return s_warning_Notes;
            }

        }

        void EventLogger::clearNotes(NOTE_TYPE type){
            /** clears the stack of Notes with this type */

            if(type == UND_ERROR) {
                s_error_Notes.clear();
            } else if (type == UND_MESSAGE) {
                s_message_Notes.clear();
            } else if (type == UND_WARNING) {
                s_warning_Notes.clear();
            }

        }

        Note EventLogger::getLastNote(NOTE_TYPE type) {
            /** returns the last Note with the Note type
            * will then clear this Note from the stack of Notes with this type */

            Note requested_Note;

            if(type == UND_ERROR) {

                if(s_error_Notes.size()){
                    requested_Note = s_error_Notes.back();
                    s_error_Notes.pop_back();
                }

            } else if (type == UND_MESSAGE) {

                if(s_message_Notes.size()) {
                    requested_Note =  s_message_Notes.back();
                    s_message_Notes.pop_back();
                }

            } else if (type == UND_WARNING) {

                if(s_warning_Notes.size()) {
                    requested_Note =  s_warning_Notes.back();
                    s_warning_Notes.pop_back();
                }

            }

            return requested_Note;
        }


    } // tools

} // undicht

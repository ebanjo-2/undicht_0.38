#include "file.h"
#include <sys/stat.h> // to check if a file exists
#include <stdio.h> // to delete files

#include "event_logger.h"

/// the implementation of the "file" class using fstream, which is a default c++ class

namespace undicht {

    namespace tools {


        File::File() {

        }

        File::File(const std::string& file_name, bool open_binary) {

            open(file_name, open_binary);
        }


        File::~File() {

            close();
        }

        ////////////////////////////////////// opening / closing files ////////////////////////////////////


        bool File::open(const std::string& file_name, bool open_binary) {
            /** opens a file from the hard drive
            * @return whether or not the file could be opened */

            close();

            m_file_name = file_name;

            // opening the file
			if (open_binary) {

				m_file_stream.open(file_name, std::ios::in | std::ios::out | std::ios::binary); // open for both input and output
			}
			else {

				m_file_stream.open(file_name, std::ios::in | std::ios::out); // open for both input and output
			}


            if(m_file_stream.fail()) {
                return false;
            }

            m_file_size = getSize();

            return true;
        }


        void File::close() {
            /// to be called when the file is no longer used

            m_file_size = -1; // -1 for not open
            m_file_stream.close();
        }


        void File::createFile() {
            /// initializes a File with the file_name provided with the open() function

            // dont think this would work
            if(!m_file_stream.is_open()) {

                // creating the file
                m_file_stream.open(m_file_name); // open for both input and output


                m_file_size = 0;

            }

        }


        void File::deleteFile() {
            /// deletes the file

            close();
            if(remove(m_file_name.data()) != 0 ) {
                // something went wrong

                EventLogger::storeNote(Note(UND_ERROR, "FILE:ERROR: failed to delete file " + m_file_name, UND_CODE_ORIGIN));

            }
        }


        const std::string& File::getFileName() const {
            /// @return the name of the file opened

            return m_file_name;
        }


        //////////////////////////////////// reading from the file ////////////////////////////////////


        std::string& File::getLine(std::string& line) {
            /// loads the next line from the file (current cursor pos till next line ending)

            std::getline(m_file_stream, line);

            return line;
        }


        std::string File::getLine() {
            // the string containing the line content is local
            std::string line;

            return getLine(line);
        }


        std::string& File::getLineWithStart(std::string& line, const std::string& start) {
            /**  @return the line if it starts with the string provided as the second param
            * if not, the string will be erased, the "cursor" will still be moved */

            // getting the line
            getLine(line);

            // checking if it starts with the start
            if(line.substr(0, start.length()).compare(start)) {
                // line does not start with the start string
                line.clear();
            }

            return line;
        }


        std::string File::getLineWithStart(const std::string& start) {

            std::string line;
            getLineWithStart(line, start);

            return line;
        }


        char File::getChar() {
            /// @return the next character in the file

            char c;
            m_file_stream.get(c);

            return c;
        }


        std::string& File::getAll(std::string& loadTo) {
            /** reads the whole content of the file */

            if(getSize() == (size_t)-1) {
                // file not open
                EventLogger::storeNote(Note(UND_ERROR, "failed to load file content: " + getFileName(), UND_CODE_ORIGIN));
                return loadTo;
            }

            size_t old_pos = getPosition();
            setPosition(0);

            char* buffer = new char[getSize()];

            m_file_stream.read(buffer, getSize());
            loadTo.insert(0, buffer, getSize());

            delete[] buffer;

            setPosition(old_pos);

            return loadTo;
        }


        unsigned int File::getAll(char*& loadTo) {
            /** allocates new memory for the char array
            * which needs to be deleted once its no longer used
            * @return the new size of loadTo */

            size_t file_size = getSize();

            if(file_size == (size_t)-1) {
                // file not open
                EventLogger::storeNote(Note(UND_ERROR, "failed to load file content: " + getFileName(), UND_CODE_ORIGIN));
                return 0;
            }

            loadTo = new char[file_size];

            size_t old_pos = getPosition();
            setPosition(0);

            m_file_stream.read(loadTo, file_size);

            setPosition(old_pos);

            return file_size;
        }

		/////////////////////   ////// reading / writing a binary file ////////////////////////////////////

		void File::readBinary(char* loadTo, unsigned int element_size, int count) {

			size_t data_size = element_size * count;

			m_file_stream.read(loadTo, data_size);

		}

		void File::writeBinary(char* data, unsigned int element_size, int count) {

			size_t data_size = element_size * count;

			// types bigger than 1 byte will be written to the file as they are stored in the data buffer
			// which probably means high bytes before low bytes
			// i think this is called big endian
			m_file_stream.write(data, data_size);

			if(getWritePosition() > m_file_size)
                m_file_size = getWritePosition();
		}

        //////////////////////////////////// writing to the file ////////////////////////////////////


        void File::eraseAll(bool binary) {
            /// erases the content from the file

            close();
            m_file_stream.open(m_file_name, std::ofstream::out | std::ofstream::in | std::ofstream::trunc); // deletes all its content

            // using open() to reopen the file correctly
            open(m_file_name, binary);
        }


        void File::write(const std::string& str) {
            /**  writes the string to the file
            * the "cursor" will be moved behind the inserted string */

        }


        //////////////////////////////////// getting information about the state of the file ////////////////////////////////////


        size_t File::getPosition() {
            /** @return an index into the file
            * representing the position of the "cursor" which is used to read */

            return m_file_stream.tellg();
        }

        void File::setPosition(const size_t& position) {

            m_file_stream.seekg(position);
        }

		size_t File::getWritePosition() {

			return m_file_stream.tellp();
		}

		void File::setWritePosition(const size_t& position) {

			m_file_stream.seekp(position, std::ios_base::beg);
		}

        bool File::eof()  const {
            /** @return whether the "cursor" has reached the end of the file */

            return m_file_stream.eof();
        }


        size_t File::getSize()  const {
            /** @return the size of the file in bytes (equal to the number of characters) */

            if(getFileName().compare("") && (m_file_size == (size_t)-1)) {
                // if the file size was not determined jet
                // source: https://stackoverflow.com/questions/5840148/how-can-i-get-a-files-size-in-c
                struct stat stat_buf;
                int rc = stat(getFileName().c_str(), &stat_buf);

                return rc == 0 ? stat_buf.st_size : -1;
            }

            return m_file_size;
        }

        void File::clearErrors() {

            m_file_stream.clear();
        }


    } // tools

} // undicht

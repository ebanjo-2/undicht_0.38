#include "xml_file.h"

const int XML_START_TAG = 1;
const int XML_END_TAG = 2;
const int XML_ONE_LINE_ELEMENT = 3;


namespace undicht {

	namespace tools {

		XmlFile::XmlFile() {
			//ctor
		}

		XmlFile::XmlFile(const std::string& file_name) {
			//ctor

			open(file_name);
		}

		XmlFile::~XmlFile() {
			//dtor
		}

		bool XmlFile::open(const std::string& file_name) {

			if (!open(file_name)) {
				// failed to open
				close();
				return false;
			}

			m_child_elements.clear();


			// the line in which the xml information is stored
			std::string xml_info = getLine();

			setData(xml_info);
			m_last_element = this;


			while (!eof()) {

				if (!readNextLine()) {

					if (eof()) {
						// a valid end to the reading
						close();
						return true;
					}
					// reached invalid line
					close();
					return false;
				}

			}
			 
			close();
			return true; // it could have actually worked
		}


		/////////////////////////////////// reading line by line /////////////////////////////////////


		bool XmlFile::readNextLine() {
			/// reads and processes the next line of the file

			std::string line = getLine();
			if (!removeIndentation(line)) return 0; // no tag in line (line invalid)

			int line_tag_type = getLineTagType(line);

			if (line_tag_type == XML_START_TAG) {

				m_last_element = m_last_element->addChildElement();
				m_last_element->setData(line);

			}
			else if (line_tag_type == XML_END_TAG) {

				m_last_element = m_last_element->getParentElement();

				if (!m_last_element) {
					// the highest element should be the xml info element
					// which has no end tag, so there is something wrong with the file
					return false;
				}

			}
			else if (line_tag_type == XML_ONE_LINE_ELEMENT) {

				m_last_element->addChildElement()->setData(line);

			}
			else if (line_tag_type == 0) {
				// invalid

				return false;
			}

			return true;
		}


		int XmlFile::getLineTagType(const std::string& tag) {
			/** @return the type of element tag stored in the line
			* one of XML_START_TAG, XML_END_TAG, XML_ONE_LINE_ELEMENT
			* @return 0 if invalid line */


			if (tag.at(0) == '<') {
				if (tag.back() == '>') {
					// it seems to be a valid xml tag
					if (tag.at(1) == '/') {
						// end tag

						return XML_END_TAG;

					}
					else if ((tag.find("/>") != std::string::npos) || (tag.find("</") != std::string::npos)) {
						// @if one line element (tag only) || one liner with content

						return XML_ONE_LINE_ELEMENT;
					}

					return XML_START_TAG;
				}
			}


			return 0; // invalid line
		}

		bool XmlFile::removeIndentation(std::string& str) {
			/// removes all free space characters in front of the '<'

			int tag_start = str.find("<");
			if (tag_start == std::string::npos) {
				// no tag
				return false;
			}

			str = str.substr(tag_start);

			return true;
		}

	} // tools

} // undicht

#ifndef WORLD_FILE_H
#define WORLD_FILE_H


#include <stdint.h>
#include <files/file.h>

#include <world/chunk.h>

namespace cell {

	class WorldFile : public undicht::tools::File {

	public:
		// file layout

		struct Header {
			// first 8 bytes of every file
			char type0 = 'U';
			char type1 = 'n';
			char type2 = 'd';
			char type3 = '\0';
			int32_t version = 1;

			// every world file stores 16 * 16 * 16 chunks
			// this is the position of the chunk with the smallest
			// x, y, and z coordinates
            int32_t chunk_x = 0, chunk_y = 0, chunk_z = 0;
		};

		struct ChunkRegister {
			// located right after the Header
			// there is a fixed number of ChunkRegisters in the file (16 * 16 * 16 = 4096)

			uint32_t offset = sizeof(Header) + 4096 * sizeof(ChunkRegister); // the position where the chunk is located in the file (in bytes from the very start of the file)
			uint32_t byte_size = 0; // the size of the chunk

			bool operator<(const ChunkRegister& r) {

                if(offset < r.offset)
                    return true;

                if((offset == r.offset) && (byte_size < r.byte_size))
                    return true;

                //return (offset + byte_size) < (r.offset + r.byte_size);
                return false;
			}

		};

		struct CellStruct {
			// the cells are located after the reserved memory for the ChunkRegisters
			// the cells are grouped in chunks, but there may be no space between chunks

			// pray that this struct ends up being 8 bytes

			uint8_t pos0x, pos0y, pos0z;
			uint8_t pos1x, pos1y, pos1z;

			uint16_t mat;
		};

    public:

        Header m_header;
        std::array<ChunkRegister, 4096> m_registers;
        std::array<ChunkRegister, 4096> m_used_mem;

    public:

		virtual bool open(const std::string& file_name);

    public:
        // managing the storage of chunks and their registers

        /** there are 4096 ChunkRegisters in the file
        * where they are stored depends on the chunk they belong to
        * @return -1, if the position is outside of this files range */
        int getRegID(const glm::ivec3& pos);

        /** finds a position in the file where it is safe to write byte_size bytes
        * without overwriting existing chunk data
        * (returns the end of the file as a position if no other sufficiently large space can be found) */
        int findFreeSpace(int byte_size);

	private:
		// writing

		virtual void writeHeader(unsigned int version = 1, const glm::ivec3& origin = glm::ivec3(0,0,0));

    public:

        /** @param pos: should be within 16 chunks of this files origin chunk
        * otherwise false is going to be returned and the chunk will no be written */
		virtual bool writeChunk(const Chunk& c, const glm::ivec3& pos);

	private:
		// reading

		/** @return false, if the file does not start with a correct Header */
		virtual bool readHeader();

    public:

		virtual bool readChunk(Chunk& loadTo, const glm::ivec3& pos);
	};


} // cell


#endif // !WORLD_FILE_H

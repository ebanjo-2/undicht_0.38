#ifndef WORLD_FILE_H
#define WORLD_FILE_H


#include <stdint.h>
#include <files/file.h>

#include <world/world_chunk.h>

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

		};

		struct ChunkRegister {
			// located right after the Header
			// there is a fixed number of ChunkRegisters in the file (16 * 16 * 16 = 4096)

			uint32_t offset = sizeof(Header) + 4096 * sizeof(ChunkRegister); // the position where the chunk is located in the file (in bytes from the very start of the file)
			uint32_t byte_size = 0; // the size of the chunk
			uint32_t opt_need = 0;

			bool operator<(const ChunkRegister& r) {
                // used to sort registers by the memory block they use to store their cells
                // when the registers are sorted this way, its easy to find unused memory between them

                if(offset < r.offset)
                    return true;

                // this register might be uninitialized and r may use the first memory location for cells
                if((offset == r.offset) && (byte_size < r.byte_size))
                    return true;

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


        /** finds a position in the file where it is safe to write byte_size bytes
        * without overwriting existing chunk data
        * (returns the end of the file as a position if no other sufficiently large space can be found) */
        int findFreeSpace(int byte_size);

	private:
		// writing

		virtual void writeHeader(unsigned int version = 1);

    public:

        /** writes the chunk to the file and uses the register id
        * which can be uesd to retrieve the chunk data again */
		virtual bool writeChunk(const WorldChunk& c, unsigned int register_id = 0);

	private:
		// reading

		/** @return false, if the file does not start with a correct Header */
		virtual bool readHeader();

    public:

		virtual bool readChunk(WorldChunk& loadTo, unsigned int register_id = 0);
	};


} // cell


#endif // !WORLD_FILE_H

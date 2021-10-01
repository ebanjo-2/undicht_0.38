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
			char type[4] = { 'U', 'n', 'd', '\0' };
			int32_t version = 1;
		};

		struct ChunkRegisterHeader {
			// located right after the header

			// the number of chunks stored in the file
			int32_t num_chunks = 0;
			
			// since the chunk registers are located at the start of the file,
			// adding new registers would require everything following them to be moved backwards
			// thats why there will be more chunk registers reserved then needed
			int32_t num_chunks_reserved = 0; // number of chunk register spaces reserved (used + unused)
		};

		struct ChunkRegister {
			// located right after the ChunkRegisterHeader
			// there is a ChunkRegister for every Chunk in the file
			// + some more space reserved for future ones
			// they contain information that makes finding the right chunk easier

			int32_t chunk_x = 0, chunk_y = 0, chunk_z = 0;
			int32_t offset = 0; // the position where the chunk is located in the file (in bytes from the very start of the file)
			int32_t byte_size = 0; // the size of the chunk

		};

		struct Cell {
			// the cells are located after the reserved memory for the ChunkRegisters
			// the cells are grouped in chunks, but there may be no space between chunks

			// pray that this struct ends up being 8 bytes

			uint8_t pos0x, pos0y, pos0z;
			uint8_t pos1x, pos1y, pos1z;

			uint16_t mat;
		};

	public:

		virtual void open(const std::string& file_name);

		virtual void reserveRegisters(int num);

	public:
		// writing

		virtual void writeHeader(unsigned int version = 1);

		virtual void writeChunkRegisterHeader(unsigned int num_chunks = 0);

		virtual void writeChunk(const Chunk& c);

	public:
		// reading

		virtual Header readHeader();

		virtual ChunkRegisterHeader readChunkRegisterHeader();

		/** the pointer you are passing should have enough mem reserved to fit all the chunk registers
		* to get the total number of registers call readChunkRegisterHeader()*/
		virtual void readChunkRegisters(ChunkRegister* registers, int num);

		virtual void readChunk(const glm::ivec3& chunk_pos, Chunk& loadTo);
	};


} // cell


#endif // !WORLD_FILE_H

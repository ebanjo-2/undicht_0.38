#include "world_file.h"

namespace cell {

	using namespace undicht;
	using namespace tools;

	// layouts for parts of the file
	const BufferLayout header_layout({ UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT32 });
	const BufferLayout chunk_register_header_layout({ UND_UINT32, UND_UINT32 });
	const BufferLayout chunk_register_layout({ UND_UINT32, UND_UINT32, UND_UINT32, UND_UINT32, UND_UINT32 });
	const BufferLayout cell_layout({ UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_INT16 });


	void WorldFile::open(const std::string& file_name) {

		File::open(file_name, true);

	}

	void WorldFile::reserveRegisters(int num) {

		ChunkRegisterHeader register_info = readChunkRegisterHeader();

		// moving the cell data
		int cell_data_size = 
		char* cell_data = new char[File::getSize()];
		File::readBinary(cell_data, { UND_UINT8 }, );


		register_info.num_chunks_reserved = num;
		register_info.num_chunks = std::min(register_info.num_chunks, num);


	}

	///////////////////////////////////////////// writing /////////////////////////////////////////////

	void WorldFile::writeHeader(unsigned int version) {

		Header h;
		h.version = version;

		setWritePosition(0);
		File::writeBinary((char*)&h, header_layout);

	}		

	void WorldFile::writeChunkRegisterHeader(unsigned int num_chunks) {

		ChunkRegisterHeader h;
		h.num_chunks = num_chunks;

		setWritePosition(0 + sizeof(Header));
		File::writeBinary((char*)&h, chunk_register_header_layout);

	}

	
	///////////////////////////////////////////// reading /////////////////////////////////////////////

	WorldFile::Header WorldFile::readHeader() {

		Header h;

		setPosition(0);
		File::readBinary((char*)&h, header_layout);

		return h;
	}

	WorldFile::ChunkRegisterHeader WorldFile::readChunkRegisterHeader() {

		ChunkRegisterHeader h;

		setPosition(0 + sizeof(Header));
		File::readBinary((char*)&h, chunk_register_header_layout);

		return h;
	}

	void WorldFile::readChunkRegisters(WorldFile::ChunkRegister* registers, int num) {

		setPosition(0 + sizeof(Header) + sizeof(ChunkRegisterHeader));
		File::readBinary((char*)&registers, chunk_register_layout, num);

	}

} // cell
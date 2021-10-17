#include "world_file.h"
#include <algorithm>

namespace cell {

	using namespace undicht;
	using namespace tools;


	bool WorldFile::open(const std::string& file_name) {

        bool opened = File::open(file_name, true);

        //std::cout << "here" << "\n";

        if(!opened) return false;

        eraseAll(true);

        bool correct_header = readHeader();

        if(!correct_header) {
            clearErrors();
            writeHeader(2);
        }

		return true;
	}


    /////////////////////////// managing the storage of chunks and their registers /////////////////////////////////


    int WorldFile::findFreeSpace(int byte_size) {
        /** finds a position in the file where it is safe to write byte_size bytes
        * without overwriting existing chunk data
        * (returns the end of the file as a position if no other sufficiently large space can be found) */

        m_used_mem = m_registers;

        std::sort(m_used_mem.begin(), m_used_mem.end());

        for(int i = 0; i < 4095; i++) {

            int current_chunk_end = m_used_mem[i].offset + m_used_mem[i].byte_size;
            int empty_space_size = m_used_mem[i+1].offset - current_chunk_end;

            if(empty_space_size >= byte_size) {

                return current_chunk_end;
            }

        }

        // no unused space found, return end of file as position
        return m_used_mem[4095].offset + m_used_mem[4095].byte_size;
    }

	///////////////////////////////////////////// writing /////////////////////////////////////////////

	void WorldFile::writeHeader(unsigned int version) {

        m_header = Header();
		m_header.version = version;


		setWritePosition(0);
		File::writeBinary((char*)&m_header, sizeof(Header));

		// reserving space for the chunk registers
        setWritePosition(sizeof(Header));

        for(int i = 0; i < 4096; i++)
            m_registers[i] = ChunkRegister();

		File::writeBinary((char*)&m_registers, sizeof(ChunkRegister), 4096);
	}

    bool WorldFile::writeChunk(const Chunk& c, unsigned int register_id) {
        /** @param pos: should be within 16 chunks of this files origin chunk
        * otherwise false is going to be returned and the chunk will no be written */

        if((register_id < 0) || (register_id >= 4096)) return false;

        // updating the register
        ChunkRegister& reg = m_registers[register_id];
        reg.byte_size = 0; // "freeing" the old chunk

        int chunk_size = c.getCellCount() * sizeof(CellStruct);

        reg.offset = findFreeSpace(chunk_size);
        reg.byte_size = chunk_size;


        setWritePosition(sizeof(Header) + register_id * sizeof(ChunkRegister));
        File::writeBinary((char*)&reg, sizeof(ChunkRegister));

        // writing the chunks cells
        setWritePosition(reg.offset);

        for(const Cell& cell : c.m_cells) {

            CellStruct cell_data;
            cell_data.pos0x = cell.m_pos0.x;
            cell_data.pos0y = cell.m_pos0.y;
            cell_data.pos0z = cell.m_pos0.z;

            cell_data.pos1x = cell.m_pos1.x;
            cell_data.pos1y = cell.m_pos1.y;
            cell_data.pos1z = cell.m_pos1.z;

            cell_data.mat = cell.m_material;

            writeBinary((char*)&cell_data, sizeof(CellStruct));
        }

        return true;
    }

	///////////////////////////////////////////// reading /////////////////////////////////////////////

	bool WorldFile::readHeader() {
		/** @return false, if the file does not start with a correct Header */

		setPosition(0);
		File::readBinary((char*)&m_header, sizeof(Header));

		if(std::string(&m_header.type0).compare("Und") || eof()) {

            std::cout << "Failed to read header of file " << m_file_name << "\n";
            return false;
		}

		// reading the chunk registers
        for(int i = 0; i < 4096; i++) {

            if(eof()) return false;

            readBinary((char*)& m_registers[i], sizeof(ChunkRegister));
        }

		return true;
	}

    bool WorldFile::readChunk(Chunk& loadTo, unsigned int register_id) {

        if((register_id < 0) || (register_id >= 4096)) return false;

        ChunkRegister& reg = m_registers[register_id];

        // reading the chunks cells
        setPosition(reg.offset);

        int cell_count = reg.byte_size / sizeof(CellStruct);

        if(!cell_count) return false;

        loadTo.m_cells.resize(cell_count);
        loadTo.m_unused_cells.clear();

        for(int i = 0; i < 4096; i++)
            loadTo.m_mini_chunks[i / 256][(i / 16) % 16][i % 16].clear();


        for(int i = 0; i < cell_count; i++){

            CellStruct cell_data;
            readBinary((char*)& cell_data, sizeof(CellStruct));

            loadTo.m_cells[i].m_pos0 = glm::ivec3(cell_data.pos0x, cell_data.pos0y, cell_data.pos0z);
            loadTo.m_cells[i].m_pos1 = glm::ivec3(cell_data.pos1x, cell_data.pos1y, cell_data.pos1z);
            loadTo.m_cells[i].m_material = cell_data.mat;

            if(cell_data.mat == VOID_CELL) {

                loadTo.m_cells[i].m_visible_faces = 0x00;
            } else {

                loadTo.m_cells[i].m_visible_faces = 0xFF;
            }

            loadTo.addToMiniChunks(i);

        }

        return true;
    }


} // cell

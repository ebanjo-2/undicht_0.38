#include "world_file.h"
#include <algorithm>

namespace cell {

	using namespace undicht;
	using namespace tools;

	// layouts for parts of the file
	const BufferLayout header_layout({ UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT32, UND_INT, UND_INT, UND_INT});
	const BufferLayout chunk_register_layout({ UND_UINT32, UND_UINT32 });
	const BufferLayout cell_struct_layout({ UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT8, UND_UINT16 });


	bool WorldFile::open(const std::string& file_name) {

        bool opened = File::open(file_name, true);

        if(!opened) return false;

        //eraseAll(true);

        bool correct_header = readHeader();

        if(!correct_header) {
            clearErrors();
            writeHeader(2, glm::ivec3(-8,-8,-8));
        }

		return true;
	}


    /////////////////////////// managing the storage of chunks and their registers /////////////////////////////////


    int WorldFile::getRegID(const glm::ivec3& pos) {
        /** @return -1, if the position is outside of this files range */

        glm::ivec3 rel_pos = pos - glm::ivec3(m_header.chunk_x, m_header.chunk_y, m_header.chunk_z);

        if((rel_pos.x > 15) || (rel_pos.y > 15) || (rel_pos.z > 15))
            return -1;

        if((rel_pos.x < 0) || (rel_pos.y < 0) || (rel_pos.z < 0))
            return -1;

        int reg_num = rel_pos.x * 256 + rel_pos.y * 16 + rel_pos.z;

        return reg_num;
    }


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

	void WorldFile::writeHeader(unsigned int version, const glm::ivec3& origin) {

        m_header = Header();
		m_header.version = version;
        m_header.chunk_x = origin.x;
        m_header.chunk_y = origin.y;
        m_header.chunk_z = origin.z;

		setWritePosition(0);
		File::writeBinary((char*)&m_header, header_layout);

		// reserving space for the chunk registers
        setWritePosition(sizeof(Header));

        for(int i = 0; i < 4096; i++)
            m_registers[i] = ChunkRegister();

		File::writeBinary((char*)&m_registers, chunk_register_layout, 4096);
	}

    bool WorldFile::writeChunk(const Chunk& c, const glm::ivec3& pos) {
        /** @param pos: should be within 16 chunks of this files origin chunk
        * otherwise false is going to be returned and the chunk will no be written */

        int reg_id = getRegID(pos);
        if(reg_id == -1) return false;

        // updating the register
        ChunkRegister& reg = m_registers[reg_id];
        reg.byte_size = 0; // "freeing" the old chunk

        int chunk_size = c.getCellCount() * sizeof(CellStruct);

        reg.offset = findFreeSpace(chunk_size);
        reg.byte_size = chunk_size;


        setWritePosition(sizeof(Header) + reg_id * sizeof(ChunkRegister));
        File::writeBinary((char*)&reg, chunk_register_layout);

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

            writeBinary((char*)&cell_data, cell_struct_layout);
        }

        return true;
    }

	///////////////////////////////////////////// reading /////////////////////////////////////////////

	bool WorldFile::readHeader() {
		/** @return false, if the file does not start with a correct Header */

		setPosition(0);
		File::readBinary((char*)&m_header, header_layout);

		if(std::string(&m_header.type0).compare("Und") || eof()) {

            std::cout << "Failed to read header of file " << m_file_name << "\n";
            return false;
		}

		// reading the chunk registers
        for(int i = 0; i < 4096; i++) {

            if(eof()) return false;

            readBinary((char*)& m_registers[i], chunk_register_layout);
        }

		return true;
	}

    bool WorldFile::readChunk(Chunk& loadTo, const glm::ivec3& pos) {

        int reg_id = getRegID(pos);
        if(reg_id == -1) return false;

        ChunkRegister& reg = m_registers[reg_id];

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
            readBinary((char*)& cell_data, cell_struct_layout);

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

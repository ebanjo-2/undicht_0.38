#include "draw_chunk.h"
#include <iostream>

namespace cell {

    std::array<float, 12 * 3 * 8> DrawChunk::s_cube_vertices;
    undicht::tools::BufferLayout DrawChunk::s_cube_layout;

    bool DrawChunk::s_cube_initialized = false;

    using namespace undicht;
    using namespace tools;
    using namespace graphics;


    DrawChunk::DrawChunk() {

        if(!s_cube_initialized) initCubeVertices();

        m_buffer.setLayout(s_cube_layout);
        m_buffer.setData(s_cube_vertices.data(), sizeof(s_cube_vertices));

		BufferLayout instance_layout({ UND_VEC3I, UND_VEC3I, UND_INT });
		m_buffer.setInstanceLayout(instance_layout);

        // init to complete void
        // m_cells.push_back({0,0,0, 255,255,255, 0});
        // this would work, but very large cells are slower to edit

        // thats why the chunk is initialized with smaller cells
        for(int x = 0; x < 5; x++) {
            for(int y = 0; y < 5; y++) {
                for(int z = 0; z < 5; z++) {

					glm::uvec3 pos = glm::uvec3(x * 51, y * 51, z * 51);
					glm::uvec3 siz = glm::uvec3(51, 51, 51);

					addCell(Cell(pos, pos + siz, 0));
                }
            }
        }

		//m_cells.push_back(Cell(glm::uvec3(0,0,0), glm::uvec3(255, 255, 255), 0));

    }

    DrawChunk::~DrawChunk() {


    }

	void DrawChunk::writeToBuffer(const std::vector<Cell>& cells, int offset) {

		std::vector<int> cell_data;

		for (const Cell c : cells) {

			// if (c.mat == 0) continue;
			// dont skip, because it may be used in the future

			if (c.gets_drawn) {

				cell_data.push_back(c.getPoint1()[0]);
				cell_data.push_back(c.getPoint1()[1]);
				cell_data.push_back(c.getPoint1()[2]);

				cell_data.push_back(c.getSize()[0]);
				cell_data.push_back(c.getSize()[1]);
				cell_data.push_back(c.getSize()[2]);

				cell_data.push_back(c.mat);

			}
			else {
				cell_data.insert(cell_data.end(), { 0,0,0, 0,0,0, 0 });
			}

		}

		// each instance of the cube drawn has got its own position, size and material

		m_buffer.setInstanceData(cell_data.data(), cell_data.size() * sizeof(int), offset * 7 * sizeof(int));

	}

	void DrawChunk::writeToBuffer(const Cell& cell, int offset) {

		std::vector<int> cell_data;

		cell_data.push_back(cell.getPoint1()[0]);
		cell_data.push_back(cell.getPoint1()[1]);
		cell_data.push_back(cell.getPoint1()[2]);

		cell_data.push_back(cell.getSize()[0]);
		cell_data.push_back(cell.getSize()[1]);
		cell_data.push_back(cell.getSize()[2]);

		cell_data.push_back(cell.mat);


		m_buffer.setInstanceData(cell_data.data(), cell_data.size() * sizeof(int), offset * 7 * sizeof(int));
	}


	int DrawChunk::addCell(const Cell& c) {
		/** adds the cell to the m_cells vector */

		if (!c.mat) {
			// invisble

			// maybe an already allocated cell can be reused ?
			if (m_invisible_cells_unused.size()) {

				int id = m_invisible_cells_unused.back();
				m_invisible_cells_unused.pop_back();

				m_invisible_cells[-1 * (id + 1)] = c;

				return id;
			} else {

				m_invisible_cells.push_back(c);

				// invisible cells get an negative index starting at -1
				return -1 * m_invisible_cells.size();
			}

		} else {

			// maybe an already allocated cell can be reused ?
			if (m_cells_unused.size()) {

				int id = m_cells_unused.back();
				m_cells_unused.pop_back();

				m_visible_cells[id] = c;
				m_cells_to_update.push_back(id);

				return id;
			} else {

				m_visible_cells.push_back(c);

				// visible cells get a positive index
				int id = m_visible_cells.size() - 1;
				m_cells_to_update.push_back(id);

				return id;
			}

		}

	}

	int DrawChunk::updateCell(const Cell& c, int id) {
		/** updates the existing cell
		* @return: if the cell is turned visible or invisible a new id will be assigned */

		if (id < 0) {
			// accessing an invisible cell

			if (c.mat) {
				// turning it visible

				// removing the cell from the list of invisible cells
				// m_invisible_cells.erase(m_invisible_cells.begin() + -1 * (id + 1));
				// !WARNING! : cant do that, would mess up ids

				m_invisible_cells_unused.push_back(id);
				m_invisible_cells.at(-1 * (id + 1)).setSize(glm::uvec3(0, 0, 0));

				// adding the cell to the list of visible cells
				return addCell(c);
			} else {
				// cell remains invisible

				m_invisible_cells.at(-1 * (id + 1)) = c;

				return id;
			}

		} else {
			// accessing a visible cell

			if (c.mat) {
				// cell remains visible

				m_visible_cells.at(id) = c;
				m_cells_to_update.push_back(id);

				return id;
			} else {
				// turning it invisible

				m_visible_cells.at(id).setSize(glm::uvec3(0, 0, 0));
				m_visible_cells.at(id).mat = 0;

				m_cells_unused.push_back(id);
				m_cells_to_update.push_back(id); // making sure it doesnt get rendered anymore

				return addCell(c);
			}
		}

	}

	const Cell& DrawChunk::getCell(int id) {

		if (id < 0) {
			// invisible cell

			return m_invisible_cells[-1 * (id + 1)];
		} else {
			// visible cell

			return m_visible_cells[id];
		}

	}

    bool DrawChunk::updateCellBuffer() {

		bool needs_resize = getDrawnCellCount() * 7 * sizeof(int) > m_buffer.getInstanceBufferSize();
		/** @return true, if the cell buffer on the video memory needed to be resized */

		if (needs_resize) {
			// need resize
			std::cout << "resizing the buffer to " << getDrawnCellCount() * 7 * sizeof(int) * 2 << "\n";
			m_buffer.resizeInstanceBuffer(getDrawnCellCount() * 7 * sizeof(int) * 2); // *2 
			updateCellBufferTotal();
		} else {
			// no resize needed

			updateCellBufferOnlyNew();
		}

		m_cells_to_update.clear();

		return needs_resize;
    }

	void DrawChunk::updateCellBufferTotal() {
		/** needed when the cell buffer is resized */

		writeToBuffer(m_visible_cells, 0);

	}

	void DrawChunk::updateCellBufferOnlyNew() {
		/** updates the cells that are changed since the last time this was called */

		if (!m_cells_to_update.size()) return;

		for (int i : m_cells_to_update) {

			writeToBuffer(getCell(i), i);
		}

	}

    unsigned int DrawChunk::getDrawnCellCount() {

        return m_visible_cells.size();
    }

	unsigned int DrawChunk::getTotalCellCount() {

		return m_visible_cells.size() + m_invisible_cells.size();
	}


    ////////////////////////////////////////// building cube vertices //////////////////////////////////////////


    // cube mesh build with help from here:
    // https://pastebin.com/DXKEmvap

    // starting with the 4 vertices on the back
#define CUBE_POSITIONS {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,1, 1,0,1, 1,1,1, 0,1,1}
#define CUBE_UVS {0,0, 1,0, 1,1, 0,1}
#define CUBE_NORMALS {0,0,1, 1,0,0, 0,0,-1, -1,0,0, 0,1,0, 0,-1,0}

    //#define CUBE_INDICES {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7, 4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1}
#define CUBE_INDICES {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4, 4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5}
    //#define CUBE_UV_INDICES {0, 1, 3, 3, 1, 2}
#define CUBE_UV_INDICES {0, 3, 1, 3, 2, 1}

    void DrawChunk::initCubeVertices() {


        // building the vertices of a cube


        float positions[] = CUBE_POSITIONS;
        float uvs[] = CUBE_UVS;
        float normals[] = CUBE_NORMALS;

        int indices[] = CUBE_INDICES;
        int uv_indices[] = CUBE_UV_INDICES;


        for (int i = 0; i < 12 * 3; i++) { // going through every vertex of the cube

            s_cube_vertices[i * 8 + 0] = positions[indices[i] * 3 + 0];
            s_cube_vertices[i * 8 + 1] = positions[indices[i] * 3 + 1];
            s_cube_vertices[i * 8 + 2] = positions[indices[i] * 3 + 2];

            s_cube_vertices[i * 8 + 3] = uvs[uv_indices[(i % 6)] * 2 + 0];
            s_cube_vertices[i * 8 + 4] = uvs[uv_indices[(i % 6)] * 2 + 1];

            s_cube_vertices[i * 8 + 5] = normals[(i / 6) * 3 + 0];
            s_cube_vertices[i * 8 + 6] = normals[(i / 6) * 3 + 1];
            s_cube_vertices[i * 8 + 7] = normals[(i / 6) * 3 + 2];
        }

        s_cube_layout = BufferLayout({ UND_VEC3F, UND_VEC2F, UND_VEC3F });

        s_cube_initialized = true;
    }

} // cell

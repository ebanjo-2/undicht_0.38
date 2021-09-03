#include <rendering/cell_renderer.h>
#include <files/file.h>

#include <string_tools.h>
#include <iostream>
#include <geometry/geometry.h>

#include <buffer_layout.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>

#define CELL_SHADER_H getFilePath(UND_CODE_SRC_FILE) + "cell_shader.glsl"


using namespace undicht;
using namespace graphics;
using namespace tools;


namespace cell {

    CellRenderer::CellRenderer() {

        init();
    }

    CellRenderer::~CellRenderer() {

    }

    void CellRenderer::init() {

        std::string shader_src;
        File(CELL_SHADER_H).getAll(shader_src);
        m_shader.loadSource(shader_src);

        std::vector<float> vertices;
        std::vector<int> indices;
        Geometry::useIndices(true);
        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genRectangle(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), vertices, indices);

        m_vertex_buffer.setLayout(BufferLayout({UND_VEC3F, UND_VEC2F}));
        m_vertex_buffer.setData(vertices);
        m_vertex_buffer.setIndexData(indices);

        enableDepthTest(true);
        enableBackFaceCulling(true);
        enableBlending(false);

        m_view.setName("view");
        m_proj.setName("proj");
        m_chunk_pos.setName("chunk_pos");
        m_pos0.setName("pos0");
        m_pos1.setName("pos1");

    }


    void CellRenderer::term() {

    }


    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        m_proj.setData(glm::value_ptr(player_cam.getCameraProjectionMatrix()), UND_MAT4F);
        m_shader.loadUniform(m_proj);

        m_view.setData(glm::value_ptr(player_cam.getViewMatrix()), UND_MAT4F);
        m_shader.loadUniform(m_view);

    }

    void CellRenderer::loadTextureAtlas(const TextureAtlas& atlas) {

        m_shader.loadTexture(atlas.m_tex);

    }

    void CellRenderer::draw(const glm::vec3& cell_pos) {

        glm::vec3 chunk_pos = glm::vec3(0,0,0);
        glm::vec3 pos0 = cell_pos;
        glm::vec3 pos1 = pos0 + glm::vec3(1,1,1);

        m_chunk_pos.setData(glm::value_ptr(chunk_pos), UND_VEC3F);
        m_pos0.setData(glm::value_ptr(pos0), UND_VEC3F);
        m_pos1.setData(glm::value_ptr(pos1), UND_VEC3F);

        m_shader.loadUniform(m_chunk_pos);
        m_shader.loadUniform(m_pos0);
        m_shader.loadUniform(m_pos1);

        submit(&m_shader);
        submit(&m_vertex_buffer);

        Renderer::draw();

    }

    void CellRenderer::draw(const Cell& c) {

        glm::vec3 pos0 = glm::vec3(c.m_pos0);
        glm::vec3 pos1 = glm::vec3(c.m_pos1);

        m_pos0.setData(glm::value_ptr(pos0), UND_VEC3F);
        m_pos1.setData(glm::value_ptr(pos1), UND_VEC3F);

        m_shader.loadUniform(m_pos0);
        m_shader.loadUniform(m_pos1);

        submit(&m_shader);
        submit(&m_vertex_buffer);

        Renderer::draw();
    }


    void CellRenderer::draw(const Chunk& c, const glm::vec3& chunk_pos) {

        m_chunk_pos.setData(glm::value_ptr(chunk_pos), UND_VEC3F);
        m_shader.loadUniform(m_chunk_pos);

        for(const Cell& cell : c.m_cells) {

            draw(cell);

        }

    }



} // cell

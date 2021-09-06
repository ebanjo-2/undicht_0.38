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

        enableDepthTest(true);
        enableBackFaceCulling(true);
        enableBlending(false);

        m_view.setName("view");
        m_proj.setName("proj");
        m_chunk_pos.setName("chunk_pos");

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

    void CellRenderer::draw(Chunk& c, const glm::vec3& chunk_pos) {

        m_chunk_pos.setData(glm::value_ptr(chunk_pos), UND_VEC3F);
        m_shader.loadUniform(m_chunk_pos);

        submit(&c.m_vertex_buffer);
        submit(&m_shader);

        Renderer::draw(c.getCellCount());

    }



} // cell

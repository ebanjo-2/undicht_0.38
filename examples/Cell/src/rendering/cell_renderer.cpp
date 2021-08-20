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
        Geometry::genCuboid(glm::vec3(0.5f,0.5f,0.5f), glm::vec3(-0.5f,-0.5f,-0.5f), vertices, indices);

        m_vertex_buffer.setLayout(BufferLayout({UND_VEC3F}));
        m_vertex_buffer.setData(vertices);
        m_vertex_buffer.setIndexData(indices);

        enableDepthTest(true);
        enableBackFaceCulling(true);
        enableBlending(false);

        m_view.setName("view");
        m_proj.setName("proj");
        m_pos.setName("pos");

    }


    void CellRenderer::term() {

    }


    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        m_proj.setData(glm::value_ptr(player_cam.getCameraProjectionMatrix()), UND_MAT4F);
        m_shader.loadUniform(m_proj);

        m_view.setData(glm::value_ptr(player_cam.getViewMatrix()), UND_MAT4F);
        m_shader.loadUniform(m_view);

    }

    void CellRenderer::draw(const glm::vec3& cell_pos) {

        m_pos.setData(glm::value_ptr(cell_pos), UND_VEC3F);
        m_shader.loadUniform(m_pos);

        submit(&m_shader);
        submit(&m_vertex_buffer);

        Renderer::draw();

    }


} // cell

#include "cell_final_renderer.h"

#include <files/file.h>
#include <string_tools.h>
#include <types.h>

#include <geometry/geometry.h>

#include <glm/gtc/type_ptr.hpp>

#define CELL_FINAL_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_final_shader.glsl"

namespace cell {

    using namespace undicht;
    using namespace graphics;
    using namespace tools;

    CellFinalRenderer::CellFinalRenderer() {

    }

    CellFinalRenderer::~CellFinalRenderer() {

    }

    void CellFinalRenderer::init() {

        // initializing the shader
        std::string shader_src;
        File(CELL_FINAL_SHADER).getAll(shader_src);
        m_shader.loadSource(shader_src);

        // creating the screen quad
        std::vector<float> vertices;
        std::vector<int> indices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genRectangle(glm::vec2(-1.0f,1.0f), glm::vec2(1.0f,-1.0f), vertices, indices);

        m_screen_quad.setLayout(BufferLayout({UND_VEC2F, UND_VEC2F}));
        m_screen_quad.setData(vertices);
        m_screen_quad.setIndexData(indices);

        // uniforms
        // camera related
        m_view.setName("view");
        m_proj.setName("proj");

        m_inv_proj.setName("inv_proj");
        m_inv_view.setName("inv_view");

        // Renderer settings

        submit(&m_shader);
        submit(&m_screen_quad);

        enableDepthTest(true);
        enableBackFaceCulling(false);
        enableBlending(false);
    }

    void  CellFinalRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        const glm::mat4& view = player_cam.getViewMatrix();
        const glm::mat4& proj = player_cam.getCameraProjectionMatrix();

        glm::mat4 inv_view = glm::inverse(view);
        glm::mat4 inv_proj = glm::inverse(proj);

        m_view.setData(glm::value_ptr(proj), UND_MAT4F);
        m_proj.setData(glm::value_ptr(view), UND_MAT4F);

        m_inv_view.setData(glm::value_ptr(inv_view), UND_MAT4F);
        m_inv_proj.setData(glm::value_ptr(inv_proj), UND_MAT4F);

        m_shader.loadUniform(m_view);
        m_shader.loadUniform(m_proj);
        m_shader.loadUniform(m_inv_view);
        m_shader.loadUniform(m_inv_proj);
    }

    void  CellFinalRenderer::loadGeometry(const undicht::graphics::Texture& geometry_texture, const undicht::graphics::Texture& depth_map) {
        /** the name of the geometry_texture should be "geo_texture" */

        m_shader.loadTexture(geometry_texture);
        m_shader.loadTexture(depth_map);
    }

    void  CellFinalRenderer::loadLightMap(const undicht::graphics::Texture& light_map) {

        m_shader.loadTexture(light_map);
    }

    void CellFinalRenderer::loadTextureAtlas(const undicht::graphics::Texture& texture_atlas) {

        m_shader.loadTexture(texture_atlas);
    }

    //////////////////////////////////////// draw functions ////////////////////////////////////////

    void  CellFinalRenderer::draw() {

        Renderer::draw();
    }

} // cell

#include "cell_geometry_renderer.h"

#include <files/file.h>
#include <string_tools.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>


#define CELL_GEOMETRY_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_geometry_shader.glsl"

namespace cell {

    using namespace undicht;
    using namespace graphics;
    using namespace tools;

    CellGeometryRenderer::CellGeometryRenderer() {

    }

    CellGeometryRenderer::~CellGeometryRenderer() {

    }

    void CellGeometryRenderer::init() {

        // initializing the shader
        std::string shader_src;
        File(CELL_GEOMETRY_SHADER).getAll(shader_src);
        m_shader.loadSource(shader_src);

        // initializing the uniforms
        m_view.setName("view");
        m_proj.setName("proj");
        m_chunk_pos.setName("chunk_pos");

        // initializing the geometry buffer
        m_geometry_buffer.setSize(400, 400);

        // depth map
        m_depth_texture.setName("depth_map");
        m_geometry_buffer.addAttachment(m_depth_texture, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);

        // m_geo_texture
        // layout for the geometry buffer
        // 2 channel texture to store:
        // material (2 bytes)
        // normal/face (1 byte)
        m_geo_texture.setName("geo_texture");
        m_geo_texture.setSize(0,0,1);
        m_geo_texture.setPixelFormat(BufferLayout({ UND_UINT16, UND_UINT8})); // will get translated into GL_RG16UI by undicht
        m_geo_texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
        m_geo_texture.setWrappingMethod(UND_REPEAT);
        m_geometry_buffer.addAttachment(m_geo_texture, UND_COLOR_ATTACHMENT, 0);

        // general Renderer settings
        enableDepthTest(true);
        enableBackFaceCulling(true);
        enableBlending(false);

        submit(&m_geometry_buffer);
        submit(&m_shader);

    }

    void CellGeometryRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        m_proj.setData(glm::value_ptr(player_cam.getCameraProjectionMatrix()), UND_MAT4F);
        m_view.setData(glm::value_ptr(player_cam.getViewMatrix()), UND_MAT4F);

        m_shader.loadUniform(m_proj);
        m_shader.loadUniform(m_view);

    }


    //////////////////////////////////////////// draw functions ////////////////////////////////////////////

    void CellGeometryRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

        m_geometry_buffer.setSize(width, height);
        Renderer::setViewport(width, height, offset_x, offset_y);
    }

    void CellGeometryRenderer::draw(WorldChunk& c, const glm::ivec3& chunk_pos) {

        m_chunk_pos.setData(glm::value_ptr(chunk_pos), UND_VEC3I);
        m_shader.loadUniform(m_chunk_pos);

        submit(&c.getDrawBuffer());

        Renderer::draw(c.getCellCount());
    }

    void CellGeometryRenderer::draw(World& w) {

        for(int i = 0; i < w.m_loaded_chunks.size(); i++) {

            draw(w.m_loaded_chunks[i], w.m_loaded_chunks[i].getOrigin());
        }

    }


} // cell

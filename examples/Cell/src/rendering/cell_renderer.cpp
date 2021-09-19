#include <rendering/cell_renderer.h>
#include <files/file.h>

#include <string_tools.h>
#include <iostream>
#include <geometry/geometry.h>

#include <buffer_layout.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>

#include <core/frame_buffer.h>

#define CELL_GEOMETRY_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_geometry_shader.glsl"

#define CELL_FINAL_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_final_shader.glsl"

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

        m_view.setName("view");
        m_proj.setName("proj");
        m_chunk_pos.setName("chunk_pos");

        initGeometryStage();
        initFinalStage();

    }

    void CellRenderer::initGeometryStage() {

        std::string shader_src;
        File(CELL_GEOMETRY_SHADER).getAll(shader_src);
        m_geometry_shader.loadSource(shader_src);

        m_geometry_buffer.setSize(400, 400);

        // depth map
		m_depth_texture.setName("depth_map");
		m_geometry_buffer.addAttachment(m_depth_texture, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);

        // uv texture
        m_uv_texture.setName("uv_texture");
        m_uv_texture.setSize(0,0,1);
		m_uv_texture.setPixelFormat(BufferLayout({ UND_UINT8, UND_UINT8, UND_UINT8})); // 3 channels with one byte each
		m_uv_texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
		m_uv_texture.setWrappingMethod(UND_REPEAT);
        m_geometry_buffer.addAttachment(m_uv_texture, UND_COLOR_ATTACHMENT, 0);


		// normal texture
        m_normal_texture.setName("normal_texture");
        m_normal_texture.setSize(0,0,1);
		m_normal_texture.setPixelFormat(BufferLayout({ UND_UINT2, UND_UINT2, UND_UINT2, UND_UINT2})); // 3 channels needed for the normal, 1 channel used to fill up to 1 byte per pixel
		m_normal_texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
		m_normal_texture.setWrappingMethod(UND_REPEAT);
		m_geometry_buffer.addAttachment(m_normal_texture, UND_COLOR_ATTACHMENT, 1);

    }

    void CellRenderer::initFinalStage() {

        std::string shader_src;
        File(CELL_FINAL_SHADER).getAll(shader_src);
        m_final_shader.loadSource(shader_src);

        // creating the screen quad
        std::vector<float> vertices;
        std::vector<int> indices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genRectangle(glm::vec2(-1.0f,1.0f), glm::vec2(1.0f,-1.0f), vertices, indices);

        m_screen_quad.setLayout(BufferLayout({UND_VEC2F, UND_VEC2F}));
        m_screen_quad.setData(vertices);
        m_screen_quad.setIndexData(indices);
    }

    void CellRenderer::drawFinalScene() {

        enableDepthTest(true);
        enableBackFaceCulling(false);
        enableBlending(false);

        m_final_shader.loadTexture(m_uv_texture);
        m_final_shader.loadTexture(m_normal_texture);

        submit(&m_screen_quad);
        submit(&m_final_shader);
        submit((FrameBuffer*)0);

        Renderer::draw();
    }



    void CellRenderer::term() {

    }

    void CellRenderer::clearFramebuffer() {

        submit(&m_geometry_buffer);
        Renderer::clearFramebuffer();

        submit((FrameBuffer*)0);
        Renderer::clearFramebuffer();

    }

    void CellRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

        m_geometry_buffer.setSize(width, height);
        Renderer::setViewport(width, height, offset_x, offset_y);

    }


    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        m_proj.setData(glm::value_ptr(player_cam.getCameraProjectionMatrix()), UND_MAT4F);
        m_geometry_shader.loadUniform(m_proj);

        m_view.setData(glm::value_ptr(player_cam.getViewMatrix()), UND_MAT4F);
        m_geometry_shader.loadUniform(m_view);

    }

    void CellRenderer::loadTextureAtlas(const TextureAtlas& atlas) {

        m_final_shader.loadTexture(atlas.m_tex);

    }

    void CellRenderer::draw(Chunk& c, const glm::ivec3& chunk_pos) {

        m_chunk_pos.setData(glm::value_ptr(chunk_pos), UND_VEC3I);
        m_geometry_shader.loadUniform(m_chunk_pos);

        enableDepthTest(true);
        enableBackFaceCulling(true);
        enableBlending(false);

        submit(&c.m_vertex_buffer);
        submit(&m_geometry_shader);
        submit(&m_geometry_buffer);

        Renderer::draw(c.getCellCount());

    }

    void CellRenderer::draw(World& w) {

        for(int i = 0; i < w.m_loaded_chunks.size(); i++) {

            draw(w.m_loaded_chunks[i], w.m_chunk_positions[i]);
        }

    }


} // cell

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

#define CELL_LIGHT_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_light_shader.glsl"

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
        m_inv_view.setName("inv_view");;

        m_proj.setName("proj");
        m_inv_proj.setName("inv_proj");

        m_cam_pos.setName("cam_pos");

        m_chunk_pos.setName("chunk_pos");

        initGeometryStage();
        initLightStage();
        initFinalStage();

    }

    void CellRenderer::initGeometryStage() {

        std::string shader_src;
        File(CELL_GEOMETRY_SHADER).getAll(shader_src);
        m_geometry_shader.loadSource(shader_src);

        m_geometry_buffer.setSize(400, 400);

        // layout for the geometry buffer
        // 2 channel texture to store:
        // material (2 bytes)
        // normal/face (1 byte)
        // will get translated into GL_RG16 by undicht


        // depth map
        m_depth_texture.setName("depth_map");
        m_geometry_buffer.addAttachment(m_depth_texture, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);

        // m_geo_texture
        m_geo_texture.setName("geo_texture");
        m_geo_texture.setSize(0,0,1);
        m_geo_texture.setPixelFormat(BufferLayout({ UND_UINT16, UND_UINT8}));
        m_geo_texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
        m_geo_texture.setWrappingMethod(UND_REPEAT);
        m_geometry_buffer.addAttachment(m_geo_texture, UND_COLOR_ATTACHMENT, 0);


        /*// normal texture
        m_normal_texture.setName("normal_texture");
        m_normal_texture.setSize(0,0,1);
        m_normal_texture.setPixelFormat(BufferLayout({ UND_UINT2, UND_UINT2, UND_UINT2, UND_UINT2})); // 3 channels needed for the normal, 1 channel used to fill up to 1 byte per pixel
        m_normal_texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
        m_normal_texture.setWrappingMethod(UND_REPEAT);
        m_geometry_buffer.addAttachment(m_normal_texture, UND_COLOR_ATTACHMENT, 1);*/

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

        m_final_shader.loadTexture(m_geo_texture);
        m_final_shader.loadTexture(m_depth_texture);
        m_final_shader.loadTexture(m_light_map);

        submit(&m_screen_quad);
        submit(&m_final_shader);
        submit((FrameBuffer*)0);

        Renderer::draw();
    }

    ////////////////////////////////////////// rendering lights //////////////////////////////////////////

    void CellRenderer::initLightStage() {

        std::string shader_src;
        File(CELL_LIGHT_SHADER).getAll(shader_src);
        m_light_shader.loadSource(shader_src);

        m_light_buffer.setSize(400, 400);
        m_light_buffer.addAttachment(m_depth_texture, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);

        m_light_map.setName("light_map");
        m_light_map.setSize(0,0,1);
        m_light_map.setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
        m_light_map.setFilteringMethod(UND_NEAREST, UND_NEAREST);
        m_light_map.setWrappingMethod(UND_REPEAT);
        m_light_buffer.addAttachment(m_light_map, UND_COLOR_ATTACHMENT, 0);

        // creating the light model
        std::vector<float> vertices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genCuboid(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f), vertices);

        m_light_model.setLayout(BufferLayout({UND_VEC3F, UND_VEC2F}));
        m_light_model.setData(vertices);

        // light uniforms
        m_light_pos.setName("light_origin");
        m_light_radius.setName("light_radius");
        m_light_color.setName("light_color");
        m_db_light_radius.setName("db_light_radius");

        m_viewport_size.setName("viewport_size");
    }

    void CellRenderer::drawLight(const glm::vec3& pos, const glm::vec3& color, float radius) {

        m_light_shader.loadTexture(m_depth_texture);
        m_light_shader.loadTexture(m_geo_texture);

        m_light_pos.setData(glm::value_ptr(pos), UND_VEC3F);
        m_light_shader.loadUniform(m_light_pos);

        m_light_color.setData(glm::value_ptr(color), UND_VEC3F);
        m_light_shader.loadUniform(m_light_color);

        m_light_radius.setData(&radius, UND_FLOAT);
        m_light_shader.loadUniform(m_light_radius);

        float db_lr = 1.0f / radius;
        m_db_light_radius.setData(&db_lr, UND_FLOAT);
        m_light_shader.loadUniform(m_db_light_radius);

        float viewport[] = { 1.0f / m_light_buffer.m_width, 1.0f / m_light_buffer.m_height };
        m_viewport_size.setData(viewport, UND_VEC2F);
        m_light_shader.loadUniform(m_viewport_size);

        submit(&m_light_shader);
        submit(&m_light_buffer);
        submit(&m_light_model);

        enableDepthTest(true, false, UND_GREATER);
        enableBackFaceCulling(true, UND_FRONT_FACE);
        enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

        Renderer::draw();
    }

    //////////////////////////////////////////// general //////////////////////////////////////////

    void CellRenderer::term() {

    }

    void CellRenderer::clearFramebuffer() {

        submit(&m_geometry_buffer);
        Renderer::clearFramebuffer();

        submit(&m_light_buffer);
        Renderer::clearFramebuffer(0,0,0,0);

        submit((FrameBuffer*)0);
        Renderer::clearFramebuffer();

    }

    void CellRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

        m_geometry_buffer.setSize(width, height);
        m_light_buffer.setSize(width, height);
        Renderer::setViewport(width, height, offset_x, offset_y);

    }


    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        glm::mat4 proj = player_cam.getCameraProjectionMatrix();
        glm::mat4 inv_proj = glm::inverse(proj);

        glm::mat4 view = player_cam.getViewMatrix();
        glm::mat4 inv_view = glm::inverse(view);

        glm::vec3 cam_pos = player_cam.getPosition();

        m_proj.setData(glm::value_ptr(proj), UND_MAT4F);
        m_inv_proj.setData(glm::value_ptr(inv_proj), UND_MAT4F);
        m_view.setData(glm::value_ptr(view), UND_MAT4F);
        m_inv_view.setData(glm::value_ptr(inv_view), UND_MAT4F);
        m_cam_pos.setData(glm::value_ptr(cam_pos), UND_VEC3F);

        m_geometry_shader.loadUniform(m_proj);
        m_light_shader.loadUniform(m_proj);

        m_final_shader.loadUniform(m_inv_proj);
        m_light_shader.loadUniform(m_inv_proj);

        m_geometry_shader.loadUniform(m_view);
        m_light_shader.loadUniform(m_view);

        m_final_shader.loadUniform(m_inv_view);
        m_light_shader.loadUniform(m_inv_view);

        m_light_shader.loadUniform(m_cam_pos);
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

#include "cell_light_renderer.h"

#include <files/file.h>
#include <string_tools.h>
#include <types.h>

#include <geometry/geometry.h>

#include <glm/gtc/type_ptr.hpp>

#define CELL_LIGHT_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_light_shader.glsl"

namespace cell {

    using namespace undicht;
    using namespace graphics;
    using namespace tools;

    CellLightRenderer::CellLightRenderer() {

    }

    CellLightRenderer::CellLightRenderer(undicht::graphics::Texture& depth_map) {

        init(depth_map);
    }

    CellLightRenderer::~CellLightRenderer() {

    }

    void CellLightRenderer::init(undicht::graphics::Texture& depth_map) {

        // initializing the shader
        std::string shader_src;
        File(CELL_LIGHT_SHADER).getAll(shader_src);
        m_shader.loadSource(shader_src);
        m_shader.loadTexture(depth_map);


        // init the light map + framebuffer
        m_light_buffer.setSize(400, 400);
        m_light_buffer.addAttachment(depth_map, UND_DEPTH_ATTACHMENT_READ_AND_WRITE); // reusing the depth map from the geometry stage for depth testing

        m_light_map.setName("light_map");
        m_light_map.setSize(0,0,1);
        m_light_map.setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
        m_light_map.setFilteringMethod(UND_NEAREST, UND_NEAREST);
        m_light_map.setWrappingMethod(UND_REPEAT);
        m_light_buffer.addAttachment(m_light_map, UND_COLOR_ATTACHMENT, 0);

        // init the light model
        std::vector<float> vertices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genCuboid(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f), vertices);

        m_light_model.setLayout(BufferLayout({UND_VEC3F, UND_VEC2F}));
        m_light_model.setData(vertices);

        // uniforms
        m_viewport_size.setName("viewport_size");

        // camera related
        m_view.setName("view");
        m_proj.setName("proj");

        m_inv_view.setName("inv_view");
        m_inv_proj.setName("inv_proj");

        m_cam_pos.setName("cam_pos");

        // light uniforms
        m_light_pos.setName("light_origin");
        m_light_color.setName("light_color");
        m_light_radius.setName("light_radius");
        m_db_light_radius.setName("db_light_radius");


        // Renderer settings
        submit(&m_shader);
        submit(&m_light_buffer);
        submit(&m_light_model);

        enableDepthTest(true, false, UND_GREATER);
        enableBackFaceCulling(true, UND_FRONT_FACE);
        enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

    }

    void CellLightRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        const glm::mat4& view = player_cam.getViewMatrix();
        const glm::mat4& proj = player_cam.getCameraProjectionMatrix();

        glm::mat4 inv_view = glm::inverse(view);
        glm::mat4 inv_proj = glm::inverse(proj);

        m_view.setData(glm::value_ptr(view), UND_MAT4F);
        m_proj.setData(glm::value_ptr(proj), UND_MAT4F);

        m_inv_view.setData(glm::value_ptr(inv_view), UND_MAT4F);
        m_inv_proj.setData(glm::value_ptr(inv_proj), UND_MAT4F);

        m_cam_pos.setData(glm::value_ptr(player_cam.getPosition()), UND_VEC3F);

        m_shader.loadUniform(m_view);
        m_shader.loadUniform(m_proj);
        m_shader.loadUniform(m_inv_view);
        m_shader.loadUniform(m_inv_proj);
        m_shader.loadUniform(m_cam_pos);

    }

    void CellLightRenderer::loadGeometry(const undicht::graphics::Texture& geometry_texture, const undicht::graphics::Texture& depth_map) {

        m_shader.loadTexture(geometry_texture);
        m_shader.loadTexture(depth_map);
    }


    ////////////////////////////////////////////// draw functions //////////////////////////////////////////////

    void CellLightRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

        m_light_buffer.setSize(width, height);
        Renderer::setViewport(width, height, offset_x, offset_y);
    }

    void CellLightRenderer::draw(const glm::vec3& pos, const glm::vec3& color, float radius) {

        m_light_pos.setData(glm::value_ptr(pos), UND_VEC3F);
        m_light_color.setData(glm::value_ptr(color), UND_VEC3F);
        m_light_radius.setData(&radius, UND_FLOAT);

        m_shader.loadUniform(m_light_pos);
        m_shader.loadUniform(m_light_color);
        m_shader.loadUniform(m_light_radius);

        float db_lr = 1.0f / radius;
        m_db_light_radius.setData(&db_lr, UND_FLOAT);
        m_shader.loadUniform(m_db_light_radius);

        float viewport[] = { 1.0f / m_light_buffer.m_width, 1.0f / m_light_buffer.m_height };
        m_viewport_size.setData(viewport, UND_VEC2F);
        m_shader.loadUniform(m_viewport_size);

        Renderer::draw();
    }


} // cell

#include "cell_renderer.h"
#include <files/file.h>
#include <string_tools.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>

#include <algorithm>

using namespace undicht;
using namespace graphics;
using namespace tools;

#define CELL_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_shader.glsl"

namespace cell {

    // camera uniforms
    Uniform* CellRenderer::s_proj;
    Uniform* CellRenderer::s_view;

    // chunk uniforms
    Uniform* CellRenderer::s_chunk_offset;

    Shader* CellRenderer::s_shader;

    int CellRenderer::s_renderer_count = 0;


    void CellRenderer::init() {

        s_proj = new Uniform;
        s_view = new Uniform;
        s_chunk_offset = new Uniform;
        s_shader = new Shader;

        // loading the shader src file
        File cell_shader_file(CELL_SHADER);
        std::string shader_src;
        cell_shader_file.getAll(shader_src);

        s_shader->loadSource(shader_src);

        // camera uniforms
        s_proj->setName("proj");
        s_view->setName("view");

        // chunk uniforms
        s_chunk_offset->setName("chunk_offset");

        // initializing the material texture atlas



    }


    void CellRenderer::term() {

        delete s_proj;
        delete s_view;
        delete s_chunk_offset;
        delete s_shader;
    }

    CellRenderer::CellRenderer() {

        if (!s_renderer_count)
            init();

        submit(s_shader);
        enableDepthTest(true);
        enableBackFaceCulling(true);

        s_renderer_count++;

    }

    CellRenderer::~CellRenderer() {

        s_renderer_count--;

        if(!s_renderer_count)
            term();
    }

    ////////////////////////////////////////////// drawing ///////////////////////////////////////////////

    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& cam) {

        // loading camera uniforms
        s_proj->setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
        s_shader->loadUniform(*s_proj);

        s_view->setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
        s_shader->loadUniform(*s_view);
    }

    void CellRenderer::drawChunk(DrawChunk& chunk) {

        // loading the chunk position to the shader
        s_chunk_offset->setData(glm::value_ptr(chunk.m_position), UND_VEC3I);
        s_shader->loadUniform(*s_chunk_offset);

        // drawing the chunk
        submit(&chunk.m_buffer);
        draw(chunk.getDrawnCellCount());

    }

} // cell

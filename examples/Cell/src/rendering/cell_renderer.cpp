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


    std::vector<CellRenderer::RegisteredMaterial> CellRenderer::s_materials;

    std::vector<std::string> CellRenderer::s_prefixes;

    Texture* CellRenderer::s_texture_atlas;

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

        {

            s_texture_atlas = new Texture;
            s_texture_atlas->setName("texture_atlas");
            s_texture_atlas->setSize(16, 16, 16); // 16 materials is the limit for now

            BufferLayout layout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR });
            s_texture_atlas->setPixelFormat(layout);

            s_texture_atlas->setFilteringMethod(UND_NEAREST, UND_NEAREST);

            // first material is void

            registerMaterial("default", "void");

        }


    }


    void CellRenderer::term() {

        delete s_proj;
        delete s_view;
        delete s_chunk_offset;
        delete s_shader;

        delete s_texture_atlas;
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

    ///////////////////////////////////////// managing materials /////////////////////////////////////////

    Material CellRenderer::registerMaterial(const std::string& prefix, const std::string& name) {

        RegisteredMaterial reg_mat;
        reg_mat.name = name;

        // searching for the prefix
        for (int i = 0; i < s_prefixes.size(); i++) {

            if (!prefix.compare(s_prefixes.at(i))) {

                reg_mat.prefix = i;
                break;
            }

        }

        Material new_mat;
        new_mat.setName(prefix, name);
        new_mat.setID(s_materials.size());

        s_materials.push_back(reg_mat);

        return new_mat;
    }

    void CellRenderer::setMaterialTexture(const Material& mat, TextureData& data) {

        if (mat.m_id > 15) {

            std::cout << "ERROR: maximum material count is 16" << "\n";
            return;
        }

        s_texture_atlas->setData(data.pixels.data(), sizeof(data.pixels), mat.m_id);
        s_texture_atlas->generateMipMaps();

    }


    ////////////////////////////////////////////// drawing ///////////////////////////////////////////////

    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& cam) {

        // loading camera uniforms
        s_proj->setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
        s_shader->loadUniform(*s_proj);

        s_view->setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
        s_shader->loadUniform(*s_view);
    }

    void CellRenderer::drawChunk(DrawChunk& chunk, int x, int y, int z) {

        // storing the cellchunks in the vertex buffers per instance data

        s_shader->loadTexture(*s_texture_atlas);

        // loading the chunk position to the shader
        float offsets[] = {x * 256, y * 256, z * 256};

        s_chunk_offset->setData(offsets, UND_VEC3F);
        s_shader->loadUniform(*s_chunk_offset);

        // drawing the chunk
        submit(&chunk.m_buffer);
        draw(chunk.m_drawn_cells);

    }

} // cell

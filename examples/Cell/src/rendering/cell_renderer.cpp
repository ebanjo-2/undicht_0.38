#include "cell_renderer.h"
#include <files/file.h>
#include <string_tools.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>

using namespace undicht;
using namespace graphics;
using namespace tools;

#define CELL_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_shader.glsl"

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

    for(unsigned int& i : m_cells_per_chunk) i = 0;

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

/////////////////////////////// the data that gets drawn by the renderer ///////// ////////////////////

void CellRenderer::updateChunk(const CompactChunk<8>& chunk_data, int x, int y, int z) {
    /** (0,0,0) is the center chunk, x,y and z are offsets from there */

    // finding the buffer to update
    int buffer_index = getChunkID(x,y,z);
    ChunkBuffer<8>* buffer = &m_chunk_buffers[buffer_index];

    buffer->setCellData(chunk_data);

    m_cells_per_chunk[buffer_index] = chunk_data.m_cells.size();
}

int CellRenderer::getChunkID(int x, int y, int z) {

    int chunk_id = x * CR_VIEW_RANGE * CR_VIEW_RANGE + y * CR_VIEW_RANGE + z;
    chunk_id += CR_VIEW_RANGE_CUBED / 2; // integers !

    return chunk_id;
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

void CellRenderer::drawChunks() {

    // storing the cellchunks in the vertex buffers per instance data

    s_shader->loadTexture(*s_texture_atlas);

    int i = CR_VIEW_RANGE / 2; // integers !!


    for(float x = -1.0 * i; x <= i; x++) {
        for(float y = -1.0 * i; y <= i; y++) {
            for(float z = -1.0 * i; z <= i; z++) {

                float offsets[] = {x * 256, y * 256, z * 256};


                s_chunk_offset->setData(offsets, UND_VEC3F);
                s_shader->loadUniform(*s_chunk_offset);

                submit(&m_chunk_buffers[getChunkID(x,y,z)]);
                draw(m_cells_per_chunk[getChunkID(x,y,z)]);

            }
        }
    }

}


#include <rendering/cell_renderer.h>
#include <files/file.h>

#include <string_tools.h>
#include <iostream>
#include <geometry/geometry.h>

#include <buffer_layout.h>
#include <types.h>

#include <glm/gtc/type_ptr.hpp>

#include <core/frame_buffer.h>


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

        m_geometry_renderer.init();
        m_light_renderer.init(m_geometry_renderer.m_depth_texture);
        m_final_renderer.init();

    }




    void CellRenderer::drawFinalScene() {

        m_final_renderer.loadGeometry(m_geometry_renderer.m_geo_texture, m_geometry_renderer.m_depth_texture);
        m_final_renderer.loadLightMap(m_light_renderer.m_light_map);

        m_final_renderer.draw();

    }



    void CellRenderer::drawLight(const glm::vec3& pos, const glm::vec3& color, float radius) {

        m_light_renderer.loadGeometry(m_geometry_renderer.m_geo_texture, m_geometry_renderer.m_depth_texture);
        m_light_renderer.draw(pos, color, radius);

    }

    //////////////////////////////////////////// general //////////////////////////////////////////

    void CellRenderer::term() {

    }

    void CellRenderer::clearFramebuffer() {

        m_geometry_renderer.clearFramebuffer();
        m_light_renderer.clearFramebuffer(0,0,0,0);
        m_final_renderer.clearFramebuffer();
    }

    void CellRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

        m_geometry_renderer.setViewport(width, height, offset_x, offset_y);
        m_light_renderer.setViewport(width, height, offset_x, offset_y);
        m_final_renderer.setViewport(width, height, offset_x, offset_y);

    }


    void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& player_cam) {

        m_geometry_renderer.loadCam(player_cam);
        m_light_renderer.loadCam(player_cam);
        m_final_renderer.loadCam(player_cam);

    }

    void CellRenderer::loadTextureAtlas(const TextureAtlas& atlas) {

        m_final_renderer.loadTextureAtlas(atlas.m_tex);
    }

    void CellRenderer::draw(WorldChunk& c, const glm::ivec3& chunk_pos) {

        m_geometry_renderer.draw(c, chunk_pos);
    }

    void CellRenderer::draw(World& w) {

        m_geometry_renderer.draw(w);

    }


} // cell

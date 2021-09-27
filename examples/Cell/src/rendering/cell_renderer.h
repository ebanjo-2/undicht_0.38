#ifndef CELL_RENDERER_H
#define CELL_RENDERER_H

#include <core/renderer.h>
#include <camera/perspective_camera_3d.h>

#include <world/cell.h>
#include <world/chunk.h>
#include <world/world.h>

#include <rendering/texture_atlas.h>

#include <rendering/cell_geometry_renderer.h>
#include <rendering/cell_light_renderer.h>
#include <rendering/cell_final_renderer.h>

namespace cell {

    class CellRenderer {

    public:
            CellGeometryRenderer m_geometry_renderer;
            CellLightRenderer m_light_renderer;
            CellFinalRenderer m_final_renderer;

        public:

            CellRenderer();
            virtual ~CellRenderer();

            void init();
            void term();

            virtual void clearFramebuffer();
            virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);

            void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            void loadTextureAtlas(const TextureAtlas& atlas);

            void draw(Chunk& c, const glm::ivec3& chunk_pos);

            void draw(World& w);

            void drawLight(const glm::vec3& pos, const glm::vec3& color, float radius);

            void drawFinalScene();



    };


} // cell


#endif // CELL_RENDERER_H

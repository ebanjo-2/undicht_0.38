#ifndef CELL_GEOMETRY_RENDERER_H
#define CELL_GEOMETRY_RENDERER_H

#include <world/world.h>
#include <player/player.h>

#include <core/renderer.h>

namespace cell {

    class CellGeometryRenderer : public undicht::graphics::Renderer {

        public:
            // graphics objects

            undicht::graphics::Shader m_shader;

            undicht::graphics::Texture m_geo_texture;
            undicht::graphics::Texture m_depth_texture;
            undicht::graphics::FrameBuffer m_geometry_buffer;


        public:
            // uniforms

            undicht::graphics::Uniform m_view;
            undicht::graphics::Uniform m_proj;
            undicht::graphics::Uniform m_chunk_pos;

        public:

            CellGeometryRenderer();
            virtual ~CellGeometryRenderer();

            virtual void init();

            void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

        public:
            // draw functions

            virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);

            virtual void draw(Chunk& c, const glm::ivec3& chunk_pos);
            virtual void draw(World& w);

    };


} // cell


#endif // CELL_GEOMETRY_RENDERER_H

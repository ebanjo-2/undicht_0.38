#ifndef VG_RENDERER_H
#define VG_RENDERER_H

#include <core/renderer.h>
#include <glm/glm.hpp>


namespace undicht {

    namespace graphics {


        class VGRenderer : public Renderer {

            protected:

                Shader m_line_shader;
                Shader m_circle_shader;

                // the square every vector graphics is based on
                VertexBuffer m_base_shape;

            protected:

                Uniform m_aspect_ratio;

                Uniform m_pos0;
                Uniform m_pos1;

                Uniform m_radius;

                Uniform m_color;
                Uniform m_width;

            public:

                VGRenderer();
                virtual ~VGRenderer();

                void init();

                virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);

                void setColor(glm::vec4 color);

            public:
                // drawing primitives
                // y positions are always in screen space (-1 to 1)
                // the max drawable x positions depend on your aspect ratio (xmax = 1 * width / height)
                // but 0,0 is always the center

                void drawLine(glm::vec2 pos0, glm::vec2 pos1, float width);

                void drawCircle(glm::vec2 pos, float radius, float width);


        };

    } // graphics

} // undicht


#endif // VG_RENDERER_H

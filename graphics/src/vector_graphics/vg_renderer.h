#ifndef VG_RENDERER_H
#define VG_RENDERER_H

#include <core/renderer.h>
#include <glm/glm.hpp>


namespace undicht {

    namespace graphics {


        class VGRenderer : public Renderer {

            protected:

                Shader m_line_shader;

                // the square every vector graphics is based on
                VertexBuffer m_base_shape;

            protected:

                Uniform m_pos0;
                Uniform m_pos1;

                Uniform m_color;
                Uniform m_width;

            public:

                VGRenderer();
                virtual ~VGRenderer();

                void init();

                void setColor(glm::vec4 color);

            public:
                // drawing primitives
                // positions are always in screen space (-1 to 1)

                void drawLine(glm::vec2 pos0, glm::vec2 pos1, float width);


        };

    } // graphics

} // undicht


#endif // VG_RENDERER_H

#include <vector_graphics/vg_renderer.h>

#include <files/file.h>
#include <string_tools.h>

#include <geometry/geometry.h>

#include <glm/gtc/type_ptr.hpp>


using namespace undicht::tools;

#define LINE_SHADER getFilePath(UND_CODE_SRC_FILE) + "line_shader.glsl"


namespace undicht {

    namespace graphics {

        VGRenderer::VGRenderer() {

            init();
        }


        VGRenderer::~VGRenderer() {

        }

        void VGRenderer::init() {

            // initializing the shaders
            File shader_file;
            std::string shader_src;

            shader_file.open(LINE_SHADER);

            m_line_shader.loadSource(shader_file.getAll(shader_src));

            // setting opengl settings
            enableDepthTest(false);
            enableBackFaceCulling(false);
            enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

            // initializing the base shape
            m_base_shape.setLayout(BufferLayout({ UND_VEC2F }));

            std::vector<float> vertices;
            std::vector<int> indices;

            Geometry::buildUVs(false);
            Geometry::buildNormals(false);
            Geometry::genRectangle(glm::vec2(-1, -1), glm::vec2(1, 1), vertices, indices);

            m_base_shape.setData(vertices);
            m_base_shape.setIndexData(indices);

            submit(&m_base_shape);

            // initializing the uniforms
            m_pos0.setName("pos0");
            m_pos1.setName("pos1");

            m_color.setName("color");
            m_width.setName("width");
        }

        void VGRenderer::setColor(glm::vec4 color) {

            m_color.setData(glm::value_ptr(color), UND_VEC4F);
            m_line_shader.loadUniform(m_color);

        }

        ///////////////////////////////////////////// drawing primitives ///////////////////////////////////////////////
        //////////////////////////////// positions are always in screen space (-1 to 1) ////////////////////////////////


        void VGRenderer::drawLine(glm::vec2 pos0, glm::vec2 pos1, float width) {


            m_pos0.setData(glm::value_ptr(pos0), UND_VEC2F);
            m_pos1.setData(glm::value_ptr(pos1), UND_VEC2F);

            m_width.setData(&width, UND_FLOAT);


            m_line_shader.loadUniform(m_pos0);
            m_line_shader.loadUniform(m_pos1);
            m_line_shader.loadUniform(m_width);

            submit(&m_line_shader);

            Renderer::draw();

        }



    } // graphics

} // undicht

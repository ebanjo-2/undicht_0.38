#include <core/renderer.h>
#include <core/frame_buffer.h>

/** the renderer that draws the scene data to the geometry buffer */

using namespace undicht::graphics;
using namespace undicht::tools;

extern Renderer* light_renderer;
extern Shader* light_shader;
extern FrameBuffer* light_buffer;
extern Texture* light_map;
extern VertexBuffer* light_model;

extern Uniform* light_pos;
extern Uniform* light_color;
extern Uniform* light_radius;

extern Uniform* viewport_size;

void initLightRenderer();
void termLightRenderer();
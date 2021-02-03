#include <core/renderer.h>
#include <deferred_shading/geometry_buffer.h>

/** the renderer that draws the scene data to the geometry buffer */

using namespace undicht::graphics;
using namespace undicht::tools;

extern Renderer* final_renderer;
extern Shader* final_shader;
extern VertexBuffer* screen_quad;

void initFinalRenderer();
void termFinalRenderer();
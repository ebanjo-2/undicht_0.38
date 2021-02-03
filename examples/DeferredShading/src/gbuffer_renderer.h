#include <core/renderer.h>
#include <deferred_shading/geometry_buffer.h>

/** the renderer that draws the scene data to the geometry buffer */

using namespace undicht::graphics;
using namespace undicht::tools;

extern Renderer* gbuffer_renderer;
extern Shader* gbuffer_shader;
extern GeometryBuffer* gbuffer;

void initGBufferRenderer();
void termGBufferRenderer();
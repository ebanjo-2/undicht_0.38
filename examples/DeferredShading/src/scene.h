#include <vector>
#include <core/vertex_buffer.h>
#include <core/texture.h>

using namespace undicht::graphics;
using namespace undicht::tools;

extern std::vector<VertexBuffer*> vbos;
extern std::vector<Texture*> texs;
extern std::vector<int> tex_ids;


void initScene();
void termScene();
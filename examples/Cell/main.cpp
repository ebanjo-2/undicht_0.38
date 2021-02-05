#include <iostream>
#include <vector>
#include <array>

#include <material.h>
#include <cell.h>
#include <chunk.h>

using namespace std;

int main() {

    Chunk<4> c;
    std::cout << c.m_cells.size() << " : " << sizeof(c) << "\n";

    CellChunk<2> cc;
    cc.m_pos_x = 3;
    std::cout << sizeof(cc) << " " << cc.m_pos_x << "\n";

    return 0;
}

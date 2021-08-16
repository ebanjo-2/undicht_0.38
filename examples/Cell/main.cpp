#include <stdio.h>

#include <iostream>

#include <font_rendering/font.h>
#include <font_rendering/font_renderer.h>
#include <window/window.h>

#include <string_tools.h>

#include <undicht_time.h>

using namespace undicht;
using namespace graphics;
using namespace tools;

int main(int argc, char **argv) {

    Window window(1000, 105, "HELLO WORLD");

    initEngineTime();

    {

        Font arial("res/arial.ttf", 40);

        FontRenderer renderer;
        renderer.setFontColor(glm::vec3(1,1,1));
        renderer.setViewport(1000, 105);

        int x, y;
        arial.getFontMap().getSize(x,y);


        while(!window.shouldClose()) {

            renderer.clearFramebuffer(0.5, 0.5, 0.5);

            renderer.draw(arial, "Hello World! " + toStr((int)getEngineTime()) + " s ", glm::vec2(-1, 1.0f/3.0f), 40);
            renderer.draw(arial, "Fontmap size: " + toStr(x) + " , " + toStr(y) + " | size in mem: " + toStr(x*y) + " bytes", glm::vec2(-1,2.0f/3.0f - 1.0f), 30);


            window.update();

        }

    }

    return 0;
}

#if 0

char ttf_buffer[1<<25];

int main(int argc, char **argv) {

    FontCharacter fc;
    fc = 'a';
    std::cout << "size of buffer: "<< (1<<25) << "\n";
    std::cout << fc << "\n";
    std::cout << fc.getID() << "\n";

    stbtt_fontinfo font; // stb class for holding the font data
    unsigned char *bitmap;
    int w,h; // width, height
    int i,j; // iterators for the bitmap
    int c = 'e'; // char to be printed
    int s = 20; // scale

    // read the contents of the ttf file into the ttf_buffer
    fread(ttf_buffer, 1, 1<<25, fopen("res/arial.ttf", "rb"));

    // load the font data to the stb fontinfo class
    stbtt_InitFont(&font, (const unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)ttf_buffer,0));

    // generate a bitmap for the character c for the given scale s
    bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0,0);

    std::cout << "width: " << w << " height: " << h << "\n";

    // draw the bitmap into console
    for (j=0; j < h; ++j) {
        for (i=0; i < w; ++i)
            putchar(" .:ioVM@"[bitmap[j*w+i]>>5]);
        putchar('|');
        putchar('\n');
    }
    return 0;
}

#endif

#pragma once
#include <stdint.h>

class Window{
public:
    unsigned int Width;
    int Height;
    int positionX;
    int positionY;
    unsigned int windowColour;
    const char* windowTitle;

    static bool LeftMouseButtonPressed;

    void makeWindow(unsigned int x, unsigned int y, unsigned int posX, unsigned int posY, unsigned int colour, const char* title);

    // Draw Content
    void DrawPix(unsigned int posX, unsigned int posY, unsigned int colour);
    void DrawLine(int x, int y, int length, bool directionY, uint32_t colour);
    void Fill(unsigned int colour);
    void Println(const char* text, unsigned int colour);
};

/*namespace info
{
    int Width;
}*/

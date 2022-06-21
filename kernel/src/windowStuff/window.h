#pragma once
#include <stdint.h>
#include "../Math.h"

class Window{
public:
    int Width;
    int Height;
    int positionX;
    int positionY;
    unsigned int windowColour;
    const char* windowTitle;

    int oldPositionX;
    int oldPositionY;

    Point windowCursorPosition;

    // Main functions
    void makeWindow(unsigned int x, unsigned int y, unsigned int posX, unsigned int posY, unsigned int colour, const char* title);

    // Content functions
    void DrawPix(unsigned int posX, unsigned int posY, unsigned int colour);
    void DrawLine(int x, int y, int length, bool directionY, uint32_t colour);
    void Fill(unsigned int colour);
    void Println(const char* text, unsigned int colour);
    void Nextln();


    void HandleMouse();
    void RedrawWindow();
    void deleteOldWindow();
};

class TerminalWindow{
public:
    int Width;
    int Height;
    int positionX;
    int positionY;
    unsigned int windowColour;
    const char* windowTitle;

    int oldPositionX;
    int oldPositionY;

    Point windowCursorPosition;

    // Main functions
    void makeWindow(unsigned int x, unsigned int y, unsigned int posX, unsigned int posY, unsigned int colour, const char* title);

    // Content functions
    void DrawPix(unsigned int posX, unsigned int posY, unsigned int colour);
    void DrawLine(int x, int y, int length, bool directionY, uint32_t colour);
    void Fill(unsigned int colour);
    void Println(const char* text, unsigned int colour);
    void Nextln();


    void HandleMouse();
    void RedrawWindow();
    void deleteOldWindow();
};


extern bool LeftMouseButtonPressed;
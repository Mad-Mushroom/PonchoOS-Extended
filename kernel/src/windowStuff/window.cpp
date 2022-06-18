#include <stdint.h>
#include "window.h"
#include "../BasicRenderer.h"
#include "../Math.h"
#include "../CStr.h"
#include "../Scheduling/PIT/PIT.h"
#include "../UserInput/Mouse.h"

Window window;

void Window::makeWindow(unsigned int x, unsigned int y, unsigned int posX, unsigned int posY, unsigned int colour, const char* title){
    if(posX > (GlobalRenderer->TargetFramebuffer->Width)) return;
    if(posY > (GlobalRenderer->TargetFramebuffer->Height)) return;
    if (x > (GlobalRenderer->TargetFramebuffer->Width)) return;
    if(y > (GlobalRenderer->TargetFramebuffer->Height)) return;
    

    Point oldCursorPosition;
    unsigned int oldTextColour;
    // Declare Variables
    window.Width = x;
    window.Height = y;
    window.positionX = posX;
    window.positionY = posY;
    window.windowColour = colour;
    window.windowTitle = title;

    // Render Window Form
    GlobalRenderer->DrawRectAngle(window.Width, window.Height, window.positionX, window.positionY, window.windowColour);
    GlobalRenderer->DrawLine(window.positionX, (window.positionX + 15), window.Width, false, window.windowColour);
    // Render Title
    oldCursorPosition = GlobalRenderer->CursorPosition;
    oldTextColour = GlobalRenderer->Colour;
    GlobalRenderer->Colour = window.windowColour;
    GlobalRenderer->CursorPosition = {(window.positionX + 2), window.positionY};
    GlobalRenderer->Print(window.windowTitle);
    GlobalRenderer->Colour = oldTextColour;
    GlobalRenderer->CursorPosition = oldCursorPosition;
    // Render Buttons
    GlobalRenderer->CursorPosition = {(window.positionX + window.Width - 8*7),window.positionY};
    GlobalRenderer->Print("_ [] X ");
    // Restore to factory Settings :)
    GlobalRenderer->Colour = oldTextColour;
    GlobalRenderer->CursorPosition = oldCursorPosition;

    if(window.LeftMouseButtonPressed == true){
        window.Println("Haha du hast gedruckt...", 0xffffffff);
    }
}

void Window::DrawPix(unsigned int posX, unsigned int posY, unsigned int colour){
    //Window window;
    GlobalRenderer->PutPix((window.positionX + posX),(window.positionY + 15 + posY),colour);
    //GlobalRenderer->Print(ToString((uint64_t)posX));
}

void Window::DrawLine(int x, int y, int length, bool directionY, uint32_t colour){
    if(directionY == false){
        for(int xf = 0; xf < length; xf++){
            DrawPix(x, y, colour);
            x++;
        }
    }
    else
    {
        for(int yf = 0; yf < length; yf++){
            DrawPix(x, y, colour);
            y++;
        }
    }
}

void Window::Fill(unsigned int colour){
    for(int i=0; i < (window.Height - 2); i++){
        DrawLine(1,i,(window.Width - 1),false,colour);
    }
}

void Window::Println(const char* text, unsigned int colour){
    Point oldCursorPosition;
    oldCursorPosition = GlobalRenderer->CursorPosition;
    GlobalRenderer->CursorPosition = {(window.positionX + 2),(window.positionY + 2 + 15)};
    unsigned int oldColour;
    oldColour = GlobalRenderer->Colour;
    GlobalRenderer->Colour = colour;
    GlobalRenderer->Print(text);
    GlobalRenderer->CursorPosition = oldCursorPosition;
    GlobalRenderer->Colour = oldColour;
}
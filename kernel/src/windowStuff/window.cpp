#include <stdint.h>
#include "window.h"
#include "../BasicRenderer.h"
#include "../Math.h"
#include "../CStr.h"
#include "../Scheduling/PIT/PIT.h"
#include "../UserInput/Mouse.h"

void Window::makeWindow(unsigned int x, unsigned int y, unsigned int posX, unsigned int posY, unsigned int colour, const char* title){
    if(posX > (GlobalRenderer->TargetFramebuffer->Width)) return;
    if(posY > (GlobalRenderer->TargetFramebuffer->Height)) return;
    if (x > (GlobalRenderer->TargetFramebuffer->Width)) return;
    if(y > (GlobalRenderer->TargetFramebuffer->Height)) return;
    
    Window window;
    Point oldCursorPosition;
    unsigned int oldTextColour;
    // Declare Variables
    window.Width = x;
    window.Height = y;
    window.positionX = posX;
    window.positionY = posY;
    window.windowColour = colour;
    window.windowTitle = title;

    info::Width = x;
    // Render Window Form
    GlobalRenderer->DrawRectAngle(window.Width, window.Height, window.positionX, window.positionY, window.windowColour);
    GlobalRenderer->DrawLine(window.positionX, (window.positionX + 15), window.Width, false, window.windowColour);
    // Render Text
    oldCursorPosition = GlobalRenderer->CursorPosition;
    oldTextColour = GlobalRenderer->Colour;
    GlobalRenderer->Colour = window.windowColour;
    GlobalRenderer->CursorPosition = {(window.positionX + 2), window.positionY};
    GlobalRenderer->Print(window.windowTitle);
    GlobalRenderer->Colour = oldTextColour;
    GlobalRenderer->CursorPosition = oldCursorPosition;

    /*for(int i=0; i < 1000; i++){
        if(MousePosition.X > window.positionX && MousePosition.X < (window.positionX + window.Width)){
            if(MousePosition.Y > window.positionY && MousePosition.Y < (window.positionY + 15)){
                GlobalRenderer->Print("Could Move!");
            }
        }

        PIT::Sleep(1000);
    }*/
}

void Window::DrawPix(unsigned int posX, unsigned int posY, unsigned int colour){
    Window window;
    GlobalRenderer->PutPix((window.positionX + posX),(posY),colour);
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
    Window window;
    for(int i=0; i < window.Height; i++){
        DrawLine(0,i,window.Width,false,colour);
    }
}
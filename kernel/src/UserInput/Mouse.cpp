#include <stdint.h>
#include "Mouse.h"
#include "../windowStuff/window.h"
#include "../CStr.h"
#include "../Scheduling/PIT/PIT.h"

uint8_t MousePointer[] = {
    0b11111111, 0b11100000, 
    0b11111111, 0b11000000, 
    0b11111111, 0b10000000, 
    0b11111111, 0b00000000, 
    0b11111110, 0b00000000, 
    0b11111100, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
};

void MouseWait(){
    uint64_t Timeout = 100000;
    while (Timeout--){
        if ((InByte(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t Timeout = 100000;
    while (Timeout--){
        if (InByte(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t Value){
    MouseWait();
    OutByte(0x64, 0xD4);
    MouseWait();
    OutByte(0x60, Value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return InByte(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;

void HandlePS2Mouse(uint8_t Data){

    ProcessMousePacket();
    static bool Skip = true;
    if (Skip) { Skip = false; return; }

    switch(MouseCycle){
        case 0:
           
            if ((Data & 0b00001000) == 0) break;
            MousePacket[0] = Data;
            MouseCycle++;
            break;
        case 1:
           
            MousePacket[1] = Data;
            MouseCycle++;
            break;
        case 2:
            
            MousePacket[2] = Data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

/*void SendPress(const char* index){
    Input input;
    if(index == "LEFT"){
        input.LeftMouseButtonPressed = true;
        PIT::Sleepd(5);
        input.LeftMouseButtonPressed = false;
        return;
    }
}*/

void ProcessMousePacket(){
    if (!MousePacketReady) return;

        //input.PositionMouse = MousePosition;

        bool XNegative, YNegative, XOverflow, YOverflow;

        if (MousePacket[0] & PS2XSign){
            XNegative = true;
        }else XNegative = false;

        if (MousePacket[0] & PS2YSign){
            YNegative = true;
        }else YNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            XOverflow = true;
        }else XOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            YOverflow = true;
        }else YOverflow = false;

        if (!XNegative){
            MousePosition.X += MousePacket[1];
            if (XOverflow){
                MousePosition.X += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            MousePosition.X -= MousePacket[1];
            if (XOverflow){
                MousePosition.X -= 255;
            }
        }

        if (!YNegative){
            MousePosition.Y -= MousePacket[2];
            if (YOverflow){
                MousePosition.Y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            MousePosition.Y += MousePacket[2];
            if (YOverflow){
                MousePosition.Y += 255;
            }
        }

        if (MousePosition.X < 0) MousePosition.X = 0;
        if (MousePosition.X > GlobalRenderer->TargetFramebuffer->Width-1) MousePosition.X = GlobalRenderer->TargetFramebuffer->Width-1;
        
        if (MousePosition.Y < 0) MousePosition.Y = 0;
        if (MousePosition.Y > GlobalRenderer->TargetFramebuffer->Height-1) MousePosition.Y = GlobalRenderer->TargetFramebuffer->Height-1;

        //input.PositionMouse = MousePosition;
        
        GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
        GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0xffffffff);

        //input.GetMousePosition(MousePosition);

        //Input input;

        //input.PositionMouse = MousePosition;

        /*for(int f=0; f < 100; f++){
            input.PositionMouse = MousePosition;
            PIT::Sleep(100);
        }*/

        /*input.MouseX = MousePosition.X;
        input.MouseY = MousePosition.Y;*/

        /*for(int i=0; i < 100; i++){
            GlobalRenderer->Print(ToString((uint64_t)MousePosition.X));
            GlobalRenderer->Print(",");
            GlobalRenderer->Print(ToString((uint64_t)MousePosition.Y));
            GlobalRenderer->Print(" ; ");
            PIT::Sleepd(5);
        }*/

        //input.MouseX = MousePosition.X;
        //input.MouseY = MousePosition.Y;

        if (MousePacket[0] & PS2Leftbutton){
            GlobalRenderer->Print("LEFT");
        }
        if (MousePacket[0] & PS2Middlebutton){
            
        }
        if (MousePacket[0] & PS2Rightbutton){

        }

        MousePacketReady = false;
        MousePositionOld = MousePosition;
}

void InitPS2Mouse(){
 
    OutByte(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    OutByte(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t Status = InByte(0x60);
    Status |= 0b10;
    MouseWait();
    OutByte(0x64, 0x60);
    MouseWait();
    OutByte(0x60, Status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}
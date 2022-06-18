#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* NewTargetFramebuffer, PSF1_FONT* NewPSF1Font)
{
    TargetFramebuffer = NewTargetFramebuffer;
    PSF1Font = NewPSF1Font;
    Colour = 0xffffffff;
    CursorPosition = {0, 0};
}

void BasicRenderer::PutPix(uint32_t X, uint32_t Y, uint32_t Colour){
    *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (X*4) + (Y * TargetFramebuffer->PixelsPerScanLine * 4)) = Colour;
}

uint32_t BasicRenderer::GetPix(uint32_t X, uint32_t Y){
    return *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (X*4) + (Y * TargetFramebuffer->PixelsPerScanLine * 4));
}

void BasicRenderer::DrawLine(int x, int y, int length, bool directionY, uint32_t colour){
    if(directionY == false){
        for(int xf = 0; xf < length; xf++){
            GlobalRenderer->PutPix(x, y, colour);
            x++;
        }
    }
    else
    {
        for(int yf = 0; yf < length; yf++){
            GlobalRenderer->PutPix(x, y, colour);
            y++;
        }
    }
}

//void BasicRenderer::DrawTextCursorPosition(){
//    DrawLine(GlobalRenderer->CursorPosition.X, GlobalRenderer->CursorPosition.Y, 16, true, GlobalRenderer->Colour);
//    for(int yf = 0; yf < 16; yf++){
//        //GlobalRenderer->GetPix(CursorPosition.X, y);
//        if(GlobalRenderer->CursorPosition.Y > 15){
//            GlobalRenderer->PutPix((GlobalRenderer->CursorPosition.X - 16), (GlobalRenderer->CursorPosition.Y - yf),  GlobalRenderer->GetPix((GlobalRenderer->CursorPosition.X),(GlobalRenderer->CursorPosition.Y + 1)));
//        }
//        
//    }
//}

void BasicRenderer::DrawRectAngle(int x, int y, int posX, int posY, uint32_t colour){
    DrawLine(posX, posY, x, false, colour); // Upper Line
    DrawLine(posX, posY, y, true, colour); // Left Line
    posY = posY + y;
    DrawLine(posX, posY, x, false, colour); // Down Line
    posY = posY - y;
    posX = posX + x;
    DrawLine(posX, posY, y, true, colour); // Right Line
}

void BasicRenderer::ClearMouseCursor(uint8_t* MouseCursor, Point Position){
    if (!MouseDrawn) return;

    int XMax = 16;
    int YMax = 16;
    int DifferenceX = TargetFramebuffer->Width - Position.X;
    int DifferenceY = TargetFramebuffer->Height - Position.Y;

    if (DifferenceX < 16) XMax = DifferenceX;
    if (DifferenceY < 16) YMax = DifferenceY;

    for (int Y = 0; Y < YMax; Y++){
        for (int X = 0; X < XMax; X++){
            int Bit = Y * 16 + X;
            int Byte = Bit / 8;
            if ((MouseCursor[Byte] & (0b10000000 >> (X % 8))))
            {
                if (GetPix(Position.X + X, Position.Y + Y) == MouseCursorBufferAfter[X + Y *16]){
                    PutPix(Position.X + X, Position.Y + Y, MouseCursorBuffer[X + Y * 16]);
                }
            }
        }
    }
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t* MouseCursor, Point Position, uint32_t Colour){
    int XMax = 16;
    int YMax = 16;
    int DifferenceX = TargetFramebuffer->Width - Position.X;
    int DifferenceY = TargetFramebuffer->Height - Position.Y;

    if (DifferenceX < 16) XMax = DifferenceX;
    if (DifferenceY < 16) YMax = DifferenceY;

    for (int Y = 0; Y < YMax; Y++){
        for (int X = 0; X < XMax; X++){
            int Bit = Y * 16 + X;
            int Byte = Bit / 8;
            if ((MouseCursor[Byte] & (0b10000000 >> (X % 8))))
            {
                MouseCursorBuffer[X + Y * 16] = GetPix(Position.X + X, Position.Y + Y);
                PutPix(Position.X + X, Position.Y + Y, Colour);
                MouseCursorBufferAfter[X + Y * 16] = GetPix(Position.X + X, Position.Y + Y);
            }
        }
    }

    MouseDrawn = true;
}

void BasicRenderer::Clear(){
    uint64_t FBBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t BytesPerScanline = TargetFramebuffer->PixelsPerScanLine * 4;
    uint64_t FBHeight = TargetFramebuffer->Height;
    uint64_t FBSize = TargetFramebuffer->BufferSize;

    for (int VerticalScanline = 0; VerticalScanline < FBHeight; VerticalScanline ++){
        uint64_t PixelPtrBase = FBBase + (BytesPerScanline * VerticalScanline);
        for (uint32_t* PixelPtr = (uint32_t*)PixelPtrBase; PixelPtr < (uint32_t*)(PixelPtrBase + BytesPerScanline); PixelPtr ++){
            *PixelPtr = ClearColour;
        }
    }
}

void BasicRenderer::ClearChar(){

    if (CursorPosition.X == 0){
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

    unsigned int XOffset = CursorPosition.X;
    unsigned int YOffset = CursorPosition.Y;

    unsigned int* PixelPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long Y = YOffset; Y < YOffset + 16; Y++){
        for (unsigned long X = XOffset - 8; X < XOffset; X++){
                    *(unsigned int*)(PixelPtr + X + (Y * TargetFramebuffer->PixelsPerScanLine)) = ClearColour;
        }
    }

    CursorPosition.X -= 8;

    if (CursorPosition.X < 0){
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

}

void BasicRenderer::Next(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
}

void BasicRenderer::Print(const char* String)
{
    
    char* Char = (char*)String;
    while(*Char != 0){
        PutChar(*Char, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > TargetFramebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        Char++;
    }
}

void BasicRenderer::PutChar(char Char, unsigned int XOffset, unsigned int YOffset)
{
    unsigned int* PixelPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* FontPtr = (char*)PSF1Font->GlyphBuffer + (Char * PSF1Font->PSF1Header->CharSize);
    for (unsigned long Y = YOffset; Y < YOffset + 16; Y++){
        for (unsigned long X = XOffset; X < XOffset+8; X++){
            if ((*FontPtr & (0b10000000 >> (X - XOffset))) > 0){
                    *(unsigned int*)(PixelPtr + X + (Y * TargetFramebuffer->PixelsPerScanLine)) = Colour;
                }

        }
        FontPtr++;
    }
}

void BasicRenderer::PutChar(char Char)
{
    PutChar(Char, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > TargetFramebuffer->Width){
        CursorPosition.X = 0; 
        CursorPosition.Y += 16;
    }
}
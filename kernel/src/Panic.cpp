#include "Panic.h"
#include "BasicRenderer.h"

void Panic(const char* PanicMessage){
    GlobalRenderer->ClearColour = 0x000000ff;
    GlobalRenderer->Clear();
    GlobalRenderer->CursorPosition = {0, 0};

    //GlobalRenderer->Colour = 0;

    GlobalRenderer->Print("PonchoOS Extended BSOD");
    GlobalRenderer->Next();
    GlobalRenderer->Print("Kernel Panic");
    GlobalRenderer->Next();
    GlobalRenderer->Next();
    GlobalRenderer->Print(PanicMessage);
}
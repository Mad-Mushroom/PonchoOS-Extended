#include "KernelUtil.h"
#include "Memory/Heap.h"
#include "Scheduling/PIT/PIT.h"
#include "windowStuff/window.h"
#include "CStr.h"

void startup(){
    GlobalRenderer->CursorPosition = {0,0};
    GlobalRenderer->ClearColour = 0x00b0b0;
    GlobalRenderer->Clear();
    GlobalRenderer->Print("PonchoOS Extended Codename Pluto [Version 0.5.8]");
    GlobalRenderer->Next();
    GlobalRenderer->Next();
}

extern "C" void KernelStart(BootInfo* BootInfo){
    KernelInfo KernelInfo = InitializeKernel(BootInfo);

    startup();

    Window window;
    window.makeWindow(600,400,200,200,0xffffffff,"Test Window 1");
    window.Fill(0x000000ff);
    window.Println("Hallo ich bin Gustav!", 0xffffffff);

    
    //PIT::SetDivisor(2000);

    /*for(int t = 0; t < 2000; t++){
        GlobalRenderer->Print("g");
        PIT::Sleepd(5);
    }*/

    while(true){
        asm ("hlt");
        GlobalRenderer->DrawTextCursorPosition();
    }

}
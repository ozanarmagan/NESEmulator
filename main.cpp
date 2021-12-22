#include "System/System.h"


int WinMain(int argc,char** argv)
{


    if(argc < 2)
        std::cout << "No NES File to read!";
    
    string fn = string("C:\\Users\\Ozan\\Desktop\\C++\\NES_Emulator\\smb.nes");

    NES nes(fn);

    
    

    nes.log();


    nes.start();

    return 0;
}
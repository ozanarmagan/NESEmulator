
#include "System/System.h"


int WinMain(int argc,char** argv)
{


    if(argc < 2)
        std::cout << "No NES File to read!";
    
    string fn = string("donkey kong.nes");

    NES nes(fn);


    nes.log();

    nes.start();

    return 0;
}

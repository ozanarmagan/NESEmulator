
#include "System/System.h"


int main(int argc,char** argv)
{
    if(argc < 2)
        std::cout << "No NES File to read!";
    
    std::string fn = std::string("smb.nes");

    nesemulator::NES nes(fn);

    nes.log();


    nes.start();

    return 0;
}

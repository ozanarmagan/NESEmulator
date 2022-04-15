
#include "System/System.h"


int main(int argc,char** argv)
{
    if(argc < 2)
        std::cout << "No NES File to read!";
    
    std::string fn = std::string(argv[1]);

    nesemulator::NES nes(fn);

    nes.log();

    nes.start();

    return 0;
}

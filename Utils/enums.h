#ifndef ENUMS_H
#define ENUMS_H


namespace nesemulator
{
    enum class MIRRORING { // Mirroring mode of the cartridge
        HORIZONTAL,VERTICAL,FOURSCREENS,ONSCREENLOWER,ONSCREENHIGHER
    };

    struct OBJECT_ATTRIBUTE // PPU OAM Object Attribute
    {
        BYTE y,id,attribute,x;
    };
}

#endif // !
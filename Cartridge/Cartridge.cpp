#include <fstream>
#include "Cartridge.h"

Cartridge::Cartridge(const std::string fileName)
{
    std::ifstream file(fileName,std::ifstream::binary);
    FileHeader header;
    if(file.is_open())
    {
        file.read((char*)&header, sizeof(FileHeader));
        if(header.mapper1 & 0x04)
            file.seekg(512,std::ios_base::cur);
        
        mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

        BYTE fileType = 1;

        if(fileType == 0) { }
        else if(fileType == 1)
        {
            PRGnum = header.PRGROMChunks;
            PRGmemory = new BYTE[PRGnum * 16384];
            file.read((char*) PRGmemory,PRGnum * 16384);

            CHRnum = header.CHRROMChunks;
            CHRmemory = new BYTE[CHRnum * 8192];
            file.read((char*) CHRmemory,CHRnum * 8192);
        } 

        file.close();
    }
}
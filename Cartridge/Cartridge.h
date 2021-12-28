#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "../Utils/handler.h"



class Cartridge 
{
    public:
        ~Cartridge() { delete [] PRGmemory; delete [] CHRmemory; };
        BYTE getPRGNum() { return PRGNum; };
        BYTE getCHRNum() { return CHRNum; };
        BYTE getPRGData(ADDRESS address) { return PRGmemory[address]; };
        BYTE getCHRData(ADDRESS address) { return CHRmemory[address]; };
        BYTE getMapperID() { return mapperID; };
        void setPRGData(ADDRESS address,BYTE value) { PRGmemory[address] = value; };
        void setCHRData(ADDRESS address,BYTE value) { CHRmemory[address] = value; };
        void setMapperID(int ID) { mapperID = ID; };
        void loadPRGData(BYTE* PRGHead) { PRGmemory = PRGHead; };
        void loadCHRData(BYTE* CHRHead) { CHRmemory = CHRHead; };
        void setPRGNum(BYTE _PRGNum) { PRGNum = _PRGNum;};
        void setCHRNum(BYTE _CHRNum) { CHRNum = _CHRNum;};
        friend std::ostream& operator<<(std::ostream& stream,Cartridge& cart);
    private:    
        BYTE* PRGmemory;
        BYTE* CHRmemory;

        BYTE mapperID;
        BYTE PRGNum;
        BYTE CHRNum;
};

#endif
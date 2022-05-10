#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "../Utils/handler.h"



namespace nesemulator
{
    /**
     * Cartridge class to store ROM data
     */
    class Cartridge 
    {
        public:
            BYTE getPRGNum() { return PRGNum; };
            BYTE getCHRNum() { return CHRNum; };
            BYTE getPRGData(ADDRESS32 address) { return PRGmemory.get()[address]; };
            BYTE getCHRData(ADDRESS32 address) { return CHRmemory.get()[address]; };
            BYTE getMapperID() { return mapperID; };
            void setPRGData(ADDRESS address,BYTE value) { PRGmemory.get()[address] = value; };
            void setCHRData(ADDRESS address,BYTE value) { CHRmemory.get()[address] = value; };
            void setMapperID(int ID) { mapperID = ID; };
            void loadPRGData(BYTE* PRGHead) { PRGmemory = std::unique_ptr<BYTE[]>(PRGHead); };
            void loadCHRData(BYTE* CHRHead) { CHRmemory = std::unique_ptr<BYTE[]>(CHRHead); };
            void setPRGNum(BYTE _PRGNum) { PRGNum = _PRGNum;};
            void setCHRNum(BYTE _CHRNum) { CHRNum = _CHRNum;};
            friend std::ostream& operator<<(std::ostream& stream,Cartridge& cart);
        private:    
            friend class NES;
            std::unique_ptr<BYTE[]> PRGmemory;
            std::unique_ptr<BYTE[]> CHRmemory;
            BYTE mapperID;
            BYTE PRGNum;
            BYTE CHRNum;
    };
}

#endif
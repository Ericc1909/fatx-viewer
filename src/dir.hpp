#ifndef DIR_HPP
#define DIR_HPP

#include <vector>
#include <string>

struct fatEntry;

class fatDir {
    public:
        std::vector<fatEntry> entries;
        std::vector<fatEntryLong> longEntries;
        std::string name;
        //unsigned int address;
        
        bool read(std::ifstream& img, unsigned int start = 0);
        bool open(std::ifstream& img, unsigned int number);
        bool saveFile(std::ifstream& img, unsigned int start);
        void print();
        void clear();
        
    protected:
        bool getNextCluster(std::ifstream& img, unsigned int& clusterNum);
        unsigned int clusterToBytes(unsigned int clusterNum);
        unsigned char checkSum(unsigned char *shortName);
        bool gotLongName(unsigned int number);
        std::string getLongName(unsigned int index);
        std::string nameToString(unsigned int number);
        std::string longNameToString(unsigned int number);
        bool isEntryAllowed(unsigned char letter, unsigned char attr);
};

#endif

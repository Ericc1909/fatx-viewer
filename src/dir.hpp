#ifndef DIR_HPP
#define DIR_HPP

#include <vector>
#include <string>

struct fatEntry;

class fatDir {
    public:
        std::vector<fatEntry> entries;
        std::string name;
        unsigned int address;
        
        bool read(std::ifstream& img, unsigned int start = 0);
        bool open(std::ifstream& img, unsigned int number);
        bool saveFile(std::ifstream& img, unsigned int start);
        void print();
        void clear();
        
    protected:
        bool getNextCluster(std::ifstream& img, unsigned int& clusterNum);
        unsigned int clusterToBytes(unsigned int clusterNum);
        std::string nameToString(unsigned int number);
        bool isEntryAllowed(unsigned char letter, unsigned char attr);
};

#endif

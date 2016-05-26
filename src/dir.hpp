#ifndef DIR_HPP
#define DIR_HPP

#include <vector>
#include <string>

struct fatEntry;

class fatDir {
    public:
        std::vector<std::pair<fatEntry, std::string> > entries;
        
        bool read(std::ifstream& img, unsigned int start = 0);
        bool open(std::ifstream& img, unsigned int number);
        bool saveFile(std::ifstream& img, unsigned int start);
        void print();
        void clear();
        
    protected:
        bool getNextCluster(std::ifstream& img, unsigned int& clusterNum);
        bool isEntryAllowed(unsigned char letter, unsigned char attr);
        unsigned int clusterToBytes(unsigned int clusterNum);
        std::string getLongName(const fatEntryLong& longEntry);
        std::string getName(const fatEntry& entry);
};

#endif

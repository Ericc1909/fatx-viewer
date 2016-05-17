#ifndef DIR_HPP
#define DIR_HPP

#include <vector>
#include <string>

struct fatEntry;

class fatDir {
    public:
        std::vector<std::pair<unsigned int, fatEntry> > entries;
        std::string name;
        unsigned int address;
        
        bool read(std::ifstream& img, unsigned int start);
        bool open(std::ifstream& img, unsigned int number);
        void print();
        void clear();
};

#endif

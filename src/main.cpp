#include "main.hpp"
#include <iostream>

fatBPB bpb;
fatInfo info;

int main(int argc, char* argv[]) {
    std::ifstream img;
    img.open(argv[1], std::ios::binary);
    
    if ( !img ) {
        std::cout << "Error: can't open disk or image." << std::endl;
        return -1;
    }
    
    else {

        bpb.set(img);
        info.set(bpb);
        
        fatDir* dir = new fatDir;
        unsigned int num = 0;
        
        std::cout << "[ FATx Viewer ]" << std::endl;
        std::cout << "You can browse directories and save files by entering a number" 
            << " in square brackets." << std::endl;
        std::cout << "Enter any non-numeric char for exit.\n" << std::endl;
        
        if ( !dir->read(img) ) {
            std::cout << "Disk is empty!" << std::endl;
        }
        dir->print();
        std::cout << "> ";
        
        while ( std::cin >> num ) {
            dir->open(img, num);
            dir->print();
            std::cout << "> ";
        }
        
        delete dir;
    }
    
    img.close();
    
    return 0;   
}

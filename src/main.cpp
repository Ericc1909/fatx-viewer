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
        dir->read(img, info.first_root_dir_bytes);
        dir->print();

        while ( std::cin >> num ) {
            dir->open(img, num);
            dir->print();
        }
        
        delete dir;
    }
    
    img.close();
    
    return 0;   
}

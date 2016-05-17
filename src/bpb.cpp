#include "bpb.hpp"
#include <iostream>

void fatBPB::set(std::ifstream& img) {
        img.seekg(11, img.beg);
        img.read((char*) &byts_per_sec, sizeof(byts_per_sec));
        
        img.seekg(13, img.beg);
        img.read((char*) &sec_per_clus, sizeof(sec_per_clus));
        
        img.seekg(14, img.beg);
        img.read((char*) &rsvd_sec_cnt, sizeof(rsvd_sec_cnt));
        
        img.seekg(16, img.beg);
        img.read((char*) &num_fats, sizeof(num_fats));
        
        img.seekg(17, img.beg);
        img.read((char*) &root_ent_cnt, sizeof(root_ent_cnt));
        
        img.seekg(19, img.beg);
        img.read((char*) &tot_sec16, sizeof(tot_sec16));
        
        img.seekg(22, img.beg);
        img.read((char*) &fats_z16, sizeof(fats_z16));
        
        img.seekg(32, img.beg);
        img.read((char*) &tot_sec32, sizeof(tot_sec32));
        
        img.seekg(36, img.beg);
        img.read((char*) &fats_z32, sizeof(fats_z32));
        
        img.seekg(44, img.beg);
        img.read((char*) &root_clus, sizeof(root_clus));
}

void fatBPB::print() {
    std::cout << byts_per_sec << std::endl;
    std::cout << (int) sec_per_clus << std::endl;
    std::cout << rsvd_sec_cnt << std::endl;
    std::cout << (int) num_fats << std::endl;
    std::cout << root_ent_cnt << std::endl;
    std::cout << tot_sec16 << std::endl;
    std::cout << fats_z16 << std::endl;
    std::cout << tot_sec32 << std::endl;
    std::cout << fats_z32 << std::endl;
    std::cout << root_clus << std::endl;
}

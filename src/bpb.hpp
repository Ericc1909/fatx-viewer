#ifndef BPB_HPP
#define BPB_HPP

#include <fstream>

class fatBPB {
    public:
        unsigned short byts_per_sec;
        unsigned char sec_per_clus;
        unsigned short rsvd_sec_cnt;
        unsigned char num_fats;
        unsigned short root_ent_cnt;
        unsigned short tot_sec16;
        unsigned short fats_z16;
        unsigned int tot_sec32;
        unsigned int fats_z32;
        unsigned int root_clus;
        
        void set(std::ifstream& img);
        void print();
};

extern fatBPB bpb;

#endif

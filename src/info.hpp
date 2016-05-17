#ifndef INFO_HPP
#define INFO_HPP

class fatBPB;

class fatInfo {
    public:
        unsigned short fat_type;
        unsigned int root_dir_sectors;
        unsigned int fats_z;
        unsigned int tot_sec;
        unsigned int data_sec;
        unsigned int count_of_clusters;
        unsigned int first_root_dir_sector;
        unsigned int first_root_dir_bytes;
        unsigned int first_data_sector;
        
        void set(const fatBPB& bpb);
        void print();
};

extern fatInfo info;

#endif

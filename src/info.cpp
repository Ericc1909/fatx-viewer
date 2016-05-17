#include "main.hpp"
#include <iostream>

void fatInfo::set(const fatBPB& bpb) {
        root_dir_sectors = ((bpb.root_ent_cnt * 32) + (bpb.byts_per_sec - 1)) 
                            / bpb.byts_per_sec;
        
        if ( bpb.fats_z16 ) {
            fats_z = bpb.fats_z16;
        } 
        else {
            fats_z = bpb.fats_z32;
        }
        
        if ( bpb.tot_sec16 ) {
            tot_sec = bpb.tot_sec16;
        }
        else {
            tot_sec = bpb.tot_sec32;
        }
        
        data_sec = tot_sec - (bpb.rsvd_sec_cnt + (bpb.num_fats * fats_z) 
                    + root_dir_sectors);
    
        count_of_clusters = data_sec / bpb.sec_per_clus;
        
        if ( count_of_clusters < 4085 ) {
            fat_type = 12;
        }
        else if ( count_of_clusters < 65525 ) {
            fat_type = 16;
        }
        else {
            fat_type = 32;
        }
        
        first_data_sector = bpb.rsvd_sec_cnt + (bpb.num_fats * fats_z)
                            + root_dir_sectors;
        
        if ( fat_type == 16 ) {
            first_root_dir_sector = first_data_sector - root_dir_sectors;
        }
        else if ( fat_type == 32 ) {
            first_root_dir_sector = ((bpb.root_clus-2) * bpb.sec_per_clus) 
                                    + first_data_sector;
        }
        
        first_root_dir_bytes = first_root_dir_sector * bpb.byts_per_sec;
}

void fatInfo::print() {
    std::cout << fat_type << std::endl;
    std::cout << root_dir_sectors << std::endl;
    std::cout << fats_z << std::endl;
    std::cout << tot_sec << std::endl;
    std::cout << data_sec << std::endl;
    std::cout << count_of_clusters << std::endl;
    std::cout << first_root_dir_sector << std::endl;
    std::cout << first_root_dir_bytes << std::endl;
    std::cout << first_data_sector << std::endl;
}

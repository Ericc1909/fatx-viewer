#include "main.hpp"
#include <iostream>
#include <iomanip>

bool fatDir::read(std::ifstream& img, unsigned int start) {
    fatEntry buf;
    bool canRead = true;
    unsigned int counter = 0;
    
    for ( unsigned int i = 0; canRead ; i += sizeof(fatEntry) ) {
        img.seekg(start + i, img.beg);
        img.read((char *) &buf, sizeof(fatEntry));
        
        if ( !buf.name[0] ) {
            canRead = false;
        }
        // no long names for now
        else if ( buf.attr != 0x0F ) {
            entries.push_back(std::make_pair(counter, buf));
            ++counter;
        }
    }
    
    if ( canRead ) {
        return false;
    }
    
    return true;
}

bool fatDir::open(std::ifstream& img, unsigned int number) {
    if ( number > entries.size() ) {
        return false;
    }
    
    for ( const auto& entry : entries ) {
        if ( number == entry.first ) {
            if ( entry.second.attr == 0x0010 ) {
                unsigned int firstSecBytes = 0;
                unsigned int clusterNum = (entry.second.fst_clus_hi << 16)
                    + entry.second.fst_clus_lo;
                
                if ( !clusterNum ) {
                    firstSecBytes = info.first_root_dir_bytes;
                }
                else {
                    firstSecBytes = ((clusterNum - 2) * bpb.sec_per_clus)
                        + info.first_data_sector;
                    
                    firstSecBytes *= bpb.byts_per_sec;
                }
                
                clear();
                read(img, firstSecBytes);
            }
            else {
                // not a dir
                return false;
            }
        }
    }
    
    return true;
}

void fatDir::print() {
    
    for ( const auto& entry : entries ) {
        std::cout << "[" << entry.first << "] ";
        for ( unsigned int i = 0; i < sizeof(entry.second.name); ++i) {
            if ( entry.second.name[i] != ' ' ) {
                std::cout << entry.second.name[i];
            }
            if ( i == 7 && entry.second.name[i + 1] != ' ' ) {
                std::cout << ".";
            }
        } 
        
        /* print size if entry is file
        if ( entry.second.attr != 0x0010 ) {
            std::cout << " " << std::setw(20) << std::right 
                << entry.second.file_size / 1024.0 << " Kb";
        }
        */
        
        std::cout << std::endl;
    }
}

void fatDir::clear() {
    if ( entries.size() ) {
        entries.clear();
    }
}

#include "main.hpp"
#include <string.h>
#include <iostream>
#include <iomanip>

bool fatDir::read(std::ifstream& img, unsigned int start) {
    fatEntry buf;
    fatEntryLong bufLong;
    std::string tmpName = "";
    bool canRead = true;
    
    if ( !start ) {
        start = clusterToBytes(0);
    }
    
    for ( unsigned int i = 0; canRead ; i += sizeof(fatEntry) ) {
        // if lastSec (in bytes) == start + i && fat == 16 -> break?
        img.seekg(start + i, img.beg);
        img.read((char *) &buf, sizeof(fatEntry));
        
        if ( !buf.name[0] ) {
            canRead = false;
        }
        else if ( buf.attr == 0x0F ) {
            memcpy(&bufLong, &buf, sizeof(bufLong));
            tmpName = getLongName(bufLong) + tmpName;
        }
        else if ( isEntryAllowed(buf.name[0], buf.attr) ) {
            if ( tmpName.length() ) {
                entries.push_back( std::make_pair(buf, tmpName) );
                tmpName = "";
            }
            else {
                entries.push_back( std::make_pair(buf, getName(buf)) );
            }
        }
    }
    
    // if nextCluster && fat16 -> read(img, newStart) ?
    
    if ( canRead ) {
        return false;
    }
    
    return true;
}

bool fatDir::isEntryAllowed(unsigned char letter, unsigned char attr) {
    if ( letter < 0x20 && letter != 0x05 ) {
        return false;
    }
    
    switch ( letter ) {
        case 0x00:
        case 0x20:
        case 0x22:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2F:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x7C:
        case 0xE5:
            return false;
        default:
            break;
    }
    
    switch ( attr ) {
        case 0x02:
            return false;
        default:
            return true;
    }
}

bool fatDir::open(std::ifstream& img, unsigned int number) {
    if ( number >= entries.size() ) {
        return false;
    }
    
    for ( size_t i = 0; i < entries.size(); ++i ) {
        if ( number == i ) {
            if (entries[i].first.attr == 0x0010) {
                unsigned int clusterNum = (entries[i].first.fst_clus_hi << 16)
                    + entries[i].first.fst_clus_lo;
                
                clear();
                
                if ( !read(img, clusterToBytes(clusterNum)) ) {
                    return false;
                }
            }
            else {
                if ( !saveFile(img, number) ) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

unsigned int fatDir::clusterToBytes(unsigned int clusterNum) {
    unsigned int firstSecBytes = 0;
    
    if ( !clusterNum ) {
        if ( info.fat_type == 16 ) {
            firstSecBytes = info.first_root_dir_sector * bpb.byts_per_sec;
        }
        else if ( info.fat_type == 32 ) {
            firstSecBytes = info.first_root_dir_bytes;
        }
    }
    else {
        firstSecBytes = ((clusterNum - 2) * bpb.sec_per_clus)
            + info.first_data_sector;
        
        firstSecBytes *= bpb.byts_per_sec;
    }
    
    return firstSecBytes;
}

bool fatDir::saveFile(std::ifstream& img, unsigned int number) {
    std::string filename = entries[number].second;
    
    std::ofstream file;
    file.open(filename, std::fstream::binary 
        | std::fstream::trunc | std::fstream::out);
    
    unsigned int clusterNum = (entries[number].first.fst_clus_hi << 16) 
        + entries[number].first.fst_clus_lo;
    unsigned char buffer[4096];
    unsigned int clusterSize = bpb.sec_per_clus * bpb.byts_per_sec;
    
    long fileLeft = entries[number].first.file_size;
    size_t clusterLeft = clusterSize;
    size_t bytesToRead = 0;
    size_t bytesRead = 0;
    
    bool nextClusterExists = true;

    // Go to first data cluster
    img.seekg(clusterToBytes(clusterNum), img.beg);
    
    // Read until we run out of file or clusters
    while( fileLeft > 0 && nextClusterExists ) {
        bytesToRead = sizeof(buffer);
        
        // don't read past the file or cluster end
        if (bytesToRead > (unsigned int) fileLeft) {
            bytesToRead = fileLeft;
        }
        
        if (bytesToRead > clusterLeft) {
            bytesToRead = clusterLeft;
        }
        
        // read data from cluster, write to file
        img.read((char*) &buffer, bytesToRead);
        bytesRead = img.gcount();
        file.write((char*) &buffer, bytesRead);
        
        // decrease byte counters for current cluster and whole file
        clusterLeft -= bytesRead;
        fileLeft -= bytesRead;
        
        // if we have read the whole cluster, read next cluster # from FAT
        if (clusterLeft == 0) {
            nextClusterExists = getNextCluster(img, clusterNum);
            img.seekg(clusterToBytes(clusterNum), img.beg);
            clusterLeft = clusterSize; // reset cluster byte counter
        }
    }
    
    std::cout << filename << " is saved to a local drive!\n" << std::endl;
    
    file.close();
    
    return true;
}

bool fatDir::getNextCluster(std::ifstream& img, unsigned int& clusterNum) {
    unsigned int fatOffset = 0;
    unsigned int fatEntOffset = 0;
    unsigned int nextCluster = 0;
    unsigned int fatSecNum = 0;
    unsigned int lastValidCluster = 0;
    
    if ( info.fat_type == 16 ) {
        fatOffset = clusterNum * 2;
    }
    else if ( info.fat_type == 32 ) {
        fatOffset = clusterNum * 4;
    }
    
    fatSecNum = bpb.rsvd_sec_cnt + (fatOffset / bpb.byts_per_sec);
    fatEntOffset = fatOffset % bpb.byts_per_sec;
    
    img.seekg(fatSecNum * bpb.byts_per_sec + fatEntOffset, img.beg);
    img.read((char*) &nextCluster, sizeof(nextCluster));
    
    if ( info.fat_type == 16 ) {
        nextCluster = (unsigned short) nextCluster;
        lastValidCluster = 0xFFF8;
    }
    else if ( info.fat_type == 32 ) {
        nextCluster &= 0x0FFFFFFF;
        lastValidCluster = 0x0FFFFFF8;
    }
    
    if ( nextCluster < lastValidCluster ) {
        if ( nextCluster == (lastValidCluster - 1) ) {
            // bad cluster
            return false;
        }
        else {
            clusterNum = nextCluster;
            return true;
        }
    }
    
    return false;
}

std::string fatDir::getName(const fatEntry& entry) {
    std::string converted = "";
    
    for ( unsigned int i = 0; i < sizeof(entry.name); ++i ) {
        if ( entry.name[i] != ' ' ) {
            converted += (char) entry.name[i];
        }
        
        if ( i == 7 && entry.name[i + 1] != ' ' ) {
            converted += '.';
        }
    }
    
    return converted;
}

std::string fatDir::getLongName(const fatEntryLong& longEntry) {
    std::string converted = "";
    
    for ( unsigned int i = 0; i < sizeof(longEntry.name_1); ++i ) {
        //ignore non-breaking space (255) and NULL (0) characters
        if ( longEntry.name_1[i] != 255 && longEntry.name_1[i] != 0  ) {
            converted += (char) longEntry.name_1[i];
        }
    }
    
    for ( unsigned int i = 0; i < sizeof(longEntry.name_2); ++i ) {
        if ( longEntry.name_2[i] != 255 && longEntry.name_2[i] != 0 ) {
            converted += (char) longEntry.name_2[i];
        }
    }
    
    for ( unsigned int i = 0; i < sizeof(longEntry.name_3); ++i ) {
        if ( longEntry.name_3[i] != 255 && longEntry.name_3[i] != 0 ) {
            converted += (char) longEntry.name_3[i];
        }
    }
    
    return converted;
}

void fatDir::print() {
    std::string buffer = "";
    
    for ( size_t i = 0; i < entries.size(); ++i ) {
        std::cout << "[" << i << "] ";
        
        std::cout << std::left << entries[i].second;
        
        // print size if entry is file
        if ( entries[i].first.attr != 0x0010 ) {
            std::cout << " % " << std::setprecision(2) << std::fixed 
                << entries[i].first.file_size / 1024.0 << " Kb";
        }
        else {
            std::cout << " % dir";
        }
        
        std::cout << std::endl;
    }
}

void fatDir::clear() {
    if ( entries.size() ) {
        entries.clear();
    }
}

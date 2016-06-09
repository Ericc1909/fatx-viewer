#ifndef ENTRY_HPP
#define ENTRY_HPP

#pragma pack(push,1)
struct fatEntry {
    unsigned char name[11];
    unsigned char attr;
    unsigned char nt_res;
    unsigned char crt_time_tenth;
    unsigned short crt_time;
    unsigned short crt_date;
    unsigned short lst_acc_date;
    unsigned short fst_clus_hi;
    unsigned short wrt_time;
    unsigned short wrt_date;
    unsigned short fst_clus_lo;
    unsigned int file_size;
};
#pragma pack(pop)

#pragma pack(push,1)
struct fatEntryLong {
    unsigned char ord;
    unsigned char name_1[10];
    unsigned char attr;
    unsigned char type;
    unsigned char chksum;
    unsigned char name_2[12];
    unsigned short fst_clus_lo;
    unsigned char name_3[4];
};
#pragma pack(pop)

#endif

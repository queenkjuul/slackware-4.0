/*
 *
 *    datadefs.h
 *
 *
 */

#define HDR_MAGIC       0xE69FAABB
#define MAX_NUM_DIRS    0x10000

struct file_header {
    long    magic;
    long    header_size;
    long    filesize;
    long    num_words;
    long    num_dirs;
    long    num_files;
    long    num_blocks;
    long    strings_size;
    long    hash_prime;
    long    hash_table_size;
    long    bytes_per_bitmap;
    long    filelists_size;
    long    bitmaps_size;
    long    block_table_offset;
    long    dir_table_offset;
    long    file_table_offset;
    long    strings_offset;
    long    hash_table_offset;
    long    filelists_offset;
    long    bitmaps_offset;
};

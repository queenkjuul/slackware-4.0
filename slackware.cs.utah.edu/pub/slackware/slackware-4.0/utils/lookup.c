/*
 *    lookup.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#ifdef unix
#include <unistd.h>
/* #include <strerror.h>*/
#define strnicmp  strncasecmp
#else
#include <io.h>
#include <dir.h>
#endif

#include "rockridg.h"
#include "datadef.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

static void Read(int fd, void *buf, int n, int swap);
#if 0
static void _Read(int fd, void *buf, int n, int swap, int line, char *file);
#define  Read(fd, buf, n, swap) _Read((fd), (buf), (n), (swap), __LINE__, __FILE__)
#endif

#ifdef unix
static const char datafile[] = "lookup.dat";
#else
static const char datafile[] = "/utils/lookup.dat";
#endif
char datafileroot[30];
static char word[256];
static struct file_header hdr;
static long rehash6;
static long rehash16;
static int fd;

static long
hash2(const unsigned char *s) {
  unsigned long h, g, r;
  int c;
  int i;

  r = h = ((long) s[0] << 16) + (s[1] << 8);
  for (i = 0; s[i] != 0; ++i) {
      h += ((long) s[i] << i) + s[i];
      h ^= h >> i;
  }
  while ((c = *s++) != '\0') {
      h <<= 4;
      h += c;
      g = h & 0xf0000000;
      h ^= g;
      h ^= (g >> 24);
      r ^= h;
  }
  rehash6 = r % 61;
  rehash16 = r % 65521;
  return h % hdr.hash_prime;
}

static int
search_file(const char *filename) {
    FILE *fp;
    int rv;
    static char line[257];
    char *s;
    int lc, uc, len;


/*    fprintf(stderr, "searching file %s\n", filename); */

    lc = *word;
    if (islower(lc)) {
        uc = lc + 'A' - 'a';
    } else {
        uc = lc;
    }
    len = strlen(word);
    if ((fp = fopen(filename, "rt")) == NULL) {
        fprintf(stderr, "Cannot open %s: %s\n", filename, strerror(errno));
        return 0;
    }
    rv = 0;
    while (fgets(line, 256, fp) != NULL) {
	line[255] = '\n';
	line[256] = '\0';
        for (s = line; *s != 0; ++s) {
            if (*s == lc || (*s == uc && isupper(*s))) {
                if (strnicmp(s, word, len) == 0) {
                    ++rv;
                    printf("%s: %s", filename, line);
		    break;
                }
            }
        }
    }
    fclose(fp);
    return rv;
}

static char *
lookup_dir(int n) {
    static char path[1024];
    unsigned char buf[300];
    unsigned char *p;
    short dir_num;
    long x;

    if (n == 0) {
#if unix
        path[0] = '.';
        path[1] = '\0';
#else	
        path[0] = '\0';
#endif	
        return path;
    }
    lseek(fd, hdr.dir_table_offset + n * sizeof(long), SEEK_SET);
    Read(fd, &x, sizeof(x), 1);
    lseek(fd, x, SEEK_SET);
    Read(fd, buf, sizeof(buf), 0);
    dir_num = buf[0] + (buf[1] << 8);
    p = lookup_dir(dir_num);
    strcat(p, "/");
    strcat(p, buf + 2);
    return p;
}

static char *
lookup_filename(long n) {
    unsigned char buf[300];
    unsigned char *p;
    short dir_num;

    lseek(fd, n, SEEK_SET);
    Read(fd, buf, sizeof(buf), 0);
    dir_num = buf[0] + (buf[1] << 8);
    p = lookup_dir(dir_num);
    strcat(p, "/");
    strcat(p, buf + 2);
    return p;
}

static int
search_block(int block) {
    int rv, i;
    long *p;
    long b[2];
    int nfiles;

/*    fprintf(stderr, "searching_block( %d == %x )\n", block, block);  */
    if (lseek(fd, hdr.block_table_offset + block * 4, SEEK_SET) < 0) {
	fprintf(stderr, "lseek <1> failed: %s\n", strerror(errno));
	exit(1);
    }
    Read(fd, &b, sizeof(b), 1);
    if (block == (hdr.num_blocks - 1)) {
        nfiles = hdr.num_files - b[0];
	assert(nfiles > 0);
    } else {
        nfiles = b[1] - b[0];
	assert(nfiles > 0);
    }
    p = malloc(nfiles * sizeof(*p));
    if (lseek(fd, hdr.file_table_offset + (b[0] * 4), SEEK_SET) < 0) {
	fprintf(stderr, "lseek <2> failed: %s\n", strerror(errno));
	exit(1);
    }
    assert(lseek(fd, 0L, SEEK_CUR) ==
           hdr.file_table_offset + (b[0] * 4));
    Read(fd, p, nfiles * 4, 1);
    for (rv = i = 0; i < nfiles; ++i) {

#ifndef unix
	char dir[3];

	sprintf(dir, "%c:", getdisk() + 'A');
        rv += search_file(get_real_filename(dir, lookup_filename(p[i])));
#else
        rv += search_file(lookup_filename(p[i]));
#endif	
    }
    free(p);
    return rv;
}

static int
search(unsigned long x) {
    int rv;
    unsigned char p[512];
    int i, j, n, b;
    long offset;

    rv = 0;
    offset = x & 0xffffffL;
    switch ((x >> 30) & 0x3) {

    case 0:
        lseek(fd, offset, SEEK_SET);
        Read(fd, p, sizeof(p), 0);
        for (i = 0; i < hdr.num_blocks; ++i) {
            if (p[i/8] & (1 << (i % 8))) {
                rv += search_block(i);
            }
        }
        break;

    case 1:
        rv = search_block(x & 0xfff);
        break;

    case 2:
        rv =  search_block(x & 0xfff);
        rv += search_block((x >> 12) & 0xfff);
        break;

    case 3:

        lseek(fd, offset, SEEK_SET);
        Read(fd, p, sizeof(p), 0);
        j = 0;
        n = p[j++];
        n += ((p[j] & 0x0f) << 8);
        for (i = 1; i < n + 1; ++i) {
            if (i & 1) {
                b = (p[j++] & 0xf0) >> 4;
                b += p[j++] << 4;
            } else {
                b = p[j++];
                b += (p[j] & 0x0f) << 8;
            }
            rv += search_block(b);
        }
        break;
    }
    return rv;
}

void
main(int argc, char **argv) {
    unsigned int len;
    unsigned int match;
    unsigned long h;
    unsigned long x;
    unsigned short i;

    if (argc != 2) {
        fprintf(stderr,
		"usage: lookup <word>\n"
		"       Search a CDROM for all files containing\n"
	        "       the specified word.\n");
        exit(1);
    }
    strcpy(word, argv[1]);
    len = strlen(word);
    for (i = 0; i < len; ++i) {
        if (isupper(word[i])) {
            word[i] = tolower(word[i]);
        }
    }
    strcpy(datafileroot, "utils/");
    if ((fd = open("utils/lookup.dat", O_RDONLY|O_BINARY)) < 0) {
	fprintf(stderr, "Cannot open %s: %s.\n",
		"utils/lookup.dat", strerror(errno));
	fprintf(stderr,
		"Cd to the root directory of the cdrom and try again.\n");
	exit(1);
    }

    Read(fd, &hdr, sizeof(hdr), 1);
    assert(hdr.header_size == sizeof(hdr));
    assert(hdr.filesize == lseek(fd, 0L, SEEK_END));
    assert(hdr.hash_prime != 0);
    h = hash2(word);
    match = 0;
    for (i = 0;; ++i) {
        lseek(fd, (h * 4) + hdr.hash_table_offset, SEEK_SET);
        Read(fd, &x, sizeof(x), 1);
        if (x == 0) {
            break;
        }
        if (((x >> 30) == 1 && ((x >> 12) & 0xffff) == rehash16) ||
            ((x >> 30) != 1 && ((x >> 24) & 0x003f) == rehash6)) {
            match = search(x);
            break;
        }
        h = (h + rehash6 + i) % hdr.hash_prime;
    }
    if (match == 0) {
        printf("sorry, no match\n");
    }
    exit(match == 0);
}

static void
Read(int fd, void *buf, int n, int swap) {

    assert(n > 0);
    assert(swap == 0 || (n & 3) == 0);
    if (read(fd, buf, n) < 0) {
        fprintf(stderr, "read failed: %s\n", strerror(errno));
        exit(1);
    }
#ifdef sparc
    if (swap) {
        int i;
        unsigned char c;
        unsigned char *p;

        p = buf;
        for (i = 0; i < n; i += 4) {
            c = p[i];
            p[i] = p[i + 3];
            p[i + 3] = c;
            c = p[i + 1];
            p[i + 1] = p[i + 2];
            p[i + 2] = c;
        }
    }
#endif
    return;
}
   
#if defined(sparc) && ! defined(__svr4__)
const char *
strerror(error)
    int error;
{
    static char defaultMsg[32];
    extern int sys_nerr;
    extern char *sys_errlist[];

    if ((error < sys_nerr) && (error > 0)) {
        return sys_errlist[error];
    }
    (void) sprintf(defaultMsg, "unknown error (%d)", error);
    return defaultMsg;
}
#endif


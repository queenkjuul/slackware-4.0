/*
 * Rockridg.c      handles filename conversion.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "rockridg.h"

static const char trans_tbl[] = "\\00_trans.tbl";
#if 0
static const char trans_tbl[] = "\\trans.tbl";
#endif

int
is_a_slash(char c) {
  return (c == '\\' || c == '/');
}

/*  If the disc has Rock Ridge extensions, the names in the 00_index.txt
 *  files are not the real names on the disc.  Look up the
 *  actual names in the 00_trans.tbl files.
 */
char *
get_real_filename(char *base_dir, char *rockridge_name) {
  static char rtn_str[80];
  char in_line[128];
  FILE *fp;
  char *s, *t, c, *eof;
  int i, flen, len;

  assert(base_dir[0] != '\0');
  assert(base_dir[1] == ':');

  strcpy(rtn_str, base_dir);
  s = rockridge_name;
  t = rtn_str + strlen(rtn_str) - 1;
  if (is_a_slash(*t))
    *t = '\0';
  else
    t++;
  if (is_a_slash(*s))
    s++;

 /*
  * Step through the rockridge path one directory at a time and build
  * the "real" path by reading the 00_trans.tbl at each level.
  */

  while (1) {
    flen = strlen(rtn_str);
    strcpy(rtn_str + flen, trans_tbl);

    for (t = s; *t != '\0' && *t != '/' && *t != '\\'; ++t);
    c = *t;
    *t = '\0';

    if ((fp = fopen(rtn_str, "rt")) == NULL) {
      *t = c;                           /* Can't open 00_trans.tbl. */
      return rockridge_name;            /* Return the original filename. */
    }

    while ((eof = fgets(in_line, sizeof(in_line) - 1, fp)) != NULL) {
      if (*in_line == 'L')
        continue;

      assert(*in_line == 'F' || *in_line == 'D');
      assert(in_line[1] == ' ');
      assert(in_line[36] == '\t');
      len = strlen(in_line);
      assert(in_line[len - 1] == '\n');
      in_line[--len] = '\0';
      if (stricmp(s, in_line + 37) == 0)
        break;
    }
    fclose(fp);
    if (eof == NULL)        /* Entry not found.  Return original filename. */
      return rockridge_name;

    for (i = 36; in_line[i] <= ' '; --i)
      in_line[i] = '\0';

    strcpy(rtn_str + flen + 1, in_line + 2);
    if (c == '\0')
      return rtn_str;

    *t = c;
    s = t + 1;
  }
}

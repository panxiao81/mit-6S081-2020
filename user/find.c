#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  // +1 for '\0'
  memmove(buf, p, strlen(p)+1);
  return buf;
}

void
find(char *path, char *filename) {
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    int fd;

    // printf("Debug: find path:%s, filename:%s\n", path, filename);

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:
        // printf("Debug: in file fmtname(path):%s filename:%s strcmp: %d\n", fmtname(path),filename,  strcmp(fmtname(path), filename));
        if(strcmp(fmtname(path), filename) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            fprintf(2, "find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0 || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)) {
                continue;
            }
            // printf("Debug: in switch dir de.name %s\n", de.name);
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // printf("Debug: in switch dir buf %s\n", buf);
            find(buf, filename);
        }
        break;
    }
    close(fd);
}

int 
main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(2, "Usage: find [path] [filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
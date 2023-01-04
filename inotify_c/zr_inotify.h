//
// Created by zr on 2023/1/2.
//

#ifndef TEST_ZR_INOTIFY_H
#define TEST_ZR_INOTIFY_H
#define BUF_LEN ((sizeof (struct inotify_event)+NAME_MAX+1)*10)
#define ZR_INOTIFY_WATCH 2233


#include <ostream>
#include <sys/inotify.h>
#include <dirent.h>
#include <iostream>
#include <map>
#include <cstring>
#include <csignal>
#include <sys/stat.h>
#include <mytime.h>
#include "../color/myout.h"


using namespace std;
typedef struct inotify_watch_struct watch_struct;
typedef struct dirent dirent;
typedef struct stat file_stat;

typedef map<int, watch_struct> i_map;
typedef map<const string,int> w_map;

struct inotify_watch_struct{
    char *path;
    w_map * inotifyMaps;
};

class zr_inotify {
private:
    int inotifyFd;
    int num;
    i_map watch_fd_map;
    int for_dir(char *path,w_map *wmap);
   int add_watch(char *path,char *name,w_map *wmap,char **path2);
    static void print_log(unsigned int flag,const char *str1,const char *str2);

protected:
    virtual int func_moveto(file_stat st,char *path);
    virtual int func_create_dir(file_stat st,char *path);
    virtual int func_create_file(file_stat st,char *path);
    virtual int func_movefrom(file_stat st,char *path);
    virtual int func_delete(file_stat st,char *path);

public:
    zr_inotify(char *path);
    ~zr_inotify();
    char *startinotify();
    friend std::ostream &operator<<(std::ostream &os, const zr_inotify &inotify);
};


#endif //TEST_ZR_INOTIFY_H

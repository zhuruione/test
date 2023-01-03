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
#include <vector>


using namespace std;

typedef vector<int > int_vector;
typedef struct inotify_watch_struct{
    char *path;
    int_vector inotifyFds;
} watch_struct;

typedef map<int, watch_struct> i_map;
typedef struct dirent dirent;



class zr_inotify {

private:
    int inotifyFd;
    int num;
    i_map watch_fd_map;
    int for_dir(char *path);
    char * add_watch(char *path,char *name);
    char * remove_watch(char *path, char *name);
    static void print_log(unsigned int flag,const char *str1,const char *str2);
public:
    zr_inotify(char *path);

    ~zr_inotify();

    char *readinotify();


    friend std::ostream &operator<<(std::ostream &os, const zr_inotify &inotify);

};


#endif //TEST_ZR_INOTIFY_H

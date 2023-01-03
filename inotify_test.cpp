//
// Created by zr on 2022/12/31.
//
#include <stdlib.h>
#include <sys/inotify.h>
#include <string>
#include <fcntl.h>
#include <csignal>
#include <limits.h>
#include <mytime.h>
#define BUF_LEN ((sizeof (struct inotify_event)+NAME_MAX+1)*10)


using namespace std;
int main(){

    int inotifyFd;
    ssize_t numRead;
    struct inotify_event *event;
    char buf[BUF_LEN];
    char *p;


    inotifyFd=inotify_init();
    inotify_add_watch(inotifyFd,"/home/zr/test",IN_ALL_EVENTS);
    inotify_add_watch(inotifyFd,"/home/zr/test/a",IN_ALL_EVENTS);
    while (true){
        numRead= read(inotifyFd,buf,BUF_LEN);
        for(p=buf;p<buf+numRead;){
            event=(struct inotify_event *)p;
            printf("%s%s\n",getTime(),event->name);
            p+=sizeof (struct inotify_event)+event->len;
        }
    }
}

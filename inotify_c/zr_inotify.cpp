//
// Created by zr on 2023/1/2.
//


#include "zr_inotify.h"

zr_inotify::zr_inotify(char *path) {
    //add watch the root path
    inotifyFd = inotify_init();
    add_watch(path, "");
    for_dir(path);

}

zr_inotify::~zr_inotify() {

}

char *zr_inotify::readinotify() {
    char buf[BUF_LEN];
    int numRead;
    struct inotify_event *event;
    char *p;
    while (true) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        for (p = buf; p < buf + numRead;) {

            if (p == "NULL22")
                return 0;
            event = (struct inotify_event *) p;
            char *path =watch_fd_map.at(event->wd).path;

            int maxlen = strlen(path) + strlen(event->name) + 2;
            char filepath[maxlen];
            snprintf(filepath, maxlen, "%s/%s", path, event->name);
            if (event->mask & IN_CREATE) {                                      //create
                struct stat stat1;
                stat(filepath, &stat1);
                print_log(IN_CREATE, nullptr, nullptr);
                if (S_ISDIR(stat1.st_mode)) {                                   //create dir
                    myprintf(YELLOW,filepath);
                    cout <<  " dir be created" << endl;
                    add_watch(path, event->name);
                } else {                                                          //create file
                    myprintf(YELLOW,filepath);
                    cout <<  " file be created" << endl;
                }

            } else if (event->mask & IN_DELETE) {                              //delete
                print_log(IN_DELETE,filepath, nullptr);
            } else if (event->mask & IN_MOVED_TO) {                             //move to
                print_log(IN_MOVED_TO,filepath,path);


            } else if (event->mask & IN_MOVED_FROM) {                           //move from

                print_log(IN_MOVED_FROM,filepath,path);
            }

            p += event->len + sizeof(struct inotify_event);
        }
    }
}


std::ostream &operator<<(std::ostream &os, const zr_inotify &inotify) {
    os << " num: " << inotify.num;
    return os;
}

char *zr_inotify::add_watch(char *path, char *name) {
    int maxlen = strlen(path) + strlen(name) + 2;
    char *path2 = (char *) malloc(sizeof(char) * maxlen);
    snprintf(path2, maxlen, "%s/%s", path, name);
    int watch_id = inotify_add_watch(inotifyFd, path2, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE | IN_MOVE_SELF);
    print_log(ZR_INOTIFY_WATCH,path2, to_string(watch_id).c_str() );
    if (watch_id == -1) {
        return path2;
    } else {
        watch_fd_map[watch_id] .path= path2;
    }

    return path2;
}

//char *zr_inotify::remove_watch(char *path) {
//    int watch_id = inotify_rm_watch(inotifyFd,)
//    print_log(ZR_INOTIFY_WATCH,path2, to_string(watch_id).c_str() );
//    if (watch_id == -1) {
//        return path2;
//    } else {
//        watch_fd_map[watch_id] = path2;
//    }
//
//    return path2;
//}

int zr_inotify::for_dir(char *path) {

    DIR *dir = opendir(path);
    if (!dir) {
        return 0;
    }
    if (!strcmp("/", path)) {
        path = "";
    }
    dirent *d = readdir(dir);
    int i = 0;
    while (d) {
        if (d->d_type == DT_DIR && strcmp(".", d->d_name) && strcmp("..", d->d_name)) {
            for_dir(add_watch(path, d->d_name));
        }
        d = readdir(dir);
    }
    return 1;
}

 void zr_inotify::print_log(unsigned int flag, const char *str1,const char *str2) {
    myprintf(LIGHT_BLUE, getTime());
    printfblock();
    switch (flag) {
        case IN_MOVED_TO:
            myprintf(RED,"MOVE_TO ");
            myprintf(YELLOW,str1);
            cout <<  " has being moved to " ;
            myprintf(YELLOW,str2);
            cout<<endl;
            break;
        case IN_CREATE:
            myprintf(RED,"CREATE  ");
            break;
        case IN_MOVED_FROM:
            myprintf(RED,"MOVE_FM ");
            myprintf(YELLOW,str1);
            cout << " has being moved from " ;
            myprintf(YELLOW,str2);
            cout<<endl;

            break;
        case IN_DELETE:
            myprintf(RED,"DELETE  ");
            myprintf(YELLOW,str1);
            cout  << " has being deleted " << endl;
            break;
        case ZR_INOTIFY_WATCH:

            if (strcmp("-1",str2)) {
                myprintf(RED,"WATCH   ");
                myprintf(YELLOW,str1);
                cout  << " has being watched --watch id=" << str2 << endl;
            } else {
                myprintf(LIGHT_RED,"ERROR   ");
                cout<<"watch" << str1<<"failed" << endl;

            }

            break;
    }

    if (flag == IN_MOVED_TO) {

    }
}
//
// Created by zr on 2023/1/2.
//


#include "zr_inotify.h"


zr_inotify::zr_inotify(char *path) {
    printf("father\n");
    //add watch the root path
    inotifyFd = inotify_init();
    w_map *wmap = new w_map;
    int watch_fd = add_watch(path, "", wmap, nullptr);
    for_dir(path, watch_fd_map.at(watch_fd).inotifyMaps);

}

zr_inotify::~zr_inotify() {

}

char *zr_inotify::startinotify() {
    char buf[BUF_LEN];
    int numRead;
    struct inotify_event *event;
    char *p;
    struct stat stat1;

    while (true) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        for (p = buf; p < buf + numRead;) {
            if (p == "NULL22")
                return 0;
            event = (struct inotify_event *) p;
            char *path = watch_fd_map.at(event->wd).path;  //where happen
            int maxlen = strlen(path) + strlen(event->name) + 2;
            char filepath[maxlen];
            snprintf(filepath, maxlen, "%s/%s", path, event->name);
            stat(filepath, &stat1);
            if (event->mask & IN_CREATE) {                                      //create
                print_log(IN_CREATE, nullptr, nullptr);

                if (S_ISDIR(stat1.st_mode)) {                                   //create dir
                    myprintf(YELLOW, filepath);

                    func_create_dir(stat1,filepath);
                    cout << " dir be created" << endl;
                    add_watch(path, event->name, watch_fd_map.at(event->wd).inotifyMaps, nullptr);
                } else {                                                          //create file
                    myprintf(YELLOW, filepath);
                    func_create_file(stat1,filepath);
                    cout << " file be created" << endl;
                }

            } else if (event->mask & IN_DELETE) {                              //delete


                func_delete(stat1,filepath);
                print_log(IN_DELETE, filepath, nullptr);
            } else if (event->mask & IN_MOVED_TO) {                             //move to
                char *path2;
                int wd = add_watch(path, event->name, watch_fd_map.at(event->wd).inotifyMaps, &path2);
                for_dir(path2, watch_fd_map.at(wd).inotifyMaps);


                func_moveto(stat1,filepath);
                print_log(IN_MOVED_TO, basename(filepath), path);


            } else if (event->mask & IN_MOVED_FROM) {                           //move from
                w_map wMap = *watch_fd_map.at(event->wd).inotifyMaps;
                int wd = wMap.at(event->name);
                wMap.erase(event->name);

                func_movefrom(stat1,filepath);
                print_log(IN_MOVED_FROM, filepath, path);
            }
            p += event->len + sizeof(struct inotify_event);
        }
    }
}


int zr_inotify::add_watch(char *path, char *name, w_map *wmap, char **watchpath) {
    int maxlen = strlen(path) + strlen(name) + 2;
    char *path2 = (char *) malloc(sizeof(char) * maxlen);
    if (watchpath)
        *watchpath = path2;
    snprintf(path2, maxlen, "%s/%s", path, name);
    int watch_id = inotify_add_watch(inotifyFd, path2, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE | IN_MOVE_SELF);
    print_log(ZR_INOTIFY_WATCH, path2, to_string(watch_id).c_str());
    if (watch_id == -1) {
        return 0;
    } else {
        watch_fd_map[watch_id].path = path2;
        watch_fd_map[watch_id].inotifyMaps = new w_map;
        wmap->insert({name, watch_id});
        return watch_id;
    }

}

int zr_inotify::for_dir(char *path, w_map *wmap) {
    DIR *dir = opendir(path);
    if (!dir) {
        return 0;
    }
    if (!strcmp("/", path)) {
        path = "";
    }
    dirent *d = readdir(dir);
    while (d) {
        if (d->d_type == DT_DIR && strcmp(".", d->d_name) && strcmp("..", d->d_name)) {
            char *path2;
            int watch_id = add_watch(path, d->d_name, wmap, &path2);
            for_dir(path2, watch_fd_map.at(watch_id).inotifyMaps);
        }
        d = readdir(dir);
    }
    return 1;
}

void zr_inotify::print_log(unsigned int flag, const char *str1, const char *str2) {
    myprintf(LIGHT_BLUE, getTime());
    printfblock();
    switch (flag) {
        case IN_MOVED_TO:
            myprintf(RED, "MOVE_TO ");
            cout << "Dir ";
            myprintf(YELLOW, str1);
            cout << " has being moved to ";
            myprintf(YELLOW, str2);
            cout << endl;
            break;
        case IN_CREATE:
            myprintf(RED, "CREATE  ");
            break;
        case IN_MOVED_FROM:
            myprintf(RED, "MOVE_FM ");
            myprintf(YELLOW, str1);
            cout << " has being moved from ";
            myprintf(YELLOW, str2);
            cout << endl;
            break;
        case IN_DELETE:
            myprintf(RED, "DELETE  ");
            myprintf(YELLOW, str1);
            cout << " has being deleted " << endl;
            break;
        case ZR_INOTIFY_WATCH:
            if (strcmp("-1", str2)) {
                myprintf(RED, "WATCH   ");
                myprintf(YELLOW, str1);
                cout << " has being watched --watch id=" << str2 << endl;
            } else {
                myprintf(LIGHT_RED, "ERROR   ");
                cout << "watch" << str1 << "failed" << endl;

            }

            break;
    }
}



std::ostream &operator<<(std::ostream &os, const zr_inotify &inotify) {
    os << " num: " << inotify.num;
    return os;
}

int zr_inotify::func_moveto(file_stat st, char *path) {
    return 1;
}

int zr_inotify::func_create_dir(file_stat st, char *path) {
    return 1;
}

int zr_inotify::func_create_file(file_stat st, char *path) {
    return 1;
}

int zr_inotify::func_movefrom(file_stat st, char *path) {
    return 1;
}

int zr_inotify::func_delete(file_stat st, char *path) {
    return 1;
}

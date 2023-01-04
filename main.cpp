#include <iostream>
#include <mysql/mysql.h>
#include "inotify_c/zr_inotify.h"
#include "color/myout.h"
using namespace std;

#include <mytime.h>

class my_inotify:public zr_inotify{
public:
    my_inotify(char *path) : zr_inotify(path) {
        printf("son\n");
    }
protected:
    int func_moveto(file_stat st, char *path) override {
        printf("my   !!!!!!!!!!!!\n");
        return zr_inotify::func_moveto(st, path);
    }
};

int main() {

    clock_t t=clock();
    my_inotify z("/home/zr/test");

    z.startinotify();

}

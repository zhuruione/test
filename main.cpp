#include <iostream>
#include <mysql/mysql.h>
#include "inotify_c/zr_inotify.h"
#include "color/myout.h"
using namespace std;

#include <mytime.h>

int main() {
    map<int,int> m;
    clock_t t=clock();
    zr_inotify z= zr_inotify("/home/zr/test");
    z.readinotify();



}

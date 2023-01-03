//
// Created by zr on 2023/1/1.
//
#include <mysql/mysql.h>
#include <iostream>
using namespace std;
int main(){
    MYSQL *mysql;
    mysql=mysql_init(NULL);
    if(!mysql){
        cout<<"failed to initialize the mysql"<<endl;
    }
    if(!mysql_real_connect(mysql,"zrone.top","root","657431zhurui","file",0,NULL,0)){
        cout<<"connect to zrone.top failed"<<endl;
    } else{
        cout<<"connect to zrone.top successful"<<endl;
    }


}


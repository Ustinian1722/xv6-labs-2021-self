#include "kernel/types.h"
#include "user/user.h"

//就是从终端读取信息后 使用系统调用sleep来实现睡眠操作
int main(int argc,char *argv[]){
    //参数个数 and 参数的值
    if(argc!=2){
        fprintf(2,"usage: sleep[ticknums]");
        exit(1);
    }
    int ticks=atoi(argv[1]);
    sleep(ticks);
    exit(0);
}
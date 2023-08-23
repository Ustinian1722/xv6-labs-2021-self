#include "kernel/types.h"
#include "user/user.h"

//我们要建立两个管道
//管道pipe 作用是让父子进程进行通信
// //i am parent process
        // p1[0];//pipe1 read parent
        // p1[1];//pipe1 write
        // p2[0];//pipe2 read  child
        // p2[1];//pipe2 write
        // //0 stdread 1 stdwrite
int main(int argc,char*argv[]){
    //首先创建两个长度为2的数组再调用pipe
    int pid;//process id
    int p1[2],p2[2];
    char buf[]={'a'};
    pipe(p1);
    pipe(p2);
    if(fork()==0){//create child process return pid
    //now i am child process
        pid=getpid();
        close(p1[1]);//free p1[1] 要使父进程进行写就要在child释放
        close(p2[0]);
        read(p1[0],buf,1);
        printf("%d:received ping\n",pid);
        write(p2[1],buf,1);
        exit(0);
    }else{
        pid=getpid();
        close(p2[1]);
        close(p1[0]);
        write(p1[1],buf,1);
        read(p2[0],buf,1);
        printf("%d:received pong\n",pid);
        exit(0);
    }
}
#include "kernel/types.h"
#include "user/user.h"



//做一个并发的素数筛 我们要及时的关闭不用的管道
//首先找到第一个素数然后输出 并将该数倍数都置为ZERO
void prime(int readfd){
    //receive a read filedescptior
    //从读端接受到了数
    int n;
    read(readfd,&n,4);
    printf("prime %d\n",n);
    //若是素数则给标志 没有新建线程
    int created=0;
    int fd[2];
    int num;
    //一直接受一直判断
    while (read(readfd,&num,4)!=0)
    {
        if(created==0){
            pipe(fd);
            created=1;
            int pid=fork();
            if(pid==0){
                close(fd[1]);
                prime(fd[0]);
                return;
            }else{
                close(fd[0]);
                }
        }
        if(num%n!=0){
            write(fd[1],&num,4);
        }
    }
    close(readfd);
    close(fd[1]);
    wait(0);
    
}


int main(int argc,char *agrv[]){
    int fd[2];
    pipe(fd);//创建管道
    int pid=fork();
    if(pid>0){
        //parent main process
        close(fd[0]);//用不到输出流就关掉
        for(int i=2;i<=35;i++){
            //把数字写入
            write(fd[1],&i,4);
        }
        //使用完了输入流也要即使关掉
        close(fd[1]);
        //等待子进程关闭才推出
        wait(0);
    }

    if(pid==0){
        //child process 子进程是要使用read
        close(fd[1]);
        //中间使对于接受到素数输出 过滤创建新线程
        prime(fd[0]);
        close(fd[0]);
    }
    exit(0);
}
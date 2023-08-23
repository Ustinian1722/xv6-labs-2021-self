#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MAXARG       32 
#define buf_size         512
/*使用fork 和exec 处理每一行的输入,在父进程中使用wait函数等待子进程先结束.

读输入中的每一行, 你可以每次只读一个char 直到遇见'\n'

kernel/param.h 声明了MAXARG, 这对你如何声明一个argv数组很有帮助

#define MAXARG       32  // max exec arguments

添加xargs用户程序 到Makefile的 UPROGS中

当你修改文件系统的时候 你的修改并不会随着qemu的退出而清除; 使用make clean 获得未被修改过的文件系统

*/

int
main(int argc,char* argv[]){
    //what is xargs?
    //1 read lines from standard input
    //2 and run a command for each line,
    //3 supplying the line as arguments to the command.
      //不用入参把 直接从标准输出中读
    
    char buf[buf_size+1]={0};
    char* xargv[MAXARG]={0};
    
    int stdin_end=0;
    int occupy=0;//占用了的

    for(int i=1;i<argc;i++){
        xargv[i-1]=argv[i];
    }
    while(!(stdin_end&&occupy)){
        //可以是同str
        //如果输入的停止了 我们应该把剩下的所有读入buffer
        if(!stdin_end){
            int remain_size=buf_size-occupy;
            int read_bytes=read(0,buf+occupy,remain_size);
            if(read_bytes<0){
                fprintf(2,"xargs:read retunrns -1 error");
            }
            if(read_bytes==0){
                close(0);//关闭标准输入流 不允许输出了
                stdin_end=1;
            }
            occupy+=read_bytes;
        }

        char *line_end=strchr(buf,'\n');
        while(line_end){
            //发现换行符
            //截取并复制到新的
            char nbuf[buf_size+1]={0};
            memcpy(nbuf,buf,line_end-buf);
            //把一行指令当作参数传递给xargs
            xargv[argc-1]=nbuf;
            int pid=fork();
            if(pid==0){
            // //是否可以读的判断
            // if(read(0,&buf,sizeof(char))==0){
            //  fprintf(2,"input is empty！");
            //  }
            // if(read(0,&buf,sizeof(char))<0){
            //     fprintf(2,"read error！");
            // }
            if(!stdin_end){
                close(0);
            }
            if(exec(argv[1],xargv)<0){
                //没执行成功
                fprintf(2,"xargs:exec fails with -1\n");
                exit(1);//异常退出
            }
        //我们要维护一个滑动窗口来解决 1 在啥时候输入端关闭了 并且 buf中读取的指令已经执行完了

        //child process 读取每一行 这是读一个字节 读一行的话就是遇到\n
        //读完之后 argv[] 数组里面就存放了参数
    }else{
        //parent process
        //移除已经执行过的那一端 将剩下的覆盖到前面
        memmove(buf,line_end+1,occupy-(line_end-buf)-1);
        occupy-=line_end-buf+1;\
        memset(buf+occupy,0,buf_size-occupy);//后续补充上0
        int k;
        wait(&k);
        //更新分界点
        line_end=strchr(buf,'\n');
        }
    }
}
exit(0);
}
//
//  main.cpp
//  select-demo
//
//  Created by 王晓鹏 on 2020/10/14.
//

#include <iostream>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, const char * argv[]) {
    
    fd_set rfds;
    printf("size of fd_set %ld \n", sizeof(fd_set));
    
    struct timeval tv;
    int ret;
    int fd;
    
    const char *fifo_name = "/tmp/my_fifo";

    unlink( fifo_name );
    ret = mkfifo(fifo_name, 0666); // 创建有名管道
    if(ret < 0 && std::codecvt_base::error != EEXIST){
        perror("mkfifo：");
    }
    
    fd = open(fifo_name, O_RDWR); // 读写方式打开管道
    if(fd < 0){
        perror("open fifo");
        return -1;
    }
    
    ret = 0;
    
    while(1){
        // 这部分内容，要放在while(1)里面
        FD_ZERO(&rfds);        // 清空
        FD_SET(0, &rfds);   // 标准输入描述符 0 加入集合
        FD_SET(fd, &rfds);  // 有名管道描述符 fd 加入集合
        
        // 超时设置
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        // 监视并等待多个文件（标准输入，有名管道）描述符的属性变化（是否可读）
        // 没有属性变化，这个函数会阻塞，直到有变化才往下执行，这里没有设置超时
        // FD_SETSIZE 为 <sys/select.h> 的宏定义，值为 1024
        printf("select 监视并等待多个文件\n");
        ret = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
//        ret = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
 
        if(ret == -1){ // 出错
            perror("select()");
        }else if(ret > 0){ // 准备就绪的文件描述符
            printf("准备就绪的文件描述符");
            
            char buf[100] = {0};
            if( FD_ISSET(0, &rfds) ){ // 标准输入
                read(0, buf, sizeof(buf));
                printf("stdin buf = %s\n", buf);
                
            }else if( FD_ISSET(fd, &rfds) ){ // 有名管道
                read(fd, buf, sizeof(buf));
                printf("fifo buf = %s\n", buf);
            }
            
        }else if(0 == ret){ // 超时
            printf("time out\n");
        }
    
    }

    return 0;
}

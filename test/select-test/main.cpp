//
//  main.cpp
//  select-test
//
//  Created by 王晓鹏 on 2020/10/14.
//

#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>


int main(int argc, const char * argv[]) {
    const char *fifo_name = "/tmp/my_fifo";
    
    int fd = open(fifo_name, O_RDWR); // 读写方式打开管道
    if(fd < 0){
        perror("open fifo");
        return -1;
    }
    
    while(1){
        std::string str = "this is from test client";
        write(fd, str.c_str(), str.length()); // 往管道里写内容
        printf("after write to fifo\n");
        sleep(2);
    }

    return 0;
}

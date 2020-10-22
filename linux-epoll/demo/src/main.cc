#include <iostream>
#include <thread>
#include <iomanip>
#include <logging.h>
#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, const char *argv[])
{
    //初始化参数
    FLAGS_logtostderr = false;              //TRUE:标准输出,FALSE:文件输出
    FLAGS_alsologtostderr = true;           //除了日志文件之外是否需要标准输出
    FLAGS_colorlogtostderr = true;          //标准输出带颜色
    FLAGS_logbufsecs = 0;                   //设置可以缓冲日志的最大秒数，0指实时输出
    FLAGS_max_log_size = 50;                //日志文件大小(单位：MB)
    FLAGS_stop_logging_if_full_disk = true; //磁盘满时是否记录到磁盘
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./logs/");

    const char *fifo_name = "/tmp/my_fifo";

    int fd = open(fifo_name, O_RDWR); // 读写方式打开管道
    if (fd < 0)
    {
        perror("open fifo");
        return -1;
    }

    while (1)
    {
        std::string str = "this is from test client";
        write(fd, str.c_str(), str.length()); // 往管道里写内容
        printf("after write to fifo\n");
        sleep(2);
    }

    LOG(INFO) << "end";
    google::ShutdownGoogleLogging();
}

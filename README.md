# ecron
用cpp11实现的高性能网络库，在性能一般的虚拟机下通过 JMeter 100K TCP连接测试。

## 功能
- 结构和接口使用muduo库的设计
- 利用boost::asio实现多线程的Reactor高并发模式
- 利用标准库容器封装char，实现自动增长的缓冲区
- 基于简单时间轮的定时器，最小精度1秒
- 使用标准库和通用库，可跨平台

## 环境要求
- Linux
- CPP11

## 构建
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
工程目录下的output目录会生成静态库 libecron 和其他示例程序

## 测试
- Ubuntu18.04 server
- JMeter
- webbench

## TODO
- config配置
- log改进
- 信号处理
- 更多示例

## 致谢
[muduo](https://github.com/chenshuo/muduo)

# RPC
以 ecron 为网络基础的 rpc 库，基于 protobuf rpc 框架。  

## 功能
* 用户编写proto文件，实现函数调用服务。  

## 环境要求
* Linux  
* CPP11  

## TODO
* 接口框架参考其他开源库比如brpc/sofa-rpc  
* 同步和异步调用  
* 并发问题(多个客户端的并发连接，同一个客户端的并发调用)  
* 内存管理，如读写缓冲区，request/response管理，message queue等  
* 调用超时控制  
* config配置  
* 基于zookeeper管理服务  
* 连接管理(心跳，自动重连，长/短连接控制)  
* 流量控制，透明压缩传输  

## 致谢
[muduo](https://github.com/chenshuo/muduo/tree/master/muduo/net/protorpc)
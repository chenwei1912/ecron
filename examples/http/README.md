# http
用netlib实现的高性能http服务器，经过webbenchh压力测试可以实现上万的QPS。  
resoures目录下缺失的三个大文件资源 xxx.jpg xxx.mp4 test1.jpg 可去下载 [链接](https://github.com/qinguoyi/TinyWebServer/tree/raw_version/root)

## 功能
* 利用 http_parser 开源库解析HTTP请求报文，实现处理静态资源的请求。
* 利用RAII机制实现了数据库连接池，减少数据库连接建立与关闭的开销，同时实现了用户注册登录功能。

## 环境要求
* Linux
* CPP11
* MySql

## 项目启动
需要先配置好对应的数据库
```bash
// 建立yourdb库
create database webserver;

// 创建user表
USE webserver;
CREATE TABLE user(
    username char(50) NULL,
    password char(50) NULL
)ENGINE=InnoDB;

// 添加数据
INSERT INTO user(username, password) VALUES('name', 'password');
```

## 压力测试
```bash
./webbench-1.5/webbench -c 100 -t 10 http://ip:port/
./webbench-1.5/webbench -c 1000 -t 10 http://ip:port/
./webbench-1.5/webbench -c 2400 -t 10 http://ip:port/
```
* 测试环境: Ubuntu:18.04 虚拟机
* QPS 10000+

## TODO
* config配置

## 致谢
[@qinguoyi](https://github.com/qinguoyi/TinyWebServer)  
[@markparticle](https://github.com/markparticle/WebServer)

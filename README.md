# WebIMGpp：基于Boost-Asio的图床应用/HTTP框架

**注意**：本项目仍处于开发中，仅实现基本应用框架及部分接口



## 简介

本项目是基于Boost-Asio实现的非阻塞式多线程HTTP/1.1框架及其图床应用

是个人学习网络编程的一个实践项目




## 特点

- Reactor设计模式：监听线程与工作线程分离，实现非阻塞的高并发连接
- API-Only：提供登录/注册/文件上传等API，不响应静态页面的请求，应当配合支持反向代理的服务器软件使用(例如Nginx)
- Java Servlet-like架构：通过继承Servlet对象并覆写doGet与doPost方法，提供简便的应用编程接口
- 消息队列与线程池：线程池与存储活动连接的消息队列结合，并发执行处理任务，保证线程安全
- 基于Boost-MySQL的数据库连接池与持久层：通过C++20信号量机制并发提供数据库连接，并对CURD操作提供简单的持久层实现



## 依赖

- ISO C++20
- spdlog
- Boost 1.82
- Boost-Asio
- Boost-MySQL

你可以使用[vcpkg](https://github.com/microsoft/vcpkg)安装以上依赖：

~~~shell
vcpkg install spdlog boost-asio boost-mysql
~~~

本项目包含若干单元测试，需要在`CMakeLists.txt`中启用`ENABLE_UNIT_TEST`，并且安装Catch2测试框架：

~~~shell
vcpkg install catch2
~~~



## 构建

####  已测试的平台：

- Windows: Windows 10 22H2 专业版，Visual Studio 2022 Community
- Linux: Ubuntu 22.04 LTS，GCC-11

#### 使用Git克隆本仓库：

~~~shell
git clone https://github.com/SignedWhiskeyXD/WebIMGpp.git
~~~

#### 使用CMake构建项目：

- 通过支持CMake的IDE打开本项目（例如VSCode CLion等），并通过`CMakeLists.txt`对项目进行配置
- 或者，你可以通过以下命令构建：
~~~shell
mkdir build
cd build
cmake ..
cmake --build .
~~~
#### 配置MySQL

应更改SQL相关源码中的参数选项，确保正确打开数据库连接，并执行`WebIMGpp.sql`中的SQL命令创建本项目的登录/注册模块所需要的数据表：

~~~mysql
CREATE TABLE IF NOT EXISTS WebIMGppUsers (
   id INT AUTO_INCREMENT PRIMARY KEY,
   username VARCHAR(50) NOT NULL UNIQUE,
   password VARCHAR(255) NOT NULL
);
~~~



## TODO

目前来说，本项目仍处于开发中，存在若干问题待解决：

- 尚不明确正确处理持续连接(keep-alive)的机制

- 应当超时释放连接
- 不支持HTTPS
- 应整合登录接口与文件上传接口，**实现可用的图床服务**
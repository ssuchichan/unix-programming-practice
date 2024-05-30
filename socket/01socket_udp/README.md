# 网络通信

网间进程通信解决的是不同主机进程间的相互通信问题（可把同机进程通信看做是其中的特例）。因为，要首先解决的是网间进程标识的问题。统一主机上，不同进程可用进程号（PID）唯一标识。

## 创建套接字

```
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

* `domain`：即协议域或协议族（family）。常用的协议有AF_INET、AF_INET6、AF_LOCAL（或称AF_UNIX，Unix域socket）、AF_ROUTE等。协议族决定了socket的地址类型，在通信中必须采用相应的地址。如AF_INET决定了要用ipv4地址（32位）与端口号（16位）的组合、AF_UNIX决定了要用一个绝对路径作为地址。对于TCP/IP协议族，该参数置AF_INET。
* `type`：套接字类型
    * `SOCK_STREAM`：提供了一个面向连接、可靠的数据传输服务，数据无差错、无重复地发送，且按发送顺序接收。内设流量控制，避免数据流超限；数据被看作是字节流，无长度限制。它使用TCP协议。
    * `SOCK_DGRAM`：提供了一个无连接服务。数据报以独立包形式发送，不提供无错保证，数据可能丢失或重复，并且接收顺序混乱。它使用UDP协议。
    * `SOCK_RAW`：接口允许对较低层协议，如IP、ICMP直接访问。常用于检验新的协议实现或访问现有服务中配置的新设备或者新协议的开发。
* `protocol`：指定应用程序使用的通信协议。此参数可以指定单个协议系列中的不同传输协议。在Internet通讯域中，此参数一般置为0，系统会根据套接字的类型决定应用的传输层协议。

## 绑定

```
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
```

* `sockfd`：套接字描述符
* `addr`：指向`sockaddr`结构的指针，标识绑定的本地地址信息，如果是IP信息，则要求IP地址必须为本机IP地址，端口必须为一个未占用的本地端口。
* `addrlen`：地址的长度

## 接收数据

```
#include <sys/socket.h>

int recvfrom(
    int              sockfd, 
    void*            buf, 
    int              len, 
    unsigned int     flags, 
    struct sockaddr* from, 
    int*             fromlen
);
```

* `sockfd`：套接字描述符
* `buf`：接收数据缓冲区
* `len`：缓冲区长度
* `flags`：调用操作方式，一般为0
* `from`：对方主机的网络地址
* `fromlen`：对方主机地址的长度

## 发送数据

```
#include <sys/socket.h>

ssize_t sendto(
    int              sockfd, 
    void             *buf, 
    size_t           len, 
    int              flags, 
    struct sockaddr* dest_addr, 
    socklen_t        addrlen
);
```

* `sockfd`：套接字描述符
* `buf`：发送数据缓冲区
* `len`：缓冲区长度
* `flags`：调用操作方式，一般为0
* `to`：欲传送的地址
* `tolen`：欲传送地址长度



## sockaddr

```
struct sockaddr {
    sa_family_t  sa_family;      /* Address family */
    char         sa_data[];      /* Socket address */
};
```

`struct sockaddr`只提供地址类型规范，根据不同的应用，`sockaddr`需要选择不同的类型。但是每一种地址都是有`sockaddr_`前缀，如IPV4选择的是`sockaddr_in`，如

```
typedef uint32_t in_addr_t;
typedef uint16_t in_port_t;

struct in_addr {
	in_addr_t s_addr;
};

struct sockaddr_in {
    sa_family_t     sin_family;     // 协议族
    in_port_t       sin_port;       // 端口
    struct in_addr  sin_addr;       // IP地址
    unsigned char   sin_zero[8];    // 保留位
};
```


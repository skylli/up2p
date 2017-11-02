# up2p README
## 简介
up2p 是一套非标准，针对多app端控制单设备模型设计的协议。该工程实现了服务端，手机app端、嵌入式端会话层。
**该工程主要用于提供 *up2p* 库以及，以及调试用**
## 目录
>**app** 端需要调用 *ulink* 库，对应的动态静态库在 ./lib/libulink.xx.xx.so  ./lib/libulink.xx.xx.a 同时还必须包含 *./include* 里的所有头文件，才能调用该库。
>嵌入式端需要根据对应的平台参考 *./src/up2pc/up2p_posix.c* 实现对应的接口既可使用该库。
>服务端仅仅用于测试.
```bash
.
├── bin         //编译生成的可执行文件
├── build       //cmake 编译生成的中间件
├── build.sh    //一键编译脚本
├── CMakeLists.txt  // 根cmake
├── build.android	// android 平台编译文件
├── build.ios		// ios 以及 iphone 平台编译文件
├── config          //工程配置文件
├── doc             //说明文档
├── include         //工程头文件，
├── lib             //up2p 各个端的库文件
├── src             //源码文件
├── vm_up2pa.c      //app 端 sample
├── vm_up2pc.c      //嵌入式端 sample
└── vm_up2ps.c      //服务端 sample
```
```bash
├── lib
│   ├── libulink.0.1.1.so   
│   ├── libulink_a.0.1.1.a
│   ├── libup2pc.0.1.1.a
│   └── libup2ps.0.1.1.a

```

## 使用
mac 和 linux 下的使用
```shell

cd up2p/build
make 
cd ../bin/
ls
./vmup2pc  9527  127.0.0.1  9529 0000000C4326605A 0000000000000000
# 在另外一个终端 启动 模拟 app 端
cd up2p/bin
./vmup2pa   127.0.0.1  0000000C4326605A
# 出现以下字符 输入要传输的字符串
input string : abcd123

```

### 编译
生成对应的库以及可执行文件
```bash
$./build.sh
```
#### 启动模拟的设备端
```bash
$cd bin
#./vmup2pc 设备端监听的端口，服务端host或ip   服务端port  设备端16字符的设备id  设备端16字符的key
$./vmup2pc  9527  127.0.0.1  9529 0000000C4326605A 0000000000000000
```
#### 启动模拟的 app 端
```bash
$cd bin
#./vmup2pa 服务器 host 或 ip   要连接的设备 id
$./vmup2pa   127.0.0.1  0000000C4326605A  

```
#### 启动服务器
```bash
$cd bin
# ./vmup2ps  服务器监听的端口
$./vmup2ps 9529
```
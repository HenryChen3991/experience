Unix domain socket 又叫 IPC(inter-process communication 进程间通信) socket，
用于实现同一主机上的进程间通信。
socket 原本是为网络通讯设计的，
但后来在 socket 的框架上发展出一种 IPC 机制，就是 UNIX domain socket。
虽然网络 socket 也可用于同一台主机的进程间通讯(通过 loopback 地址 127.0.0.1)，
但是 UNIX domain socket 用于 IPC 更有效率：
	不需要经过网络协议栈，不需要打包拆包、计算校验和、维护序号和应答等，
	只是将应用层数据从一个进程拷贝到另一个进程。
	这是因为，IPC 机制本质上是可靠的通讯，而网络协议是为不可靠的通讯设计的。
UNIX domain socket 是全双工的，API 接口语义丰富，相比其它 IPC 机制有明显的优越性，
目前已成为使用最广泛的 IPC 机制，比如 X Window 服务器和 GUI 程序之间就是通过 UNIX domain socket 通讯的。
Unix domain socket 是 POSIX 标准中的一个组件，所以不要被名字迷惑，linux 系统也是支持它的。


总结
Unix domain socket 主要用于同一主机上的进程间通信。
与主机间的进程通信不同，它不是通过 "IP地址 + TCP或UDP端口号" 的方式进程通信，
而是使用 socket 类型的文件来完成通信，
因此在稳定性、可靠性以及效率方面的表现都很不错。

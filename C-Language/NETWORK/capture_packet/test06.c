
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
/**
ioctl函数是与内核交互的一种方法，使用ioctl函数与内核协议栈进行交互
ioctl函数可操作I/O请求，文件请求与网络接口请求
网络接口请求的几个结构体:
struct ifreq{
#define IFHWADDRLEN 6 //6个字节的硬件地址，即MAC
union{
 char ifrn_name[IFNAMESIZ];//网络接口名称
}ifr_ifrn;
union{
 struct sockaddr ifru_addr;//本地IP地址
 struct sockaddr ifru_dstaddr;//目标IP地址
 struct sockaddr ifru_broadaddr;//广播IP地址
 struct sockaddr ifru_netmask;//本地子网掩码地址
 struct sockaddr ifru_hwaddr;//本地MAC地址
 short ifru_flags;//网络接口标记
 int ifru_ivalue;//不同的请求含义不同
 struct ifmap ifru_map;//网卡地址映射
 int ifru_mtu;//最大传输单元
 char ifru_slave[IFNAMSIZ];//占位符
 char ifru_newname[IFNAMSIZE];//新名称
 void __user* ifru_data;//用户数据
 struct if_settings ifru_settings;//设备协议设置
}ifr_ifru;
}
#define ifr_name ifr_ifrn.ifrn_name;//接口名称
#define ifr_hwaddr ifr_ifru.ifru_hwaddr;//MAC
#define ifr_addr ifr_ifru.ifru_addr;//本地IP
#define ifr_dstaddr ifr_ifru.dstaddr;//目标IP
#define ifr_broadaddr ifr_ifru.broadaddr;//广播IP
#define ifr_netmask ifr_ifru.ifru_netmask;//子网掩码
#define ifr_flags ifr_ifru.ifru_flags;//标志
#define ifr_metric  ifr_ifru.ifru_ivalue;//接口侧度
#define ifr_mtu ifr_ifru.ifru_mtu;//最大传输单元
#define ifr_map ifr_ifru.ifru_map;//设备地址映射
#define ifr_slave ifr_ifru.ifru_slave;//副设备
#define ifr_data ifr_ifru.ifru_data;//接口使用
#define ifr_ifrindex ifr_ifru.ifru_ivalue;//网络接口序号
#define ifr_bandwidth ifr_ifru.ifru_ivalue;//连接带宽
#define ifr_qlen ifr_ifru.ifru_ivalue;//传输单元长度
#define ifr_newname ifr_ifru.ifru_newname;//新名称
#define ifr_seeting ifr_ifru.ifru_settings;//设备协议设置
struct ifmap{//网卡设备的映射属性
 unsigned long mem_start;//开始地址
 unsigned long mem_end;//结束地址
 unsigned short base_addr;//基地址
 unsigned char irq;//中断号
 unsigned char dma;//DMA
 unsigned char port;//端口
}
struct ifconf{//网络配置结构体是一种缓冲区
 int ifc_len;//缓冲区ifr_buf的大小
 union{
  char__user *ifcu_buf;//绘冲区指针
 struct ifreq__user* ifcu_req;//指向ifreq指针
}ifc_ifcu;
};
#define ifc_buf ifc_ifcu.ifcu_buf;//缓冲区地址
#define ifc_req ifc_ifcu.ifcu_req;//ifc_req地址
(1)获得配置选项SIOCGIFCONF获得网络接口的配置情况 需要填充struct ifreq中ifr_name变量
(2)其它选项获取填充struct ifreq的ifr_name
**/
 
int main(int argc,char*argv[]){
	int s;
	int err;
	s=socket(AF_INET,SOCK_DGRAM,0);
	if(s<0){
	perror("socket error");
	return;
	}
 
	//传入网络接口序号，获得网络接口的名称
	struct ifreq ifr;
 
	ifr.ifr_ifindex=2;//获得第2个网络接口的名称
	err=ioctl(s,SIOCGIFNAME,&ifr);
 
	if(err)
		perror("index error");
	else
		printf("the %dst interface is:%s\n",ifr.ifr_ifindex,ifr.ifr_name);
 
	//传入网络接口名称，获得标志
	memcpy(ifr.ifr_name,"enp0s3",5);
	err=ioctl(s,SIOCGIFFLAGS,&ifr);
	if(!err)
		printf("SIOCGIFFLAGS:%d\n",ifr.ifr_flags);
 
	//获得MTU和MAC
	err=ioctl(s,SIOCGIFMTU,&ifr);
	if(!err)
		printf("SIOCGIFMTU:%d\n",ifr.ifr_mtu);
 
	//获得MAC地址
	err=ioctl(s,SIOCGIFHWADDR,&ifr);
	if(!err){
		unsigned char* hw=ifr.ifr_hwaddr.sa_data;
		printf("SIOCGIFHWADDR:%02x:%02x:%02x:%02x:%02x:%02x\n",hw[0],hw[1],hw[2],hw[3],hw[4],hw[5]);
	}
 
	//获得网卡映射参数 命令字SIOCGIFMAP
	err=ioctl(s,SIOCGIFMAP,&ifr);
	if(!err)
		printf("SIOCGIFMAP,mem_start:%d,mem_end:%d,base_addr:%d,ifr_map:%d,dma:%d,port:%d\n",ifr.ifr_map.mem_start,ifr.ifr_map.mem_end,ifr.ifr_map.base_addr,ifr.ifr_map.irq,ifr.ifr_map.dma,ifr.ifr_map.port);
 
 
	//获得网卡序号
	err=ioctl(s,SIOCGIFINDEX,&ifr);
	if(!err)
		printf("SIOCGIFINDEX:%d\n",ifr.ifr_ifindex);
 
	//获取发送队列的长度
	err=ioctl(s,SIOCGIFTXQLEN,&ifr);
	if(!err)
		printf("SIOCGIFTXQLEN:%d\n",ifr.ifr_qlen);
 
	//获取网络接口IP
 
	struct sockaddr_in *sin=(struct sockaddr_in*)&ifr.ifr_addr;//保存的是二进制IP
	char ip[16];//字符数组，存放字符串
	memset(ip,0,16);
	err=ioctl(s,SIOCGIFADDR,&ifr);
	if(!err){
		inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);//转换的字符串保存到ip数组中，第二个参数是要转换的二进制IP指针，第三个参数是转换完成存放IP的缓冲区，最后一个参数是缓冲区的长度
		printf("SIOCGIFADDR:%s\n",ip);
	}
 
	//查询目标IP地址
	err=ioctl(s,SIOCGIFDSTADDR,&ifr);
	if(!err){
		inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);
		printf("SIOCGIFDSTADDR:%s\n",ip);
	}
 
	//查询子网掩码
	err=ioctl(s,SIOCGIFNETMASK,&ifr);
	if(!err){
		inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);
		printf("SIOCGIFNETMASK:%s\n",ip);
	}
 
	//设置IP地址，设置网络接口
	inet_pton(AF_INET,"222.27.253.108",&sin->sin_addr.s_addr);//将字符串IP转换成二进制
	err=ioctl(s,SIOCSIFADDR,&ifr);//发送设置本机ip地址请求命令
	if(!err){
		printf("check IP-----");  
		memset(&ifr,0,sizeof(ifr));
		memcpy(ifr.ifr_name,"eth0",5);
		ioctl(s,SIOCGIFADDR,&ifr);
		inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);
		printf("%s\n",ip);
	}
 
	//得到接口的广播地址
	memset(&ifr,0,sizeof(ifr));
	memcpy(ifr.ifr_name,"eth0",5);
	ioctl(s,SIOCGIFBRDADDR,&ifr);
	struct sockaddr_in *broadcast=(struct sockaddr_in*)&ifr.ifr_broadaddr;
	//转换成字符串
	inet_ntop(AF_INET,&broadcast->sin_addr.s_addr,ip,16);//inet_ntop将二进制IP转换成点分十进制的字符串
	printf("BROADCAST IP:%s\n",ip);
	close(s);
}

 
epoll的使用方法
那么究竟如何来使用epoll呢？其实非常简单。

 

通过在包含一个头文件#include <sys/epoll.h> 以及几个简单的API将可以大大的提高你的网络服务器的支持人数。

 

首先通过create_epoll(int maxfds)来创建一个epoll的句柄。这个函数会返回一个新的epoll句柄，之后的所有操作将通过这个句柄来进行操作。在用完之后，记得用close()来关闭这个创建出来的epoll句柄。

 

之后在你的网络主循环里面，每一帧的调用epoll_wait(int epfd, epoll_event events, int max events, int timeout)来查询所有的网络接口，看哪一个可以读，哪一个可以写了。基本的语法为：

nfds = epoll_wait(kdpfd, events, maxevents, -1);

 

其中kdpfd为用epoll_create创建之后的句柄，events是一个epoll_event*的指针，当epoll_wait这个函数操作成功之后，epoll_events里面将储存所有的读写事件。max_events是当前需要监听的所有socket句柄数。最后一个timeout是 epoll_wait的超时，为0的时候表示马上返回，为-1的时候表示一直等下去，直到有事件返回，为任意正整数的时候表示等这么长的时间，如果一直没有事件，则返回。一般如果网络主循环是单独的线程的话，可以用-1来等，这样可以保证一些效率，如果是和主逻辑在同一个线程的话，则可以用0来保证主循环的效率。

 

epoll_wait返回之后应该是一个循环，遍历所有的事件。





几乎所有的epoll程序都使用下面的框架：


for( ; ; )  
   {  
       nfds = epoll_wait(epfd,events,20,500);  
       for(i=0;i<nfds;++i)  
       {  
           if(events[i].data.fd==listenfd) //有新的连接  
           {  
               connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen); //accept这个连接  
               ev.data.fd=connfd;  
               ev.events=EPOLLIN|EPOLLET;  
               epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //将新的fd添加到epoll的监听队列中  
           }  
  
           else if( events[i].events&EPOLLIN ) //接收到数据，读socket  
           {  
               n = read(sockfd, line, MAXLINE)) < 0    //读  
               ev.data.ptr = md;     //md为自定义类型，添加数据  
               ev.events=EPOLLOUT|EPOLLET;  
               epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓  
           }  
           else if(events[i].events&EPOLLOUT) //有数据待发送，写socket  
           {  
               struct myepoll_data* md = (myepoll_data*)events[i].data.ptr;    //取数据  
               sockfd = md->fd;  
               send( sockfd, md->ptr, strlen((char*)md->ptr), 0 );        //发送数据  
               ev.data.fd=sockfd;  
               ev.events=EPOLLIN|EPOLLET;  
               epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据  
           }  
           else  
           {  
               //其他的处理  
           }  
       }  
   }  








epoll的程序实例1

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <netdb.h>  
#include <fcntl.h>  
#include <sys/epoll.h>  
#include <string.h>  
  
#define MAXEVENTS 64  
  
//函数:  
//功能:创建和绑定一个TCP socket  
//参数:端口  
//返回值:创建的socket  
static int  
create_and_bind (char *port)  
{  
  struct addrinfo hints;  
  struct addrinfo *result, *rp;  
  int s, sfd;  
  
  memset (&hints, 0, sizeof (struct addrinfo));  
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */  
  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */  
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */  
  
  s = getaddrinfo (NULL, port, &hints, &result);  
  if (s != 0)  
    {  
      fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));  
      return -1;  
    }  
  
  for (rp = result; rp != NULL; rp = rp->ai_next)  
    {  
      sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);  
      if (sfd == -1)  
        continue;  
  
      s = bind (sfd, rp->ai_addr, rp->ai_addrlen);  
      if (s == 0)  
        {  
          /* We managed to bind successfully! */  
          break;  
        }  
  
      close (sfd);  
    }  
  
  if (rp == NULL)  
    {  
      fprintf (stderr, "Could not bind\n");  
      return -1;  
    }  
  
  freeaddrinfo (result);  
  
  return sfd;  
}  
  
  
//函数  
//功能:设置socket为非阻塞的  
static int  
make_socket_non_blocking (int sfd)  
{  
  int flags, s;  
  
  //得到文件状态标志  
  flags = fcntl (sfd, F_GETFL, 0);  
  if (flags == -1)  
    {  
      perror ("fcntl");  
      return -1;  
    }  
  
  //设置文件状态标志  
  flags |= O_NONBLOCK;  
  s = fcntl (sfd, F_SETFL, flags);  
  if (s == -1)  
    {  
      perror ("fcntl");  
      return -1;  
    }  
  
  return 0;  
}  
  
//端口由参数argv[1]指定  
int  
main (int argc, char *argv[])  
{  
  int sfd, s;  
  int efd;  
  struct epoll_event event;  
  struct epoll_event *events;  
  
  if (argc != 2)  
    {  
      fprintf (stderr, "Usage: %s [port]\n", argv[0]);  
      exit (EXIT_FAILURE);  
    }  
  
  sfd = create_and_bind (argv[1]);  
  if (sfd == -1)  
    abort ();  
  
  s = make_socket_non_blocking (sfd);  
  if (s == -1)  
    abort ();  
  
  s = listen (sfd, SOMAXCONN);  
  if (s == -1)  
    {  
      perror ("listen");  
      abort ();  
    }  
  
  //除了参数size被忽略外,此函数和epoll_create完全相同  
  efd = epoll_create1 (0);  
  if (efd == -1)  
    {  
      perror ("epoll_create");  
      abort ();  
    }  
  
  event.data.fd = sfd;  
  event.events = EPOLLIN | EPOLLET;//读入,边缘触发方式  
  s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);  
  if (s == -1)  
    {  
      perror ("epoll_ctl");  
      abort ();  
    }  
  
  /* Buffer where events are returned */  
  events = calloc (MAXEVENTS, sizeof event);  
  
  /* The event loop */  
  while (1)  
    {  
      int n, i;  
  
      n = epoll_wait (efd, events, MAXEVENTS, -1);  
      for (i = 0; i < n; i++)  
        {  
          if ((events[i].events & EPOLLERR) ||  
              (events[i].events & EPOLLHUP) ||  
              (!(events[i].events & EPOLLIN)))  
            {  
              /* An error has occured on this fd, or the socket is not 
                 ready for reading (why were we notified then?) */  
              fprintf (stderr, "epoll error\n");  
              close (events[i].data.fd);  
              continue;  
            }  
  
          else if (sfd == events[i].data.fd)  
            {  
              /* We have a notification on the listening socket, which 
                 means one or more incoming connections. */  
              while (1)  
                {  
                  struct sockaddr in_addr;  
                  socklen_t in_len;  
                  int infd;  
                  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];  
  
                  in_len = sizeof in_addr;  
                  infd = accept (sfd, &in_addr, &in_len);  
                  if (infd == -1)  
                    {  
                      if ((errno == EAGAIN) ||  
                          (errno == EWOULDBLOCK))  
                        {  
                          /* We have processed all incoming 
                             connections. */  
                          break;  
                        }  
                      else  
                        {  
                          perror ("accept");  
                          break;  
                        }  
                    }  
  
                                  //将地址转化为主机名或者服务名  
                  s = getnameinfo (&in_addr, in_len,  
                                   hbuf, sizeof hbuf,  
                                   sbuf, sizeof sbuf,  
                                   NI_NUMERICHOST | NI_NUMERICSERV);//flag参数:以数字名返回  
                                  //主机地址和服务地址  
  
                  if (s == 0)  
                    {  
                      printf("Accepted connection on descriptor %d "  
                             "(host=%s, port=%s)\n", infd, hbuf, sbuf);  
                    }  
  
                  /* Make the incoming socket non-blocking and add it to the 
                     list of fds to monitor. */  
                  s = make_socket_non_blocking (infd);  
                  if (s == -1)  
                    abort ();  
  
                  event.data.fd = infd;  
                  event.events = EPOLLIN | EPOLLET;  
                  s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);  
                  if (s == -1)  
                    {  
                      perror ("epoll_ctl");  
                      abort ();  
                    }  
                }  
              continue;  
            }  
          else  
            {  
              /* We have data on the fd waiting to be read. Read and 
                 display it. We must read whatever data is available 
                 completely, as we are running in edge-triggered mode 
                 and won't get a notification again for the same 
                 data. */  
              int done = 0;  
  
              while (1)  
                {  
                  ssize_t count;  
                  char buf[512];  
  
                  count = read (events[i].data.fd, buf, sizeof(buf));  
                  if (count == -1)  
                    {  
                      /* If errno == EAGAIN, that means we have read all 
                         data. So go back to the main loop. */  
                      if (errno != EAGAIN)  
                        {  
                          perror ("read");  
                          done = 1;  
                        }  
                      break;  
                    }  
                  else if (count == 0)  
                    {  
                      /* End of file. The remote has closed the 
                         connection. */  
                      done = 1;  
                      break;  
                    }  
  
                  /* Write the buffer to standard output */  
                  s = write (1, buf, count);  
                  if (s == -1)  
                    {  
                      perror ("write");  
                      abort ();  
                    }  
                }  
  
              if (done)  
                {  
                  printf ("Closed connection on descriptor %d\n",  
                          events[i].data.fd);  
  
                  /* Closing the descriptor will make epoll remove it 
                     from the set of descriptors which are monitored. */  
                  close (events[i].data.fd);  
                }  
            }  
        }  
    }  
  
  free (events);  
  
  close (sfd);  
  
  return EXIT_SUCCESS;  
}  


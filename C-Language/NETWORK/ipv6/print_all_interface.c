#include <stdlib.h>     //exit()
#include <stdio.h>      //printf(),perror()
#include <string.h>     //memset
#include <ifaddrs.h>    //struct ifaddrs,getifaddrs()
#include <arpa/inet.h>  //INET6_ADDRSTRLEN,inet_ntop()
#include <netinet/in.h> //sockaddr_in,sockaddr_in6

void die(char * s)
{
    perror(s);
    exit(1);
}

void print_all_ip()
{
    struct ifaddrs *ifaddr, *ifa;
    char ip_str[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) < 0) {
        die("getifaddrs");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) { continue; }

        switch (ifa->ifa_addr->sa_family) {
            case AF_INET: {
                struct sockaddr_in *a = (struct sockaddr_in *)ifa->ifa_addr;
                inet_ntop(AF_INET, &(a->sin_addr), ip_str, INET_ADDRSTRLEN);

                uint32_t n = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr;
                int i = 0;
                while (n > 0) {
                     if (n & 1) i++;
                     n = n >> 1;
                }
                printf("%s: %s/%d\n", ifa->ifa_name, ip_str, i);
                break;
            }
            case AF_INET6: {
                struct sockaddr_in6 *a = (struct sockaddr_in6 *)ifa->ifa_addr;
                inet_ntop(AF_INET6, &(a->sin6_addr), ip_str, INET6_ADDRSTRLEN);

                unsigned char *c = ((struct sockaddr_in6 *)ifa->ifa_netmask)->sin6_addr.s6_addr;
                int i = 0, j = 0;
                unsigned char n = 0;
                while (i < 16) {
                    n = c[i];
                    while (n > 0) {
                        if (n & 1) j++;
                        n = n/2;
                    }
                    i++;
                }
                printf("%s: %s/%d\n", ifa->ifa_name, ip_str, j);
                break;
            }
            default:
                break;
        }
    }
    freeifaddrs(ifaddr);
    return;
}

int main()
{
    print_all_ip();
    exit(0);
}

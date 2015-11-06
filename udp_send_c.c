#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>



int main()
{
    
const char* hostname="10.100.10.108"; /* localhost */
const char* portname="20050";
struct addrinfo hints;
memset(&hints,0,sizeof(hints));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_DGRAM;
hints.ai_protocol=0;
hints.ai_flags=AI_ADDRCONFIG;
struct addrinfo* res=0;
int err=getaddrinfo(hostname,portname,&hints,&res);


char content[] = { 0x10, 0x02, 0x01, 0x01, 0x55, 0x42, 0x2E, 0x35, 0x75, 0x43, 0x05, 0x29, 0x80, 0x15, 0x6F, 0x82, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x01, 0x01, 0x49, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x82, 0xE7 };

if (err!=0) {
    printf("failed to resolve remote socket address (err=%d)",err);
}
int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
if (fd==-1) {
    printf("%s",strerror(errno));
}

if (sendto(fd,content,sizeof(content),0,
    res->ai_addr,res->ai_addrlen)==-1) {
    printf("%s",strerror(errno));
}

}
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

#define PORT 20050
#define BUFLEN 512
#define NPACK 10


void diep(char *s)
{
    perror(s);
//    exit(1);
}

int main(int argc, char *argv[])
{
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);
    const char* hostname=argv[1];//"10.5.189.81"; /* localhost */
    const char* portname=argv[2];//"30000";
    printf("%s\n", hostname);
    printf("%s\n", portname);
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_ADDRCONFIG;
    struct addrinfo* res=0;
    int err=getaddrinfo(hostname,portname,&hints,&res);
    char buf[BUFLEN];

    struct sockaddr_in si_other;
    int slen=sizeof(si_other);
    int i = 0;

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

    for (i=0; i<NPACK; i++) {
        if (recvfrom(fd, buf, BUFLEN, 0, &si_other, &slen)==-1) 
        {
            diep("recvfrom()");
	}
        printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
    }

//for (i = 0; i < sizeof(buf); i++)
//{
//	printf("%04x", buf[i]);	
//}

    close(fd);
    return 0;
}

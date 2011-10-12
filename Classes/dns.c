
#import "dns.h"
#import <sys/socket.h>
#import <sys/types.h>
#import <arpa/inet.h>
#import <netinet/in.h>
#import <netdb.h>

u_int resolv(char *host, int *error) {
    struct hostent *hp;
    u_int  host_ip;
	
    host_ip = inet_addr(host);
    if(host_ip == INADDR_NONE) {
        hp = gethostbyname(host);
        if(!hp) {
            printf("\nError: Unable to resolv hostname (%s)\n", host);
            *error = 1;
			return(host_ip);
        } else host_ip = *(u_int *)hp->h_addr;
    }
    return(host_ip);
}
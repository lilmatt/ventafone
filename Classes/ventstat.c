/*
    Copyright 2005,2006 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl.txt
*/

#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import "ventrilo_udp.h"
#import "dns.h"
#import <unistd.h>
#import <sys/socket.h>
#import <sys/types.h>
#import <arpa/inet.h>
#import <netinet/in.h>
#import <netdb.h>
#define strnicmp    strncasecmp
#define VER         "0.1"
#define PORT        3784
#define MAXPCK      32
#define MAXPCKSZ    492
#define TIMEOUT     2
#define RETRY       1

int ventrilo_get_status(int sd, u_short cmd, u_char *pass, char *output[]);
int timeout(int sock);

struct  sockaddr_in peer;

int getStatus(char *host, char *output[]) {
    int     sd,
            i,
            cmd = 2,
            commands[] = {
                1,  // generic info
                2,  // details
                7,  // info visualized differently
                -1  // no other commands
            };
    u_short port = PORT;
    u_char  *pass = "",
            *p,
            *l;
	
    setbuf(stdout, NULL);
	//setvbuf(fp, output, _IOLBF, 1000);
	
//    p = strstr(host, "://");
//    if(p) host = p + 3;
//
//    p = strchr(host, ':');
//    if(p) {
//        *p++ = 0;
//        port = atoi(p);
//    } else {
//        p = host;
//    }

//    if((l = strchr(p, ':'))) {
//        pass = l + 1;
//
//    } else if((l = strchr(p, '/'))) {
//        *l = 0;
//        do {
//            p = l + 1;
//            l = strchr(p, '=');
//            if(!l) break;
//            *l = 0;
//            if(!strnicmp(p, "serverpassword", l - p)) pass = l + 1;
//            p = l + 1;
//        } while((l = strchr(p, '&')));
//    }

	int error = 0;
    peer.sin_addr.s_addr = resolv(host, &error);
	if (error != 0) {
		printf("Ran into an error. Couldn't resolve hostname.\n");
		return(2);
	}
    peer.sin_port        = htons(port);
    peer.sin_family      = AF_INET;

    printf("- Retrieving from... %s:%hu\n",
        inet_ntoa(peer.sin_addr), port);

    if(*pass) printf("- Password %s\n", pass);

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sd < 0)
	{
		close(sd);
		return(1);
	}

	if (ventrilo_get_status(sd, cmd, pass, output) > 0)
	{
		close(sd);
		return(1);
	}

    close(sd);
    return(0);
}



int ventrilo_get_status(int sd, u_short cmd, u_char *pass, char *output[]) {
    ventrilo_udp_head   *stat;
    int     i,
            len,
            totlen,
            retry;
    u_short id,
            crc;
    u_char  buff[20 + MAXPCKSZ],
            //full[MAXPCKSZ * MAXPCK],
            *data;
	char full[MAXPCKSZ * MAXPCK];

    stat = (ventrilo_udp_head *)buff;
    data = buff + 20;

    //printf("\n- Command code %d:\n\n", cmd);

    strncpy(data, pass, 16);

    stat->zero    = 0;
    stat->cmd     = cmd;
    stat->id      = id = time(NULL);
    stat->totlen  = 16;
    stat->len     = 16;
    stat->totpck  = 1;
    stat->pck     = 0;
    stat->crc     = ventrilo_udp_crc(data, 16);
    stat->datakey = ventrilo_udp_data_enc(data, 16);
    ventrilo_udp_head_enc(buff);

    for(retry = RETRY; retry; retry--) {
        sendto(sd, buff, 20 + 16, 0, (struct sockaddr *)&peer, sizeof(peer));
        if(timeout(sd) == 1)
		{
			// Error
			return(1);
		}
		else if (!timeout(sd))
		{
			break;
		}
    }
    if(!retry) {
        fputs("\nError: no reply received, probably the server is not online\n\n", stdout);
        return 1;
    }

    i      = 0;
    totlen = 0;
    memset(full, ' ', sizeof(full));    // in case of packet loss

    for(;;) {
        len = recvfrom(sd, buff, sizeof(buff), 0, NULL, NULL);
        ventrilo_udp_head_dec(buff);

        if(stat->id != id) continue;

        if((len < 20)                 ||
           (stat->totpck < stat->pck) ||
           (stat->totpck > MAXPCK)    ||
           (stat->len    > MAXPCKSZ)) {
            fputs("\nError: wrong or incomplete reply received\n", stdout);
            return;
        }

        len    = stat->len;
        totlen += len;
        if(totlen > sizeof(full)) break;

        ventrilo_udp_data_dec(data, len, stat->datakey);
        memcpy(full + (stat->pck * MAXPCKSZ), data, len);

        if(++i == stat->totpck) break;
        if(totlen == stat->totlen) break;
        if(timeout(sd) < 0) break;
    }

    crc = ventrilo_udp_crc(full, totlen);
    if(ventrilo_udp_crc(full, totlen) != stat->crc) {
        printf("- wrong checksum: mine is %04x while should be %04x\n\n", crc, stat->crc);
    }

    //fwrite(full, totlen, 1, stdout);
	//fwrite(full, totlen, 1, output);
	sprintf(output, "%s",full);
	
	return 0;
}



int timeout(int sock) {
    struct  timeval tout;
    fd_set  fd_read;
    int     err;

    tout.tv_sec = TIMEOUT;
    tout.tv_usec = 0;
    FD_ZERO(&fd_read);
    FD_SET(sock, &fd_read);
    err = select(sock + 1, &fd_read, NULL, NULL, &tout);
    if(err < 0) return(1);	// error
    if(!err) return(-1);	// try again
    return(0);				// success
}
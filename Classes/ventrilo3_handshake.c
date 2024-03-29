/*
    Copyright 2008,2009 Luigi Auriemma

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

    http://www.gnu.org/licenses/gpl-2.0.txt
*/

#import "ventrilo_algo.h"

#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <stdint.h>
#import <time.h>
#import <unistd.h>
#import <sys/socket.h>
#import <sys/types.h>
#import <arpa/inet.h>
#import <netinet/in.h>
#import <netdb.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;



#define V3HVER      "0.3"
#define V3HBUFFSZ   0x200

#ifdef V3HPROXY
    #define V3HPROXY_PARS   , "", "", 0
#else
    #define V3HPROXY_PARS
#endif

typedef struct {
    int     vnum;
    u8      *host;
    u16     port;
#ifdef V3HPROXY
    u8      handshake_key[64];
    u8      handshake[16];
    int     ok;
#endif
} ventrilo3_auth_t;

static ventrilo3_auth_t ventrilo3_auth[] = {
    { 1,  "72.51.46.31",   6100 V3HPROXY_PARS },
    { 2,  "64.34.178.178", 6100 V3HPROXY_PARS },
    { 3,  "74.54.61.194",  6100 V3HPROXY_PARS },
    { 4,  "70.85.110.242", 6100 V3HPROXY_PARS },
    { -1, NULL,            0    V3HPROXY_PARS }
};



int ventrilo3_hdr_udp(int type, u8 *buff, u8 *pck);
int ventrilo3_send_udp(int sd, int vnum, u32 ip, u16 port, u8 *data, int len);
int ventrilo3_recv_udp(int sd, ventrilo3_auth_t *vauth, u8 *data, int maxsz, int *handshake_num);
int getbe(u8 *data, u32 *ret, int bits);
int putbe(u8 *data, u32 num, int bits);
int v3timeout(int sock, int secs);



void ventrilo3_algo_scramble(ventrilo_key_ctx *ctx, u8 *v3key) {
    int     i,
            keylen;
    u8      *key;

    key = ctx->key;
    if(ctx->size < 64) {
        memset(key + ctx->size, 0, 64 - ctx->size);
        ctx->size = 64;
    }
    keylen = ctx->size;
    for(i = 0; i < keylen; i++) {
        if(i < 64) {
            key[i] += v3key[i];
        } else {
            key[i] += i + keylen;
        }
        if(!key[i]) key[i] = i + 36;
    }
    ctx->pos = 0;
}



int ventrilo3_handshake(uint32_t ip, uint16_t port, uint8_t *handshake, int *handshake_num, uint8_t *handshake_key) {
    struct  linger  ling = {1,1};
    int     sd;
    int     i,
            len;
    u8      sbuff[V3HBUFFSZ],
            rbuff[V3HBUFFSZ];

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sd < 0) return(-1);
    setsockopt(sd, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));

    memset(rbuff, 0, 200);
    ventrilo3_hdr_udp(4, sbuff, rbuff);
    putbe(sbuff + 0xc,  2,         8);
    putbe(sbuff + 0x10, 1,         16); // useless
    putbe(sbuff + 0x12, time(NULL),16); // rand useless number

    ventrilo3_send_udp(sd, -1, ip, port, sbuff, 200);
    len = ventrilo3_recv_udp(sd, NULL, rbuff, V3HBUFFSZ, handshake_num);
    if(len < 0) goto quit;

    for(i = 0; ventrilo3_auth[i].host; i++) {
        len = ventrilo3_hdr_udp(5, sbuff, rbuff);
        ventrilo3_send_udp(sd, ventrilo3_auth[i].vnum, inet_addr(ventrilo3_auth[i].host), ventrilo3_auth[i].port, sbuff, len);
    }

    for(;;) {
        len = ventrilo3_recv_udp(sd, (void *)&ventrilo3_auth, rbuff, V3HBUFFSZ, handshake_num);
        if(len < 0) break;
        if(!len) continue;
        if(len < (0x5c + 16)) continue;
#ifdef V3HPROXY
        memcpy(ventrilo3_auth[*handshake_num].handshake_key, rbuff + 0x1c, 64);
        memcpy(ventrilo3_auth[*handshake_num].handshake,     rbuff + 0x5c, 16);
        ventrilo3_auth[*handshake_num].ok = 1;
#else
        memcpy(handshake_key, rbuff + 0x1c, 64);
        memcpy(handshake,     rbuff + 0x5c, 16);
        close(sd);
        return(0);
#endif
    }

quit:
    close(sd);
#ifdef V3HPROXY
    return(0);
#else
    return(-1);
#endif
}



int ventrilo3_hdr_udp(int type, u8 *buff, u8 *pck) {
    u8      c;

    memset(buff, 0, 0x200); // no, I'm not mad, this is EXACTLY what Ventrilo does

    switch(type - 1) {
        case 0:  c = 0xb4;  break;
        case 1:  c = 0x70;  break;
        case 2:  c = 0x24;  break;
        case 3:  c = 0xb8;  break;
        case 4:  c = 0x74;  break;
        case 5:  c = 0x5c;  break;
        case 6:  c = 0xd0;  break;
        case 7:  c = 0x08;  break;
        case 8:  c = 0x50;  break;
        default: c = 0;     break;
    }
    c += 0x10;

    putbe(buff + 8,  type,   16);
    putbe(buff + 10, c,    16);
    buff[4] = 'U';
    buff[5] = 'D';
    buff[6] = 'C';
    buff[7] = 'L';
    buff[0xc] = 1;
    putbe(buff + 0x10, 0xb401, 32);         // x[seq], recheck
    putbe(buff + 0x14, getbe(pck + 0x14, NULL, 32), 32);
    putbe(buff + 0x18, getbe(pck + 0x18, NULL, 32), 32);
    putbe(buff + 0x1c, getbe(pck + 0x1c, NULL, 32), 32);
    putbe(buff + 0x20, getbe(pck + 0x20, NULL, 32), 32);
    putbe(buff + 0x24, getbe(pck + 0x28, NULL, 32), 32);
    buff[0x28] = pck[0x30];
    buff[0x29] = 0;
    buff[0x2a] = 0;
    buff[0x2b] = 0;
    putbe(buff + 0x2c, getbe(pck + 0x24, NULL, 16), 16);
    putbe(buff + 0x2e, 0, 16);
    putbe(buff + 0x30, getbe(pck + 0x14, NULL, 16), 16);
    memcpy(buff + 0x34, pck + 0x38, 16);    // hash
    memcpy(buff + 0x44, pck + 0x88, 32);    // WIN32
    buff[0x63] = 0;
    memcpy(buff + 0x64, pck + 0xa8, 32);    // version
    buff[0x83] = 0;
    return(132);
}



int ventrilo3_send_udp(int sd, int vnum, u32 ip, u16 port, u8 *data, int len) {
    struct  sockaddr_in peer;
    int     i,
            k;
    u8      tmp[4];

    if(vnum >= 0) {
        tmp[0] = ip;
        tmp[1] = ip >> 8;
        tmp[2] = ip >> 16;
        tmp[3] = ip >> 24;
        k = (tmp[0] & 0x0f) * vnum;
        for(i = 16; i < len; i++, k++) {
            data[i] += tmp[k & 3];
        }
    }

    peer.sin_addr.s_addr = ip;
    peer.sin_port        = htons(port);
    peer.sin_family      = AF_INET;

    //printf(". %s:%hu\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
    sendto(sd, data, len, 0, (struct sockaddr *)&peer, sizeof(struct sockaddr_in));
    return(0);
}



int ventrilo3_recv_udp(int sd, ventrilo3_auth_t *vauth, u8 *data, int maxsz, int *handshake_num) {
    struct  sockaddr_in peer;
    u32     ip;
    int     len,
            i,
            k,
            vnum,
            psz;
    u8      tmp[4];

    if(v3timeout(sd, 2) < 0) return(-1);
    psz = sizeof(struct sockaddr_in);
    len = recvfrom(sd, data, maxsz, 0, (struct sockaddr *)&peer, &psz);
    if(len < 0) return(-1);
    if(!vauth) return(len);

    for(i = 0; vauth[i].host; i++) {
        ip = inet_addr(vauth[i].host);
        if(ip == peer.sin_addr.s_addr) {
            vnum = vauth[i].vnum;
            break;
        }
    }
    if(!vauth[i].host) return(0);

    *handshake_num = i;
    if(16 < (data[10] | (data[11] << 8))) { // blah, from Ventrilo
        tmp[0] = ip;
        tmp[1] = ip >> 8;
        tmp[2] = ip >> 16;
        tmp[3] = ip >> 24;
        k = (tmp[0] & 0x0f) * vnum;
        for(i = 16; i < len; i++, k++) {
            data[i] -= tmp[k & 3];
        }
    }
    return(len);
}



int getbe(u8 *data, u32 *ret, int bits) {
    u32     num;
    int     i,
            bytes;

    bytes = bits >> 3;
    for(num = i = 0; i < bytes; i++) {
        num |= (data[i] << ((bytes - 1 - i) << 3));
    }
    if(!ret) return(num);
    *ret = num;
    return(bytes);
}



int putbe(u8 *data, u32 num, int bits) {
    int     i,
            bytes;

    bytes = bits >> 3;
    for(i = 0; i < bytes; i++) {
        data[i] = (num >> ((bytes - 1 - i) << 3)) & 0xff;
    }
    return(bytes);
}



int v3timeout(int sock, int secs) {
    struct  timeval tout;
    fd_set  fd_read;

    tout.tv_sec  = secs;
    tout.tv_usec = 0;
    FD_ZERO(&fd_read);
    FD_SET(sock, &fd_read);
    if(select(sock + 1, &fd_read, NULL, NULL, &tout)
      <= 0) return(-1);
    return(0);
}


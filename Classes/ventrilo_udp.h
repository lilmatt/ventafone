/*

Ventrilo UDP status algorithm 0.1
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org


INTRODUCTION
============
This algorithm is the method used by the chat program Ventrilo
(http://www.ventrilo.com) for encoding the UDP packets used to get
the status informations.


FUNCTIONS
=========
struct ventrilo_udp_head
void ventrilo_udp_head_dec(unsigned char *data)
void ventrilo_udp_head_enc(unsigned char *data)
void ventrilo_udp_data_dec(unsigned char *data, int len, unsigned short key)
unsigned short ventrilo_udp_data_enc(unsigned char *data, int len)
unsigned short ventrilo_udp_crc(unsigned char *data, int len)


USAGE EXAMPLE
=============
Watch my "Ventrilo status retriever" code for a simple and practical example:

  http://aluigi.org/papers/ventstat.zip


LICENSE
=======
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

#import <time.h>
#import <netinet/in.h>
#import <sys/times.h>
#define VENTRILO_RAND   time(0)



typedef struct {
    unsigned short pckkey;  // key for decoding this header
    unsigned short zero;    // ever 0
    unsigned short cmd;     // command number: 1 generic info, 2 for details
    unsigned short id;      // packet ID used for tracking the replies
    unsigned short totlen;  // total data size (for data splitted in packets)
    unsigned short len;     // size of the data in this packet (max 492)
    unsigned short totpck;  // total amount of packets (max 32)
    unsigned short pck;     // current packet number
    unsigned short datakey; // key for decoding the data
    unsigned short crc;     // checksum of the total plain-text data
} ventrilo_udp_head;



const static unsigned char  ventrilo_udp_encdata_head[] =
    "\x80\xe5\x0e\x38\xba\x63\x4c\x99\x88\x63\x4c\xd6\x54\xb8\x65\x7e"
    "\xbf\x8a\xf0\x17\x8a\xaa\x4d\x0f\xb7\x23\x27\xf6\xeb\x12\xf8\xea"
    "\x17\xb7\xcf\x52\x57\xcb\x51\xcf\x1b\x14\xfd\x6f\x84\x38\xb5\x24"
    "\x11\xcf\x7a\x75\x7a\xbb\x78\x74\xdc\xbc\x42\xf0\x17\x3f\x5e\xeb"
    "\x74\x77\x04\x4e\x8c\xaf\x23\xdc\x65\xdf\xa5\x65\xdd\x7d\xf4\x3c"
    "\x4c\x95\xbd\xeb\x65\x1c\xf4\x24\x5d\x82\x18\xfb\x50\x86\xb8\x53"
    "\xe0\x4e\x36\x96\x1f\xb7\xcb\xaa\xaf\xea\xcb\x20\x27\x30\x2a\xae"
    "\xb9\x07\x40\xdf\x12\x75\xc9\x09\x82\x9c\x30\x80\x5d\x8f\x0d\x09"
    "\xa1\x64\xec\x91\xd8\x8a\x50\x1f\x40\x5d\xf7\x08\x2a\xf8\x60\x62"
    "\xa0\x4a\x8b\xba\x4a\x6d\x00\x0a\x93\x32\x12\xe5\x07\x01\x65\xf5"
    "\xff\xe0\xae\xa7\x81\xd1\xba\x25\x62\x61\xb2\x85\xad\x7e\x9d\x3f"
    "\x49\x89\x26\xe5\xd5\xac\x9f\x0e\xd7\x6e\x47\x94\x16\x84\xc8\xff"
    "\x44\xea\x04\x40\xe0\x33\x11\xa3\x5b\x1e\x82\xff\x7a\x69\xe9\x2f"
    "\xfb\xea\x9a\xc6\x7b\xdb\xb1\xff\x97\x76\x56\xf3\x52\xc2\x3f\x0f"
    "\xb6\xac\x77\xc4\xbf\x59\x5e\x80\x74\xbb\xf2\xde\x57\x62\x4c\x1a"
    "\xff\x95\x6d\xc7\x04\xa2\x3b\xc4\x1b\x72\xc7\x6c\x82\x60\xd1\x0d";

const static unsigned char  ventrilo_udp_encdata_data[] =
    "\x82\x8b\x7f\x68\x90\xe0\x44\x09\x19\x3b\x8e\x5f\xc2\x82\x38\x23"
    "\x6d\xdb\x62\x49\x52\x6e\x21\xdf\x51\x6c\x76\x37\x86\x50\x7d\x48"
    "\x1f\x65\xe7\x52\x6a\x88\xaa\xc1\x32\x2f\xf7\x54\x4c\xaa\x6d\x7e"
    "\x6d\xa9\x8c\x0d\x3f\xff\x6c\x09\xb3\xa5\xaf\xdf\x98\x02\xb4\xbe"
    "\x6d\x69\x0d\x42\x73\xe4\x34\x50\x07\x30\x79\x41\x2f\x08\x3f\x42"
    "\x73\xa7\x68\xfa\xee\x88\x0e\x6e\xa4\x70\x74\x22\x16\xae\x3c\x81"
    "\x14\xa1\xda\x7f\xd3\x7c\x48\x7d\x3f\x46\xfb\x6d\x92\x25\x17\x36"
    "\x26\xdb\xdf\x5a\x87\x91\x6f\xd6\xcd\xd4\xad\x4a\x29\xdd\x7d\x59"
    "\xbd\x15\x34\x53\xb1\xd8\x50\x11\x83\x79\x66\x21\x9e\x87\x5b\x24"
    "\x2f\x4f\xd7\x73\x34\xa2\xf7\x09\xd5\xd9\x42\x9d\xf8\x15\xdf\x0e"
    "\x10\xcc\x05\x04\x35\x81\xb2\xd5\x7a\xd2\xa0\xa5\x7b\xb8\x75\xd2"
    "\x35\x0b\x39\x8f\x1b\x44\x0e\xce\x66\x87\x1b\x64\xac\xe1\xca\x67"
    "\xb4\xce\x33\xdb\x89\xfe\xd8\x8e\xcd\x58\x92\x41\x50\x40\xcb\x08"
    "\xe1\x15\xee\xf4\x64\xfe\x1c\xee\x25\xe7\x21\xe6\x6c\xc6\xa6\x2e"
    "\x52\x23\xa7\x20\xd2\xd7\x28\x07\x23\x14\x24\x3d\x45\xa5\xc7\x90"
    "\xdb\x77\xdd\xea\x38\x59\x89\x32\xbc\x00\x3a\x6d\x61\x4e\xdb\x29";



void ventrilo_udp_head_dec(unsigned char *data);
void ventrilo_udp_head_enc(unsigned char *data);
void ventrilo_udp_data_dec(unsigned char *data, int len, unsigned short key);
unsigned short ventrilo_udp_data_enc(unsigned char *data, int len);
unsigned short ventrilo_udp_crc(unsigned char *data, int len);


#undef VENTRILO_RAND

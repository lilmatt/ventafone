/*
Ventrilo encryption/decryption algorithm 0.2a
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org

INTRODUCTION
============
This algorithm is the method used by the chat program Ventrilo
(http://www.ventrilo.com) for encrypting the communication stream
between clients and servers.


THANX TO
========
Georg Hofstetter (http://www.g3gg0.de)


LICENSE
=======
    Copyright 2004,2005,2006,2007,2008 Luigi Auriemma

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

#import <stdlib.h>
#import <stdint.h>
#import <string.h>



//#define VENTRILO_ALGO_PROTOCOL    // not needed, it's only obfuscated code



typedef struct {
    uint8_t     key[256];
    uint32_t    pos;
    uint32_t    size;
    #ifdef VENTRILO_ALGO_PROTOCOL
    int         proto;
    #endif
} ventrilo_key_ctx;



int ventrilo_read_keys(ventrilo_key_ctx *client, ventrilo_key_ctx *server, uint8_t *data, int size
#ifdef VENTRILO_ALGO_PROTOCOL
    , int protocol
#endif
					   );



void ventrilo_first_dec(uint8_t *data, int size);



void ventrilo_first_enc(uint8_t *data, int size);



void ventrilo_dec(ventrilo_key_ctx *ctx, uint8_t *data, int size);



void ventrilo_enc(ventrilo_key_ctx *ctx, uint8_t *data, int size);


/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

static char const	ident[] = "$Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/gsm_create.c,v 1.4 1996/07/02 09:59:05 jutta Exp $";

#import	"config.h"

#ifdef	HAS_STRING_H
#import	<string.h>
#else
#	include "proto.h"
	extern char	* memset P((char *, int, int));
#endif

#ifdef	HAS_STDLIB_H
#	include	<stdlib.h>
#else
#	ifdef	HAS_MALLOC_H
#		include 	<malloc.h>
#	else
		extern char * malloc();
#	endif
#endif

#import <stdio.h>

#import "gsm.h"
#import "private.h"
#import "proto.h"

gsm gsm_create ()
{
	gsm  r;

	r = (gsm)malloc(sizeof(struct gsm_state));
	if (!r) return r;

	memset((char *)r, 0, sizeof(*r));
	r->nrp = 40;

	return r;
}

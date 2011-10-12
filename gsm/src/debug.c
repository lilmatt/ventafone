/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/debug.c,v 1.2 1993/01/29 18:22:20 jutta Exp $ */

#import "private.h"

#ifndef	NDEBUG

/* If NDEBUG _is_ defined and no debugging should be performed,
 * calls to functions in this module are #defined to nothing
 * in private.h.
 */
#import "config.h"
#import <stdio.h>
#import "proto.h"

void gsm_debug_words (
	char 	      * name,
	int		from,
	int		to,
	word		* ptr)
{
	int 	nprinted = 0;

	fprintf( stderr, "%s [%d .. %d]: ", name, from, to );
	while (from <= to) {
		fprintf(stderr, "%d ", ptr[ from ] );
		from++;
		if (nprinted++ >= 7) {
			nprinted = 0;
			if (from < to) putc('\n', stderr);
		}
	}
	putc('\n', stderr);
}

void gsm_debug_longwords (
	char 	      * name,
	int		from,
	int		to,
	longword      * ptr)
{
	int 	nprinted = 0;

	fprintf( stderr, "%s [%d .. %d]: ", name, from, to );
	while (from <= to) {

		fprintf(stderr, "%d ", ptr[ from ] );
		from++;
		if (nprinted++ >= 7) {
			nprinted = 0;
			if (from < to) putc('\n', stderr);
		}
	}
	putc('\n', stderr);
}

void gsm_debug_longword (
	char		* name,
	longword	  value	)
{
	fprintf(stderr, "%s: %d\n", name, (long)value );
}

void gsm_debug_word (
	char	* name,
	word	  value	)
{
	fprintf(stderr, "%s: %d\n", name, (long)value);
}

#endif

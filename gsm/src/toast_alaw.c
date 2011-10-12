/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/kbs/jutta/src/gsm/gsm-1.0/src/RCS/toast_alaw.c,v 1.2 1996/07/05 17:23:46 jutta Exp $ */

#import	"toast.h"
#import "config.h"

/*  toast_alaw.c -- manipulate A-law encoded sound.
 */

extern FILE	* in, * out;

#define	A2S(x)	(a2s[  (unsigned char )(x)       ])
#define	S2A(x)	(s2a[ ((unsigned short)(x)) >> 4 ])

static unsigned short a2s[] = {

 60032, 60288, 59520, 59776, 61056, 61312, 60544, 60800,
 57984, 58240, 57472, 57728, 59008, 59264, 58496, 58752,
 62784, 62912, 62528, 62656, 63296, 63424, 63040, 63168,
 61760, 61888, 61504, 61632, 62272, 62400, 62016, 62144,
 43520, 44544, 41472, 42496, 47616, 48640, 45568, 46592,
 35328, 36352, 33280, 34304, 39424, 40448, 37376, 38400,
 54528, 55040, 53504, 54016, 56576, 57088, 55552, 56064,
 50432, 50944, 49408, 49920, 52480, 52992, 51456, 51968,
 65192, 65208, 65160, 65176, 65256, 65272, 65224, 65240,
 65064, 65080, 65032, 65048, 65128, 65144, 65096, 65112,
 65448, 65464, 65416, 65432, 65512, 65528, 65480, 65496,
 65320, 65336, 65288, 65304, 65384, 65400, 65352, 65368,
 64160, 64224, 64032, 64096, 64416, 64480, 64288, 64352,
 63648, 63712, 63520, 63584, 63904, 63968, 63776, 63840,
 64848, 64880, 64784, 64816, 64976, 65008, 64912, 64944,
 64592, 64624, 64528, 64560, 64720, 64752, 64656, 64688,
  5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
  7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
  2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
  3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
 22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
 30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
 11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
 15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
   344,   328,   376,   360,   280,   264,   312,   296,
   472,   456,   504,   488,   408,   392,   440,   424,
    88,    72,   120,   104,    24,     8,    56,    40,
   216,   200,   248,   232,   152,   136,   184,   168,
  1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
  1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
   688,   656,   752,   720,   560,   528,   624,   592,
   944,   912,  1008,   976,   816,   784,   880,   848

};


static unsigned char  s2a[] = {

	213,212,215,214,209,208,211,210,221,220,223,222,217,216,219,218,
	197,196,199,198,193,192,195,194,205,204,207,206,201,200,203,202,
	245,245,244,244,247,247,246,246,241,241,240,240,243,243,242,242,
	253,253,252,252,255,255,254,254,249,249,248,248,251,251,250,250,
	229,229,229,229,228,228,228,228,231,231,231,231,230,230,230,230,
	225,225,225,225,224,224,224,224,227,227,227,227,226,226,226,226,
	237,237,237,237,236,236,236,236,239,239,239,239,238,238,238,238,
	233,233,233,233,232,232,232,232,235,235,235,235,234,234,234,234,
	149,149,149,149,149,149,149,149,148,148,148,148,148,148,148,148,
	151,151,151,151,151,151,151,151,150,150,150,150,150,150,150,150,
	145,145,145,145,145,145,145,145,144,144,144,144,144,144,144,144,
	147,147,147,147,147,147,147,147,146,146,146,146,146,146,146,146,
	157,157,157,157,157,157,157,157,156,156,156,156,156,156,156,156,
	159,159,159,159,159,159,159,159,158,158,158,158,158,158,158,158,
	153,153,153,153,153,153,153,153,152,152,152,152,152,152,152,152,
	155,155,155,155,155,155,155,155,154,154,154,154,154,154,154,154,
	133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,
	132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
	135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,
	134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,
	129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
	130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,
	141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,
	140,140,140,140,140,140,140,140,140,140,140,140,140,140,140,140,
	143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,
	142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,
	137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,
	136,136,136,136,136,136,136,136,136,136,136,136,136,136,136,136,
	139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,
	138,138,138,138,138,138,138,138,138,138,138,138,138,138,138,138,
	181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,
	181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,
	180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,
	180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,
	183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,
	183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,
	182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,
	182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,
	177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,
	177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,
	176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,
	176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,
	179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,
	179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,
	178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,
	178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,
	189,189,189,189,189,189,189,189,189,189,189,189,189,189,189,189,
	189,189,189,189,189,189,189,189,189,189,189,189,189,189,189,189,
	188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
	188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
	191,191,191,191,191,191,191,191,191,191,191,191,191,191,191,191,
	191,191,191,191,191,191,191,191,191,191,191,191,191,191,191,191,
	190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
	190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
	185,185,185,185,185,185,185,185,185,185,185,185,185,185,185,185,
	185,185,185,185,185,185,185,185,185,185,185,185,185,185,185,185,
	184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,
	184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,
	187,187,187,187,187,187,187,187,187,187,187,187,187,187,187,187,
	187,187,187,187,187,187,187,187,187,187,187,187,187,187,187,187,
	186,186,186,186,186,186,186,186,186,186,186,186,186,186,186,186,
	186,186,186,186,186,186,186,186,186,186,186,186,186,186,186,186,
	165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,
	165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,
	165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,
	165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,
	164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,
	164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,
	164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,
	164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,164,
	167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,
	167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,
	167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,
	167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,167,
	166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,
	166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,
	166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,
	166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,166,
	161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,
	161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,
	161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,
	161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,161,
	160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,
	160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,
	160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,
	160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,
	163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
	163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
	163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
	163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
	162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,
	162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,
	162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,
	162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,162,
	173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,
	173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,
	173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,
	173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,
	172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,
	172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,
	172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,
	172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,172,
	175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
	175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
	175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
	175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
	174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,
	174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,
	174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,
	174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,
	169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,
	169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,
	169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,
	169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,
	168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,
	168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,
	168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,
	168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,168,
	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,
	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,
	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,
	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,
	170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,
	170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,
	170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,
	170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,
	 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
	 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
	 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
	 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
	 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
	 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
	 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
	 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
	 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
	 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
	 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
	 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
	 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
	 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
	 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
	 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
	 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
	 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
	 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
	 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
	 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
	 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
	 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
	 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
	 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
	 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
	 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
	 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
	 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
	 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
	 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
	 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
	 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
	 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
	 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
	 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
	 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
	 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
	 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
	 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
	 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
	 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
	 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
	 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
	 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
	 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
	 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
	 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
	 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
	 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
	 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
	 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
	 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
	 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
	 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
	 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
	 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
	 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
	 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
	 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
	 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
	 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
	 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
	 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
	 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
	 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
	 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
	 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52,
	 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52,
	 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
	 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
	 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
	  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
	 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
	  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
	  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
	 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27,
	 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25,
	 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31,
	 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29,
	 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19,
	 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17,
	 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21,
	106,106,106,106,107,107,107,107,104,104,104,104,105,105,105,105,
	110,110,110,110,111,111,111,111,108,108,108,108,109,109,109,109,
	 98, 98, 98, 98, 99, 99, 99, 99, 96, 96, 96, 96, 97, 97, 97, 97,
	102,102,102,102,103,103,103,103,100,100,100,100,101,101,101,101,
	122,122,123,123,120,120,121,121,126,126,127,127,124,124,125,125,
	114,114,115,115,112,112,113,113,118,118,119,119,116,116,117,117,
	 74, 75, 72, 73, 78, 79, 76, 77, 66, 67, 64, 65, 70, 71, 68, 69,
	 90, 91, 88, 89, 94, 95, 92, 93, 82, 83, 80, 81, 86, 87, 84, 85
};

int alaw_input P1((buf), gsm_signal * buf) 
{
	int  i, c;

	for (i = 0; i < 160 && (c = fgetc(in)) != EOF; i++) buf[i] = A2S( c );
	if (c == EOF && ferror(in)) return -1;
	return i;
}

int alaw_output P1((buf), gsm_signal * buf) 
{
	int  i;

	for (i = 0; i < 160; i++, buf++)
		if (fputc( S2A( *buf ), out) == EOF) return -1;
	return 0;
}


#ifdef HAVE_CONFIG_H
#import "config.h"
#endif

#import "speex_preprocess.h"
#import <stdio.h>

#define NN 160

int test_denoise()
{
   short in[NN];
   int i;
   SpeexPreprocessState *st;
   int count=0;
   float f;

   st = speex_preprocess_state_init(NN, 8000);
   i=1;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);
   i=0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
   i=8000;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
   i=0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
   f=.0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
   f=.0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
   while (1)
   {
      int vad;
      fread(in, sizeof(short), NN, stdin);
      if (feof(stdin))
         break;
      vad = speex_preprocess_run(st, in);
      /*fprintf (stderr, "%d\n", vad);*/
      fwrite(in, sizeof(short), NN, stdout);
      count++;
   }
   speex_preprocess_state_destroy(st);
   return 0;
}

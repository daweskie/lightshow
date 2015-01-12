#ifndef MYMISC_H_INCLUDED
#define MYMISC_H_INCLUDED


void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_toggle(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_way(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_min(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_now(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_max(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_min_ch(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_max_ch(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_way_ch(BaseSequentialStream *chp, int argc, char *argv[]);

void startBlinker(void);




#endif // MYMISC_H_INCLUDED

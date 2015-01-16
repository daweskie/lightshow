#ifndef COMM_H_INCLUDED
#define COMM_H_INCLUDED


void cmd_serial_test(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_serial_config(BaseSequentialStream *chp, int argc, char *argv[]);
void commInit(void);


#endif // COMM_H_INCLUDED

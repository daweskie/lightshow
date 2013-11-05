#ifndef CMDUTIL_H_INCLUDED
#define CMDUTIL_H_INCLUDED

void hexDump(BaseSequentialStream *chp, uint8_t *address, int len);

void cmd_dump(BaseSequentialStream *chp, int argc, char *argv[]);


#endif // CMDUTIL_H_INCLUDED

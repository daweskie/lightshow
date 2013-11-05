/*
 * Copyright (C) 2011 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file adc.c
    \brief ad converter
*/

#include <stdlib.h>
#include <string.h>

#include <mcuconf.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <adc_lld.h>
#include <chtypes.h>
#include <stm32f4xx.h>
#include <board.h>
#include <pal_lld.h>
#include <string.h>
#include <math.h>
#include <Flash.h>
#include <configflash.h>

#include <snprintf.h>
#include "bela.h"
#include "adc.h"
#include "misc.h"

/**
    Callback function at end of conversion
*/
static void adcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t size);

/**
    Callback function at conversion error
*/
static void adcErrorCallback(ADCDriver *adcp, adcerror_t err);

static const ADCConversionGroup adcConfig = {
    FALSE,                                   // circular buffer mode
    1,                                      // number of the analog channels
    adcCallback,                            // callback function
    adcErrorCallback,                       // error callback
    0,                                      // CR1
    ADC_CR2_SWSTART,                        // conversion started by software
    ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3),       // sample times ch10-18
    0,                                      // sample times ch0-9
    0,                                      // SQR1: conversion group sequence 13..16+sequence length
    0,                                      // SQR2: conversion group sequence 7..12
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)        // SQR3: conversion group sequence 1..6
};

/**
    Channel param variables to normalize temperature for each channel
    t=(t1-t0)/(x1-x0) * (x-x0) + t1
*/
typedef struct ChannelPar {
    float k;        // (t1-t0)/(x1-x0)
    uint16_t x0;
    uint16_t x1;
    float t0;
    float t1;
} ChannelParam;

static struct {
    volatile int running;
    volatile int convertOK;
    adcsample_t samples[BELA_ADC_SAMPLES_COUNTS];
    int errOverflow;
    int errDma;
    int errUnknown;
    int average[BELA_CHANNELS_NUM];
    float temperatures[BELA_CHANNELS_NUM];
    int channel;
    ChannelParam channelParams[BELA_CHANNELS_NUM];
} adParams;

/**
    Callback function at end of conversion.
    It sets the convertOK flag to sign the end of conversion.
*/
static void adcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t size) {
    (void) adcp;
    (void) buffer;
    (void) size;
    adParams.convertOK = 1;
}

/**
    Callback function at conversion error
    There are 2 cause of error:
    - DMA failure
    - Overflow
    other nonordered errors reported at errUnknown
    All of error reported at adParams error flags
*/
static void adcErrorCallback(ADCDriver *adcp, adcerror_t err) {
    (void) adcp;
    switch (err) {
        case ADC_ERR_DMAFAILURE : adParams.errDma++;
            return;
        case ADC_ERR_OVERFLOW : adParams.errOverflow++;
            return;
        default:
            adParams.errUnknown++;
    }
}

/**
    Check restored channel params. If the params aren't correct it set de default values.
*/
static void checkParam(uint16_t ch) {
    ChannelParam *p = &adParams.channelParams[ch];
    int chkErr = 0;
    char buffer[16];
    if (p->x1>BELA_ADC_RESOLUTION ||
        isless(p->t0,BELA_ADC_MIN_TEMP) ||
        isgreater(p->t1, BELA_ADC_MAX_TEMP) ||
        isgreaterequal(p->t0, p->t1) ||
        p->x0 >= p->x1) {
#ifdef BELA_ADC_DEBUGGING
                p->t0 = BELA_ADC_NORMALIZE_T0+ch;
                p->t1 = BELA_ADC_NORMALIZE_T1-ch;
                p->x0 = BELA_ADC_NORMALIZE_X0+ch;
                p->x1 = BELA_ADC_NORMALIZE_X1-ch;
#else
                p->t0 = BELA_ADC_NORMALIZE_T0;
                p->t1 = BELA_ADC_NORMALIZE_T1;
                p->x0 = BELA_ADC_NORMALIZE_X0;
                p->x1 = BELA_ADC_NORMALIZE_X1;
#endif
                chkErr = 1;
    }
    p->k = (p->t1-p->t0)/(p->x1-p->x0);
    if (chkErr) {
        snprintf(buffer,sizeof(buffer), "%s%i", BELA_ADC_FLASH_CONFIG_PREFIX,ch);
        cfgAddItem(buffer,p,sizeof(ChannelParam));
    }
}

static void restoreChannelParam(int ch) {
    char buffer[16];
    snprintf(buffer,16,"%s%d",BELA_ADC_FLASH_CONFIG_PREFIX,ch);
    cfgGetItem(buffer,(uint8_t *)&adParams.channelParams[ch], sizeof(ChannelParam));
    checkParam(ch);
}

/**
    Restore params from falsh or default.
*/
static void restoreChannelParams(void) {
    int i=0;
    for (;i<BELA_CHANNELS_NUM;i++)
        restoreChannelParam(i);
}

/**
    Convert digital value to temperature, based on normalization params for each channel.
*/
static float normalizeChannelValue(int channel, int x) {
    if (channel < 0 || channel > BELA_CHANNELS_NUM)
        return 0.0f;

    ChannelParam *cp = &adParams.channelParams[channel];
    float result = cp->k * (x-cp->x0) + cp->t0;
    return result;
}

static WORKING_AREA(waThread1, 128);

/**
    Analog conversion task.
    Select the next channel and start conversion.
    After the conversion finished, it converts to temperature.
*/
static msg_t Thread1(void *arg) {
    (void) arg;
    chRegSetThreadName("adconversion");

    while(adParams.running) {

        if (adParams.convertOK) {
            int i, sum;
            for (i=0,sum=0;i<BELA_ADC_SAMPLES_COUNTS;i++)
                sum += adParams.samples[i];

            adParams.average[adParams.channel]=sum/BELA_ADC_SAMPLES_COUNTS;

            adParams.temperatures[adParams.channel] =
                    normalizeChannelValue(adParams.channel, adParams.average[adParams.channel]);

            adParams.channel = (adParams.channel+1) % BELA_CHANNELS_NUM;

            palWritePort(BELA_ADC_CHANNEL_PORT, adParams.channel & BELA_ADC_CHANNEL_MASK);

            adParams.convertOK = 0;
            adcStartConversion(&ADCD1, &adcConfig, adParams.samples, BELA_ADC_SAMPLES_COUNTS);
        }
        chThdSleepMicroseconds(15);
    }
    return EXIT_SUCCESS;
}

/**
    Ad conversion user interface
*/
void cmd_ad(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int i;
    for (i=0;i<BELA_CHANNELS_NUM;i++)
        chprintf(chp, "%f ", adParams.temperatures[i]);
    chprintf(chp, "\r\n");
}

/**
    Stop conversion.
*/
void adStop(void) {
    adParams.running = 0;
}

/**
    check the ad conversion running
*/
int adIsRunning(void) {
    return adParams.running;
}

float adGetChannelValue(int ch) {
    return ch>=0 && ch < BELA_CHANNELS_NUM
        ? adParams.temperatures[ch]
        : BELA_ADC_NORMALIZE_T1;
}

void cmd_showChannelParams(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    restoreChannelParams();
    int i;
    for (i=0;i<BELA_CHANNELS_NUM;i++) {
        ChannelParam *cp = &adParams.channelParams[i];
        chprintf(chp, "ch%d: x0:%d, x1:%d, t0:%f, t1:%f, k:%f\r\n",
                 i, cp->x0,cp->x1,cp->t0,cp->t1,cp->k);
    }
}

/**
    Initialize ad conversion
*/
void adConvInit(void) {
    bzero(&adParams, sizeof(adParams));
    restoreChannelParams();
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    palSetGroupMode(BELA_ADC_PORT, PAL_PORT_BIT(BELA_ADC_PORT_BIT), 0, PAL_MODE_INPUT_ANALOG);
    palSetGroupMode(BELA_ADC_CHANNEL_PORT, BELA_ADC_CHANNEL_MASK, 0, PAL_MODE_OUTPUT_PUSHPULL);
    adcStart(&ADCD1, NULL);
    adcSTM32EnableTSVREFE();
    adcStartConversion(&ADCD1, &adcConfig, adParams.samples, BELA_ADC_SAMPLES_COUNTS);
    adParams.running = 1;
}


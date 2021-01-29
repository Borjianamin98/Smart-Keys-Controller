/*
 * File: ert_main.c
 *
 * Code generated for Simulink model 'slave'.
 *
 * Model version                  : 1.87
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Fri Jan 22 18:05:45 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "slave.h"
#include "rtwtypes.h"

volatile int IsrOverrun = 0;
static boolean_T OverrunFlag = 0;
void rt_OneStep(void)
{
    /* Check for overrun. Protect OverrunFlag against preemption */
    if (OverrunFlag++) {
        IsrOverrun = 1;
        OverrunFlag--;
        return;
    }

#ifndef _MW_ARDUINO_LOOP_

    sei();

#endif;

    slave_step();

    /* Get model outputs here */
#ifndef _MW_ARDUINO_LOOP_

    cli();

#endif;

    OverrunFlag--;
}

volatile boolean_T stopRequested;
volatile boolean_T runModel;
int main(void)
{
    float modelBaseRate = 1.0;
    float systemClock = 0;

    /* Initialize variables */
    stopRequested = false;
    runModel = false;
    init();
    MW_Arduino_Init();
    rtmSetErrorStatus(rtM, 0);
    slave_initialize();
    configureArduinoAVRTimer();
    runModel =
        rtmGetErrorStatus(rtM) == (NULL);

#ifndef _MW_ARDUINO_LOOP_

    sei();

#endif;

    sei();
    while (runModel) {
        stopRequested = !(
                          rtmGetErrorStatus(rtM) == (NULL));
        runModel = !(stopRequested);
        MW_Arduino_Loop();
    }

    /* Disable rt_OneStep() here */
    cli();
    return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/*
 * File: slave.c
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

/* Named constants for Chart: '<S1>/slave_chart' */
#define IN_bus_idle                    ((uint8_T)1U)
#define IN_process_data                ((uint8_T)2U)
#define IN_put_data                    ((uint8_T)3U)
#define IN_read_data                   ((uint8_T)4U)
#define IN_read_register_address       ((uint8_T)5U)
#define IN_startup                     ((uint8_T)6U)
#define IN_update_leds                 ((uint8_T)7U)
#define IN_wait_bus_idle               ((uint8_T)8U)
#define IN_wait_for_master             ((uint8_T)9U)

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Forward declaration for local functions */
static uint8_T combine_bits(const uint8_T bits[8]);
static uint8_T button_pressed(const uint8_T old_state[8], const uint8_T
    current_state[8]);
static void extract_bits(uint8_T data, uint8_T results[8]);

/* Function for Chart: '<S1>/slave_chart' */
static uint8_T combine_bits(const uint8_T bits[8])
{
    uint8_T output;
    uint8_T i;
    i = 0U;
    output = 0U;
    while (i < sizeof(uint8_T [8])) {
        /* NEW_PATTERN */
        output = (uint8_T)(bits[i] << i | output);
        i++;
    }

    return output;
}

/* Function for Chart: '<S1>/slave_chart' */
static uint8_T button_pressed(const uint8_T old_state[8], const uint8_T
    current_state[8])
{
    uint8_T result;
    result = 0U;
    while ((result < sizeof(uint8_T [8])) && (!((old_state[result] == 0) &&
             (current_state[result] == 1)))) {
        /* NEW_PATTERN */
        result++;
    }

    return result;
}

/* Function for Chart: '<S1>/slave_chart' */
static void extract_bits(uint8_T data, uint8_T results[8])
{
    uint8_T i;
    int16_T i_0;
    for (i_0 = 0; i_0 < 8; i_0++) {
        results[i_0] = 0U;
    }

    for (i = 0U; i < sizeof(uint8_T [8]); i++) {
        /* NEW_PATTERN */
        results[i] = (uint8_T)(data & 0x1);
        data >>= 1;
    }
}

/* Model step function */
void slave_step(void)
{
    int16_T i;
    uint8_T tmp;

    /* Chart: '<S1>/slave_chart' incorporates:
     *  Inport: '<Root>/button'
     *  Inport: '<Root>/data_available'
     *  Inport: '<Root>/physical_address'
     *  Inport: '<Root>/received_data'
     *  Outport: '<Root>/leds'
     *  Outport: '<Root>/log_data'
     *  Outport: '<Root>/put_data'
     */
    if (rtDW.is_active_c1_slave == 0U) {
        rtDW.is_active_c1_slave = 1U;
        rtDW.is_c1_slave = IN_startup;

        /* Outport: '<Root>/command' */
        rtY.command = SLAVE_IDLE;

        /* Outport: '<Root>/log_type' */
        rtY.log_type = LOG_NOTHING;

        /* Outport: '<Root>/leds' */
        for (i = 0; i < 8; i++) {
            rtY.leds[i] = 0U;
            rtDW.button_old[i] = 0U;
        }
    } else {
        switch (rtDW.is_c1_slave) {
          case IN_bus_idle:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;
            if (rtU.data_available && (rtU.received_data == rtU.physical_address))
            {
                rtDW.is_c1_slave = IN_read_register_address;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_READ;
            } else if (rtU.data_available && (rtU.received_data !=
                        rtU.physical_address)) {
                rtDW.is_c1_slave = IN_wait_bus_idle;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IGNORE;
                for (i = 0; i < 8; i++) {
                    rtDW.button_old[i] = rtU.button[i];
                }

                rtDW.idle_clocks = 0U;
            } else if (button_pressed(rtDW.button_old, rtU.button) != sizeof
                       (uint8_T [8])) {
                rtDW.from_wait_bus = false;
                rtDW.is_c1_slave = IN_update_leds;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_BUTTON_PRESSED;
                rtY.log_data[0] = button_pressed(rtDW.button_old, rtU.button);

                /* Outport: '<Root>/leds' incorporates:
                 *  Inport: '<Root>/button'
                 *  Outport: '<Root>/log_data'
                 */
                for (i = 0; i < 8; i++) {
                    rtY.leds[i] = (uint8_T)(rtU.button[i] ^ rtDW.button_old[i] ^
                                            rtY.leds[i]);
                    rtDW.button_old[i] = rtU.button[i];
                }
            } else {
                for (i = 0; i < 8; i++) {
                    rtDW.button_old[i] = rtU.button[i];
                }
            }
            break;

          case IN_process_data:
            rtDW.is_c1_slave = IN_bus_idle;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;

            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            break;

          case IN_put_data:
            rtDW.is_c1_slave = IN_bus_idle;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;

            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            break;

          case IN_read_data:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;
            if (rtU.data_available) {
                rtDW.is_c1_slave = IN_process_data;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_RECEIVE;
                rtY.log_data[0] = 0U;
                rtY.log_data[1] = rtU.received_data;
                extract_bits(rtU.received_data, rtY.leds);
            }
            break;

          case IN_read_register_address:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;
            if (rtU.data_available) {
                rtDW.is_c1_slave = IN_wait_for_master;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IDLE;
            }
            break;

          case IN_startup:
            rtDW.is_c1_slave = IN_bus_idle;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_READ;

            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            break;

          case IN_update_leds:
            tmp = button_pressed(rtDW.button_old, rtU.button);
            if (tmp != sizeof(uint8_T [8])) {
                rtDW.is_c1_slave = IN_update_leds;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_BUTTON_PRESSED;
                rtY.log_data[0] = tmp;

                /* Outport: '<Root>/leds' incorporates:
                 *  Outport: '<Root>/log_data'
                 */
                for (i = 0; i < 8; i++) {
                    rtY.leds[i] = (uint8_T)(rtU.button[i] ^ rtDW.button_old[i] ^
                                            rtY.leds[i]);
                    rtDW.button_old[i] = rtU.button[i];
                }
            } else if (!rtDW.from_wait_bus) {
                rtDW.is_c1_slave = IN_bus_idle;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_READ;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_NOTHING;
            } else {
                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_NOTHING;
                rtDW.is_c1_slave = IN_wait_bus_idle;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IGNORE;
                for (i = 0; i < 8; i++) {
                    rtDW.button_old[i] = rtU.button[i];
                }

                rtDW.idle_clocks = 0U;
            }
            break;

          case IN_wait_bus_idle:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = SLAVE_IGNORE;
            if (rtDW.idle_clocks >= 3) {
                rtDW.is_c1_slave = IN_bus_idle;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_READ;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_NOTHING;
            } else if (button_pressed(rtDW.button_old, rtU.button) != sizeof
                       (uint8_T [8])) {
                rtDW.from_wait_bus = true;
                rtDW.is_c1_slave = IN_update_leds;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_BUTTON_PRESSED;
                rtY.log_data[0] = button_pressed(rtDW.button_old, rtU.button);

                /* Outport: '<Root>/leds' incorporates:
                 *  Inport: '<Root>/button'
                 *  Outport: '<Root>/log_data'
                 */
                for (i = 0; i < 8; i++) {
                    rtY.leds[i] = (uint8_T)(rtU.button[i] ^ rtDW.button_old[i] ^
                                            rtY.leds[i]);
                    rtDW.button_old[i] = rtU.button[i];
                }
            } else if (rtU.data_available) {
                rtDW.is_c1_slave = IN_wait_bus_idle;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_IGNORE;
                for (i = 0; i < 8; i++) {
                    rtDW.button_old[i] = rtU.button[i];
                }

                rtDW.idle_clocks = 0U;
            } else {
                rtDW.idle_clocks++;
            }
            break;

          default:
            /* Outport: '<Root>/command' */
            /* case IN_wait_for_master: */
            rtY.command = SLAVE_IDLE;
            switch (rtU.received_data) {
              case 0:
                rtDW.is_c1_slave = IN_read_data;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_READ;
                break;

              case 128:
                rtDW.is_c1_slave = IN_put_data;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_WRITE;

                /* Outport: '<Root>/command' */
                rtY.command = SLAVE_PUT;

                /* Outport: '<Root>/put_data' incorporates:
                 *  Outport: '<Root>/leds'
                 */
                rtY.put_data = combine_bits(rtY.leds);

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_REPLY;
                rtY.log_data[0] = rtY.put_data;
                break;
            }
            break;
        }
    }

    /* End of Chart: '<S1>/slave_chart' */
}

/* Model initialize function */
void slave_initialize(void)
{
    /* Registration code */

    /* external outputs */
    rtY.register_operation = OPERATION_READ;

    /* SystemInitialize for Outport: '<Root>/register_operation' incorporates:
     *  Chart: '<S1>/slave_chart'
     */
    rtY.register_operation = OPERATION_READ;

    /* SystemInitialize for Outport: '<Root>/log_type' incorporates:
     *  Chart: '<S1>/slave_chart'
     */
    rtY.log_type = LOG_NOTHING;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

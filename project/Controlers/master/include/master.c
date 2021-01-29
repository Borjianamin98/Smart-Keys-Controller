/*
 * File: master.c
 *
 * Code generated for Simulink model 'master'.
 *
 * Model version                  : 1.90
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sat Jan 23 02:26:39 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "master.h"

/* Named constants for Chart: '<S1>/master_chart' */
#define IN_bus_idle                    ((uint8_T)1U)
#define IN_process_data                ((uint8_T)2U)
#define IN_put_data                    ((uint8_T)3U)
#define IN_read_data                   ((uint8_T)4U)
#define IN_retrieve_leds               ((uint8_T)5U)
#define IN_send_physical_address       ((uint8_T)6U)
#define IN_send_register_address       ((uint8_T)7U)
#define IN_startup                     ((uint8_T)8U)
#define IN_update_leds                 ((uint8_T)9U)
#define IN_wait_bus_idle               ((uint8_T)10U)

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
static uint8_T button_pressed(const uint8_T old_state[8], const uint8_T
    current_state[8]);
static void extract_bits(uint8_T data, uint8_T results[8]);
static uint8_T combine_bits(const uint8_T bits[8]);

/* Function for Chart: '<S1>/master_chart' */
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

/* Function for Chart: '<S1>/master_chart' */
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

/* Function for Chart: '<S1>/master_chart' */
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

/* Model step function */
void master_step(void)
{
    int16_T i;
    uint8_T tmp;

    /* Chart: '<S1>/master_chart' incorporates:
     *  Inport: '<Root>/button'
     *  Inport: '<Root>/data_available'
     *  Inport: '<Root>/received_data'
     *  Inport: '<Root>/selected_device'
     *  Outport: '<Root>/leds'
     *  Outport: '<Root>/log_data'
     *  Outport: '<Root>/put_data'
     */
    if (rtDW.is_active_c2_master == 0U) {
        rtDW.is_active_c2_master = 1U;
        rtDW.is_c2_master = IN_startup;

        /* Outport: '<Root>/command' */
        rtY.command = MASTER_IDLE;

        /* Outport: '<Root>/log_type' */
        rtY.log_type = LOG_NOTHING;
        rtDW.phy_addr = rtU.selected_device;
        rtDW.reg_addr = 128U;

        /* Outport: '<Root>/leds' incorporates:
         *  Inport: '<Root>/selected_device'
         */
        for (i = 0; i < 8; i++) {
            rtY.leds[i] = 0U;
            rtDW.button_old[i] = 0U;
        }
    } else {
        switch (rtDW.is_c2_master) {
          case IN_bus_idle:
            /* Outport: '<Root>/command' */
            rtY.command = MASTER_IDLE;
            if (((rtDW.retrieve_timeout == 0) | (rtDW.last_selected_device !=
                    rtU.selected_device)) != 0) {
                rtDW.is_c2_master = IN_retrieve_leds;

                /* Outport: '<Root>/command' */
                rtY.command = MASTER_IDLE;
                rtDW.phy_addr = rtU.selected_device;
                rtDW.reg_addr = 128U;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_RETRIEVE_STATE;
                rtY.log_data[0] = rtDW.phy_addr;
            } else {
                tmp = button_pressed(rtDW.button_old, rtU.button);
                if (tmp != sizeof(uint8_T [8])) {
                    rtDW.is_c2_master = IN_update_leds;

                    /* Outport: '<Root>/command' */
                    rtY.command = MASTER_IDLE;

                    /* Outport: '<Root>/log_type' */
                    rtY.log_type = LOG_BUTTON_PRESSED;
                    rtY.log_data[0] = tmp;

                    /* Outport: '<Root>/leds' incorporates:
                     *  Outport: '<Root>/log_data'
                     */
                    for (i = 0; i < 8; i++) {
                        rtY.leds[i] = (uint8_T)(rtU.button[i] ^
                                                rtDW.button_old[i] ^ rtY.leds[i]);
                        rtDW.button_old[i] = rtU.button[i];
                    }

                    rtDW.phy_addr = rtU.selected_device;
                    rtDW.reg_addr = 0U;
                } else {
                    if (rtDW.retrieve_timeout > 0) {
                        rtDW.retrieve_timeout--;
                        rtDW.is_c2_master = IN_bus_idle;

                        /* Outport: '<Root>/command' */
                        rtY.command = MASTER_IDLE;

                        /* Outport: '<Root>/log_type' */
                        rtY.log_type = LOG_NOTHING;
                        for (i = 0; i < 8; i++) {
                            rtDW.button_old[i] = rtU.button[i];
                        }

                        rtDW.last_selected_device = rtU.selected_device;
                    }
                }
            }
            break;

          case IN_process_data:
            rtDW.is_c2_master = IN_wait_bus_idle;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_IDLE;

            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            rtDW.idle_clocks = 0U;
            break;

          case IN_put_data:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;
            rtDW.is_c2_master = IN_wait_bus_idle;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_IDLE;

            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            rtDW.idle_clocks = 0U;
            break;

          case IN_read_data:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_READ;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_READ;
            if (rtU.data_available) {
                rtDW.is_c2_master = IN_process_data;

                /* Outport: '<Root>/command' */
                rtY.command = MASTER_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_RECEIVE;
                rtY.log_data[0] = rtDW.phy_addr;
                rtY.log_data[1] = rtDW.reg_addr;
                rtY.log_data[2] = rtU.received_data;
                extract_bits(rtU.received_data, rtY.leds);
            }
            break;

          case IN_retrieve_leds:
            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            rtDW.is_c2_master = IN_send_physical_address;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_PUT;

            /* Outport: '<Root>/put_data' */
            rtY.put_data = rtDW.phy_addr;
            break;

          case IN_send_physical_address:
            rtDW.is_c2_master = IN_send_register_address;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_PUT;

            /* Outport: '<Root>/put_data' */
            rtY.put_data = rtDW.reg_addr;
            break;

          case IN_send_register_address:
            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_PUT;
            switch (rtDW.reg_addr) {
              case 0:
                rtDW.is_c2_master = IN_put_data;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_WRITE;

                /* Outport: '<Root>/command' */
                rtY.command = MASTER_PUT;

                /* Outport: '<Root>/put_data' incorporates:
                 *  Outport: '<Root>/leds'
                 */
                rtY.put_data = combine_bits(rtY.leds);

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_SEND;
                rtY.log_data[0] = rtDW.phy_addr;
                rtY.log_data[1] = rtDW.reg_addr;
                rtY.log_data[2] = rtY.put_data;
                break;

              case 128:
                rtDW.is_c2_master = IN_read_data;

                /* Outport: '<Root>/register_operation' */
                rtY.register_operation = OPERATION_READ;

                /* Outport: '<Root>/command' */
                rtY.command = MASTER_READ;
                break;
            }
            break;

          case IN_startup:
            rtDW.is_c2_master = IN_send_physical_address;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_PUT;

            /* Outport: '<Root>/put_data' */
            rtY.put_data = rtDW.phy_addr;
            break;

          case IN_update_leds:
            /* Outport: '<Root>/log_type' */
            rtY.log_type = LOG_NOTHING;
            rtDW.is_c2_master = IN_send_physical_address;

            /* Outport: '<Root>/register_operation' */
            rtY.register_operation = OPERATION_WRITE;

            /* Outport: '<Root>/command' */
            rtY.command = MASTER_PUT;

            /* Outport: '<Root>/put_data' */
            rtY.put_data = rtDW.phy_addr;
            break;

          default:
            /* Outport: '<Root>/command' */
            /* case IN_wait_bus_idle: */
            rtY.command = MASTER_IDLE;
            if (rtDW.idle_clocks >= 5) {
                rtDW.retrieve_timeout = 50U;
                rtDW.is_c2_master = IN_bus_idle;

                /* Outport: '<Root>/command' */
                rtY.command = MASTER_IDLE;

                /* Outport: '<Root>/log_type' */
                rtY.log_type = LOG_NOTHING;
                for (i = 0; i < 8; i++) {
                    rtDW.button_old[i] = rtU.button[i];
                }

                rtDW.last_selected_device = rtU.selected_device;
            } else {
                rtDW.idle_clocks++;
            }
            break;
        }
    }

    /* End of Chart: '<S1>/master_chart' */
}

/* Model initialize function */
void master_initialize(void)
{
    /* Registration code */

    /* external outputs */
    rtY.register_operation = OPERATION_READ;

    /* SystemInitialize for Outport: '<Root>/register_operation' incorporates:
     *  Chart: '<S1>/master_chart'
     */
    rtY.register_operation = OPERATION_READ;

    /* SystemInitialize for Outport: '<Root>/log_type' incorporates:
     *  Chart: '<S1>/master_chart'
     */
    rtY.log_type = LOG_NOTHING;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

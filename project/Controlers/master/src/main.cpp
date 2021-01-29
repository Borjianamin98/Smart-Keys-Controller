#include <master.c>

#include "Arduino.h"

// MAX47 ports
const int BUS_MASTER_WR = 2;
const int BUS_MASTER_DI = 1;
const int BUS_MASTER_RO = 0;

// LED and button ports
const int NUM_OF_LEDS = 4;
const int MASTER_LEDS[] = {10, 11, 12, 13};
const int MASTER_BUTTONS[] = {4, 5, 6, 7};

// Device port
const int SELECTED_DEVICE = 3;

// Store last time state was updated
unsigned long previousMillis = 0;
unsigned long interval = 20;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
    // wait for serial port to connect.
    while (!Serial) {
    }

    // MAX47 ports
    pinMode(BUS_MASTER_WR, OUTPUT);
    pinMode(BUS_MASTER_DI, OUTPUT);
    pinMode(BUS_MASTER_RO, INPUT);

    // LED ports
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        pinMode(MASTER_LEDS[i], OUTPUT);
        pinMode(MASTER_BUTTONS[i], INPUT);
    }
    pinMode(SELECTED_DEVICE, INPUT);

    master_initialize();
    // Read initial value of selected device 
    rtU.selected_device = digitalRead(SELECTED_DEVICE) + 1;

    Serial3.println("Master started successfully");
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
        // Save the last time
        previousMillis = currentMillis;

        master_step();
        if (rtY.register_operation == OPERATION_WRITE) {
            digitalWrite(BUS_MASTER_WR, HIGH);
        } else if (rtY.register_operation == OPERATION_READ) {
            digitalWrite(BUS_MASTER_WR, LOW);
        }

        if (rtY.command == MASTER_PUT) {
            Serial.write(rtY.put_data);
            Serial.flush();
        } else if (rtY.command == MASTER_READ) {
            rtU.data_available = Serial.available() != 0;
            if (!rtU.data_available) {
                rtU.received_data = 0;
            } else {
                rtU.received_data = Serial.read();
            }
        }

        for (int i = 0; i < NUM_OF_LEDS; i++) {
            digitalWrite(MASTER_LEDS[i], rtY.leds[i]);
            rtU.button[i] = digitalRead(MASTER_BUTTONS[i]);
        }
        rtU.selected_device = digitalRead(SELECTED_DEVICE) + 1;

        if (rtY.log_type == LOG_SEND) {
            Serial3.print("Send '");
            Serial3.print(rtY.log_data[2], BIN);
            Serial3.print("' to Slave ");
            Serial3.println(rtY.log_data[0]);
        } else if (rtY.log_type == LOG_RECEIVE) {
            Serial3.print("Receive '");
            Serial3.print(rtY.log_data[2], BIN);
            Serial3.print("' from Slave ");
            Serial3.println(rtY.log_data[0]);
        } else if (rtY.log_type == LOG_BUTTON_PRESSED) {
            Serial3.print("Button ");
            Serial3.print(rtY.log_data[0] + 1);
            Serial3.println(" Pressed");
        } else if (rtY.log_type == LOG_RETRIEVE_STATE) {
            Serial3.print("Going to retrieve status of slave ");
            Serial3.println(rtY.log_data[0]);
        }
    }
}
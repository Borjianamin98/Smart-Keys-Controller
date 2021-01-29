#include <slave.c>

#include "Arduino.h"

// MAX47 ports
const int BUS_SLAVE_WR = 2;
const int BUS_SLAVE_DI = 1;
const int BUS_SLAVE_RO = 0;

// LED and button ports
const int NUM_OF_LEDS = 4;
const int SLAVE_LEDS[] = {10, 11, 12, 13};
const int SLAVE_BUTTONS[] = {4, 5, 6, 7};

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
    pinMode(BUS_SLAVE_WR, OUTPUT);
    pinMode(BUS_SLAVE_DI, OUTPUT);
    pinMode(BUS_SLAVE_RO, INPUT);

    // LED ports
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        pinMode(SLAVE_LEDS[i], OUTPUT);
        pinMode(SLAVE_BUTTONS[i], INPUT);
    }

    slave_initialize();

#ifdef PHYSICAL_ADDRESS
    rtU.physical_address = PHYSICAL_ADDRESS;
    Serial3.print("Slave ");
    Serial3.print(PHYSICAL_ADDRESS);
    Serial3.println(" started successfully");
#else
    Serial3.print("Slave unable to start!");
#endif
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > interval) {
        // Save the last time
        previousMillis = currentMillis;

        slave_step();
        if (rtY.register_operation == OPERATION_WRITE) {
            digitalWrite(BUS_SLAVE_WR, HIGH);
        } else if (rtY.register_operation == OPERATION_READ) {
            digitalWrite(BUS_SLAVE_WR, LOW);
        }

        if (rtY.command == SLAVE_PUT) {
            Serial.write(rtY.put_data);
            Serial.flush();
        } else if (rtY.command == SLAVE_READ) {
            rtU.data_available = Serial.available() != 0;
            if (!rtU.data_available) {
                rtU.received_data = 0;
            } else {
                rtU.received_data = Serial.read();
            }
        } else if (rtY.command == SLAVE_IGNORE) {
            rtU.data_available = Serial.available() != 0;
            uint8_t numOfRead = 0;
            while (Serial.available()) {
                rtU.received_data = Serial.read();
                numOfRead++;
            }
            if (numOfRead == 0) {
                rtU.data_available = false;
                rtU.received_data = 0;
            } else {
                rtU.data_available = true;
            }
        }

        for (int i = 0; i < NUM_OF_LEDS; i++) {
            digitalWrite(SLAVE_LEDS[i], rtY.leds[i]);
            rtU.button[i] = digitalRead(SLAVE_BUTTONS[i]);
        }

        if (rtY.log_type == LOG_REPLY) {
            Serial3.print("Reply '");
            Serial3.print(rtY.log_data[0], BIN);
            Serial3.println("' to master request");
        } else if (rtY.log_type == LOG_RECEIVE) {
            Serial3.print("Receive '");
            Serial3.print(rtY.log_data[1], BIN);
            Serial3.print("' for register ");
            Serial3.println(rtY.log_data[0]);
        } else if (rtY.log_type == LOG_BUTTON_PRESSED) {
            Serial3.print("Button ");
            Serial3.print(rtY.log_data[0] + 1);
            Serial3.println(" Pressed");
        }
    }
}
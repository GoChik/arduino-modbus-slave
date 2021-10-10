#include "config.h"

#include <Arduino.h>
#include <ArduinoModbus.h>
#include <RS485.h>

#include <math.h>

namespace {
    const int DEBUG_LED = LED_BUILTIN;

    const uint8_t MON = OUTPUT_INVERTED ? LOW : HIGH;
    const uint8_t MOFF = OUTPUT_INVERTED ? HIGH : LOW;

    ModbusRTUServerClass server;
    RS485Class serial_port{Serial, TX_PIN, OUTPUT_ENABLE_PIN, RECEIVER_ENABLE_PIN};
}

void blink_error() {
    for (int i = 0; i < 2; ++i) {
        if (i > 0)
            delay(200);

        digitalWrite(DEBUG_LED, HIGH);
        delay(200);
        digitalWrite(DEBUG_LED, LOW);
    }
}

void setup() {
    // init debug led
    pinMode(DEBUG_LED, OUTPUT);

    Serial.begin(BAUDRATE);
    server = {serial_port};
    server.begin(SLAVE_ID, BAUDRATE);
    server.configureHoldingRegisters(0x00, 2);

    // configure OUTPUTS on register 0
    for (auto o : OUTPUTS) {
        pinMode(o, OUTPUT);
        digitalWrite(o, MON);
    }

    // configure INPUTS on register 1
    // for (auto i : INPUTS) {
    //     pinMode(i, INPUT);
    // }
}

void loop() {
    const uint16_t register_value = server.holdingRegisterRead(0x00);

    if (register_value < 0) {
        blink_error();
        return;
    }

    for(int i = 0; i < OUTPUTS_SIZE; ++i) {
        const auto value = register_value & (0x0001 << i);
        if (value) {
            digitalWrite(OUTPUTS[i], MON);
        } else {
            digitalWrite(OUTPUTS[i], MOFF);
        }
    }

    uint16_t input_registers = 0x0000;
    for (auto i : INPUTS) {
        const uint8_t value = digitalRead(i);
        input_registers = input_registers | (value << i);
    }

    server.holdingRegisterWrite(0x01, input_registers);
}

void serialEventRun() {
    // poll for Modbus RTU requests
    server.poll();
}
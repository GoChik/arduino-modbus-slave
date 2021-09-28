#include "config.h"

#include <Arduino.h>
#include <ArduinoModbus.h>
#include <RS485.h>

#include <math.h>


int main(void) {
    Serial.begin(BAUDRATE);

    RS485Class serial_port(Serial, TX_PIN, OUTPUT_ENABLE_PIN, RECEIVER_ENABLE_PIN);
    ModbusRTUServerClass server(serial_port);
    server.begin(SLAVE_ID, BAUDRATE);

    // configure OUTPUTS on register 0
    for (auto i : OUTPUTS) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
    server.configureHoldingRegisters(0x00, 1);

    // configure INPUTS on register 1
    for (auto i : INPUTS) {
        pinMode(i, INPUT);
    }
    server.configureHoldingRegisters(0x01, 1);


    while (true) {
        // poll for Modbus RTU requests
        if (Serial.available()) {
            server.poll();
        }

        const uint16_t register_value = server.holdingRegisterRead(0x00);

        for(auto i : OUTPUTS) {
            const auto value = register_value & (0x0001 << i);
            if (value) {
                digitalWrite(i, HIGH);
            } else {
                digitalWrite(i, LOW);
            }
        }

        uint16_t input_registers = 0x0000;
        for (auto i : INPUTS) {
            const uint8_t value = digitalRead(i);
            input_registers = input_registers & (value << i);
        }

        server.holdingRegisterWrite(0x01, input_registers);

    }
    return 0;
}
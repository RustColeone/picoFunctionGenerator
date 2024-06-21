#include "PWMController.h"

// Initialize two PWM controllers on different pins
// Every even number is a new slice A channel and the next odd number pin is on the same slice's B channel
// Each slice shares the sameclock divider and the counter, so their phase is completely identical
// But we can have a different trigger level and if it inverts or not
PWMController pwm1(14, 10000000.0, 10, 0.0, false, 4); // Pin 15, 1 MHz, 43.75% duty cycle, no phase delay, 4-bit resolution
PWMController pwm2(15, 10000000.0, 50, 0.0, true, 4); // Pin 16, 1 MHz, 43.75% duty cycle, no phase delay, 4-bit resolution

void setup() {
    set_sys_clock_khz(200000, true);  // Overclock to 200 MHz
    //Serial.begin(115200);
    //while (!Serial) { delay(10); }  // Wait for Serial Monitor to open

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("PWM Controller Ready.");
    Serial.println("Send commands in the format: d:<dutyCycle> p:<phaseDelay>");

	PWMController::enableAll();
}

void loop() {
    return;
    // Check for serial input
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.startsWith("d1:")) {
            float newDutyCycle = input.substring(3).toFloat();
            pwm1.setDutyCycle(newDutyCycle);
            Serial.print("PWM1 duty cycle set to: ");
            Serial.println(newDutyCycle);
        } else if (input.startsWith("p1:")) {
            float newPhaseDelay = input.substring(3).toFloat();
            pwm1.setPhaseDelay(newPhaseDelay);
            Serial.print("PWM1 phase delay set to: ");
            Serial.println(newPhaseDelay);
        } else if (input.startsWith("d2:")) {
            float newDutyCycle = input.substring(3).toFloat();
            pwm2.setDutyCycle(newDutyCycle);
            Serial.print("PWM2 duty cycle set to: ");
            Serial.println(newDutyCycle);
        } else if (input.startsWith("p2:")) {
            float newPhaseDelay = input.substring(3).toFloat();
            pwm2.setPhaseDelay(newPhaseDelay);
            Serial.print("PWM2 phase delay set to: ");
            Serial.println(newPhaseDelay);
        } else if (input.equals("enableAll")) {
            PWMController::enableAll();
            Serial.println("All PWM channels enabled.");
        } else if (input.equals("disableAll")) {
            PWMController::disableAll();
            Serial.println("All PWM channels disabled.");
        } else {
            Serial.println("Invalid command. Use 'd1:<dutyCycle>' or 'd2:<dutyCycle>' to set duty cycle, 'p1:<phaseDelay>' or 'p2:<phaseDelay>' to set phase delay.");
        }
    }
}

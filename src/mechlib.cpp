#include "main.h"

bool shoot = false;

void catapultPID(void *ignore) {
    Motor catapult (catapultPort, MOTOR_GEAR_RED, true, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort, false);
    Controller master (CONTROLLER_MASTER);
    ADILightSensor lightSensor(lightSensorPort);

    int targ = 0, pos, error, deriv, prevError = 0;

    while (true) {
        if (shoot){ //PID
            catapult.move(-127);
            delay(500);
            shoot = false;
        } else if (master.get_digital(DIGITAL_R2) && lightSensor.get_value()<700 && error > -300){ //autofire
            printf("autofire\n");
            delay(1000);
            catapult.move(-127);
            delay(500);
        } else if (master.get_digital(DIGITAL_A)){ //manual
            catapult.move(-80);
            targ = rotation.get_position();
        }

        pos = rotation.get_position();

        error = targ - pos;

        deriv = error - prevError;

        catapult.move(error*kp + deriv*kd);
        printf("%d %f\n", error, (error*kp + deriv*kd));

        prevError = error;
    }
}

void fire(){
    shoot = true;
    printf("fired\n");
}

// Note: This is some voodoo magic shit i dont understand how tf it works now
// smth gets fucked when u reverse the rotation sensor so i reversed the motor
// instead which means that the move values are reverse
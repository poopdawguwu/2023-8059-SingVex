#include "main.h"

void catapultPID(void *ignore) {
    Motor catapult(catapultPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
    Rotation rotation(rotationPort, false);
    Controller master(E_CONTROLLER_MASTER);
    ADILightSensor lightSensor(lightSensorPort);

    double targ = 0, pos, error, deriv, prevError = 0;

    while (true) {
        if (shoot || (lightSensor.get_value()<1750 && master.get_digital(DIGITAL_DOWN))){
            catapult.move(127);
            delay(1000);
            shoot = false;
        }

        pos = rotation.get_position();

        error = targ - pos;

        deriv = error - prevError;

        catapult.move(error*kp + deriv*kd);

        prevError = error;
    }
}

void fire(){
    shoot = true;
}
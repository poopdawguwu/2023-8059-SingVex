#include "main.h"

bool shoot = false;

void catapultPID(void *ignore) {
    Motor catapult (catapultPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
    Rotation rotation (rotationPort, true);
    Controller master (E_CONTROLLER_MASTER);
    ADILightSensor lightSensor(lightSensorPort);

    double targ = 0, pos, error, deriv, prevError = 0;

    while (true) {
        // if (shoot || (lightSensor.get_value()<1750 && master.get_digital(DIGITAL_X))){
        //     catapult.move(127);
        //     delay(1000);
        //     shoot = false;
        // } else if (master.get_digital(DIGITAL_Y)){
        //     catapult.move(80);
        //     targ = rotation.get_position();
        // }

        // pos = rotation.get_position();

        // error = targ - pos;

        // deriv = error - prevError;

        // catapult.move(error*kp + deriv*kd);

        // prevError = error;
        if (master.get_digital(DIGITAL_Y)){
            catapult.move(80);
            targ = rotation.get_position();
        } else {
            catapult.move(0);
        }
    }
}

void fire(){
    shoot = true;
}
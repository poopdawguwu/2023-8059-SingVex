#include "main.h"

void armPID(void *ignore) {
    Motor armLeft (armLeftPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
    Motor armRight (armRightPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
    
    Controller master(E_CONTROLLER_MASTER);

    double targ = 0, posLeft, posRight, errorLeft, errorRight, derivLeft, derivRight, prevErrorLeft = 0, prevErrorRight = 0;

    while (true) {
        if (master.get_digital(DIGITAL_L1)){
            targ++;
        } else if (master.get_digital(DIGITAL_L2)){
            targ--;
        }

        posLeft = armLeft.get_position();
        posRight = armRight.get_position();

        errorLeft = targ - posLeft;
        errorRight = targ - posRight;

        derivLeft = errorLeft - prevErrorLeft;
        derivRight = errorRight - prevErrorRight;

        armLeft.move(errorLeft*kp + derivLeft*kd);
        armRight.move(errorRight*kp + derivRight*kd);

        prevErrorLeft = errorLeft;
        prevErrorRight = errorRight;
    }
}
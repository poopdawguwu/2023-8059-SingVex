#include "main.h"

bool shoot = false;

void catapultPID(void *ignore) {
    Motor catapult (catapultPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
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
        } else if (master.get_digital(DIGITAL_Y)){ //manual
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

double targ = 0;

void elevPID(void *ignore){
	Motor elevLeft (elevLeftPort, MOTOR_GEAR_RED, true, MOTOR_ENCODER_DEGREES);
	Motor elevRight (elevRightPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
    Controller master (CONTROLLER_MASTER);

	elevLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	elevRight.set_brake_mode(MOTOR_BRAKE_HOLD);

    elevLeft.tare_position();
    elevRight.tare_position();

    double posLeft, posRight, errorLeft, errorRight, derivLeft, derivRight, prevErrorLeft = 0, prevErrorRight = 0;

    while (true){
        if (master.get_digital(DIGITAL_UP)){
            targ += .05;
        } else if (master.get_digital(DIGITAL_DOWN)){
            targ -= .05;
        }

        posLeft = elevLeft.get_position();
        posRight = elevRight.get_position();

        errorLeft = targ - posLeft;
        errorRight = targ - posRight;

        derivLeft = errorLeft - prevErrorLeft;
        derivRight = errorRight - prevErrorRight;

        elevLeft.move(errorLeft*kp + derivLeft*kd);
        elevRight.move(errorRight*kp + derivRight*kd);

        prevErrorLeft = errorLeft;
        prevErrorRight = errorRight;
    }
}

void setTarg(int a){
    targ = a;
}
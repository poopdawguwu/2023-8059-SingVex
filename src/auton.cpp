#include "main.h"
#include "pros/motors.h"
#define akm 0 //Degrees of wheel turn to 1 inch
#define akt 0 //Degrees of wheel turn to 1 degree base turn


double targLeft = 0, targRight = 0, errorLeft, errorRight, derivLeft, derivRight, prevErrorLeft = 0, prevErrorRight = 0, leftSpeed, rightSpeed;
bool err = false;

void autonPID(void *ignore) {
	Motor leftFront (leftFrontPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor leftBack (leftBackPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor rightFront (rightFrontPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
	Motor rightBack (rightBackPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);


    leftFront.tare_position();
    leftBack.tare_position();
    rightFront.tare_position();
    rightBack.tare_position();

    while (true) {
        errorLeft = targLeft - leftFront.get_position(); //Get errors
        errorRight = targRight - rightFront.get_position();

        derivLeft = errorLeft - prevErrorLeft;
        derivRight = errorRight - prevErrorRight;

        leftSpeed = errorLeft*akp + derivLeft*akd;
        rightSpeed = errorRight*akp + derivRight*akd;

        leftFront.move(leftSpeed);
        leftBack.move(leftSpeed);
        rightFront.move(rightSpeed);
        rightBack.move(rightSpeed);

        err = fabs(targLeft)>15 && fabs(targRight)>15;
    
        printf("A   Left: %f %f    Right: %f %f    targReach: %s\n", errorLeft, leftSpeed, errorRight, rightSpeed, (err)? "true" : "false");
        delay(15);
    }
}

void move(double inches){
    printf("moving %f", inches);
    targLeft += inches*akm;
    targRight += inches*akm;

    while (err) {
        printf("moving");
        delay(20);
    }
}

void turn(float degrees){
    printf("turning %f", degrees);
    targLeft += degrees*akm;
    targRight += degrees*akm;

    while (err) {
        printf("turning");
        delay(20);
    }
}

void calibration(){
    Task autonPIDTask (autonPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");

    move(24);
    turn(90);

    autonPIDTask.remove();
}

void path1(){
    Task autonPIDTask (autonPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    autonPIDTask.remove();
}

void path2(){}
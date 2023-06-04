#include "main.h"
#define akm 17.62947066 //Degrees of motor turn to 1 inch
#define akt 2.5 //Degrees of motor turn to 1 degree base turn

Motor leftFront (leftFrontPort, MOTOR_GEAR_BLUE, false, MOTOR_ENCODER_DEGREES);
Motor leftBack (leftBackPort, MOTOR_GEAR_BLUE, false, MOTOR_ENCODER_DEGREES);
Motor rightFront (rightFrontPort, MOTOR_GEAR_BLUE, true, MOTOR_ENCODER_DEGREES);
Motor rightBack (rightBackPort, MOTOR_GEAR_BLUE, true, MOTOR_ENCODER_DEGREES);
Motor elevLeft (elevLeftPort, MOTOR_GEAR_RED, true, MOTOR_ENCODER_DEGREES);
Motor elevRight (elevRightPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);

double targLeft = 0, targRight = 0, errorLeft, errorRight, derivLeft, derivRight, prevErrorLeft = 0, prevErrorRight = 0, leftSpeed, rightSpeed;
bool err = false, breaker = true, limiter = true;

void autonPID(void *ignore) {
    leftBack.tare_position();
    rightBack.tare_position();

    while (breaker) {
        errorLeft = targLeft - leftBack.get_position(); //Get errors
        errorRight = targRight - rightBack.get_position();

        derivLeft = errorLeft - prevErrorLeft;
        derivRight = errorRight - prevErrorRight;

        leftSpeed = errorLeft*akp + derivLeft*akd;
        rightSpeed = errorRight*akp + derivRight*akd;

        if (limiter){
            if (leftSpeed>60){
                leftSpeed = 60;
            }
            if (rightSpeed>60){
                rightSpeed = 60;
            }
        }

        leftFront.move(leftSpeed);
        leftBack.move(leftSpeed);
        rightFront.move(rightSpeed);
        rightBack.move(rightSpeed);

        err = fabs(targLeft)>30 && fabs(targRight)>30;
    
        printf("A   Left: %f %f    Right: %f %f    targReach: %s\n", errorLeft, leftSpeed, errorRight, rightSpeed, (err)? "true" : "false");
        delay(15);
    }
}

/**
 * Moves the robot forward
 * \param inches: How far to move forward in inches
 * \param time = 0: Set max time for moving, set value to 0 for the robot to move until target is reached
 */
void move(double inches, int time = 0){
    printf("moving %f", inches);
    targLeft += inches*akm;
    targRight += inches*akm;

    if (time){
        // delay(time);
    } else {
        while (err) {
            printf("moving");
            delay(20);
        }
    }
    delay(1000);
}


/**
 * Turns the robot clockwise
 * \param degrees: How much to turn the robot in degrees
 * \param time = 0: Set max time for moving, set value to 0 for the robot to move until target is reached
 */
void turn(double degrees, int time = 0){
    printf("turning %f", degrees);
    targLeft -= degrees*akt;
    targRight += degrees*akt;

    delay(time);
}

void calibration(){
    Task autonPIDTask(autonPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");

    setTarg(1200);

    breaker = false;
    autonPIDTask.remove();
}

void path1(){
    Task autonPIDTask(autonPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "autonPIDTask");
    
    move(24);
    turn(-45, 2000);
    limiter = false;
    move(30, 1);
    limiter = true;
    move(-30, 1);
    turn(45, 2000);
    move(-24);
    turn(45, 2000);

    breaker = false;
    autonPIDTask.remove();
    
    setTarg(840);
    leftFront.move(-30);
    leftBack.move(-30);
    rightFront.move(-30);
    rightBack.move(-30);
}

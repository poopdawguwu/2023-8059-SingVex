#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Motor leftFront (leftFrontPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor leftBack (leftBackPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Motor rightFront (rightFrontPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
	Motor rightBack (rightBackPort, MOTOR_GEAR_GREEN, true, MOTOR_ENCODER_DEGREES);
    Motor catapult (catapultPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
	Motor elevLeft (elevLeftPort, MOTOR_GEAR_RED, true, MOTOR_ENCODER_DEGREES);
	Motor elevRight (elevRightPort, MOTOR_GEAR_RED, false, MOTOR_ENCODER_DEGREES);
	Motor claw (clawPort, MOTOR_GEAR_GREEN, false, MOTOR_ENCODER_DEGREES);
	Task catapultPIDTask (catapultPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "armPIDTask");
	Task elevPIDTask (elevPID, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "elevPIDTask");

	elevLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	elevRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	claw.set_brake_mode(MOTOR_BRAKE_HOLD);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	path1();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Motor leftFront (leftFrontPort, true);
	Motor leftBack (leftBackPort, true);
	Motor rightFront (rightFrontPort, false);
	Motor rightBack (rightBackPort, false);
	Motor claw (clawPort, false);
	Controller master (CONTROLLER_MASTER);

	bool invert = false;
	double left = 0, right =0;

	while (true) {
		if (invert){
			leftFront.move(-master.get_analog(ANALOG_RIGHT_Y)); //Base controls
			leftBack.move(-master.get_analog(ANALOG_RIGHT_Y));
			rightFront.move(-master.get_analog(ANALOG_LEFT_Y));
			rightBack.move(-master.get_analog(ANALOG_LEFT_Y));
		} else {
			leftFront.move(master.get_analog(ANALOG_LEFT_Y)); //Base controls
			leftBack.move(master.get_analog(ANALOG_LEFT_Y));
			rightFront.move(master.get_analog(ANALOG_RIGHT_Y));
			rightBack.move(master.get_analog(ANALOG_RIGHT_Y));
		}

		if (master.get_digital_new_press(DIGITAL_R1)){
			fire();
		}

		if (master.get_digital(DIGITAL_L1)){
			claw.move(127);
		} else if (master.get_digital(DIGITAL_L2)){
			claw.move(-127);
		} else {
			claw.move(0);
		}

		if (master.get_digital_new_press(DIGITAL_B)){
			invert = !invert;
		}

		delay(20);
	}
}

#pragma config(Sensor, dgtl1,  RightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl8,  Sonar,          sensorSONAR_cm)
#pragma config(Motor,  port2,           RightMotor,     tmotorVex269_MC29, openLoop)
#pragma config(Motor,  port3,           LeftMotor,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           armMotor,      tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Sensor, in6,    potentiometer,  sensorPotentiometer)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//
//***********************************************************************************//
//                                                                                   //
// This program makes the recbot functioning as a remotely controlled toy with       //
// limited intelligence. It is used to in the second class after the club members    //
// successfully assemble their recbot                                                //
//                                                                                   //
// Function description                                                              //
//                                                                                   //
//  1. Joystick channel 3 controls left drive motor                                  //
//  2. Joystick channel 2 controls right drive motor                                 //
//  3. Pushing Joystick button 5U or 5D will rise or lower the arm, respectively     //
//  4. Potentiometer reading is used to control the moving range of the arm          //
//     The proper thresholding values should be determined for each robot with       //
//     field measurement. Due to the effect of gravity, it should leave more margin  //
//     for the potentiometer threshold value corresponding to the lower limiting     //
//     position.
//  5. If the arm touches the limit switch, the arm will stop moving down and        //
//     move up a little to release the limit switch                                  //
//  6. If the sonar finds that an obstacle is in front of the robot within 25 cm,    //
//     the robot will stop moving forward and instead moving back a little.          //
//                                                                                   //
//  Last modified on 5/23/2017                                                       //
//                                                                                   //
//***********************************************************************************//


task main()
{

	int dist;
  int pos;
	int armSpeed;

	int dist_thres;
	dist_thres = 25;

	int pos_low;
	pos_low = 50;

	int pos_high;
	pos_high = 1900;

	while (true) {                                  // make the microconrolled repeatly doing the following tasks
	  dist = SensorValue(Sonar);                    // Measure the distance to obstacle, if any (if not, return -1)

		if (dist<dist_thres && SensorValue(potentiometer)>pos_low)
		{	                                            // if obstacle disstance < 25 cm and arm is not blocking the sona
			SensorValue[RightEncoder]=0;                // reset the encoder counter beforing using them
			motor[LeftMotor]=-30;                       // assign negative motor speed to make robot backing up
			motor[RightMotor]=-30;
			while (SensorValue[RightEncoder]>-720) {}     // wait until the wheel backs two rounds
			motor[LeftMotor]=0;                         // stop the motors
			motor[RightMotor]=0;
		}
		else {
			motor[LeftMotor]=vexRT[Ch3];                // use joystick to control motor speeds
			motor[RightMotor]=vexRT[Ch2];
		}

		armSpeed = (vexRT[Btn5U]  - vexRT[Btn5D]) * 40;  // determine arm motor speed according to joystick 5U & 5D buttons
		pos = SensorValue(potentiometer);                // find the arm position
		if (pos<pos_low) {
		  if (pos<pos_low-100) {                     // if the arm is going to touch the ground, move it up a little
		    motor[armMotor]=30;
		    wait1Msec(20);
		    armSpeed=0;
	    }
		  if (armSpeed<0) {
				armSpeed = 0;                           // if arm position is too low, make sure it will not move down further
			}
		}
		else {
			if (pos>pos_high) {
				if (armSpeed>0) {
					armSpeed = 0;                         // if arm position is too high, make sure it will not move up further
				}
			}
		}

	motor[armMotor]=armSpeed;                     // put arm motor into action
 }
}

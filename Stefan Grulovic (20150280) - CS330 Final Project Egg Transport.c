#pragma config(Sensor, S2,     light_sensor,         sensorEV3_Color)
#pragma config(Sensor, S3,     sonar_sensor,         sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     color_sensor,         sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor,  motorB,          motor_left,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          motor_right,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorA,          claw,          tmotorEV3_Large, PIDControl, encoder)
////////////////////////////////////////////////////////////////////////////////
// Stefan Grulovic (20150280) - CS330 Final Project
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//VARIABLES
////////////////////////////////////////////////////////////////////////////////
//basic storage/constant
float light_value = 0;
float current_distance = 0;
int motor_speed = 30; //speed used throught the program for the speed of the robot

//line follow case 1,2
int light_treshold = 45;

//line follow case 3
int black_treshold = 78;
int white_treshold = 8;
int scaling_factor = 1;


////////////////////////////////////////////////////////////////////////////////
//BASIC MOVEMENT
////////////////////////////////////////////////////////////////////////////////

//stop motors for a specific period of time
void stop_motors(float time){
	clearTimer(T1);

	while(time1[T1] < time*1000){
		motor[motor_left] = 0;
		motor[motor_right] = 0;
	}
}

//go forward for a specif period of time
void forward(float time){
	clearTimer(T1);

	while(time1[T1] < time*1000){
		motor[motor_left] = motor_speed;
		motor[motor_right] = motor_speed;
	}
		motor[motor_left] = 0;
		motor[motor_right] = 0;
}

//go back for a specific period of time
void back(float time){
	clearTimer(T1);

	while(time1[T1] < time*1000){
		motor[motor_left] = -motor_speed + 20;
		motor[motor_right] = -motor_speed + 20;
	}
		motor[motor_left] = 0;
		motor[motor_right] = 0;

}

//go left for a specific period of time
void left(float time){
	motor[motor_left] = motor_speed;
	motor[motor_right] = -motor_speed;
	wait1Msec(time);
	motor[motor_left] = 0;
	motor[motor_right] = 0;
}

//go right for the specific period of time
void right(float time){
	motor[motor_left] = -motor_speed;
	motor[motor_right] = motor_speed;
	wait1Msec(time);
	motor[motor_left] = 0;
	motor[motor_right] = 0;
}

////////////////////////////////////////////////////////////////////////////////
//LINE FOLLOWING
////////////////////////////////////////////////////////////////////////////////

//line following function
void follow_line(int choice){
	light_value = SensorValue(light_sensor);
	//displayCenteredBigTextLine(4, "Dist: %3d cm", light_value);

	//line following option 1
	if(choice == 1){

		if(light_value < light_treshold){
			motor[motor_left] = 0;
			motor[motor_right] = motor_speed;
		}else{
			motor[motor_left] = motor_speed;
			motor[motor_right] = 0;
		}
	//line following option 2
	}else if(choice == 0){

		if(light_value < light_treshold){
			motor[motor_left] = motor_speed;
			motor[motor_right] = 0;
		}else{
			motor[motor_left] = 0;
			motor[motor_right] = motor_speed;
		}
	//line following option 3
	}else if(choice == 3){

		motor[motor_left] = (black_treshold - light_value) * scaling_factor - 12.5;
		motor[motor_right] = (light_value - white_treshold) * scaling_factor - 12.5;
	}

}

//line following until the ultrasonic sensors sees something
void follow_until_distance(float distance, int choice){
	current_distance = SensorValue[sonar_sensor];

	while(current_distance > distance){
		current_distance = SensorValue[sonar_sensor];

		follow_line(choice);
	}
}

//line following for a specif period of time
void follow_until_time(float time, int choice){
	clearTimer(T1);

	while(time1[T1] < time*1000){
		follow_line(choice);
	}
}

//open the claw
void open_claw(){
	motor[claw] = 60;
	wait1Msec(2875);
	motor[claw] = 0;
}
//close the claw
void close_claw(){
	motor[claw] = -60;
	wait1Msec(2875);
	motor[claw] = 0;
}

//check left right for an object which is in peimiter of 20cm
bool check_object(){
		//ultrasonic sesnor
		current_distance = SensorValue[sonar_sensor];
		displayCenteredBigTextLine(4, "DIS: %3d", current_distance);

		//if in front
		if(current_distance < 20){
			return true;
		}

		//turn left and check
		left(80);
		current_distance = SensorValue[sonar_sensor];
		displayCenteredBigTextLine(4, "DIS: %3d", current_distance);
		stop_motors(1);
		if(current_distance < 20 || current_distance>250){
			return true;
		}

		//turn more left and check
		left(80);
		current_distance = SensorValue[sonar_sensor];
		stop_motors(1);
		if(current_distance < 20 || current_distance>250){
			return true;
		}

		//turn back on the right and check
		right(160);
		current_distance = SensorValue[sonar_sensor];
		displayCenteredBigTextLine(4, "DIS: %3d", current_distance);
		stop_motors(1);
		if(current_distance < 20 || current_distance>250){
			return true;
		}

		//turn more right
		right(80);
		current_distance = SensorValue[sonar_sensor];
		displayCenteredBigTextLine(4, "DIS: %3d", current_distance);
		stop_motors(1);
		if(current_distance < 20 || current_distance>250){
			return true;
		}

		//return false in case the robot didnt find any object
		return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//MAIN
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
task main(){
	int count = 1;
	int skip = 0;

	//Display for the point/stage where the robot is and what it needs to preform
	displayCenteredBigTextLine(4, "POINT: %3d", count);

	//START
	//follow_until_time(10,3);
	follow_until_distance(18,3);
	left(470);
	count++; //increment point each time if preforms a taks
	displayCenteredBigTextLine(4, "POINT: %3d", count);

	//POINT 2 -> basic turn left
	follow_until_time(6,3);
	follow_until_distance(59,3);
	left(470);
	count++;
	displayCenteredBigTextLine(4, "POINT: %3d", count);

	//POINT 3 ->basic turn left
	follow_until_distance(13,3);
	left(470);
	count++;
	displayCenteredBigTextLine(4, "POINT: %3d", count);


						//STOP 1 -> possible place of the egg
						follow_until_time(3.08,3);
						stop_motors(1);
						left(690);
						stop_motors(1);
						follow_until_time(0.5,3);
						back(0.80);
						//checking the object
						if(check_object()){
							back(0.3);
							right(690);
							right(760);
							right(150);
							back(0.3);
							close_claw(); //taking the egg
							stop_motors(1);
							back(0.95);
							left(730);
							forward(0.65);

							skip = 1;//and update skip to one in order to skip later checks as the egg has been grabbed
						//otherwise turn aroung and continue
						}else{
							forward(0.2);
							right(680);
						}
						///////////////////////////////////////////////

	//POINT 4 ->basic turn left
	follow_until_time(6,3);
	follow_until_distance(66.5,3);
	left(470);
	count++;
	displayCenteredBigTextLine(4, "POINT: %3d", count);

					//STOP 2 -> possible egg position 2 ->actions same as before -> numbers slightly different
					if(skip!=1){ // if object has been taken dont check
						follow_until_time(3.09,3);
						stop_motors(1);
						left(690);
						stop_motors(1);
						follow_until_time(0.5,3);
						back(0.85);
						if(check_object()){
							back(0.3);
							right(690);
							right(760);
							right(150);
							back(0.3);
							close_claw();
							stop_motors(1);
							back(0.95);
							left(730);
							forward(0.4);

							skip = 1;
						}else{
							forward(0.2);
							right(680);
						}
					}
					//////////////////////////////////////////////

	//POINT 5 -> basic turn left
	follow_until_distance(17.5,3);
	left(470);
	count++;
	displayCenteredBigTextLine(4, "POINT: %3d", count);

					//STOP 3 -> possible egg position 3 -> same as before
					if(skip!=1){
						follow_until_time(3.09,3);
						stop_motors(1);
						left(690);
						stop_motors(1);
						follow_until_time(0.5,3);
						back(0.85);
						if(check_object()){
							back(0.3);
							right(690);
							right(760);
							right(150);
							back(0.3);
							close_claw();
							stop_motors(1);
							back(1.025);
							left(730);
							forward(0.45);

							skip = 1;
						}else{
							forward(0.2);
							right(680);
						}
					}
					///////////////////////////////////////////////

	//POINT 6 -> go forward until it reaches end
	follow_until_time(2,3);
	follow_until_distance(59,3);
	count++;
	forward(3);
	displayCenteredBigTextLine(4, "POINT: %3d", count);
	stop_motors(1);
	open_claw();

	/*
		//checking loop for the ultrasonic sensor -> due to its sensitivity
		while(true){
			current_distance = SensorValue[sonar_sensor];
			displayCenteredBigTextLine(4, "Dist: %3d cm", current_distance);
		}
	*/
}

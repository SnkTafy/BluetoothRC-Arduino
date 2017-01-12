// Pins connected to stepper
#define IN1  7
#define IN2  8
#define IN3  9
#define IN4  10

int stepCounter = 0;
int prevStep = 0;

/*In order to have one step all the cases in the swithch
 *must be execute.
 *When the cases are executing from 0 to 7 we have clockwise step
 *and when cases are executing from 7 to 0 we have anticlockwise step ;)
*/
void Step(int whichCase){
	switch(whichCase){
		case 0:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			delayMicroseconds(1300);
		break;
		case 1:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, HIGH);
			delayMicroseconds(1300);
		break;
		case 2:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
			delayMicroseconds(1300);
		break;
		case 3:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
			delayMicroseconds(1300);
		break;
		case 4:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			delayMicroseconds(1300);
		break;
		case 5:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			delayMicroseconds(1300);
		break;
		case 6:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			delayMicroseconds(1300);
		break;
		case 7:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			delayMicroseconds(1300);
		break;
		default:
			digitalWrite(IN1, LOW); 
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
		break;
	}
}

//Anticlockwise step
void TurnLeft(){
	//Execute cases from 7 to 0
	for(int whichCase=0; whichCase<9; whichCase++){
		Step(7-whichCase);
	}
}

// Clockwise Step
void TurnRight(){
	//Execute cases from 0 to 7
	for(int whichCase=0; whichCase<9; whichCase++){
		Step(whichCase);
	}
}

void InitializeStepper(int turnValue){
	while(stepCounter != turnValue){
		if(turnValue > stepCounter){
			TurnRight();
			prevStep = stepCounter;
			stepCounter++;
		}else if(turnValue < stepCounter){
			TurnLeft();
			prevStep = stepCounter;
			stepCounter--;
		}
	}
	
	stepCounter = turnValue;
}

void TurnStepper(int turnValue){
	//If the value is positive turn right
	/*if((turnValue > prevStep)){
		stepCounter++;
		TurnRight();
		
	//If turnValue is negative turn left
	}else if((turnValue < prevStep)){
		stepCounter--;
		TurnLeft();
	}*/
	
	if(turnValue != stepCounter){
		InitializeStepper(turnValue);
	}
	prevStep = turnValue;
}
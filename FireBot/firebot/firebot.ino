#define servoMotor D3
#define waterPump D4
#define motor D1

#define smokeSensor  A0
#define sonarSensorTriggerPin  D2
#define sonarSensorEchoPin  D5

#define flameSensor1 D6
#define flameSensor2 D7

#define countFireNutralToLimit 5

#include <Servo.h>

//define variables
bool initiateOperationStatus, programmeFlag, initiateMotorStatus;
int countFireNutral;
Servo servo;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
  pinMode(servoMotor, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(sonarSensorTriggerPin, OUTPUT);

  pinMode(flameSensor1, INPUT);
  pinMode(flameSensor2, INPUT);

  pinMode(smokeSensor, INPUT);
  pinMode(sonarSensorEchoPin, INPUT);
  
  initiateOperationStatus = true;
  programmeFlag = true;
  initiateMotorStatus = true;
  countFireNutral = 0;

  servo.attach(servoMotor);
  servo.write(270);
  digitalWrite(waterPump,HIGH);

  Serial.begin(9600);
}

int getSonar(){
  //clears the triggerpin
  digitalWrite(sonarSensorTriggerPin, LOW);
  delay(1);

  //sets the trigger pin on for 10 ms
  digitalWrite(sonarSensorTriggerPin, HIGH);
  delay(10);
  digitalWrite(sonarSensorTriggerPin, LOW);

  //reads the ecopin , returns the sound wav traveltime in ms
  long duration = pulseIn(sonarSensorEchoPin,HIGH);

  //calc distannce 
  int distance = duration*0.034/2;
  // s = v*t // in the up -> (t*velocity of sound 0.034meter per milisecond)/2 as covers double distance

  //printing the distance
  Serial.print("Distance: ");
  Serial.println(distance);

  delay(100);
  return distance;
}


int countFlame(){
  int f1=0, f2=0;

  f1 = digitalRead(flameSensor1);
  // f2 = digitalRead(flameSensor2);

  Serial.print("f1 ");
  Serial.println(f1);
  // Serial.print(" f2 ");
  // Serial.println(f2);

  // Serial.println(f1 && f2);

  return f1;
}

int getSmoke(){
  int s = analogRead(smokeSensor);
  return s;
}

void executeServoMotor(int i){
  servo.write(i);  
}

void runMotor(int x){
  if(x==1){
    digitalWrite(motor, HIGH);
  } 
  else{
    digitalWrite(motor, LOW);
  }
}

void runPump(int x){
  if(x==1){
    digitalWrite(waterPump, HIGH);
  } 
  else{
    digitalWrite(waterPump, LOW);
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  if(programmeFlag){
    int x = countFlame();
    delay(500);

    if(initiateMotorStatus && x==1){
      initiateMotorStatus = false;
      delay(3000);
      runMotor(0); //MOTOR START
    }
    
    if(x==0){
      // FIRE DETECTED
      runMotor(1); //MOTOR STOP
    }
    
    if(x==0 && initiateOperationStatus){
      initiateOperationStatus = false;
      //servo + pump
      Serial.println("Execution Start");
      delay(2500);
      executeServoMotor(180);

      delay(1500);

      runPump(0);

      delay(10000);
    }

    if(!initiateOperationStatus && x==1){
      countFireNutral++;
    }

    if(countFireNutral > 20){
      executeServoMotor(270);
      delay(1500);
      runPump(1);
      
      programmeFlag = false;
    }
  }

}

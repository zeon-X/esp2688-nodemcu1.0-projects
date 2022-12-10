#define tp  D5
#define ep  D6
#define o D1

//define variables
long duration;
int distance;
bool f ;

void setup() {
  // put your setup code here, to run once:
  pinMode(tp, OUTPUT);
  pinMode(o, OUTPUT);
  pinMode(ep, INPUT);
  f = false;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //clears the triggerpin
  digitalWrite(tp, LOW);
  delay(1);

  //sets the trigger pin on for 10 ms
  digitalWrite(tp, HIGH);
  delay(10);
  digitalWrite(tp, LOW);

  //reads the ecopin , returns the sound wav traveltime in ms
  duration = pulseIn(ep,HIGH);

  //calc distannce 
  distance = duration*0.034/2;
  // s = v*t // in the up -> (t*velocity of sound 0.034meter per milisecond)/2 as covers double distance

  //printing the distance
  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance<100) {
    f=true;
    Serial.println("On the bulb");
    Serial.println("light will remain on for 10 sec...");
    digitalWrite(o,LOW);
    delay(10000); 

  }
  else {
    if(f){
      Serial.println("bulb will be off in 3 sec...");
      delay(3000);
      digitalWrite(o,HIGH);
      f=false;
    }
    else {
      Serial.println("Off bulb");
      digitalWrite(o,HIGH);
    }
  }
  delay(100);


}

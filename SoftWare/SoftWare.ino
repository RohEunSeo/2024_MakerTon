#include <SoftwareSerial.h>

#define DETECT 7 // Laser sensor pin
#define trigPin 8 // Ultrasonic sensor trigger pin
#define echoPin 9 // Ultrasonic sensor echo pin
#define buttonPin 10 // Button pin

int speed = 6; // Motor PWM pin
int In1 = 5; // Motor control signal 1
int In2 = 4; // Motor control signal 2

SoftwareSerial HC06(2, 3); // Bluetooth TX/RX pins

const int threshold = 11;
const unsigned long durationThreshold = 5000;
unsigned long startTime = 0;
bool messageShown = false;
int buttonRead = 0;
int ButtonState = 0;


void setup() {
  Serial.begin(9600);
  HC06.begin(9600);

  //laser setup
  pinMode(DETECT, INPUT);

  // Motor setup
  pinMode(speed, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);

  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buttonPin, INPUT); // Use external pull-down resistor
}

void loop() 
{

  buttonRead = digitalRead(buttonPin);
  int detected = digitalRead(DETECT);
  if (buttonRead == HIGH){
    ButtonState = ButtonState + 1;
    Serial.println(ButtonState);
    delay(500);
  }

  int Buttontoggle = ButtonState % 2;
  if (Buttontoggle == 1){  
    delay(100);
  }
  else
  {
   // Laser sensor logic
    if (detected == HIGH) 
    {
      Serial.println("Laser Detected!");
      analogWrite(speed, 0); // Stop motor
      boolean buttonRead = digitalRead(buttonPin);

    } 
    else 
    {
      Serial.println("No Laser Detected");
      boolean buttonRead = digitalRead(buttonPin);
      motorRoutine();
    }
  }
  // Ultrasonic sensor logic
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= threshold) 
  {
    if (startTime == 0) 
    {
      startTime = millis();
    }
    if (millis() - startTime >= durationThreshold && !messageShown) 
    {
      Serial.println("감지");
      HC06.println("쓰레기가 다 찼습니다.");
      messageShown = true;
    }
  } 
  else 
  {
    startTime = 0;
    messageShown = false;
  }
  delay(200); // Adjust delay to balance sensor checks
}


void motorRoutine() 
{
  // Rotate motor in one direction for 2 seconds
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  analogWrite(speed, 255); // Motor speed (0-255)
  delay(300); // Rotate for 2 seconds

  // Stop motor for 1 second
  analogWrite(speed, 0);
  buttonRead = digitalRead(buttonPin);
  delay(1000); // Stop for 1 second

  // Rotate motor in opposite direction for 2 seconds
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  analogWrite(speed, 255); // Motor speed (0-255)
  buttonRead = digitalRead(buttonPin);
  delay(300); // Rotate in opposite direction for 2 seconds
  Serial.println("움직였다");

  // Stop motor
  analogWrite(speed, 0);
}

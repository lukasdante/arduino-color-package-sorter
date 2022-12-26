#include <Servo.h>

#define servoPin1 9
#define servoPin2 10
#define servoPin3 11
#define trigPin 7
#define echoPin 6
#define s0 2 
#define s1 3
#define s2 4
#define s3 5
#define sensorOut 12
#define DCmotor 8

Servo servo1;
Servo servo2;
Servo servo3;

long _duration;
int _distance;
int distance;

int frequencyRed, frequencyGreen, frequencyBlue;
int r_g, g_b, b_r;
int r_count = 0;
int g_count = 0;
int b_count = 0;
int color;

int sense_color();
int measure_distance();
void move_conveyor(int distance_away);
void push_object(Servo servo);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DCmotor, OUTPUT);

  // Setting the Color
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}


void loop() {
  distance = measure_distance();
  
  if (distance <= 7) {
    color = sense_color();
    
    switch (color) {
      case 1:
        move_conveyor(12);
        push_object(servo1);
        break;
      case 2:
        move_conveyor(18);
        push_object(servo2);
        break;
      case 3:
        move_conveyor(24);
        push_object(servo3);
        break;
    }
  }
  
  r_count = 0;
  g_count = 0;
  b_count = 0;
}

void push_object(Servo servo) {
  servo.write(180);
  delay(1000);
  servo.write(0);
}

int measure_distance(){
  // Measure the distance in front of the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(5);
  digitalWrite(trigPin, LOW);
  _duration = pulseIn(echoPin, HIGH);
  _distance = _duration*0.034/2; //lalabas ay distance in centimeters
  delay(200);
  return _distance;
}

void move_conveyor(int distance_away) {
  for (distance; distance < distance_away; ) {
    digitalWrite(DCmotor, HIGH);
    distance = measure_distance();
  }
  digitalWrite(DCmotor, LOW);
}

int sense_color(){
  // Sense Color Red
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  frequencyRed = pulseIn(sensorOut, LOW);
  delay(100);
  
  // Sense Color Green
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  frequencyGreen = pulseIn(sensorOut, LOW);
  delay(100);

  // Sense Color Blue
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  frequencyBlue = pulseIn(sensorOut, LOW);
  delay(100);

  r_g = frequencyRed - frequencyGreen;
  g_b = frequencyGreen - frequencyBlue;
  b_r = frequencyBlue - frequencyRed;

  if (r_g <= 5) {
      r_count++;
  } else {
      g_count++;
  }
  
  if (g_b <= 5) {
      g_count++;
  } else {
      b_count++;
  }
  
  if (b_r <= 5) {
      b_count++;
  } else {
      r_count++;
  }

  if (r_count > g_count && r_count > b_count) {
    return 1;
  } else if (g_count > r_count && g_count > b_count) {
    return 2;
  } else if (b_count > r_count && b_count > g_count) {
    return 3;
  } else {
    return 4;
  }
}
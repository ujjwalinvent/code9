#include <Servo.h>

#define PAN_MAX 180 //the maximum pan angle
#define PAN_MIN 0 //the minimum pan angle
#define TILT_MAX 140 //the maximum tilt angle
#define TILT_MIN 34 //the minimum tilt angle
#define FORWARD_SPEED 150
#define BACKWARD_SPEED 150
#define LEFT_SPEED 150
#define RIGHT_SPEED 150

int left_bit1 = 28;//22;
int left_bit2 = 26;//24;

int right_bit1 = 24;//26;
int right_bit2 = 22;//28;

int left_speed = 8;
int right_speed = 9;

int grip_bit1 = 3;
int grip_bit2 = 2;
int grip_speed = 4;



Servo leftServo; // left servo motor
Servo rightServo; // right servo motor
Servo centerServo; //center servo motor
int tilt_angle= 34; // current tilt angle
int pan_angle = 90; //current pan angle
int val=0;

void left_motor_forward(int s)
{
  digitalWrite(left_bit1,LOW);
  digitalWrite(left_bit2,HIGH);
  analogWrite(left_speed,s);
}

void left_motor_backward(int s)
{
  digitalWrite(left_bit1,HIGH);
  digitalWrite(left_bit2,LOW);
  analogWrite(left_speed,s);
}

void right_motor_forward(int s)
{
  digitalWrite(right_bit1,HIGH);
  digitalWrite(right_bit2,LOW);
  analogWrite(right_speed,s);
}

void right_motor_backward(int s)
{
  digitalWrite(right_bit1,LOW);
  digitalWrite(right_bit2,HIGH);
  analogWrite(right_speed,s);
}

void gripper_motor_forward(int s)
{
  digitalWrite(grip_bit1,HIGH);
  digitalWrite(grip_bit2,LOW);
  analogWrite(grip_speed,s);
}

void gripper_motor_backward(int s)
{
  digitalWrite(grip_bit1,LOW);
  digitalWrite(grip_bit2,HIGH);
  analogWrite(grip_speed,s);
}

void stop_bot()
{
  left_motor_forward(0);
  right_motor_forward(0);
}

void forward()
{
  left_motor_forward(FORWARD_SPEED);
  right_motor_forward(FORWARD_SPEED);
}

void backward()
{
  left_motor_backward(BACKWARD_SPEED);
  right_motor_backward(BACKWARD_SPEED);
}

void left()
{
  left_motor_backward(LEFT_SPEED);
  right_motor_forward(LEFT_SPEED);
}

void right()
{
  left_motor_forward(RIGHT_SPEED);
  right_motor_backward(RIGHT_SPEED);  
}

//TODO 
/*
left_90
 right_90
 left_180
 right_180
 */

void open_gripper()
{
  gripper_motor_forward(100);
  delay(500);
  gripper_motor_forward(0);
}

void close_gripper()
{
  gripper_motor_backward(100);
  delay(500);
  gripper_motor_forward(0);
}

void tilt_up()
{
  if(tilt_angle<TILT_MAX)
  {
    tilt_angle++;
    leftServo.write(tilt_angle); // rotate servo at speed given by 'tilt_angle'  
    rightServo.write(180-tilt_angle); // go in the opposite direction  
    delay(20);
  }
}

void tilt_down()
{
  if(tilt_angle>TILT_MIN)
  {
    tilt_angle--;
    leftServo.write(tilt_angle); // rotate servo at speed given by 'tilt_angle'  
    rightServo.write(180-tilt_angle); // go in the opposite direction  
    delay(20);
  }
}

void pan_left()
{
  if(pan_angle<PAN_MAX)
  { 
    pan_angle++;
    centerServo.write(pan_angle);
    delay(20);
  }
}

void pan_right()
{
  if(pan_angle>PAN_MIN)
  {
    pan_angle--;
    centerServo.write(pan_angle);
    delay(20);
  }
}

void reset_arm()
{
  for(;tilt_angle >= 34; tilt_angle -= 1) // goes from 90 to 180 degrees
  { 
    leftServo.write(tilt_angle); // rotate servo at speed given by 'angle'  
    rightServo.write(180-tilt_angle); // go in the opposite direction
    delay(20); // waits 20ms between servo commands
  }
  if(pan_angle>90)
  {
    for(;pan_angle>=90;pan_angle--)
    {  
      centerServo.write(pan_angle);
      delay(20);
    }
  }
  {
    for(;pan_angle<=90;pan_angle++)
    {
      centerServo.write(pan_angle);
      delay(20);
    }
  }
}


void setup()
{
  pinMode(left_bit1,OUTPUT);
  pinMode(left_bit2,OUTPUT);
  pinMode(right_bit1,OUTPUT);
  pinMode(right_bit2,OUTPUT);
  pinMode(left_speed,OUTPUT);
  pinMode(right_speed,OUTPUT);
  pinMode(grip_bit1,OUTPUT);
  pinMode(grip_bit2,OUTPUT);
  pinMode(grip_speed,OUTPUT);


  leftServo.attach(7); // attaches left servo on pin 7 to servo object
  rightServo.attach(6); // attaches right servo on pin 6 to servo object
  centerServo.attach(5); //attaches center servo on pin 5 to servo object
  leftServo.write(tilt_angle); // rotate servo at speed given by 'tilt_angle'  
  rightServo.write(180-tilt_angle); // go in the opposite direction
  centerServo.write(pan_angle);
  Serial.begin(9600);   

}

void loop()
{
  
  if(Serial.available())
  {
    val = Serial.read();
    //Serial.println(tilt_angle);
    switch(val)
    {
    case 0: stop_bot();  break;
    case 1: forward(); break;
    case 2: backward(); break;
    case 3: left(); break;
    case 4: right(); break;
    case 5: tilt_up();  break;
    case 6: tilt_down(); break;
    case 7: pan_left(); break;
    case 8: pan_right(); break;
    case 9:  open_gripper(); break;
    case 10: close_gripper(); break;
    case 11: reset_arm(); break;
    default: stop_bot(); break; 
  }
  }
}


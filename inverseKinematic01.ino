#include <Servo.h>
#include <math.h>

// Servos on the robot
Servo claw;
Servo rotator;
Servo elbow;
Servo shoulder;

// Start position of hand, the decimal is to avoid nan value
float posX = 0;
float posY = 8;
float posZ = 8;

char val;

void setup() {
  // Baudrate and PWM attachments
  Serial.begin(9600);
  claw.attach(5);
  rotator.attach(9);
  elbow.attach(6);
  shoulder.attach(3);
}

// Calculates positions of servos in order to have hand in the coordinates input
void pos(float posX, float posY, float posZ) {
  // Keeps imaginary numbers from appearing and divisions with 0
  posX += 0.00001;
  posY += 0.00001;
  posZ += 0.00001;
  
  // Outputs angle of a triangle generated from X and Z vector parts
  float rotatorAngle = atan(posX / posZ) / (6.28318530718) * 360;
  rotator.write(-rotatorAngle + 90);

  // Creation of general units such as directional distances as well as angles
  float dist = sqrt(pow(posX, 2) + pow(posY, 2) + pow(posZ, 2));
  float distXZ = sqrt(pow(posX, 2) + pow(posZ, 2));
  float orthRad = atan(distXZ / posY);
  float orthDist = sqrt(pow(8, 2) - pow(dist / 2, 2));

  // Creates vertical and horizontal components of the shoulder and outputs as an angle
  float shoulderXZ = distXZ / 2 - orthDist * cos(orthRad);
  float shoulderY = posY / 2 + orthDist * sin(orthRad);
  float shoulderAngle = (atan(shoulderXZ / shoulderY) / 6.28318530718) * 360;
  shoulder.write(shoulderAngle + 50);

  // Creates vertical and horizontal components of the elbow and outputs as an angle
  float elbowDistXZ = distXZ - shoulderXZ;
  float elbowDistY = posY - shoulderY;
  float elbowAngle = atan(elbowDistY / elbowDistXZ) / (6.28318530718) * 360;
  elbow.write(elbowAngle + 90);
}

void loop() {
  // Reads keyboard input and outputs new change in position
  if (Serial.available()) {
    val = Serial.read();
    if (val == 'a') {
      posX -= 1;
    } else if (val == 'd') {
      posX += 1;
    } else if (val == 'w') {
      posZ += 1;
    } else if (val == 's') {
      posZ -= 1;
    } else if (val == 'i') {
      posY += 1;
    } else if (val == 'k') {
      posY -= 1;
    } else if (val == 'j') {
      claw.write(180);
    } else if (val == 'l') {
      claw.write(45);
    }
  }
  // Runs function; you can also choose to input your own coordinates ofcourse
  pos(posX,posY,posZ);

  // Prints the position of the hand in the serial monitor
  Serial.print("X Position: ");
  Serial.println(posX);
  Serial.print("Y Position: ");
  Serial.println(posY);
  Serial.print("Z Position: ");
  Serial.println(posZ);
  Serial.println();
  delay(100);
}

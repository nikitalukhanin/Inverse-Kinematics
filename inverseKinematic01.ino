#include <Servo.h>
#include <math.h>

Servo claw;
Servo rotator;
Servo elbow;
Servo shoulder;

float posX = 0.00001;
float posY = 8.00001;
float posZ = 8.00001;

char val;

void setup() {
  Serial.begin(9600);
  claw.attach(5);
  rotator.attach(9);
  elbow.attach(6);
  shoulder.attach(3);
}

void loop() {
   
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
       posY+= 1;
    } else if (val == 'k') {
       posY -= 1;
    } else if (val == 'j'){
        claw.write(180);
    } else if (val == 'l'){
        claw.write(45);
    }
  }
  
  Serial.print("X Position: ");
  Serial.println(posX);
  Serial.print("Y Position: ");
  Serial.println(posY);
  Serial.print("Z Position: ");
  Serial.println(posZ);
  
  float rotatorAngle = atan(posX / posZ) / (6.28318530718) * 360;
  rotator.write(-rotatorAngle+90);

  float dist = sqrt(pow(posX, 2) + pow(posY, 2) + pow(posZ, 2));
  float distXZ = sqrt(pow(posX, 2) + pow(posZ, 2));

  float orthRad = atan(distXZ / posY);
  float orthDist = sqrt(pow(8,2) - pow(dist/2,2));
  
  float shoulderXZ = distXZ / 2 - orthDist * cos(orthRad);
  float shoulderY = posY / 2 + orthDist * sin(orthRad);

  float shoulderAngle = (atan(shoulderXZ / shoulderY) / 6.28318530718) * 360;
  shoulder.write(shoulderAngle + 50);

  float elbowDistXZ = distXZ - shoulderXZ;
  float elbowDistY = posY - shoulderY;
  float elbowAngle = atan(elbowDistY / elbowDistXZ) / (6.28318530718) * 360;
  elbow.write(elbowAngle + 90);

  Serial.println();

  delay(100);
}

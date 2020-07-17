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
  // PWM attachments to board and baud rate
  Serial.begin(9600);
  claw.attach(5);
  rotator.attach(9);
  elbow.attach(6);
  shoulder.attach(3);
}

// Calculates positions of servos in order to have hand in the coordinates input
void pos(float posX, float posY, float posZ) {
  posX += 0.00001;
  posY += 0.00001;
  posZ += 0.00001;
  // Takes magnitudes of positions and outputs direction of vector
  float rotatorAngle = atan(posX / posZ) / (6.28318530718) * 360;
  // Calibrates for position of servo in the arm
  rotator.write(-rotatorAngle + 90);

  // Magnitude of the distance away the point is
  float dist = sqrt(pow(posX, 2) + pow(posY, 2) + pow(posZ, 2));
  // Magnitude along the XZ plane
  float distXZ = sqrt(pow(posX, 2) + pow(posZ, 2));

  // Angle orthogonal to the line that connects the point to the origin
  float orthRad = atan(distXZ / posY);
  // Distance the elbow is away from the midpoint of the line that connects the
  // point to the origin
  float orthDist = sqrt(pow(8, 2) - pow(dist / 2, 2));

  // Shoulder XZ component
  float shoulderXZ = distXZ / 2 - orthDist * cos(orthRad);
  // Shoulder Y component
  float shoulderY = posY / 2 + orthDist * sin(orthRad);

  // Shoulder angle
  float shoulderAngle = (atan(shoulderXZ / shoulderY) / 6.28318530718) * 360;
  // Calibrates for position of servo in the arm
  shoulder.write(shoulderAngle + 50);

  // Distance shoulder is from the hand from the XZ direction
  float elbowDistXZ = distXZ - shoulderXZ;
  // Distance shoulder is from the hand from the Y direction
  float elbowDistY = posY - shoulderY;
  // Elbow Angle from difference in position
  float elbowAngle = atan(elbowDistY / elbowDistXZ) / (6.28318530718) * 360;
  // Calibrates for position of servo in the arm
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
  // Runs function
  pos(posX,posY,posZ);

  // Prints the position of the hand
  Serial.print("X Position: ");
  Serial.println(posX);
  Serial.print("Y Position: ");
  Serial.println(posY);
  Serial.print("Z Position: ");
  Serial.println(posZ);
  Serial.println();
  delay(100);
}

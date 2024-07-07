#include <Arduino_LSM9DS1.h>
#include <Wire.h>

// Variables for the sensor fusion 
float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      gyroDriftX,        gyroDriftY,         gyroDriftZ,        // units dps
      accRoll,           accPitch,           accYaw,            // units degrees (roll and pitch noisy, yaw not possible)
      complementaryRoll, complementaryPitch;  // units degrees (excellent roll, pitch)

// Variables for the calibration
long lastTime;
long lastInterval;

// Variables for the calculation of the wheel speeds in an interval of [-1; 1]
int motionClampAngle = 30;  // 30 degrees for range of motion, determined by manual testing
float rightWheelSpeed, leftWheelSpeed;

void setup() {

  Serial.begin(1000000);
  Serial1.begin(9600);  // Initialize serial communication on pins 11 (TX) and 12 (RX)
  pinMode(LED_BUILTIN, OUTPUT);

  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  calibrateIMU(250, 250);
  lastTime = micros();
}

/*
  the gyro's x,y,z values drift by a steady amount. 
  measure drift now and correct the real values with measured drift
*/
void calibrateIMU(int delayMillis, int calibrationMillis) {
  int calibrationCount = 0;

  delay(delayMillis); // to avoid shakes after pressing reset button

  float sumX, sumY, sumZ;
  int startTime = millis();
  while (millis() < startTime + calibrationMillis) {
    if (readIMU()) {
      sumX += gyroX;
      sumY += gyroY;
      sumZ += gyroZ;

      calibrationCount++;
    }
  }

  if (calibrationCount == 0) {
    Serial.println("Failed to calibrate");
  }

  // average drift per reading
  gyroDriftX = sumX / calibrationCount;
  gyroDriftY = sumY / calibrationCount;
  gyroDriftZ = sumZ / calibrationCount;
}

/**
   Read accel and gyro data.
   returns true if value is 'new' and false if IMU is returning old cached data
*/
bool readIMU() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() ) {
    IMU.readAcceleration(accelX, accelY, accelZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    return true;
  }
  return false;
}

void loop() {
  if (readIMU()) {
    long currentTime = micros();
    lastInterval = currentTime - lastTime;
    lastTime = currentTime;

    doSensorFusion();
    calculateWheelSpeeds();
    sendSteeringSpeeds();
  }
}


void doSensorFusion() {
  // Calculate (noisy) angles of accelerometer
  accRoll = atan2(accelY, accelZ) * 180 / M_PI;
  accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;

  float lastFrequency = (float) 1000000.0 / lastInterval;

  complementaryRoll = complementaryRoll + ((gyroX - gyroDriftX) / lastFrequency);
  complementaryPitch = complementaryPitch + ((gyroY - gyroDriftY) / lastFrequency);

  complementaryRoll = 0.70 * complementaryRoll + 0.30 * accRoll;
  complementaryPitch = 0.70 * complementaryPitch + 0.30 * accPitch;
}

void calculateWheelSpeeds() {
  // The range of motion should be around 30° to control the robot comfortably
  // Clamp the angles between -30 and 30 degree
  float clampedPitch constrain(complementaryPitch, -motionClampAngle, motionClampAngle);  // Forward motion
  float clampedRoll constrain(complementaryRoll, -motionClampAngle, motionClampAngle);  // Directional motion
  // Now normalize onto the [-1,1] interval
  float normalizedPitch = clampedPitch / motionClampAngle;
  // Set the wheel speed this is the same for both wheels
  rightWheelSpeed = leftWheelSpeed = normalizedPitch;

  bool driveLeft = clampedRoll < 0 ? true : false;
  // For the direction decrease the speed of wheel by a percentage of tilt max motion angle tilt
  float tiltPercentage = abs(clampedRoll) / motionClampAngle;
  float targetPercentage = 1.0 - tiltPercentage;

  // Now decrease the left or right wheel to match the target movement percentage
  if(driveLeft)
    leftWheelSpeed *= targetPercentage;
  else
    rightWheelSpeed *= targetPercentage;
  
  // Scale the speed , theoretically this could be a factor of at least 10
  // so the data is between [-10;10]
  // however since  the robot is trash scale the speed down
  leftWheelSpeed *= 1.5;
  rightWheelSpeed *= 1.5;
}

void sendSteeringSpeeds(){
  // Create a message in the format "value1,value2"
  String message = String(leftWheelSpeed) + "," + String(rightWheelSpeed);

  // Send the message via Serial1
  Serial1.println(message);

  // Print the sent message to the USB serial for debugging
  Serial.println(message);
  
}
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Offsets for calibration
int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

// Filtered values
float accx = 0, accy = 0, accz = 0;
float gyrox = 0, gyroy = 0, gyroz = 0;

// MPU6050 sensitivity values
const float acc_sensitivity = 16384.0; // Accelerometer: 16384 LSB/g
const float gyro_sensitivity = 131.0; // Gyroscope: 131 LSB/(°/s)

void setup() {
  Serial.begin(9600);
  Serial.println("\nStarting setup...");
  Wire.begin();
  Serial.println("I2C Initialized");

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed.");
    while (1);
  } else {
    Serial.println("MPU6050 connected successfully!");
  }

  // Calibrate the sensor
  calibrate_mpu();
}

void loop() {
  measure_mpu();
  delay(500);  // Update every 500ms
}

// Function to calibrate the MPU6050
void calibrate_mpu() {
  int num_samples = 200;
  int16_t ax, ay, az, gx, gy, gz;

  long ax_total = 0, ay_total = 0, az_total = 0;
  long gx_total = 0, gy_total = 0, gz_total = 0;

  for (int i = 0; i < num_samples; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax_total += ax;
    ay_total += ay;
    az_total += az;
    gx_total += gx;
    gy_total += gy;
    gz_total += gz;
    delay(10);
  }

  ax_offset = ax_total / num_samples;
  ay_offset = ay_total / num_samples;
  az_offset = az_total / num_samples;
  gx_offset = gx_total / num_samples;
  gy_offset = gy_total / num_samples;
  gz_offset = gz_total / num_samples;

  Serial.println("Calibration completed.");
}

// Function to measure and output MPU6050 values
void measure_mpu() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Apply offsets
  ax -= ax_offset;
  ay -= ay_offset;
  az -= az_offset;
  gx -= gx_offset;
  gy -= gy_offset;
  gz -= gz_offset;

  // Convert raw values to physical units
  accx = (float)ax / acc_sensitivity * 9.81; // m/s^2
  accy = (float)ay / acc_sensitivity * 9.81; // m/s^2
  accz = (float)az / acc_sensitivity * 9.81; // m/s^2
  gyrox = (float)gx / gyro_sensitivity;      // °/s
  gyroy = (float)gy / gyro_sensitivity;      // °/s
  gyroz = (float)gz / gyro_sensitivity;      // °/s

  // Print the output values
  Serial.print("AccX: "); Serial.print(accx); Serial.print(" m/s^2, ");
  Serial.print("AccY: "); Serial.print(accy); Serial.print(" m/s^2, ");
  Serial.print("AccZ: "); Serial.print(accz); Serial.print(" m/s^2, ");
  Serial.print("GyroX: "); Serial.print(gyrox); Serial.print(" °/s, ");
  Serial.print("GyroY: "); Serial.print(gyroy); Serial.print(" °/s, ");
  Serial.print("GyroZ: "); Serial.print(gyroz); Serial.println(" °/s");
  delay(1000);
}

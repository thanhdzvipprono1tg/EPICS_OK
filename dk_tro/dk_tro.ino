#include <Wire.h>
#include <MPU9250.h>

MPU9250 imu;

void setup() {
  Wire.begin();
  imu.init();
}

void loop() {
  imu.readSensor();
  // ... (đọc dữ liệu thô) ...
}

float ax = imu.getAccelX_mss();
float ay = imu.getAccelY_mss();
float az = imu.getAccelZ_mss();
float gx = imu.getGyroX_rads();
float gy = imu.getGyroY_rads();
float gz = imu.getGyroZ_rads();
float mx = imu.getMagX_uT();
float my = imu.getMagY_uT();
float mz = imu.getMagZ_uT();

float roll, pitch, yaw;
float alpha = 0.98; // Hệ số lọc

void estimateOrientation(float ax, float ay, float az, float gx, float gy, float gz, float dt) {
  // Ước tính góc từ gia tốc kế
  float rollAcc = atan2(ay, az) * 180 / PI;
  float pitchAcc = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;

  // Ước tính góc từ con quay hồi chuyển
  roll += gx * dt * 180 / PI;
  pitch += gy * dt * 180 / PI;
  yaw += gz * dt * 180 / PI;

  // Kết hợp các ước tính
  roll = alpha * roll + (1 - alpha) * rollAcc;
  pitch = alpha * pitch + (1 - alpha) * pitchAcc;
  // (Cần thêm logic sử dụng từ kế để hiệu chỉnh yaw)
}

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

// ... (Định nghĩa UUID dịch vụ và đặc tính) ...

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

void setupBLE() {
  BLEDevice::init("mèo");
  pServer = BLEDevice::createServer();

  pCharacteristic = pService->createCharacteristic(
                                 characteristicUUID,
                                 BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE |
                                 BLECharacteristic::PROPERTY_NOTIFY
                               );
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->start();
}

void sendOrientation(float roll, float pitch, float yaw) {
  // ... (Đóng gói dữ liệu vào một mảng byte) ...
  pCharacteristic->setValue(data, sizeof(data));
  pCharacteristic->notify();
}
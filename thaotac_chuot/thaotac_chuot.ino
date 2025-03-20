#include <Wire.h>
#include <BMI160.h>

BMI160 imu;

void setup() {
  Wire.begin();
  imu.initialize();
}

void loop() {
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //đọc dữ liệu
}

int16_t ax, ay, az, gx, gy, gz;

#define CLICK_THRESHOLD 2000 // Ngưỡng cho vận tốc góc

bool detectSingleClick(int16_t gx, int16_t gy, int16_t gz) {
  return (abs(gx) > CLICK_THRESHOLD || abs(gy) > CLICK_THRESHOLD || abs(gz) > CLICK_THRESHOLD);
}

#include <BLEDevice.h>
    #include <BLEServer.h>
    #include <BLEUtils.h>
    #include <BLEAdvertising.h>
    #include <BLEHIDDevice.h>
    #include <HIDTypes.h>

    BLEHIDDevice* hid;
    BLECharacteristic* inputMouse;

    void setupHID() {
      hid = new BLEHIDDevice(server);
      inputMouse = hid->inputReport(1); // Report ID = 1
      // ... (Thiết lập bản đồ báo cáo HID) ...
      hid->setBatteryLevel(99);
      hid->startServices();
    }

    void sendMouseEvent(int8_t x, int8_t y, uint8_t buttons) {
      uint8_t report = {buttons, x, y, 0}; // Buttons, X, Y, Scroll
      inputMouse->setValue(report, 4);
      inputMouse->notify();
    }

// vòng lặp
    if (receivedOrientationData) {
      // Tính toán sự thay đổi vị trí con trỏ dựa trên dữ liệu hướng
      sendMouseEvent(deltaX, deltaY, currentButtonState);
    }

    if (detectSingleClick(gx, gy, gz)) {
      currentButtonState |= 0x01; // Nhấp chuột trái
      sendMouseEvent(0, 0, currentButtonState);
      delay(50); // Chống rung
      currentButtonState &= ~0x01;
      sendMouseEvent(0, 0, currentButtonState);
    }
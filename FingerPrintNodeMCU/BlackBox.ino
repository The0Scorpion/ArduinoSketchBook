#include "Settings.h"
#include "esp_camera.h"
#include "OfflineFunc.h"
#include "OnlineFunc.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"



void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  pinMode(resBut, INPUT_PULLUP);
  pinMode(lockfeedback, INPUT_PULLUP);
  pinMode(lock, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(Lights, OUTPUT);
  pinMode(Gled, OUTPUT);
  digitalWrite(lock, Off);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
  initOnline();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  if (!digitalRead(resBut)) {
    bst = millis();
    while (!digitalRead(resBut)) {
      Serial.println((String)"Pressed for " + (millis() - bst));
      delay(1000);
    }
    if (millis() - bst > Threshold) {
      bstate = 1;
      bst = 0;
      initOffline();
      reset();
    }

  }
  if (!(counter % lpc)) {
    pinMode(PIR, INPUT);
    CheckAll();
    Set(stURL[3], !digitalRead(lockfeedback));
  }
  delay(1000 / lps);
  counter++;
}

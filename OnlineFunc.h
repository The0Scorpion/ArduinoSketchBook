void doStuff(String vals, byte cs);
void initOnline() {
  EEPROM.begin(512);
  delay(10);
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }

  Serial.print("PASS: ");
  Serial.println(epass);
  /*String serverName1 = "";
    for (int i = 96; i < 256; ++i)
    {
    serverName1 += char(EEPROM.read(i));
    }
    Serial.print("ip: ");
    Serial.println(serverName1);
    serverName = "http://" + serverName1 + "/";*/
  WiFi.begin(esid.c_str(), epass.c_str());
  //WiFi.begin(ssid, password);
  Serial.println("Connecting");
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter > 30) {
      Serial.println("");
      Serial.println("Couldn't Connect");
      Serial.println("Starting setup sequence");
      initOffline();
      reset();
      break;
    }
  }
  if (counter <= 30) {
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("wifi done");
    Serial.println((String) "Starting Connction to:" + serverName);
  }

}
bool Set(String subURL, bool state) {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String path = (String)serverName + subURL + "?s=" + state;
    String payload;
    // Your Domain name with URL path or IP address with path
    http.begin(path.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print(path + "  HTTP Response code: ");
      Serial.println(httpResponseCode);
      payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      return 0;
    }
    // Free resources
    http.end();
    Set(stURL[3], !digitalRead(lockfeedback));
    return payload.indexOf("OK") >= 0;

  }
  else {
    Serial.println("WiFi Disconnected" + subURL);
    return 0;
  }
}

String sendPhoto() {
  String getAll;
  String getBody;

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Connecting to server: " + serverName1);

  if (client.connect(serverName1.c_str(), 80)) {
    Serial.println("Connection successful!");
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;

    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName1);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    client.println();
    client.print(head);

    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n = 0; n < fbLen; n = n + 1024) {
      if (n + 1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen % 1024 > 0) {
        size_t remainder = fbLen % 1024;
        client.write(fbBuf, remainder);
      }
    }
    client.print(tail);

    esp_camera_fb_return(fb);

    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;

    while ((startTimer + timoutTimer) > millis()) {
      Serial.print(".");
      delay(2);
      int k = 0;
      while (client.available()) {
        char c = client.read();
        if (c == '<') {
          k = 0;
          state = true;
          if (getAll.length() == 0) {
            state = true;
          }
          getAll = "";
        }

        else if (c != '\r') {
          getAll += String(c);
        }
        if (c == '>') {
          state = false;
        }
        if (state && k) {
          getBody += String(c);
        }
        if (state) {
          k++;
        }
      }
      if (getBody.length() > 0) {
        break;
      }

    }
    Serial.println();
    client.stop();
    Serial.println((String)"Response:" + getBody);
    doStuff(getBody, 0);
  }
  else {
    getBody = "Connection to " + serverName1 +  " failed.";
    Serial.println(getBody);
  }
  return getBody;
}
void doStuff(String vals, byte cs) {
  int valtem = vals.toInt();
  for (int i = 5; i >= 0; i--) {
    val[i] = valtem % 10;
    valtem /= 10;
    Serial.println(val[i]);
  }
  //motion sensor
  if (val[1]) {
    if (digitalRead(PIR)) {
      digitalWrite(Rled, 1);
      delay(2000);
      digitalWrite(Rled, 0);
      Set(stURL[0], 1 );
    }
  }
  //stream

  if (val[3] && cs) {
    Serial.println("Starting The stream");
    startTime = millis();
    streaming = 1;
    digitalWrite(Lights, FlashON);
    while (val[3]) {
      sendPhoto();
      //delay(20);
    }
    digitalWrite(Lights, 0);
  }
  //lock
  if (val[0]) {
    Serial.println("Opnening lock");
    digitalWrite(lock, On);
    digitalWrite(Gled, 1);
    delay(300);
    digitalWrite(Gled, 0);
    delay(300);
    digitalWrite(Gled, 1);
    delay(2000);
    digitalWrite(Gled, 0);
    digitalWrite(lock, Off);
    Set(stURL[2], 0);
  }
}
void CheckAll() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String path = serverName + "getall.php";
    Serial.println(path);
    String payload;
    // Your Domain name with URL path or IP address with path
    http.begin(path.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      payload = http.getString();
      doStuff(payload, 1);
      Serial.println(payload);

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);

    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected: Check ALL");
  }
}

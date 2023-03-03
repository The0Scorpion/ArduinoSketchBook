//#include "sett.h"
void createWebServer()
{{
    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String ServerIP = server.arg("ip");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 256; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        Serial.println("writing eeprom ip:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(96 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.restart();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
    });
  }
}
void initOffline() {
  WiFi.disconnect();
  WiFi.softAP(defssid, defpass);
  pinMode(Gled, OUTPUT);
  pinMode(Rled, OUTPUT);
  IPAddress IP = WiFi.softAPIP();
  delay(200);
  WiFi.softAPConfig(Ip, Ip, NMask);

  Serial.print("AP IP address: ");
  Serial.println(IP);
  createWebServer();
  server.begin();
  Serial.println("Server started");
}/*
  void reset() {
  char* header;
  String h;
  while (1) {
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
      Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      int i = 0;
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header[i] = c;
          h+=c;
          i++;
          if (c == '\n') {                    // if the byte is a newline character

            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // turns the GPIOs on and off
              if (h.indexOf("GET /setwifi?") >= 0) {
                //filter out settings
                header+=h.indexOf("GET /setwifi?");
                char *creds[2];
                char *ptr = NULL;
                byte index = 0;
                ptr = strtok(header, "&");
                while (ptr != NULL)
                {
                  creds[index] = ptr;
                  index++;
                  ptr = strtok(NULL, "&");
                }
                String qsid = (String)creds[0];
                String qpass = (String)creds[1];
                if (qsid.length() > 0 && qpass.length() > 0) {
                  Serial.println("clearing eeprom");
                  for (int i = 0; i < 96; ++i) {
                    EEPROM.write(i, 0);
                  }
                  Serial.println(qsid);
                  Serial.println("");
                  Serial.println(qpass);
                  Serial.println("");

                  Serial.println("writing eeprom ssid:");
                  for (int i = 0; i < qsid.length(); ++i)
                  {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                  }
                  Serial.println("writing eeprom pass:");
                  for (int i = 0; i < qpass.length(); ++i)
                  {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                  }
                  EEPROM.commit();


                }
                // Respond With the Mac Adress
                client.println(WiFi.macAddress());

                // The HTTP response ends with another blank line
                client.println();
                // Break out of the while loop
                // Clear the header variable
                header = "";
                // Close the connection
                client.stop();
                Serial.println("Client disconnected.\n Config Successful.");
                Serial.println("");
                server.stop();
                break;
              }




            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }

    }
  }
  }*/
void reset()
{
  while (1)
  {
    digitalWrite(Rled,1);
    digitalWrite(Gled,1);
    delay(100);
    digitalWrite(Rled,0);
    digitalWrite(Gled,0);
    delay(100);
    server.handleClient();
    
  }
}

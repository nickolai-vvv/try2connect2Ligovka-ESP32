#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>

// Параметры сети
const char* ssid = "Redmi9";
const char* password = "333444555666";

void getpr24h() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  client->setTimeout(4000);

  
  if (https.begin(*client, "https://www.knappe.pl/aaa.html")) {  // HTTPS
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server?
      if (httpCode == HTTP_CODE_OK) {
                String payload = "";
        int len = http.getSize();
        uint8_t buff[128] = { 0 };
        WiFiClient * stream = http.getStreamPtr();

                // read all data from server
                while(http.connected() && (len > 0 || len == -1)) {
                    // get available data size
                    size_t size = stream->available();

                    if(size) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                        // write it to Serial
                        Serial.write(buff,c);
                        
                        //payload += String((char*)buff);
                        char charBuff[c + 1]; // Create a character array with space for null terminator
                        memcpy(charBuff, buff, c); // Copy the data to the character array
                        charBuff[c] = '\0'; // Null-terminate the character array
                        payload += String(charBuff); // Append the character array to the payload

                        if(len > 0) {
                            len -= c;
                        }
                    }
                    delay(1);
                }
        
        ////////////////////////////////////////////////////////////////
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }
}

String ParseString(String payload, String startMask, String endMask)
{
  int startPos = payload.indexOf(startMask);
  if (startPos == -1) {
    return "!- nan -!";
  }

  int endPos = payload.indexOf(endMask, startPos + startMask.length());
  if (endPos == -1) {
    return "!- nan -!";
  }

  return payload.substring(startPos + startMask.length(), endPos);
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Соединяемся с Wi-Fi..");
  }
  
  Serial.println("Соединение с Wi-Fi установлено");
}

void loop() {
  getpr24h();
  Serial.println("Wait 20s before next round to not get banned on API server...");
  delay(20000);
}

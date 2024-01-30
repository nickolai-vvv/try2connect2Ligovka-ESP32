#include <WiFi.h>
#include <WiFiClientSecure.h>

// Параметры сети
const char* ssid = "Redmi9";
const char* password = "333444555666";

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
  String requestURL = "/";

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;

    if (client.connect("example.com", 443)) { // 443 - порт для HTTPS
      client.print(String("GET ") + requestURL + " HTTP/1.1\r\n" +
                   "Host: example.com\r\n" +
                   "Connection: close\r\n\r\n");

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      while (client.available()) {
        const String& payload = client.readString();
        //String Ppayload = ParseString(payload, "<a title=\"Калькулятор\" class=\"value value_green\" href=\"#\">", "</a>");
        String Ppayload = ParseString(payload, "<h1>", "</h1>");
        Serial.println("Полученный payload: " + Ppayload);
      }

      client.stop();
    } else {
      Serial.println("Не удалось подключиться к серверу");
    }
  }

  delay(60000); // 30 сек задержка
}

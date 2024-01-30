#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

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
  String requestURL = "https://ligovka.ru/detailed/usd";

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Используется для тестирования, не рекомендуется для реальных приложений

    if (client.connect("ligovka.ru", 443)) { // 443 - порт для HTTPS
      client.print(String("GET ") + requestURL + " HTTP/1.1\r\n" +
                   "Host: ligovka.ru\r\n" +
                   "Connection: close\r\n\r\n");

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      while (client.available()) {
        String payload = client.readString();
        String Ppayload = ParseString(payload, "<td class=\"money_price buy_price\">", "</td>");
        Serial.println("Полученный payload: " + Ppayload);
      }
      
      client.stop();
    } else {
      Serial.println("Не удалось подключиться к серверу");
    }
  }

  delay(30000); // 30 сек задержка
}

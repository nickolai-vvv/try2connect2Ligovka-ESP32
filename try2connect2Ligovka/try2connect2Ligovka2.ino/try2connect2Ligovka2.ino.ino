#include <WiFi.h>
#include <HTTPClient.h>
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
    HTTPClient http;
    http.begin(requestURL);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("HTTP код: " + String(httpCode));
      
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        //String Ppayload = ParseString(payload, "<h1>", "</h1>");
        String Ppayload = ParseString(payload, "<td class=\"money_price buy_price\">","</td>");
        Serial.println("Полученный payload: " + Ppayload);
      } else {
        Serial.println("Ошибка HTTP-запроса");
      }
    } else {
      Serial.println("Не удалось подключиться к серверу");
    }

    http.end();
  }

  delay(30000); // 30 сек задержка
}

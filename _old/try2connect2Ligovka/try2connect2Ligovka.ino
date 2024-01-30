#include <WiFi.h>
#include <HTTPClient.h>

// Параметры сети
const char* ssid = "Redmi9";
const char* password = "333444555666";

void setup() {
  Serial.begin(115200);
  // делаем небольшую задержку на открытие монитора порта
  delay(5000);
  // подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Соединяемся с Wi-Fi..");
  }
  Serial.println("Соединение с Wi-Fi установлено");
}

void loop() {
  // выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) {
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице
    http.begin("https://www.sberometer.ru/");
    // делаем GET запрос
    int httpCode = http.GET();
    // проверяем успешность запроса
    if (httpCode > 0) {
     Serial.println("HTTP код: " + String(httpCode));
     if (httpCode == HTTP_CODE_OK) {
       String payload = http.getString();
       Serial.println("Полученный payload: " + payload);
     } else {
       Serial.println("Ошибка HTTP-запроса");
     }// if http == HTTP_CODE_OK
    } else {
       Serial.println("Не удалось подключиться к серверу");
    }// if http code > 0
    // освобождаем ресурсы микроконтроллера
    http.end();
  }//if wifi status
  delay(30000); // 30 сек задержка
}

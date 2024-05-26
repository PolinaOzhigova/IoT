#include <SoftwareSerial.h>

// Пины D1 и D2 для соединения с датчиком SDS011
int rxPin = D1;
int txPin = D2;

// Объект для чтения данных с датчика SDS011
SoftwareSerial sds(rxPin, txPin);

void setup() {
  Serial.begin(9600); // Начать Serial для вывода данных в монитор порта
  sds.begin(9600); // Начать соединение с датчиком SDS011
}

void loop() {
  // Чтение данных с датчика
  if (sds.available() >= 10) {
    // Прочитать байты данных
    uint8_t buf[10];
    sds.readBytes(buf, 10);

    // Проверка, успешно ли было чтение данных
    if (buf[0] == 0xAA && buf[1] == 0xC0) {
      // Извлечь значения PM2.5 и PM10 из буфера
      uint16_t pm25 = (buf[3] * 256 + buf[2])/10; // PM2.5 значение
      uint16_t pm10 = (buf[5] * 256 + buf[4])/10; // PM10 значение

      // Вывод текущего времени
      printTime();

      // Вывод данных о PM2.5 и PM10 в монитор порта
      Serial.print("PM2.5: ");
      Serial.print(pm25);
      Serial.print(" ug/m3\tPM10: ");
      Serial.print(pm10);
      Serial.println(" ug/m3");
    }
  }

  delay(1000); // Подождать 1 секунду перед повторной итерацией
}

// Функция для вывода текущего времени в монитор порта
void printTime() {
  // Получение текущего времени
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;

  // Преобразование времени в часы, минуты и секунды
  hours = hours % 24;
  minutes = minutes % 60;
  seconds = seconds % 60;

  // Вывод времени в монитор порта
  if (hours < 10) {
    Serial.print("0");
  }
  Serial.print(hours);
  Serial.print(":");
  if (minutes < 10) {
    Serial.print("0");
  }
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) {
    Serial.print("0");
  }
  Serial.print(seconds);
  Serial.print(" ");
}

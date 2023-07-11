#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFiClient.h>

// Определение размера OLED-дисплея (128x64)
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Установка типа датчика DHT и номера пина, к которому он подключен
#define DHT_PIN D4
#define DHT_TYPE DHT11

// Номер пина, к которому подключен датчик уровня воды
const int waterSensorPin = A0;

// Калибровочные значения для датчика уровня воды
const int dryValue = 0;    // Значение для сухой среды
const int wetValue = 1023; // Значение для полностью погруженной среды

// Инициализация объектов для работы с DHT и OLED-дисплеем
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// Подключение к Wi-Fi
const char* ssid = "Kuply Slona";
const char* password = "87654321";

ESP8266WebServer server(80);

IPAddress server_addr(127, 0, 0, 1); // IP-адрес сервера базы данных
char user[] = "ilyax14"; // Имя пользователя базы данных
char password[] = "Oy58nlsU"; // Пароль для доступа к базе данных
char database[] = "ilyax14"; // Имя базы данных
WiFiClient client; // Создание объекта клиента WiFiClient
MySQL_Connection conn((Client *)&client); // Создание объекта подключения к базе данных

void setup() {
  // Инициализация последовательного порта для вывода в консоль
  Serial.begin(115200);

  // Инициализация связи с OLED-дисплеем
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  

  WiFi.begin(ssid, password);
  
  // Ожидание подключения к Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  // Подключение к базе данных MySQL
  if (conn.connect(server_addr, 3306, user, password)) {
    Serial.println("Connected to database");
  } else {
    Serial.println("Connection failed");
  }

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  // Инициализация датчика DHT11
  dht.begin();
}

void loop() {
  server.handleClient();

  // Проверка подключения к базе данных MySQL
  if (!conn.connected()) {
    Serial.println("Lost connection to database, reconnecting...");
    if (conn.connect(server_addr, 3306, user, password)) {
      Serial.println("Reconnected to database");
    } else {
      Serial.println("Reconnection failed");
    }
  }

  // Чтение данных с датчика DHT
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Чтение значения с аналогового пина датчика уровня воды
  int sensorValue = analogRead(waterSensorPin);
  int waterLevel = map(sensorValue, dryValue, wetValue, 0, 76);
  
  // Получение текущей даты и времени
  time_t now = time(nullptr);
  struct tm* timeinfo;
  timeinfo = localtime(&now);
  char datetime[20];
  sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
          timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  
  // Вывод данных на OLED-дисплей
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.print(temperature);
  display.println(" C");
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");
  display.print("Water Level: ");
  display.print(waterLevel);
  display.println(" mm");
  display.display();
  
  // Вывод данных в консоль
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" mm");

  // Вставка данных в базу данных MySQL
  char query[128];
  sprintf(query, "INSERT INTO sensor_data (datetime, temperature, humidity, water_level) VALUES ('%s', %.2f, %.2f, %d)",
          datetime, temperature, humidity, waterLevel);
  MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;

  delay(2000); // Задержка 2 секунды
}

void handle_OnConnect() 
{
  float Temperature = dht.readTemperature(); 
  float Humidity = dht.readHumidity();       
  int sensorValue = analogRead(waterSensorPin);
  int waterLevel = map(sensorValue, dryValue, wetValue, 0, 76);
  server.send(200, "text/html", SendHTML(Temperature, Humidity, waterLevel)); 
}
void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}
String SendHTML(float Temperaturestat,float Humiditystat, int waterLevelstat)
{
  String str = "<!DOCTYPE html> <html>\n";
  str +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  str +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  str +="<title>ESP8266 Weather</title>\n";
  str +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  str +="body{margin-top: 50px;}\n";
  str +="h1 {margin: 50px auto 30px;}\n";
  str +=".img3 {width:35px; height: 35px;}\n";
  str +=".img2 {width:35px; height: 35px;}\n";
  str +=".img4 {width:35px; height: 35px;}\n";
  str +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  str +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  str +=".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  str +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  str +=".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n";
  str +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  str +=".data{padding: 10px;}\n";
  str +="</style>\n";
  str +="<script>\n";
  str +="setInterval(loadDoc,200);\n";
  str +="function loadDoc() {\n";
  str +="var xhttp = new XMLHttpRequest();\n";
  str +="xhttp.onreadystatechange = function() {\n";
  str +="if (this.readyState == 4 && this.status == 200) {\n";
  str +="document.getElementById(\"webpage\").innerHTML =this.responseText}\n";
  str +="};\n";
  str +="xhttp.open(\"GET\", \"/\", true);\n";
  str +="xhttp.send();\n";
  str +="}\n";
  str +="</script>\n";
  str +="</head>\n";
  str +="<body>\n";
  str +="<div id=\"webpage\">\n";
  str +="<h1>ESP8266 Weather</h1>\n";
  str +="<div class=\"data\">\n";
  str +="<div class=\"side-by-side temperature-icon\">\n";
  str +="<span><img class=\"img3\" src=\"https://cdn-icons-png.flaticon.com/512/2100/2100130.png\"></span>\n";
  str +="</div>\n";
  str +="<div class=\"side-by-side temperature-text\">Temperature</div>\n";
  str +="<div class=\"side-by-side temperature\">";
  str +=(int)Temperaturestat;
  str +="<span class=\"superscript\">°C</span></div>\n";
  str +="</div>\n";
  str +="<div class=\"data\">\n";
  str +="<div class=\"side-by-side humidity-icon\">\n";
  str +="<span><img class=\"img2\" src=\"https://cdn-icons-png.flaticon.com/512/8923/8923690.png\"></span>\n";
  str +="</div>\n";
  str +="<div class=\"side-by-side humidity-text\">Humidity</div>\n";
  str +="<div class=\"side-by-side humidity\">";
  str +=(int)Humiditystat;
  str +="<span class=\"superscript\">%</span></div>\n";
  str +="</div>\n";
  str +="<div class=\"data\">\n";
  str +="<div class=\"side-by-side humidity-icon\">\n";
  str +="<span><img class=\"img4\" src=\"https://cdn-icons-png.flaticon.com/512/932/932532.png\"></span>\n";
  str +="</div>\n";
  str +="<div class=\"side-by-side humidity-text\">Water Level</div>\n";
  str +="<div class=\"side-by-side humidity\">";
  str +=(int)waterLevelstat;
  str +="<span class=\"superscript\">mm</span></div>\n";
  str +="</div>\n";
  str +="</div>\n";
  str +="</body>\n";
  str +="</html>\n";
  return str;
}
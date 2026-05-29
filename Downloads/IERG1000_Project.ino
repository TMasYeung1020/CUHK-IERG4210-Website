// SSD Part
  #include <Wire.h>
  #include <Adafruit_SSD1306.h>
  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels
  #define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Web Part
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>

  #include <Arduino.h>
  #include <WiFiMulti.h>
  #include <HTTPClient.h>

  #define USE_SERIAL Serial
  WiFiMulti wifiMulti;

  #include "time.h"
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 0;

  const char* ssid = "wifi";
  const char* password = "12345678";
  String wpc = "";
  WebServer server(80);
void handleRoot(){
  server.send(200,"text/plain", wpc);
}

//void handleNotFound(){}

// AHT Part
  #include <Adafruit_AHT10.h>
  Adafruit_AHT10 aht;

// Button Part
  #define buttonPin 12
  #define ledPin 2
  int buttonState = 0;

// Others
  String BrightNess = "BRIGHT";

void setup() {
  analogReadResolution(12);
// Declare All
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP); 

// Begin All
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  aht.begin();
  Serial.begin(115200);
  wifiMulti.addAP(ssid,"12345678");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp32")){
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  delay(100); 
}

void loop() {
  wpc="";
  delay(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1.5);
  digitalWrite(ledPin, LOW);
  checktemp();
  ShaTinweather();
  brightness();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Timer();
  display.print("\nYeung Man, 1155203181");
  wpc += "\nYeung Man, 1155203181";
  display.display();
  digitalWrite(ledPin, HIGH);
  Serial.print(wpc);
  handleRoot();
  server.handleClient();
  delay(10000);
}

void checktemp(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Temp.: "); display.print(temp.temperature);display.println("C");
  display.print("Humidity: "); display.print(humidity.relative_humidity);display.println("%");
  wpc += "Indoor Temperature: "+(String)temp.temperature+"C\n";
  wpc += "Indoor Humidity: "+(String)humidity.relative_humidity+"%\n\n";
}

void brightness(){
  int BrightNess = analogRead(34);
  if (BrightNess < 1950){
    display.println("Dark");
    wpc+="Dark";
    }
  else {
    display.println("BRIGHT");
    wpc+="BRIGHT";
  }
}

void ShaTinweather(){
  if((wifiMulti.run() == WL_CONNECTED)){
    HTTPClient http;
    http.begin("https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en"); //api link
    int httpCode = http.GET();
    String payload = http.getString();
    int TPosofShaTin = payload.lastIndexOf("Sha Tin");
    String ShaTintemp = payload.substring(TPosofShaTin + 17, TPosofShaTin + 19);
    int RainPosofShaTin =payload.indexOf("Sha Tin");
    String ShaTinrain = payload.substring(RainPosofShaTin + 15,RainPosofShaTin + 16);   //print data from api
    int HMDPosofShaTin = payload.indexOf("humidity");
    String ShaTinHMD = payload.substring(HMDPosofShaTin + 86,HMDPosofShaTin + 88);
    display.print("Sha Tin: ");display.print(ShaTintemp);display.print("C\n");
    wpc += "Outdoor Humidity: " + ShaTinHMD +" %\n";
    wpc += "Sha Tin Temperature: " + ShaTintemp +" C\n";
    wpc += "Sha Tin Rainfall: " + ShaTinrain +" mm\n";
  }
}

void Timer(){
  struct tm timeinfo;
  int hour, minute , second, month, day, year;
  String Rsecond, Rminute, Rhour, Rmonth;
  String DayTime = "am";
  String Month[12]={"January","February","March","April","May","June","July","Auguest","September","October","November","December"};  //initalize arrays for storing text used to print date in webpage
  String Day[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
  configTime(28800, 0, "pool.ntp.org");
  if (!getLocalTime(&timeinfo)){
      display.print("Unknown!");
      return;
    }
  second = timeinfo.tm_sec;
  minute = timeinfo.tm_min;
  hour = timeinfo.tm_hour;
  month = timeinfo.tm_mon;
  day = timeinfo.tm_mday;
  year = timeinfo.tm_year;
  buttonState = digitalRead(buttonPin);

  if (second < 10){
    Rsecond = "0" + (String)second;
  }else{
    Rsecond = (String)second;
  }
  if (minute < 10){
    Rminute = "0" + (String)minute;
  }else{
    Rminute = (String)minute;
  }
  if (hour < 10){
    Rhour = "0" + (String)hour;
  }else{
    Rhour = (String)hour;
  }
  if (month < 10){
    Rmonth = "0" + (String)month;
  }else{
    Rmonth = (String)month;
  }
  
  wpc += "Date = " + (String)day + "-" + Rmonth + "-" + (String)(year + 1900) + "\n";
  wpc += "DATE = " + (String)day + ", " + Month[month] + ", " + (String)(year + 1900) + "(" + Day[timeinfo.tm_wday] + ")\n";
  
  if (buttonState == HIGH){
    display.print("Time: " + Rhour + ":" + Rminute + ":" + Rsecond);
    wpc += Rhour + ":" + Rminute + ":" + Rsecond + " (24-H format) ";
  }
  else{
    if (hour > 12){
      Rhour = (String)(hour - 12);
      DayTime = "pm";
    }
    display.print("Time: " + Rhour + ":" + Rminute + ":" + Rsecond + DayTime); 
  }
}
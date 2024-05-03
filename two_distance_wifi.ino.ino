#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// 定義 WiFi 和 ThingSpeak 設定
const char* ssid = "CCIT";
const char* password = "04260426";
const char* thingSpeakApiKey = "EAUARCISBGQIJ0QN";
const char* thingSpeakServer = "api.thingspeak.com";

// 初始化 WiFiClient 物件
WiFiClient client;

// 第一個傳感器的引腳
int trigPin1 = 12; // 發出聲波腳位 (ESP8266 GPIO12)
int echoPin1 = 14; // 接收聲波腳位 (ESP8266 GPIO14)

// 第二個傳感器的引腳
int trigPin2 = 13; // 為第二個傳感器設定新的觸發腳位
int echoPin2 = 15; // 為第二個傳感器設定新的回聲腳位


void setup() {
  Serial.begin(9600);
  setup_wifi(); // 確保此函數進行 WiFi 連接

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  unsigned long distance1 = ping(trigPin1, echoPin1) / 58;
  unsigned long distance2 = ping(trigPin2, echoPin2) / 58;

  // 構建 HTTP 請求
  if (WiFi.status() == WL_CONNECTED) { // 確保 WiFi 連接
    HTTPClient http;  // 創建對象
    String postData = "api_key=" + String(thingSpeakApiKey) + "&field1=" + String(distance1) + "&field2=" + String(distance2);
    http.begin(client, "http://" + String(thingSpeakServer) + "/update");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(postData); // 發送 POST 請求

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();  // 關閉連接
  } else {
    Serial.println("WiFi not connected");
  }
  delay(20000);  // ThingSpeak 至少間隔 15 秒的更新限制
}

unsigned long ping(int trigPin, int echoPin) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"
#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define API_KEY "AIzaSyAFsMNPaH5J5TSx3iyo8AUaYKchvHjJyv0"
#define DATABASE_URL "https://smart-parking-92913-default-rtdb.europe-west1.firebasedatabase.app/"
#define ssid "HUAWEI-3b8f"
#define pass "58h4nr6a"

FirebaseData fbData;
FirebaseConfig fbConfig;
FirebaseAuth fbAuth;
bool isConnected = false;

SoftwareSerial espSerial(4, 5);  // (D2 = 4) RX, (D1 = 5) TX pins for ESP8266

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(9600);     // Initialize the hardware serial port
  espSerial.begin(9600);  // Initialize the software serial port

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi successfully");

  fbConfig.api_key = API_KEY;
  fbConfig.database_url = DATABASE_URL;
  
  if(Firebase.signUp(&fbConfig, &fbAuth, "", "")) {
    Serial.println("Connected to Firebase");
    isConnected = true;
  } else {
    Serial.println("Couldn't connect to Firebase");
  }

  fbConfig.token_status_callback = tokenStatusCallback;
  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);

  // Initialize NTP Client
  timeClient.begin();
  timeClient.setTimeOffset(0);  // Set the time offset (in seconds) as needed
}

void loop() {
  timeClient.update();

  if (espSerial.available()) {
    String data = espSerial.readStringUntil('\n');
    Serial.println("Received: " + data);

    int space1, space2, space3, carWantToEnter;
    sscanf(data.c_str(), "%d,%d,%d,%d", &space1, &space2, &space3, &carWantToEnter);

    if (Firebase.ready() && isConnected) {
      Firebase.RTDB.setInt(&fbData, "/Space1", space1);
      Firebase.RTDB.setInt(&fbData, "/Space2", space2);
      Firebase.RTDB.setInt(&fbData, "/Space3", space3);
      Firebase.RTDB.setInt(&fbData, "/CarWantToEnter", carWantToEnter);
      Firebase.RTDB.setString(&fbData, "/LastUpdateTime", timeClient.getFormattedTime());
    }
  }
}

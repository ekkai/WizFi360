#include "WizFi360.h"

/* Baudrate */
#define SERIAL_BAUDRATE   115200
#define SERIAL3_BAUDRATE  115200

char ssid[] = "nepes_wireless_guest";       // your network SSID (name)
char pass[] = "gu@st12345";   // your network password
int status = WL_IDLE_STATUS;  // the Wifi radio's status

const char* server = "www.dweet.io";

WiFiClient client;

void printWifiStatus();

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_BAUDRATE);
  // initialize serial for WizFi360 module
  Serial3.begin(SERIAL3_BAUDRATE);
  // initialize WizFi360 module
  WiFi.init(&Serial3);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true)
      ;
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {

  float sensor = analogRead(A0);

  if (client.connect(server, 80)) {
    String s = "POST /dweet/for/{thing}?{sensor}=";
    s.concat(celsius);
    Serial.println(s);
    client.println(s);

    client.print("Host: www.dweet.io");
    client.print("Connection: close\n");
    client.println();
  }
  client.stop();

  Serial.println("Waiting…");
  delay(5000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

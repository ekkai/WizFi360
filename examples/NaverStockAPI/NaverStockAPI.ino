#include "WizFi360.h"

/* Baudrate */
#define SERIAL_BAUDRATE   115200
#define SERIAL3_BAUDRATE  115200

char ssid[] = "nepes_wireless_guest";    // your network SSID (name)
char pass[] = "gu@st12345";          // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status

char server[] = "api.finance.naver.com";
String stockCode = "033640";

// Initialize the Ethernet client object
WiFiClient client;

void printWifiStatus();

void setup()
{
  // initialize serial for debugging
  Serial.begin(SERIAL_BAUDRATE);
  // initialize serial for WizFi360 module
  Serial3.begin(SERIAL3_BAUDRATE);
  // initialize WizFi360 module
  WiFi.init(&Serial3);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.print("GET /service/itemSummary.nhn?itemcode=");
    client.print(stockCode);
    client.println(" HTTP/1.1");
    client.println("Host: api.finance.naver.com");
    client.println("Connection: close");
    client.println();
  }
}



void loop()
{
  while (client.available()) {
    //Serial.println("reading.......");
    char c = client.read();
    Serial.write(c);
    //delay(1);
  }
  // wait to let all the input command in the serial buffer
  delay(5);

  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    while (true);
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

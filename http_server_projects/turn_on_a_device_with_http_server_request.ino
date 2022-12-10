#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#define relayPin D1

// server
ESP8266WebServer server(80);

// WiFi CREDENTIALS
const char *ssid = "Lantern.inc";
const char *password = "8sq655@friday";


//server function
void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();
void sendResIndex ();
void livingroomled1off();
void livingroomled1on();


void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  pinMode(relayPin,OUTPUT);
  delay(10);
  Serial.println("");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  // server 
  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/livingroomled1on", livingroomled1on);
  server.on("/livingroomled1off", livingroomled1off);
  server.on("/ok", sendResIndex); // route -> function
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started...");
}

void loop() { 
  server.handleClient();
  delay(1);
}

void sendResIndex(){
  server.send(200,"text/plain","okhitted");
  Serial.println("ok hitted");
}
void livingroomled1on(){
  server.send(200,"text/plain","living room led 1 on");
  digitalWrite(relayPin, LOW);
  Serial.println("light on hitted");
}
void livingroomled1off(){
  server.send(200,"text/plain","living room led 1 off");
  digitalWrite(relayPin, HIGH);
  Serial.println("light off hitted");
}
void handleRoot() {
  server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
  Serial.println("root hitted");
}
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
  Serial.println("notfound hitted");
}
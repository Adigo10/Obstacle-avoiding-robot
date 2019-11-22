#include <ESP8266WiFi.h>
#define EN1 D6 // Enable PIN to control speed
#define EN2 D7
int x=0;
*9
+long duration=0;
const int trigP=D1;
const int echoP=D2;

int LS=0;
int RS=0;


const char* ssid = "rpi35";
const char* password = "HS340000";

WiFiClient client;
WiFiServer server(80);

String data = "";


void setup() {
Serial.begin(115200);
pinMode(EN1, OUTPUT);
pinMode(EN2,OUTPUT);
// Connect to WiFi network
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("WiFi connected");

// Start the server
server.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
}

String checkClient (void)
{
while (!client.available())
delay(1);
String request = client.readStringUntil('\r');
Serial.println(request);

client.flush();
return request;
}

void loop() {
client = server.available();
if (!client) {
return;
}

data = checkClient ();
if(data.indexOf('L')!=-1)
LS=(data.substring(data.indexOf('L')+1,data.indexOf('&'))).toInt();

if(data.indexOf('R')!=-1)
RS=(data.substring(data.indexOf('R')+1,data.indexOf('&'))).toInt();

digitalWrite(trigP,LOW);
delayMicroseconds(5);
digitalWrite(trigP,HIGH);
delayMicroseconds(10);
digitalWrite(trigP,LOW);
duration=pulseIn(echoP,HIGH);
x=duration*0.034/2;

if(x>100)
{
  LS=1000;
  RS=1000;
}
else
{
  LS=1.80113206+0.701539125*x+1.3606634*x*x-1.104234199*x*x*x+100;
  RS=1.32365665+0.604539125*x+1.1026634*x*x-1.547234199*x*x*x+100;

}

LS=LS-(LS%100);
RS=RS-(RS%100);
analogWrite(EN1,LS);
analogWrite(EN2,RS);
Serial.println(RS);
Serial.println(data);

client.println("HTTP/1.1 200 OK");

delay(1);
data="";

}

#include <Wifi.h>

#include <WS2812FX.h>
#include <LiquidCrystal.h>

const char* ssid = "Lamp_No1";
const char* password = "skolakrizik";

WiFiServer server(80);

//Dont forget 18kOhm Resistor
#define LED_COUNT 8
#define LED_PIN 21
#define NEXT_BUTTON 19
#define WIFI_SWITCH 18

int ledModes[] = {0,3,7,10,12,13,17,22,32,39,43,46,55,62};
int currentState = 0;

bool nextButtonState = LOW;

//LED strip settings
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

//Change LED mode
static void ledMode(int num) {
  ws2812fx.setMode(ledModes[num]);
}

//Next mode function
static void nextMode() {
  if(currentState < 13) {
    currentState++;
  }else{
    currentState = 0;
  }
  ledMode(currentState);
}

// Function to handle when station gets IP
void onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println("Station connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Redirect to the desired webpage
  WiFiClient client;
  if (client.connect(WiFi.gatewayIP(), 80)) {
    client.print(String("GET / HTTP/1.1\r\n") +
                 "Host: 192.168.4.1\r\n" +
                 "Connection: close\r\n\r\n");
    delay(100);
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Připojení k WiFi síti
  Serial.println();
  Serial.println("Připojování k WiFi síti...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP adresa WiFi sítě: ");
  Serial.println(IP);

  server.begin();

  //Setting LED strip
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0x289aff);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  //setPins
  pinMode(NEXT_BUTTON, INPUT);
  pinMode(WIFI_SWITCH, INPUT);

  //setMode1
  ledMode(currentState);

  // Attach the event handler for when station gets IP
  WiFi.onStationModeGotIP(onStationModeGotIP);
}

void loop() {

  //ledsNessesery
  ws2812fx.service();

  //Mode Button
  if(digitalRead(NEXT_BUTTON) == HIGH) {
    if(nextButtonState == LOW) {
      nextButtonState = HIGH;
      nextMode();
    }
  }else{
    nextButtonState = LOW;
  }

  //Wifi staff
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("Nový klient připojen.");
    while (!client.available()) {
      delay(1);
    }

    String request = client.readStringUntil('\r');
    Serial.println("Požadavek od klienta: ");
    Serial.println(request);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    // Stránka pro ovládání LED
    client.println("    <html lang=\"en\">");
    client.println("    <head>");
    client.println("        <meta charset=\"UTF-8\">");
    client.println("        <title>Title</title>");
    client.println("    </head>");
    client.println("    <body>");
    client.println("        <h1> Logo školy elektrotechnické Františka Křižíka</h1>");
    client.println("        <p>Zde můžete upravovat módy vašeho modelu</p>");
    client.println("        <a href=\"/?nextmode\">");
    client.println("            <button type=\"button\" name=\"myButton\">");
    client.println("                Click me!");
    client.println("            </button>");
    client.println("        </a>");
    client.println("    </body>");
    client.println("</html>");

    if (request.indexOf("/?nextmode") != -1) {
      //next mode
      Serial.println("NextMode");
      nextMode();
    }

    client.flush();
    client.stop();
    Serial.println("Klient odpojen.");
  }
}

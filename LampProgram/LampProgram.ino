#include <WiFi.h>

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
    if (request.indexOf("/?prog1") != -1) {
      //next mode
      Serial.println("Program1");
      ledMode(0);
      currentState = 0;
    }
    if (request.indexOf("/?prog2") != -1) {
      //next mode
      Serial.println("Program2");
      ledMode(1);
      currentState = 1;
    }
    if (request.indexOf("/?prog3") != -1) {
      //next mode
      Serial.println("Program3");
      ledMode(2);
      currentState = 2;
    }
    if (request.indexOf("/?prog4") != -1) {
      //next mode
      Serial.println("Program4");
      ledMode(3);
      currentState = 3;
    }
    if (request.indexOf("/?prog5") != -1) {
      //next mode
      Serial.println("Program5");
      ledMode(4);
      currentState = 4;
    }
    if (request.indexOf("/?prog6") != -1) {
      //next mode
      Serial.println("Program6");
      ledMode(5);
      currentState = 5;
    }
    if (request.indexOf("/?prog7") != -1) {
      //next mode
      Serial.println("Program7");
      ledMode(6);
      currentState = 6;
    }
    if (request.indexOf("/?prog8") != -1) {
      //next mode
      Serial.println("Program8");
      ledMode(7);
      currentState = 7;
    }
    if (request.indexOf("/?prog9") != -1) {
      //next mode
      Serial.println("Program9");
      ledMode(8);
      currentState = 8;
    }
    if (request.indexOf("/?prog10") != -1) {
      //next mode
      Serial.println("Program10");
      ledMode(9);
      currentState = 9;
    }
    if (request.indexOf("/?prog11") != -1) {
      //next mode
      Serial.println("Program11");
      ledMode(10);
      currentState = 10;
    }
    if (request.indexOf("/?prog12") != -1) {
      //next mode
      Serial.println("Program12");
      ledMode(11);
      currentState = 11;
    }
    if (request.indexOf("/?prog13") != -1) {
      //next mode
      Serial.println("Program13");
      ledMode(12);
      currentState = 12;
    }
    if (request.indexOf("/?prog14") != -1) {
      //next mode
      Serial.println("Program14");
      ledMode(13);
      currentState = 13;
    }

    

    client.flush();
    client.stop();
    Serial.println("Klient odpojen.");
  }

  if(digitalRead(WIFI_SWITCH) == LOW) {
    // Turn off Wi-Fi and server
    WiFi.softAPdisconnect(true); // Disconnect from the AP and disable the soft-AP
    server.close(); // Stop the server
    Serial.println("Wi-Fi and server turned off.");
  } else {
    // Turn on Wi-Fi and server
    WiFi.softAP(ssid, password); // Re-enable the soft-AP
    server.begin(); // Restart the server
    Serial.println("Wi-Fi and server turned on.");
  }

}
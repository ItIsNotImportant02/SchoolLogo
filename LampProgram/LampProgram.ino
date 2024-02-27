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
  ws2812fx.setColor(0x4528ff);
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
    client.println("<html lang=\"cs\">");
    client.println("<head>");
    client.println("<meta charset=\"UTF-8\">");
    client.println("<title>SkolaKrizikLogo</title>");
    client.println("<style>");
    client.println("* {");
    client.println("margin: 0;");
    client.println("padding: 0;");
    client.println("}");
    client.println("");
    client.println("html {");
    client.println("text-align: center;");
    client.println("background: rgb(0,28,47);");
    client.println("background: linear-gradient(133deg, rgba(0,28,47,1) 0%, rgba(0,20,54,1) 71%);");
    client.println("}");
    client.println("");
    client.println("button {");
    client.println("position: relative;");
    client.println("left: 0;");
    client.println("margin-top: 15px;");
    client.println("margin-bottom: 65px;");
    client.println("font-size: 45px;");
    client.println("padding: 0.6em 1.45em;");
    client.println("border-style: solid;");
    client.println("border-color: darkcyan;");
    client.println("outline: none;");
    client.println("color: rgb(255, 255, 255);");
    client.println("background: #000413;");
    client.println("cursor: pointer;");
    client.println("z-index: 0;");
    client.println("border-radius: 25px;");
    client.println("user-select: none;");
    client.println("-webkit-user-select: none;");
    client.println("touch-action: manipulation;");
    client.println("box-shadow: 0 0 16px rgba(123, 251, 255, 0.2), 0 0 16px rgba(0, 255, 255, 0.27);");
    client.println("display: inline-block;");
    client.println("}");
    client.println("");
    client.println(".nextbutton {");
    client.println("border-style: solid;");
    client.println("border-color: #1f0035;");
    client.println("font-size: 65px;");
    client.println("margin-top: 100px;");
    client.println("text-decoration: none;");
    client.println("position: relative;");
    client.println("padding: 0.6em 2em;");
    client.println("outline: none;");
    client.println("color: rgb(255, 255, 255);");
    client.println("background: #000413;");
    client.println("cursor: pointer;");
    client.println("z-index: 0;");
    client.println("border-radius: 10px;");
    client.println("user-select: none;");
    client.println("-webkit-user-select: none;");
    client.println("touch-action: manipulation;");
    client.println("box-shadow: 0 0 16px rgba(176, 123, 255, 0.2), 0 0 16px rgba(123, 0, 255, 0.27);");
    client.println("display: inline-block;");
    client.println("}");
    client.println("");
    client.println(".nextmode, a {");
    client.println("text-decoration: none;");
    client.println("}");
    client.println("");
    client.println(".krizik {");
    client.println("top: 50px;");
    client.println("margin-top: 100px;");
    client.println("font-size: 40px;");
    client.println("font-weight: 800;");
    client.println("font-family: Arial, serif;");
    client.println("text-transform: uppercase;");
    client.println("background: -webkit-linear-gradient(45deg, #ff8282, #ffbe89, #f7fd9c);");
    client.println("-webkit-background-clip: text;");
    client.println("-webkit-text-fill-color: transparent;");
    client.println("}");
    client.println(".text {");
    client.println("top: 50px;");
    client.println("margin-top: 100px;");
    client.println("font-size: 80px;");
    client.println("font-weight: 800;");
    client.println("font-family: Arial, serif;");
    client.println("text-transform: uppercase;");
    client.println("background: -webkit-linear-gradient(45deg, #b482ff, #9289ff, #9cecfd);");
    client.println("-webkit-background-clip: text;");
    client.println("-webkit-text-fill-color: transparent;");
    client.println("}");
    client.println(".text1 {");
    client.println("text-align: right;");
    client.println("padding: 10px 10px 10px 10px;");
    client.println("display: inline-block;");
    client.println("color: #9289ff;");
    client.println("margin-left: 75px;");
    client.println("text-decoration: none;");
    client.println("font-size: 50px;");
    client.println("outline: solid;");
    client.println("margin-top: 34px;");
    client.println("}");
    client.println(".buttons {");
    client.println("text-align: left;");
    client.println("margin-left: 75px;");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<!--- Top --->");
    client.println("<div>");
    client.println("<p class=\"krizik\">VOŠ a SPŠE Františka Křižíka</p>");
    client.println("<span class=\"logo\"></span>");
    client.println("<p class=\"text\">Zde můžete upravovat módy vašeho modelu</p>");
    client.println("</div>");
    client.println("");
    client.println("<!--- Tlačítko Další --->");
    client.println("<div>");
    client.println("<a class=\"nextmode\" href=\"/?nextmode\">");
    client.println("<button class=\"nextbutton\" type=\"button\" name=\"nextmode\">Další mód</button>");
    client.println("</a>");
    client.println("</div>");
    client.println("");
    client.println("<!--- Tlačítka --->");
    client.println("<div class=\"buttons\">");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 1 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog1\">");
    client.println("<button type=\"button\" name=\"prog1\">Mód č.1</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Static</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 2 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog2\">");
    client.println("<button type=\"button\" name=\"prog2\">Mód č.2</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Color Wipe</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 3 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog3\">");
    client.println("<button type=\"button\" name=\"prog3\">Mód č.3</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Color Wipe Random</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 4 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog4\">");
    client.println("<button type=\"button\" name=\"prog4\">Mód č.4</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Multi Dynamic</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 5 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog5\">");
    client.println("<button type=\"button\" name=\"prog5\">Mód č.5</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Rainbow Cycle</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 6 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog6\">");
    client.println("<button type=\"button\" name=\"prog6\">Mód č.6</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Scan</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 7 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog7\">");
    client.println("<button type=\"button\" name=\"prog7\">Mód č.7</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Theater Chase Rainbow</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 8 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog8\">");
    client.println("<button type=\"button\" name=\"prog8\">Mód č.8</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Twinkle Fade Random</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 9 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog9\">");
    client.println("<button type=\"button\" name=\"prog9\">Mód č.9</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Chase Random</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 10 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog10\">");
    client.println("<button type=\"button\" name=\"prog10\">Mód č.10</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Color Sweep Random</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 11 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog11\">");
    client.println("<button type=\"button\" name=\"prog11\">Mód č.11</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Larson Scanner</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 12 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog12\">");
    client.println("<button type=\"button\" name=\"prog12\">Mód č.12</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Fireworks Random</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 13 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog13\">");
    client.println("<button type=\"button\" name=\"prog13\">Mód č.13</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">TwinkleFOX</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- Tlačítko 14 --->");
    client.println("<div>");
    client.println("<a href=\"/?prog14\">");
    client.println("<button type=\"button\" name=\"prog14\">Mód č.14</button>");
    client.println("</a>");
    client.println("<p class=\"text1\">Rainbow Slide</p>");
    client.println("</div>");
    client.println("<br><br>");
    client.println("");
    client.println("<!--- xxx --->");
    client.println("</div>");
    client.println("</body>");
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
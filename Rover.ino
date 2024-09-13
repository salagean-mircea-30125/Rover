#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "SMCI";
const char* password = "mirceagabi";

const int ftdr = 12;  // Pin for LED 1
const int spdr = 14;  // Pin for LED 2
const int spst = 13; // Pin for LED 3
const int ftst = 15; // Pin for LED 4

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>Controller</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .container {
        display: flex;
        justify-content: space-between;
        padding: 20px;
      }
      .left, .right {
        display: flex;
        flex-direction: column;
        gap: 20px;
      }
      .right {
        flex-direction: row;
      }
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <div class="container">
      <div class="left">
        <button class="button" onmousedown="toggleCheckbox('on1');" ontouchstart="toggleCheckbox('on1');" onmouseup="toggleCheckbox('off1');" ontouchend="toggleCheckbox('off1');">/\</button>
        <button class="button" onmousedown="toggleCheckbox('on2');" ontouchstart="toggleCheckbox('on2');" onmouseup="toggleCheckbox('off2');" ontouchend="toggleCheckbox('off2');">\/</button>
      </div>
      <div class="right">
        <button class="button" onmousedown="toggleCheckbox('on3');" ontouchstart="toggleCheckbox('on3');" onmouseup="toggleCheckbox('off3');" ontouchend="toggleCheckbox('off3');"><</button>
        <button class="button" onmousedown="toggleCheckbox('on4');" ontouchstart="toggleCheckbox('on4');" onmouseup="toggleCheckbox('off4');" ontouchend="toggleCheckbox('off4');">></button>
      </div>
    </div>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

void setup() {

  pinMode(spdr, OUTPUT);
digitalWrite(spdr, LOW);

  pinMode(ftdr, OUTPUT);
digitalWrite(ftdr, LOW); 

  pinMode(spst, OUTPUT);
digitalWrite(spst, LOW);

  pinMode(ftst, OUTPUT);
digitalWrite(ftst, LOW);



  delay(2000);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Nu s-a putut conecta");
    return;
  }
  Serial.println();
  Serial.print("IP ");
  Serial.println(WiFi.localIP());
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("sus");
    digitalWrite(ftdr, HIGH); 
    digitalWrite(ftst, HIGH);
    request->send(200, "text/plain", "ok");
  });

  server.on("/off1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(ftdr, LOW);
    digitalWrite(ftst, LOW);
    request->send(200, "text/plain", "ok");
  });

  server.on("/on2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("jos");
    digitalWrite(spdr, HIGH); 
    digitalWrite(spst, HIGH);
    request->send(200, "text/plain", "ok");
  });

  server.on("/off2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(spdr, LOW); 
    digitalWrite(spst, LOW);
    request->send(200, "text/plain", "ok");
  });

  server.on("/on3", HTTP_GET, [] (AsyncWebServerRequest *request) {
     Serial.println("stanga"); 
    digitalWrite(ftst, HIGH);
    request->send(200, "text/plain", "ok");
  });

  server.on("/off3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(ftst, LOW);  
    request->send(200, "text/plain", "ok");
  });

  server.on("/on4", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("dreapta");
    digitalWrite(ftdr, HIGH);
    request->send(200, "text/plain", "ok");
  });

  server.on("/off4", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(ftdr, LOW);  // Turn off LED 4
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Empty loop, as everything is handled by the server
}
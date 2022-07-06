#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  D8         // L298N in1 motors Right           GPIO15(D8)
#define IN_2  D7        // L298N in2 motors Right           GPIO13(D7)
#define IN_3  D4            // L298N in3 motors Left            GPIO2(D4)
#define IN_4  D3          // L298N in4 motors Left            GPIO0(D3)
  
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "nuar046_2.4G@unifi";
const char* pwd = "PASSWORD";
WiFiServer server(80);

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, pwd);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
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

void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void goBackRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  char* value = "STOP";
  if (request.indexOf("/GO=FWD") != -1)  {
    goAhead();
    value = "FORWARD";
  }
  if (request.indexOf("/GO=BACK") != -1)  {
    goBack();
    value = "BACK";
  }
  if (request.indexOf("/GO=RIGHT") != -1)  {
    goRight();
    value = "RIGHT";
  }
  if (request.indexOf("/GO=LEFT") != -1)  {
    goLeft();
    value = "LEFT";
  }
  if (request.indexOf("/GO=STOP") != -1)  {
    stopRobot();
    value = "STOP";
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Car is now: ");
  client.print(value); 
  client.println("<br><br>");
  client.println("<a href=\"/GO=FWD\"\"><button>Forward</button></a>");
  client.println("<a href=\"/GO=BACK\"\"><button>Back</button></a><br />");
  client.println("<a href=\"/GO=LEFT\"\"><button>LEFT</button></a>");
  client.println("<a href=\"/GO=RIGHT\"\"><button>RIGHT</button></a><br />");
  client.println("<a href=\"/GO=STOP\"\"><button>STOP</button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
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
  else if (request.indexOf("/GO=ALEFT") != -1)  {
    goAheadLeft();
    value = "AHEAD LEFT";
  }
  else if (request.indexOf("/GO=ARIGHT") != -1)  {
    goAheadRight();
    value = "AHEAD RIGHT";
  }
  else if (request.indexOf("/GO=BACK") != -1)  {
    goBack();
    value = "BACK";
  }
  else if (request.indexOf("/GO=BLEFT") != -1)  {
    goBackLeft();
    value = "BACK LEFT";
  }
  else if (request.indexOf("/GO=BRIGHT") != -1)  {
    goBackRight();
    value = "BACK RIGHT";
  }
  else if (request.indexOf("/GO=RIGHT") != -1)  {
    goRight();
    value = "RIGHT";
  }
  else if (request.indexOf("/GO=LEFT") != -1)  {
    goLeft();
    value = "LEFT";
  }
  else {
    stopRobot();
    value = "STOP";
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!doctype html> <html lang=\"en\"> <head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\"> <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.8.3/font/bootstrap-icons.css\"> <title>Arduino Car Controller</title> </head> <body> <h1>Car is now</h1>");
  client.print(value); 
  client.println("<div class=\"container\"> <div class=\"row\"> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=ALEFT\" role=\"button\"> <svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-up-left-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0zm-5.904 2.803a.5.5 0 1 0 .707-.707L6.707 6h2.768a.5.5 0 1 0 0-1H5.5a.5.5 0 0 0-.5.5v3.975a.5.5 0 0 0 1 0V6.707l4.096 4.096z\"></path> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=FWD\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-up-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M16 8A8 8 0 1 0 0 8a8 8 0 0 0 16 0zm-7.5 3.5a.5.5 0 0 1-1 0V5.707L5.354 7.854a.5.5 0 1 1-.708-.708l3-3a.5.5 0 0 1 .708 0l3 3a.5.5 0 0 1-.708.708L8.5 5.707V11.5z\"/> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=ARIGHT\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-up-right-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M0 8a8 8 0 1 0 16 0A8 8 0 0 0 0 8zm5.904 2.803a.5.5 0 1 1-.707-.707L9.293 6H6.525a.5.5 0 1 1 0-1H10.5a.5.5 0 0 1 .5.5v3.975a.5.5 0 0 1-1 0V6.707l-4.096 4.096z\"/> </svg></a></div> </div> </div> </br> <div class=\"row\"> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=LEFT\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-left-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M8 0a8 8 0 1 0 0 16A8 8 0 0 0 8 0zm3.5 7.5a.5.5 0 0 1 0 1H5.707l2.147 2.146a.5.5 0 0 1-.708.708l-3-3a.5.5 0 0 1 0-.708l3-3a.5.5 0 1 1 .708.708L5.707 7.5H11.5z\"/> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-danger\" href=\"/GO=STOP\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-stop-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0zM6.5 5A1.5 1.5 0 0 0 5 6.5v3A1.5 1.5 0 0 0 6.5 11h3A1.5 1.5 0 0 0 11 9.5v-3A1.5 1.5 0 0 0 9.5 5h-3z\"/> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=RIGHT\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-right-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M8 0a8 8 0 1 1 0 16A8 8 0 0 1 8 0zM4.5 7.5a.5.5 0 0 0 0 1h5.793l-2.147 2.146a.5.5 0 0 0 .708.708l3-3a.5.5 0 0 0 0-.708l-3-3a.5.5 0 1 0-.708.708L10.293 7.5H4.5z\"/> </svg></a></div> </div> </div> </br> <div class=\"row\"> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=BLEFT\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-down-left-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M16 8A8 8 0 1 0 0 8a8 8 0 0 0 16 0zm-5.904-2.803a.5.5 0 1 1 .707.707L6.707 10h2.768a.5.5 0 0 1 0 1H5.5a.5.5 0 0 1-.5-.5V6.525a.5.5 0 0 1 1 0v2.768l4.096-4.096z\"/> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=BACK\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-down-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0zM8.5 4.5a.5.5 0 0 0-1 0v5.793L5.354 8.146a.5.5 0 1 0-.708.708l3 3a.5.5 0 0 0 .708 0l3-3a.5.5 0 0 0-.708-.708L8.5 10.293V4.5z\"/> </svg></a></div> </div> <div class=\"col\"> <div class=\"p-3 border bg-light\"><a class=\"btn btn-primary\" href=\"/GO=BRIGHT\" role=\"button\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-arrow-down-right-circle-fill\" viewBox=\"0 0 16 16\"> <path d=\"M0 8a8 8 0 1 1 16 0A8 8 0 0 1 0 8zm5.904-2.803a.5.5 0 1 0-.707.707L9.293 10H6.525a.5.5 0 0 0 0 1H10.5a.5.5 0 0 0 .5-.5V6.525a.5.5 0 0 0-1 0v2.768L5.904 5.197z\"/> </svg></a></div></div></div></div></body></html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
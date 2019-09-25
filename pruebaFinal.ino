//============================================================= L I B R E R I A S ==================================
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//============================================================= L I B R E R I A S ==================================
//=================================================================== P I N E S ==========================================
int btn1_pin_d5 = 14;
int btn2_pin_d6 = 12;
int sw1_pin_d2 = 4;
int sw2_pin_d1 = 5;
//=================================================================== P I N E S ==========================================
//============================================================= C R E D E N C I A L E S  WI FI======================

const char *ssid = "lalo-pc";           //"TP-LINK_A89012";
const char *password = "RjrbWLxi"; //"63004496";

//const char *ssid = "IZZI-1950";           //"TP-LINK_A89012";
//const char *password = "3C0461011950"; //"63004496";
//============================================================= C R E D E N C I A L E S  WI FI======================

//============================================================= V A R I A B L E S ==================================
int valorBt1 = LOW;
int valorBt2 = LOW;
int valorSw1 = LOW;
int valorSw2 = LOW;

int estado = 0;

int gBt1 = 0;
int gBt2 = 0;
int gS1 = 0;
int gS2 = 0;

int pBt = 0;
int pSw = 0;
//============================================================= V A R I A B L E S ==================================
void setup()
{
  Serial.begin(9600);       //Serial connection
                              //============================================================ W I F I  S E T  U P
  WiFi.begin(ssid, password); //WiFi connection

  while (WiFi.status() != WL_CONNECTED)
  { //Wait for the WiFI connection
    delay(500);
    Serial.println("Waiting for connection...");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //============================================================ W I F I  S E T  U P =================
  //============================================================== P I N M O D E S ===================
  pinMode(btn1_pin_d5, INPUT_PULLUP);
  pinMode(btn2_pin_d6, INPUT_PULLUP);
  pinMode(sw1_pin_d2, INPUT);
  pinMode(sw2_pin_d1, INPUT);
}

void loop()
{
  //edo sensores
  valorBt1 = digitalRead(btn1_pin_d5);
  valorBt2 = digitalRead(btn2_pin_d6);
  valorSw1 = digitalRead(sw1_pin_d2);
  valorSw2 = digitalRead(sw2_pin_d1);

  switch (estado)
  {
  case 0:
    gBt1 = valorBt1;
    gBt2 = valorBt2;
    gS1 = valorSw1;
    gS2 = valorSw2;
    if (valorBt1 == HIGH)
    {
      estado = 1;
    }
    if (valorBt2 == HIGH)
    {
      estado = 1;
    }
    break;
  case 1:
    //aqui se envia o se imprimen los datos
    if (gBt1 == 1 && gBt2 == 0)
    {
      pBt = 1;
    }
    else if (gBt1 == 0 && gBt2 == 1)
    {
      pBt = 2;
    }
    
    if (gS1 ==1  && gS2 == 0)
    {
      pSw = 1;
    }
    else if (gS1 == 0 && gS2 == 1)
    {
      pSw = 2;
    }
    else if (gS1 == 0 && gS2 == 0)
    {
      pSw = 0;
    }else if (gS1 == 1 && gS2 == 1)
    {
      pSw = 3;
    }

    
    estado = 2;
    break;
  case 2:
    postDatos(pBt,pSw);
    estado = 0;
    //aqui termina
    break;
  }

  Serial.println("-------------------------------------------------");
  delay(1000);
}

//=================================================================== funciones
void postDatos(int btn, int sw)
{
  //creacion de cliente http
  HTTPClient http;
  //ruta de api
  //http.begin("http://172.21.70.65:5000/postjson");
  http.begin("http://10.42.0.1:8000/api/postjson");
  http.addHeader("Content-Type", "application/json");
  //creacion de json
  StaticJsonBuffer<200> jsonBuffer;
  char json[256];
  JsonObject &root = jsonBuffer.createObject();
  root["btn"] = btn;
  root["sw"] = sw;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  //envio de solicitud
  int httpCode = http.POST(json);
  http.end(); //Close connection
}

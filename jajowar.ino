//************************************************************
// nodeId = 2133788377
//
//************************************************************
#include "painlessMesh.h"
#include "mash_parameter.h"
#include "CRCMASH.h"


Scheduler userScheduler; 

class Button {
public:
void read_But () {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        //Serial.println("Кнопка натиснута");
        pinMode(5, OUTPUT);
        sendB("jajo_start");
      }
    }
  }
  
  lastButtonState = reading;
}
private:

const int buttonPin = 4;
int buttonState;         
int lastButtonState = LOW;   
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 20; 
} button;


void handleBody( const String &msg ) {

  String str1 = msg.c_str();
  String str2 = "jajo";
  String str3 = "jajoeh";

  if (str1.equals(str2)) {
    pinMode(5, OUTPUT);
    sendB("jajo_start");
  }
  if (str1.equals(str3)) {
    sendB("jaeh");
  }
}

unsigned long previousMillis = 0;
const long interval = 15000; 
bool messageSent = false;  // Прапорець для відстеження відправки повідомлення

void setup() { 
  Serial.begin(115200);

  pinMode(5, INPUT);
  pinMode(4, INPUT_PULLUP);

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);

  previousMillis = millis();

}

void loop() {

  if (!messageSent) { // Перевіряємо, чи повідомлення ще не було відправлено
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      sendB("jajo_on");
      // Встановлюємо прапорець, щоб більше не відправляти повідомлення
      messageSent = true;
    }
  }

  // === CRCMASH queue processing ===
  for (uint8_t _i=0; _i<4; ++_i){ String _b; if (!qPop(_b)) break; handleBody(_b); }
  mesh.update();

  button.read_But();
}
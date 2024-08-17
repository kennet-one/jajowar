//************************************************************
// nodeId = 2133788377
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "kennet"
#define   MESH_PASSWORD   "kennet123"
#define   MESH_PORT       5555

Scheduler userScheduler; 
painlessMesh  mesh;

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
        Serial.println("Кнопка натиснута");
        pinMode(5, OUTPUT);
        mesh.sendSingle(624409705,"jajo_start");
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


void receivedCallback( uint32_t from, String &msg ) {

  String str1 = msg.c_str();
  String str2 = "jajo";

  if (str1.equals(str2)) {
    pinMode(5, OUTPUT);
    mesh.sendSingle(624409705,"jajo_start");
  }
}

unsigned long previousMillis = 0;
const long interval = 15000; 
bool messageSent = false;  // Прапорець для відстеження відправки повідомлення

void setup() { 
  Serial.begin(115200);

  pinMode(5, INPUT);
  pinMode(4, INPUT_PULLUP);

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);

  previousMillis = millis();

}

void loop() {

  if (!messageSent) { // Перевіряємо, чи повідомлення ще не було відправлено
    unsigned long currentMillis = millis();

    // Перевіряємо, чи минуло 5 секунд
    if (currentMillis - previousMillis >= interval) {
      // Відправляємо повідомлення після 5 секунд
      mesh.sendSingle(624409705, "jajo_on");
      
      // Встановлюємо прапорець, щоб більше не відправляти повідомлення
      messageSent = true;
    }
  }

  mesh.update();

  button.read_But();
}

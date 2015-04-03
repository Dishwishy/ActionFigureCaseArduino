#define WITH_LCD 0

#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

int led = 3;           // the pin that the LED is attached to
int led2 = 4;
int brightness = 0;    // how bright the LED is
int brightness2 = 0;
int selectedLed = -1;
int dblClickState = 0;

void setup() {
  Serial.begin(9600);
  encoder = new ClickEncoder(7,5,6);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  pinMode(led, OUTPUT);
  pinMode(led2,OUTPUT);
  last = -1;
}

void loop() {  
  value += encoder->getValue();
  
  if (value != last) {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);

  }
  
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      case ClickEncoder::DoubleClicked:
          Serial.println("ClickEncoder::DoubleClicked");
          encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
          Serial.print("  Acceleration is ");
          Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");

        break;
    }
  }
  if( b == ClickEncoder::DoubleClicked ){
    Serial.print("caught dbl click\n");  
    if(dblClickState == 0){
        dblClickState = 1;
        Serial.print("LED1 selected\n");
        selectedLed = led;
        for(int x=0;x<3;x++){
        analogWrite(selectedLed, 255);
        delay(1000);
        analogWrite(selectedLed,0);  
        }   
         while (b != ClickEncoder::Clicked){
          value += encoder->getValue();
          analogWrite(selectedLed, value);
           b = encoder->getButton();
            }

        }
    
    if(dblClickState == 1){
      Serial.print("LED2 selected\n");

        b = encoder->getButton();
       selectedLed = led2;
        for(int y=0;y<3;y++){
        analogWrite(selectedLed, 255);
        delay(500);
        analogWrite(selectedLed,0); 
        }
        while (b != ClickEncoder::Clicked){
          value += encoder->getValue();
          analogWrite(selectedLed, value);
          b = encoder->getButton();
        }
      dblClickState = 0;

     }
  }  
}


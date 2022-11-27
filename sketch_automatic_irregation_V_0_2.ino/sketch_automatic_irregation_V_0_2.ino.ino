#define MOISTER_DEFAULT_LEVEL 850 //970
#define RELAY_PIN A2
#define GREEN_PIN 10
#define BLUE_PIN 11
#define RED_PIN 12

//Music
#define BEAT 300
#define PIN 10

//misture sensor
int water_count = 0;

//Timer
int counter = 0;


void setup()
 {
    Serial.begin(9600);    
    
    // put your setup code here, to run once:
    pinMode(A0, INPUT_PULLUP);           // Soil Moisture Sensor 1 PIN A0
    pinMode(RELAY_PIN,OUTPUT);                             // Relay Module PIN D8

    digitalWrite(RELAY_PIN, HIGH);

    digitalWrite(RED_PIN, HIGH);
    delay(500);  
    digitalWrite(BLUE_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_PIN, LOW);
    delay(500);
    digitalWrite(BLUE_PIN, LOW);
    delay(500);
    digitalWrite(RED_PIN, LOW);  
    delay(500);

}

void loop() 
{

  //Check soil humidity once a minute.
  if(counter == 1){
    checkMoisture();
  }

  delay(10000);//10sec 10000
  counter++;
  
  if(counter >= 6){
    counter = 0;
  }
  showWateringStatus();
}


void checkMoisture () {

  int moistureValue = analogRead(A0);
  Serial.print("Moisture Sensor Value:");
  Serial.println(moistureValue);

  if (moistureValue > MOISTER_DEFAULT_LEVEL){
    water_count++;
    Serial.print("water_count Value:");
    Serial.println(water_count);
    if (water_count == 5) {//To wait for the water to go through the pot.
      watering();
      water_count = 0;
    }
  } 
}

//Watering
void watering(){
  digitalWrite(RELAY_PIN, LOW);
  delay(8000);
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);
  completeWatering();
  counter++; 
}

//Music
void completeWatering(){
  //Let them know that watering is complete.
     /* tone(PIN,DO,BEAT) ; // C
      delay(BEAT) ;
      tone(PIN,RE,BEAT) ; // D
      delay(BEAT) ;
      tone(PIN,MI,1200) ; // E
      delay(BEAT) ;
      delay(BEAT) ;
      delay(BEAT) ;
      tone(PIN,RE,BEAT) ; // D
      delay(BEAT) ;
      tone(PIN,DO,BEAT) ; // C
      delay(BEAT) ;
      delay(BEAT) ;
      tone(PIN,DO,BEAT) ; // C
      delay(BEAT) ;
      tone(PIN,RE,BEAT) ; // D
      delay(BEAT) ;
      tone(PIN,MI,BEAT) ; // E
      delay(BEAT) ;
      tone(PIN,RE,BEAT) ; // D
      delay(BEAT) ;
      tone(PIN,DO,BEAT) ; // C
      delay(BEAT) ;
      tone(PIN,RE,1200) ; // D
      delay(BEAT) ;
      delay(BEAT) ;*/
  delay(4400);
      
  counter++;
}

void showWateringStatus() {

 int value = analogRead(A0);
  if (value < 300) { // HIGH
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, LOW);  
  } else if (value > 300 && value < MOISTER_DEFAULT_LEVEL) { // MID
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);  
  } else if (value > MOISTER_DEFAULT_LEVEL) { // DRY
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);  
  }
}



#include <Wire.h>
//BME280
#include <Adafruit_BME280.h>
//OLED
#include <I2CLiquidCrystal.h>

//------------------------------

//BME280
#define I2C_SCL 5
#define I2C_SDA 4
Adafruit_BME280 bme;

//OLED
#define I2C_ADDR 0x3c
#define BRIGHT 127
I2CLiquidCrystal oled(I2C_ADDR, (uint8_t)BRIGHT);

//Music
#define BEAT 300
#define PIN 10

#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SO 392
#define RA 440
#define SI 494
#define HDO 523

//----------------------

//Timer
int counter = 0;

//Ultrasonic Sensor
int Trig = 8;
int Echo = 9;
int Duration;
float Distance;

//LED
int led = 7;

//misture sensor
int water_count = 0;

//relay
int relay = 13;

void setup() {
  //BME280
  Serial.begin(9600);
  Wire.begin();
  if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  delay(100);

  //OLED
  oled.begin(16, 2);
  oled.display();
  oled.clear();
  oled.noBlink();
  oled.noCursor();
  oled.home();
  oled.print("Wait a minutes.");

  //Ultrasonic Sensor
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);

  //LED
  pinMode(led,OUTPUT);

  //relay
  pinMode(relay, OUTPUT);
}

void loop() {
  
  //Check temperature, humidity, and soil humidity once a minute.
  if(counter == 1){
    checkMoisture();
  }

  delay(10000);//10sec
  counter++;
  
  if(counter >= 6){
    counter = 0;
  }

}

//BME280
void ckeckBME280(){
  // Get data from BME280 sensor
  float t = bme.readTemperature();       // C
  float h = bme.readHumidity();          // %
  float p = bme.readPressure() / 100.0F; // hPa
  Serial.print("Temperature:");
  Serial.println(t);
  Serial.print("Humidity:");
  Serial.println(h);
  Serial.print("Pressure:");
  Serial.println(p);
  Serial.println();

  oled.setCursor(0, 0);
  oled.print(String(t)+"C ");
  oled.print(String(h)+"%    ");
}

//Ultrasonic Sensor
void checkWater(){
  //Check the water level in the bucket.
  digitalWrite(Trig,LOW);
  delayMicroseconds(1);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(11);
  digitalWrite(Trig,LOW);
  Duration = pulseIn(Echo,HIGH);
  
  if (Duration>0) {
    Distance = Duration/2;
    Distance = Distance*340*100/1000000; // ultrasonic speed is 340m/s = 34000cm/s = 0.034cm/us 
    Serial.print(Distance);
    Serial.println(" cm");
    
    if(Distance > 12){
      digitalWrite(led, HIGH);
    }else{
      digitalWrite(led, LOW);
    }
    
  }
}

//moisture sensor
void checkMoisture(){
  //Measure soil humidity
  int moisture = analogRead(A0);
  Serial.print("Moisture Sensor Value:");
  Serial.println(moisture);
  oled.setCursor(0, 1);
  oled.print("Moisture: " + String(moisture) + "   ");
  
  if(moisture <= 300){
    water_count++;
    if(water_count == 5){//To wait for the water to go through the pot.
      watering();
      water_count = 0;
    }
  }
}


//Watering
void watering(){
  digitalWrite(relay, HIGH);
  delay(2000);
  digitalWrite(relay, LOW);
  delay(8000);
  completeWatering();
  counter++; 
}

//Music
void completeWatering(){
  //Let them know that watering is complete.
  oled.setCursor(0, 0);
  oled.print("   Thank you!   ");
  oled.setCursor(0, 1);
  oled.print("   (^ O ^)/     ");
  
      tone(PIN,DO,BEAT) ; // C
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
      delay(BEAT) ;
  delay(4400);
      
  counter++;
  oled.setCursor(0, 0);
  oled.print("                  ");
  oled.setCursor(0, 1);
  oled.print("                  ");
  ckeckBME280();
  int moisture = analogRead(A0);
  oled.setCursor(0, 1);
  oled.print("Moisture: " + String(moisture) + "   ");
}
#include <max6675.h>
#include <SoftwareSerial.h> 
#include <TinyGPS.h> 


#define Pressure12bar_analogPin A1
#define max6675_SO 8
#define max6675_CS 9
#define max6675_CLK 10


#define GPS_Tx 4
#define GPS_Rx 3

float lat = 0.000000, lon = 0.000000;

MAX6675 sensor(max6675_CLK, max6675_CS, max6675_SO);

SoftwareSerial gpsSerial(GPS_Tx,GPS_Rx);
TinyGPS gps;

void setup() {

  gpsSerial.begin(9600);
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
}

void loop() {
  
  float Vcc = readVcc() / 1000.0;
  float Pressure_voltage = (analogRead(Pressure12bar_analogPin) / 1024.0) * Vcc;
  float Press = (Pressure_voltage/Vcc-0.1)/0.6666666666667; 
  float temp=sensor.readCelsius();

  while(gpsSerial.available()){ 
  if(gps.encode(gpsSerial.read())) 
    gps.f_get_position(&lat,&lon);
  }
  
  
  Serial.print("Vcc=");
  Serial.print(Vcc);
  Serial.print(" Volt,  ");
  Serial.print("A1=");
  Serial.print(Pressure_voltage);
  Serial.print(" Volt,  ");
  Serial.print("Pressure=");
  Serial.print(Press);
  Serial.print("MPa;;  ");
  Serial.print("Temp=");
  Serial.print(temp);
  Serial.println(" ºC");
  String latitude = String(lat,6); 
  String longitude = String(lon,6); 
  Serial.println("Lon: "+longitude+";"+" Lat: "+latitude);
  delay(30000);  
}

long readVcc() {
   ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
   delay(2);
   ADCSRA |= _BV(ADSC);
   while (bit_is_set(ADCSRA, ADSC));
   
   long result = ADCL;
   result |= ADCH << 8;
   
   result = 1126400L / result; 
   return result;
}

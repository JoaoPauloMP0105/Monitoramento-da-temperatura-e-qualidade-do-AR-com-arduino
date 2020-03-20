/*
 * Codigo desenvolvido por JOÃO PAULO 
 * Parte do codigo tirado do site da flip flop e utilização da biblioteca deles também
 * mas a ideia e como mostrar partiu de minha autoria mas agradeço muito o pessoal da 
 * flip flop por disponibilizar muitos materias de ajuda.
 * Sistema de monitoramento da umidade do ar e temperatura apresentando 
 * dados no serial e no lcd facilitando assim a leitura dos dados.
 */
 /*
  * Foi incrementado o acionamento do servo motor (9G) através   
  * do acionamento do sensor ultrasonic (SR04)
  * para acionar uma carga quando a distancia estiver a menos de 25cm de distancia.
  */

#include <Servo.h>
#include "Ultrasonic.h"

Ultrasonic ultrasonic(10,11);

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int distancia;

int temp2= 2000;

#include "DHT.h"

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
 
// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) 
// e ao pino 1 (VCC) do sensor

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void monitoramento();

void setup() 
{
   // initialize the lcd 
  lcd.init();                     
  lcd.init();
  
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  
  dht.begin();

  myservo.attach(13);  // attaches the servo on pin 9 to the servo object
  
}
 
void loop() 
{
  lcd.backlight();
  apresentacao();
  monitoramento();
  acionamento_alavanca();
}
void monitoramento()
{
  acionamento_alavanca();
  
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    
    lcd.setCursor(1,0);
    lcd.print("Failed to read from DHT");
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Umi.AR: ");
    lcd.print(h);
    lcd.setCursor(14,0);
    lcd.print("%");
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print("%");
    lcd.setCursor(0,1);
    lcd.print("Temperatura:");
    lcd.setCursor(12,1);
    lcd.print(t);
    lcd.setCursor(14,1);
    lcd.print("C");
    lcd.setCursor(15,1);
    lcd.write(B11011111); // codigo binario para mostrar o grau celcius na tela do lcd unica forma de ser apresentado com o caracter
    Serial.print(" \t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    delay(temp2);
    lcd.clear();
    
  }
}
void apresentacao()
{
  acionamento_alavanca();
  lcd.setCursor(1,0);
  lcd.print("Monitoramento");
  lcd.setCursor(1,1);
  lcd.print("DO AR e TEMP");
  delay(temp2);
}
void acionamento_alavanca()
{
   distancia = (ultrasonic.Ranging(CM));
  if (distancia >= 25) {
    Serial.println("Não acionar");
    pos = 170;
    myservo.write(pos); 
  }
  if (distancia < 25){
    Serial.println(" acionar");
    pos = 20;
    myservo.write(pos); 
    
  }
  
}

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SparkFun_APDS9960 apds = SparkFun_APDS9960();

uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int color = 0;

char red_hex[2] = "00", green_hex[2] = "00", blue_hex[2] = "00";

void convertDECtoHEX(uint16_t value, char (&hex)[2])
{
  uint16_t remainder, quotient = value;
  uint8_t i=1;

  while(quotient!=0)
  {
    remainder = quotient%16;

    if(remainder<10)
    hex[i--]=48+remainder;
    else
    hex[i--]=55+remainder;

    quotient=quotient/16;
  }
}

void zbierz_dane_z_czujnika()
{
  if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
    lcd.println("Error reading light values");
  } else {
    lcd.print("Ambient: ");
    lcd.print(ambient_light);
    lcd.print(" Red: ");
    lcd.print(red_light);
    lcd.print(" Green: ");
    lcd.print(green_light);
    lcd.print(" Blue: ");
    lcd.println(blue_light);
  }
  
  // Wait 1 second before next reading
  delay(1000);
}

void konwertuj_dane_do_HEX()
{
convertDECtoHEX(red_light, red_hex);
convertDECtoHEX(blue_light, blue_hex);
convertDECtoHEX(green_light, green_hex);
}

void okresl_kolor()//miejsce na implementacje wyswietlania iformacji o kolorkach  (Milosz wsadz swoj kod tutaj) ( Dobra ~Miłosz)
{

 if (red_light > 250 && green_light> 250 && blue_light > 250)
 {
   color = 1;
 }
 else if (red_light < 25 && green_light < 25 && blue_light < 25) 
 {
   color = 2;
 }
 else if (red_light > 200 &&  green_light > 200 && blue_light < 100)
 {
   color = 4;
 }
 else if (red_light > 200 &&  green_light > 25 && blue_light < 100) 
 {
   color = 3;
 }
 else if (red_light > 200 &&  green_light < 100 && blue_light > 200) 
 {
   color = 5;
 }
 else if (red_light > 250 && green_light < 200 && blue_light < 200)
 {
   color = 6;
 }
 else if (red_light< 200 && green_light > 250 && blue_light < 200) 
 {
   color = 7;
 }
 else if (red_light< 200 && green_light < 200 && blue_light > 250) 
 {
   color = 8;
 }
 else 
 {
   color = 0;
 }
}




void wyswietl_kod_HEX()
{
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print(&red_hex);
  lcd.setCursor(2,0);
  lcd.print(&green_hex);
  lcd.setCursor(4,0);
  lcd.print(&blue_hex);
}

void wyswietl_kolor()//tutaj przekazywanie nazw kolorow po kodzie hex
{
  lcd.setCursor(0,1);
  switch (color) {
   case 1: lcd.println("WHITE"); break;
   case 2: lcd.println("BLACK"); break;
   case 3: lcd.println("ORANGE"); break;
   case 4: lcd.println("YELLOW"); break;
   case 5: lcd.println("PURPLE"); break;
   case 6: lcd.println("RED"); break;
   case 7: lcd.println("GREEN"); break;
   case 8: lcd.println("BLUE"); break;
   default: lcd.print("unknown"); break;


}
}
void wyswietl()
{
  wyswietl_kod_HEX();
  wyswietl_kolor();
}

void setup() 
{
  lcd.init();
  lcd.backlight();

    // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    lcd.println(F("APDS-9960 initialization complete"));
  } else {
    lcd.println(F("Something went wrong during APDS-9960 init!"));
  }
    // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    lcd.println(F("Light sensor is now running"));
  } else {
    lcd.println(F("Something went wrong during light sensor init!"));
  }
  
  // Wait for initialization and calibration to finish
  delay(1000);
}
 

void loop() 
{
zbierz_dane_z_czujnika();
konwertuj_dane_do_HEX();
okresl_kolor();
wyswietl();
}
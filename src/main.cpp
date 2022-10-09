//    ATTINY 85 Seindecoder met Neopixel en prog via Drukknop
//    by P de Heij
//    Versie 1.0 23-9-2022
//                       --------
//       Reset!/PB5 1  -|        |- 8 VCC
//     Drukknop/PB3 2  -| ATTiny |- 7 PB2/DCC_PIN
//              PB4 3  -|   85   |- 6 PB1/statusLED
//              GND 4  -|        |- 5 PB0/seinpin
//                       --------
// High Fuse D4
//      - Brown-out detection level at VCC=4,3;[BODLEVEL=100]
//      - Enable Serial Programming en data loading[1]
//      - EESAVE memory is preserved toriugh the chip erase [1]
// Low Fuse E2
//      - CKSEL=0010 SUT-10 Int RC osc 8Mhz
// Extend Fuse FF





// gebruikte libraries

#include <Arduino.h>
#include <NmraDcc.h>  // LET OP CV Programming en Functie dec UIT!
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// Definities

#define Versie 10
#define seinpin 0
#define aantalseinen 4
#define DCC_PIN 2
#define Drukknop 3
#define statusLED 1

// Variabele

int MODE;  // 0 = Nederlands 1 = Duits
bool program_stand = false;
long DCC_Adres;
long rood = 0xFF0000;       // rood
long groen = 0x00FF00;      // groen
long geel = 0x0000ff;       // geel
long geel_groen = 0x00FFFF; // geel/groen

Adafruit_NeoPixel sein(aantalseinen, seinpin, NEO_RGB);
NmraDcc Dcc;
DCC_MSG Packet;

// vertaal DCC naar sein
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{

  if ((Addr == DCC_Adres) & (Direction == 0))
  {
    sein.setPixelColor(0, rood);
    sein.show();
  }
  else if ((Addr == DCC_Adres) & (Direction == 1))
  {
    sein.setPixelColor(0, groen);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 1) & (Direction == 0))
  {
    if (MODE == 0)
    {
      sein.setPixelColor(0, geel);
      sein.show();
    }
    else if (MODE == 1)
    {
      sein.setPixelColor(0, geel_groen);
      sein.show();
    }
  }
  else if ((Addr == DCC_Adres + 2) & (Direction == 0))
  {
    sein.setPixelColor(1, rood);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 2) & (Direction == 1))
  {
    sein.setPixelColor(1, groen);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 3) & (Direction == 0))
  {
    if (MODE == 0)
    {
      sein.setPixelColor(1, geel);
      sein.show();
    }
    else if (MODE == 1)
    {
      sein.setPixelColor(1, geel_groen);
      sein.show();
    }
  }
  else if ((Addr == DCC_Adres + 4) & (Direction == 0))
  {
    sein.setPixelColor(2, rood);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 4) & (Direction == 1))
  {
    sein.setPixelColor(2, groen);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 5) & (Direction == 0))
  {
    if (MODE == 0)
    {
      sein.setPixelColor(2, geel);
      sein.show();
    }
    else if (MODE == 1)
    {
      sein.setPixelColor(2, geel_groen);
      sein.show();
    }
  }
  else if ((Addr == DCC_Adres + 6) & (Direction == 0))
  {
    sein.setPixelColor(3, rood);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 6) & (Direction == 1))
  {
    sein.setPixelColor(3, groen);
    sein.show();
  }
  else if ((Addr == DCC_Adres + 7) & (Direction == 0))
  {
    if (MODE == 0)
    {
      sein.setPixelColor(3, geel);
      sein.show();
    }
    else if (MODE == 1)
    {
      sein.setPixelColor(3, geel_groen);
      sein.show();
    }
  }

  if ((program_stand == 1) & (Direction == 0))
  {
    MODE = 0;  // Nederlands
    EEPROM.write(0, Addr);
    EEPROM.write(1, MODE);
    delay(1000);
    DCC_Adres = EEPROM.read(0);
    program_stand = 0;
    digitalWrite(Drukknop, HIGH);
    pinMode(Drukknop, INPUT_PULLUP);
  }

  if ((program_stand == 1) & (Direction == 1))
  {
    MODE = 1; // Duits
    EEPROM.write(0, Addr);
    EEPROM.write(1, MODE);
    delay(1000);
    DCC_Adres = EEPROM.read(0);
    program_stand = 0;
    digitalWrite(Drukknop, HIGH);
    pinMode(Drukknop, INPUT_PULLUP);
  }  
}

void setup()
{

  // init neopixels

  sein.begin();
  sein.setBrightness(25);
  sein.setPixelColor(0, rood);
  sein.setPixelColor(1, rood);
  sein.setPixelColor(2, rood);
  sein.setPixelColor(3, rood);
  sein.show();

  // zet pinnen

  pinMode(Drukknop, INPUT_PULLUP);

  // Lees adres en mode

  DCC_Adres = EEPROM.read(0);
  MODE = EEPROM.read(1);

  // start DCC

  Dcc.init(MAN_ID_DIY, Versie, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);
}

void loop()
{

  // luister naar DCC signaal

  Dcc.process();

  // kijk of programmeer schakelaar ingedrukt is

  byte program_schakelaar = digitalRead(Drukknop);

  if (program_schakelaar == 0)
  {
    program_stand = 1;
    pinMode(Drukknop, OUTPUT);
    digitalWrite(Drukknop, LOW);
  }
}

// include the library code:
#include <LiquidCrystal.h>

// ------------------------------------------------ //
//   CONSTANTES LCD                                 //
// ------------------------------------------------ //

#define LCDD4    A2
#define LCDD5    A3
#define LCDD6    A4
#define LCDD7    A5

#define LCDRS    A0
#define LCDEN    A1

// ------------------------------------------------ //
//    CONSTANTES ENCODEUR                           //
// ------------------------------------------------ //

#define encoder0PinA 2 
#define encoder0PinB 3 
volatile int encoder0Pos = 0;
int encodermov = 0;

#define LedR 5
#define LedV 6
#define LedB 7

// ------------------------------------------------ //
//                        Boutons                   //
// ------------------------------------------------ //

#define Btn 4

boolean BtnChange  = false;
boolean BtnPressed = false; 

// ------------------------------------------------ //
//   CONSTANTES PROJECTEUR                          //
// ------------------------------------------------ //

#define LedWarmPin    9
#define LedWarmTcolor 3000  // K
#define LedWarmPower  1080  // lm = 6 * 180
float   LedWarmPowerP = 90;

#define LedColdPin    10
#define LedColdTcolor 6500  // K
#define LedColdPower  1320  // lm = 6 * 220
float   LedColdPowerP = 110; 

float MinPower  =    0;
float MaxPower  = 2400;    

//float VPower =    0 ;
//float TColor = 6500 ;

float Color;
float Power;
 
// ------------------------------------------------ //
//                 Encoder                          //
// ------------------------------------------------ //

void doEncoder() {
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encodermov=-1;
  } else {
    encodermov=+1;
  }
}

#define clRED     1
#define clGREEN   2
#define clBLUE    3
#define clYELLOW  4
#define clPURPLE  5
#define clWHITE   7

void EncoderColor (int col ) {
  switch (col) {
    case clRED    :
      digitalWrite(LedR, LOW);
      digitalWrite(LedV, HIGH);
      digitalWrite(LedB, HIGH);
      break;
    case clGREEN  : 
      digitalWrite(LedR, HIGH);
      digitalWrite(LedV, LOW);
      digitalWrite(LedB, HIGH);
      break;
    case clBLUE   : 
      digitalWrite(LedR, HIGH);
      digitalWrite(LedV, HIGH);
      digitalWrite(LedB, LOW);
      break;
    case clYELLOW :
      digitalWrite(LedR, LOW);
      digitalWrite(LedV, LOW);
      digitalWrite(LedB, HIGH);
      break;
    case clPURPLE :
      digitalWrite(LedR, LOW);
      digitalWrite(LedV, HIGH);
      digitalWrite(LedB, LOW);
      break;
    case clWHITE :
      digitalWrite(LedR, LOW);
      digitalWrite(LedV, LOW);
      digitalWrite(LedB, LOW);
      break;
    default :    
      digitalWrite(LedR, HIGH);
      digitalWrite(LedV, HIGH);
      digitalWrite(LedB, HIGH);
    }
}
  
// ------------------------------------------------ //
//                  Boutons                         //
// ------------------------------------------------ //

boolean UpdateBtn () {
  boolean val;
  val=digitalRead(Btn);
  if (val==LOW) {
    BtnChange=true;    
  } else {
    if ( BtnChange) {
      BtnPressed=true;
    }
    BtnChange=false;
  }
}

boolean BtnPress () {
  boolean result;
  result=BtnPressed;
  BtnPressed=false;
  return result;
}

// ================================================ //
//                    SETUP                         //
// ================================================ //

 // initialize the library with the numbers of the interface pins
 LiquidCrystal lcd(LCDRS, LCDEN, LCDD4, LCDD5, LCDD6, LCDD7);

void setup () {
  Serial.begin(9600);      // open the serial port at 9600 bps:    

  // LCD 
  // ----------
  lcd.begin(8, 2);
  lcd.print("Init");
  
   // Leds Projecteur
  // ----------
  pinMode(LedWarmPin  , OUTPUT);
  pinMode(LedColdPin  , OUTPUT);
  digitalWrite(LedWarmPin, LOW);
  digitalWrite(LedColdPin, LOW);
 
//  MaxPower  = ( 100.0 * ( LedWarmPower + LedColdPower ) ) / min(LedWarmPower,LedColdPower);
  Color=5800;
  Power=10;
//  lcd.setCursor(0, 1);
//  lcd.print(int(MaxPower) );

  // encoder  
  // ----------
  pinMode(Btn         , INPUT);
  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
  attachInterrupt(0, doEncoder, CHANGE);    

  // Leds  
  // ----------
  pinMode(LedR   , OUTPUT);
  pinMode(LedV   , OUTPUT);
  pinMode(LedB   , OUTPUT);
  digitalWrite(LedR, HIGH);
  digitalWrite(LedV, HIGH);
  digitalWrite(LedB, HIGH);

  
  EncoderColor (clRED    ); delay(200);EncoderColor (0);   delay(50);
  EncoderColor (clGREEN  ); delay(200);EncoderColor (0);   delay(50);
  EncoderColor (clBLUE   ); delay(200);EncoderColor (0);   delay(50);
  EncoderColor (clYELLOW ); delay(200);EncoderColor (0);   delay(50);
  EncoderColor (clPURPLE ); delay(200);EncoderColor (0);   delay(50);
  EncoderColor (clWHITE  ); delay(200);EncoderColor (0);   delay(50);

  delay(100);

}

// ------------------------------------------------ //
//                                                  //
// ------------------------------------------------ //

void SetColdLed(float Pwr) {   
  float VCold;
  VCold=255.0 * Pwr;
//  VCold=VCold/LedColdPower;
  VCold = min (VCold, 255);
//  analogWrite(LedColdPin,255-VCold);
  analogWrite(LedColdPin,VCold);
}

void SetWarmLed(float Pwr) {   
  float VWarm; 
  VWarm=255.0 * Pwr;
//  VWarm=VWarm/LedWarmPower;
  VWarm = min (VWarm, 255);
//  analogWrite(LedWarmPin,255-VWarm);
  analogWrite(LedWarmPin,VWarm);
}

// ================================================ //
//                      LOOP                        //
// ================================================ //

int mode=0;

void loop () {
     
  float coef;
  float PwCold; 
  float PwWarm; 

  UpdateBtn();
  if ( BtnPress() ) {
    mode++;
    if (mode>2) mode=0;
  }
  
// Calcule 
//  VPower = getPower () ;
//  TColor = getTcolor() ;
 
 if (mode==0) {               // Encoder define total Power
    EncoderColor(clRED);
    Power+=5*encodermov;
    encodermov=0;
    Power=max(Power,0);
//    Power=min(Power,0);
    Power=min(Power,MaxPower);
  }

 if (mode==1) {              // Encoder define  Temperature
    EncoderColor(clGREEN);
    Color+=100*encodermov;
    encodermov=0;
    Color=max(Color,LedWarmTcolor);
    Color=min(Color,LedColdTcolor);
  }

  if (mode==2) {             // Encoder do Nothing , Safe mode
    EncoderColor(clBLUE);
  }
  
  delay(100); 
  
  coef = ( Color - LedColdTcolor );
  coef = coef / ( LedWarmTcolor - LedColdTcolor );
  
  PwWarm = Power * coef;
  PwWarm = min (PwWarm, LedWarmPowerP);
  
  PwCold = Power - PwWarm;
  PwCold = min (PwCold, LedColdPowerP);

  PwWarm = Power -  PwCold;
  PwWarm = min (PwWarm, LedWarmPowerP);
  
  SetColdLed(PwCold/LedColdPowerP);   
  SetWarmLed(PwWarm/LedWarmPowerP);
//  Serial.print("C:");
//  Serial.println(PwCold);
//  Serial.print("W:");
//  Serial.println(PwWarm);
  
  // Calcule temperature de couleur
  Color = ( LedColdTcolor * PwCold + LedWarmTcolor * PwWarm ) / ( PwCold + PwWarm );
  Color=max(Color,LedWarmTcolor);
  Color=min(Color,LedColdTcolor);
  
//  Power = 100 * ( PwCold + PwWarm ) / ( LedWarmPower + LedColdPower );
  Power = ( PwCold + PwWarm ) ;
  Power = min(Power,200);
  
  Serial.print("C:");
  Serial.println(Color);
  Serial.print("P:");
  Serial.println(Power);
      
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(int(Color));
  lcd.print(" K");

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(int(Power));
  lcd.print("% ");
}


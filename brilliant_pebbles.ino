#include <Adafruit_BMP280.h>

#define RED_LED 8
#define BLUE_LED 9
#define speakerOut 4
#define POWER_PIN 6

#define BEEP_LONG 238
#define BEEP_MED 126
#define BEEP_SHORT 119
#define D_SHORT 210
#define D_MED 1000
#define D_LONG 3000

Adafruit_BMP280 bmp;

unsigned long start;
unsigned long jumptime;
int alt0;
int alt, altm1=0, altm2=0, altm3=0, altm4=0, altm5=0;
bool altstate = 0;
int count = 0;

//beepLength, pauseLength, pulseLength
#define NUM_TONES 8
const int tones[9][3] = {{210,70,125}, //tone 1 high
                         {150,50,125}, //tone 2 high
                         {500,70,125}, //tone 3 high
                         {300,50,121}, //tone 4 mid
                         {500,70,121}, //tone 5 mid
                         {210,70,83},  //tone 6 mid
                         {150,50,83}, //tone 7 low
                         {300,70,297}, //tone 8 low
                         {500,70,297}, //tone 9 low
                         };
byte activeAlarms = 3;
int alarmAltitudes[3] = {305,1220,1676}; //altitudes in metres
int alarmDurations[3] = {1000,3000,3000}; //durations in milliseconds
byte alarmTones[3] = {5,3,2};

int hardDeckAlarm[3] = {600, 4000, 2}; //altitude in metres, duration in msecs and alarm tone
bool harddeckEnabled = true;

#define alt_trigger 30 //jump mode trigger altitude
bool climb_beeps_en = true;

void setup() {
  pinMode(speakerOut,OUTPUT);
  pinMode(POWER_PIN,INPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(RED_LED, LOW);
  playTone(210,D_SHORT,70,BEEP_SHORT); //power on beep
//################################################################
//--------------check if the barometer is working-----------------
//################################################################
  if(!bmp.begin(0x76, 0x58)){
    while(1) {
      playTone(210,D_SHORT,70,BEEP_MED);
    }
  }
  zero_alti();
}
//################################################################
//################################################################
void loop() {
  if (altstate) {
    jumpMode();
  }
  else {
      standbyMode();
  }

}
//################################################################
//################################################################
void standbyMode() {
  digitalWrite(BLUE_LED,HIGH);
  delay(10);
  digitalWrite(BLUE_LED, LOW);
   
  altm5 = altm4;
  altm4 = altm3;
  altm3 = altm2;
  altm2 = altm1;
  altm1 = alt;
  alt = (int) bmp.readAltitude() - alt0;
    
  if (alt < 0) {
    alt = 0;
    zero_alti();
  }
  if ((alt - altm5) >= 20) {
    //if climbing at just under 200fpm
    altstate = 1; //jump mode
    jumptime = 0;
    playTone(210,210,70,BEEP_MED);
  }
  else if ((alt - altm1) >=4) {
    //climb rate detected, do another loop
  }
  else if ((count%5)==0) {
    zero_alti();
  }
  delay(4000);
}
//################################################################
//################################################################
void jumpMode() {
  digitalWrite(BLUE_LED,HIGH);
  delay(3);
  digitalWrite(BLUE_LED, LOW);
  jumptime++;

  altm1 = alt;
  alt = (int) bmp.readAltitude() - alt0;

  for (int i=0;i<activeAlarms;i++) {
    int k = alarmTones[i];
    alarmat(alarmAltitudes[i],tones[k][0],tones[k][1],500,alarmDurations[i],tones[k][2]);
  }
 
  //harddeck, on the way down.
  if( harddeckEnabled )
    alarmat(hardDeckAlarm[0], tones[hardDeckAlarm[2]][0], tones[hardDeckAlarm[2]][1], 500, \
            hardDeckAlarm[1], tones[hardDeckAlarm[2]][2]);

  if ((alt < alt_trigger) && (jumptime > 260)) {
    altstate = 0; //go to standby mode
    jumptime = 0;
    return; //go back to loop()
  }
  delay(30);
}
//################################################################
//################################################################
void alarmat(int alarm_alt, int d1, int d2, int t1, int t2, int pulseLength) {
  if (alt >= alarm_alt && altm1 < alarm_alt) {
    //passing altitude going up
    if (climb_beeps_en) {
      playTone(d1,t1,d2,pulseLength);
    }
  }
  else if (alt <= alarm_alt && altm1 > alarm_alt) {
    //passing altitude going down
    playTone(d1,t2,d2,pulseLength);
  }
}
//################################################################
//################################################################
void playTone(int beepLength, int duration, int pauseLength, int pulseLength) {
  start = millis();
  while (millis()-start < duration) {
    for (int i=0; i<beepLength; i++) {
      // UP
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(pulseLength);
      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(pulseLength);
    }
    delay(pauseLength);
  }
}
//################################################################
//################################################################
void zero_alti() {  
  alt0 = (int) bmp.readAltitude();
  altm5 = 0;
  altm4 = 0;
  altm3 = 0;
  altm2 = 0;
  altm1 = 0;
}



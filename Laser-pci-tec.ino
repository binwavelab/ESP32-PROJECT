
/*
 * 
 * programma di gestione termica Laser TEC cloned by test
    per microcontroller ESP32 woorm
    adatto a dialogare con porta seriale usb da pc 
    con il programma serial_esp32_test.cpp contenuto nella work_dir/c_usb_9.0 asus 
    risponde ai comandi seguenti 
    inc -> incrementa  inc_105  incrementa la temperatura di 10.5°Celsius 
    dec -> decrementa  dec_10   decrementa la temperatura di -10 °celsius 
    ret -> visualizza  lla temeratura raggiunta 
    set -> set_921  imposta la temperatura a 92.1 °celsius 
    
    Imposta una frequenza PWM a pilotare scheda potenza per termocoppie di Peltier 

    arduino infoboard ESP 32_Woorom
    BN: Unknown board
    VID: 10C4
    PID: EA60
    SN: Upload any sketch to obtain it

    RIferimenti ADC porte 

    //ADC_6db 3.3 Volt 4.700 ohm 100kntc
  
  /*                    display     r ohm       mvolt
    26.5°C     --->   150 
    30.0 °C           175            44K    0.266  4.75
    50~52°C    --->   490 
    71°C       --->   686
    78°C       ---->  795
    92°C    ------>  1103
  /*
   * 
   ADC_11db: The input voltage of ADC will be attenuated,
   extending the range of measurement to up to approx. 
   2600 mV. (1V input = ADC reading of 1575).
    analogSetPinAttenuation(pin, attenuation): sets the input attenuation for the specified pin. 
                                                The default is ADC_11db. Attenuation values are the same from previous function.
    adcAttachPin(pin): Attach a pin to ADC (also clears any other analog mode that could be on).
                        Returns TRUE or FALSE result.
    adcStart(pin), adcBusy(pin) and resultadcEnd(pin):
                    starts an ADC convertion on attached pin’s bus.
                    Check if conversion on the pin’s ADC bus is currently
                    running (returns TRUE or FALSE). Get the result of the
                    conversion: returns 16-bit integer
    * 
*/

  /*---> tim_clock ---- https://www.electronicwings.com/esp32/esp32-tim_clock-interrupts    */

    /* impostazione tim_clock 
      // Global Variable 
      volatile int interruptCounter;  //for counting interrupt
      int totalInterruptCounter;    //total interrupt counting
      int LED_STATE=LOW;
      hw_timer_t * tim_clock = NULL; 

      portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


      void IRAM_ATTR timetic() {      //Defining Inerrupt function with IRAM_ATTR for faster access
       portENTER_CRITICAL_ISR(&timerMux);
       interruptCounter++;
       
      }*/

      /*
      // in setup
      tim_clock = timerBegin(0, 80, true);            // tim_clock 0, prescalar: 80, UP counting
      timerAttachInterrupt(tim_clock, &timetic, true);   // Attach interrupt
      timerAlarmWrite(tim_clock, 1000000, true);     // Match value= 1000000 for 1 sec. delay.
      timerAlarmEnable(tim_clock);      */
      
      //alimetatore pwm frquenza 11.765 Khz ( 25us minimo potenza pwm ) 


#include <Arduino.h>

#include "esp_system.h"

#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include<iostream>
#include <sstream>
#include <map>

#include "tools.h"
#include "init.h"
#include <math.h>


#define USE_SERIAL Serial

#define MAX_DISP_PAGE 5
#define CS 880       //conversione termico alluminio in Watt al secondo in 50grammi di alluminio 
#define CONVERSION_GC 44    //conversione termico alluminio in cal al secondo in 50grammi di alluminio 
#define MAX_WATT_COOL 100    //MAssima Potenza Del Peltier
#define MAXCOOL_PWM 4000    //Massima Frequenza Peltier


using namespace std;



//costruittori pulsanti ( keybourd 3 button touch )
//                 pin  refvalue state  
Button BUT_INC  = { 27,   0,     false };   //GPIO32        27      25
Button BUT_DEC  = { 26,   0,     false };   //GPIO33        26      26
Button BUT_SEL_UP  = { 25,   0,     false };   //GPIO35     25      27
Button BUT_SEL_DW  = { 14,   0,     false };   //GPIO35     14      14




//Set constant gpio for  PWM channel end  AD temperature 
const int GP_PWMCH1 = 17;  // Diode_888
const int GP_PWMCH2 = 4;  // Cool

const int GP_PWMCH3 = 2;  // Diode 
const int GP_PWMCH4 = 5;  // not used


const int GP_ADCH1 = 36; //GPIO-ADC1_0ch   T1
const int GP_ADCH2 = 39; //GPIO-ADC21_2ch  T2

//const int Probe_TC_DIODE = 33; //GPIO-ADC21_2ch



volatile int interruptCounter;  //for counting interrupt
int totalInterruptCounter;    //total interrupt counting
int LED_STATE=LOW;
hw_timer_t * tim_clock = NULL; 
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


//set ready probe interval timer 
//to read ancd calculate temperature filter eand setting pwm 

void IRAM_ATTR timetic() {      //Defining Inerrupt function with IRAM_ATTR for faster access
//portENTER_CRITICAL_ISR(&timerMux);
    Temp_probe_ready = true ;

}


//select keybord touch pin and increments/decrement value 
void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg); 

  if(s->PIN == 27 ){
        Touch_idx  = 2 ;
        TC_selection = true ;
       last_button_time = button_time;
      
    }
    
  if(s->PIN == 26 ){
        Touch_idx  = -2 ;
        TC_selection = true ;
       last_button_time = button_time;
    }

}

void setup()
{  


 USE_SERIAL.begin(115200);
 
 
  /* Initialize USE_SERIAL Port */
 
  /* Initialize PWM Channels with Frequency and Resolution */
  //ledcSetup(redPWMChannel, redPWMFreq, redPWMResolution);
  ledcSetup(TC1_PWMChannel, TC1_PWMFreq, TC1_PWMResolution);
  ledcSetup(TC2_PWMChannel, TC2_PWMFreq, TC2_PWMResolution);
  
  ledcSetup(TC3_PWMChannel, TC3_PWMFreq, TC3_PWMResolution);
  ledcSetup(TC4_PWMChannel, TC4_PWMFreq, TC4_PWMResolution);
  
  
  /* Attach the LED PWM Channel to the GPIO Pin */
  //ledcAttachPin(redLEDPin, redPWMChannel);
  ledcAttachPin(GP_PWMCH1, TC1_PWMChannel);
  ledcAttachPin(GP_PWMCH2, TC2_PWMChannel);
  
  ledcAttachPin(GP_PWMCH3, TC3_PWMChannel);
  ledcAttachPin(GP_PWMCH4, TC4_PWMChannel);
  
  
  //impostazione ADC porta 36 39 LBO RES
  adcAttachPin(GP_ADCH1);
  adcAttachPin(GP_ADCH2);
  
  analogSetPinAttenuation(GP_ADCH1, ADC_6db);
  analogSetPinAttenuation(GP_ADCH2, ADC_6db);
//inizializza display LCD 164 * 80 pixel 
  display.init();

  // display.flipScreenVertically();

  display.setContrast(255);
  display.setFont(ArialMT_Plain_16);
  delay(300);


//imposta porte 3 touch keybord per temperatura e selezione sonda e menu pagina 

  pinMode(BUT_INC.PIN, INPUT_PULLUP);
  pinMode(BUT_DEC.PIN, INPUT_PULLUP);
  pinMode(BUT_SEL_UP.PIN, INPUT_PULLUP);
  pinMode(BUT_SEL_DW.PIN, INPUT_PULLUP);

  //imposta interrupt pulsanti keyboard 
    //attachInterrupt(BUT_SEL.PIN, isr, &BUT_INC , HIGH);
    attachInterruptArg(BUT_INC.PIN, isr, &BUT_INC , RISING);
    attachInterruptArg(BUT_DEC.PIN, isr, &BUT_DEC , RISING);
 // attachInterrupt(BUT_SEL.PIN, isrc, RISING);
    
 
  //imposta timer rivelatore costate campionamento valori termici 
  
  tim_clock = timerBegin(0, 80, true);            // tim_clock 0, prescalar: 80, UP counting
  // Attach timetic function to our tim_clock.
  timerAttachInterrupt(tim_clock , &timetic , true);
  // Set alarm to call timetic function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(tim_clock, 1000000, true);
  // Start an alarm
  timerAlarmEnable(tim_clock); 
  


}


void loop()
{
    
    
  /* Read Analog Input from three ADC Inputs */
  //struttura dedicata alla cella termica 
  //redDutyCycle = analogRead(A0);
 // TC_FQBBO = analogRead(0);
 // TC_FQLBO = analogRead(0); 
  /* Map ADC Output to maximum possible dutycycle */
  /*redDutyCycle = map(redDutyCycle, 0, ADC_RESOLUTION, 0, RED_MAX_DUTY_CYCLE);*/
  //TC_FQBBO = map(TC_FQBBO, 0, ADC_RESOLUTION, 0, TC1_MAX_DUTY_CYCLE);
  //TC_FQLBO = map(TC_FQLBO, 0, ADC_RESOLUTION, 0, TC2_MAX_DUTY_CYCLE);
  /* Set PWM Output of Channel with desired dutycycle */ 
  //redDutyCycle = 1 ;
  
  TC_FQLBO = 100 ;
  TC_FQDIO = 100 ;
  
    TCOOL = 0 ;       //TC sonda NTC 100xx
    TCDIODE = 0 ;

  //Data_temp tec_LBO = {2 , 200 , 400 , 400 } ;     // stend-by
                    //indx Temp Stemp Freq 
  Data_temp tec_DIO = {2 , 0 , 40 , 200 } ; 
  Data_temp tec_YAG = {1 , 0 , 40 , 200 } ;   // stand-by  sleep-mode
  
  Diode_888 Las_diode = { 975 , 27 , 20 , 400 } ;

  std::map<String,Data_temp> tc_map;
  
  std::map<String,Diode_888> Diode_map;
  
  tc_map["PWR_DIODE"]  = tec_DIO ;
  
  tc_map["PWR_COOL"] = tec_YAG ;     //ok
  
  Diode_map["Diode_888"] = Las_diode ;


print_LCD("YAG System \n LASER TEC \n Version 1.2 ");
delay(500);

  //setup pwm ch 1 2 
  ledcWrite(TC1_PWMChannel, TC_FQBBO);
  ledcWrite(TC2_PWMChannel, TC_FQLBO);


//print_LCD("Programa \n Temperature \n Laser  ");
  //print mapp
    int t = 0;
    int di = 0;
    int te = 0;
    
    //char buffer[10];
    //itoa(analogRead(GP_ADCH1 ) ,buffer,10);

    while(1){

      //seleziona pagina Display Rolla a DX o SX
      if(  digitalRead(BUT_SEL_UP.PIN) == HIGH ) {            
      
        display_page++;
        if(display_page >= MAX_DISP_PAGE ){

          display_page = MAX_DISP_PAGE ;
        }
        Touch_idx = 0 ;
        delay(140);
      }
      if(  digitalRead(BUT_SEL_DW.PIN) == HIGH ) {            
      
        display_page--;
        if(display_page <= 0 ){

          display_page = 0 ;
        }
        Touch_idx = 0 ;
        delay(140);
      }
      

        //Mostra la pagina del displayLCD incrementa variabili >> pulsanti piu meno  Touch_idx
      switch(display_page){

          case 0://imposta frequenza per diodo potenza pwm 
          //display TCDIODE
          if(TC_selection){
                Diode_map.find("Diode_888")->second.diode_fq += Touch_idx ;
                TC_selection = false ;}
            
          di = 0;
          di = Diode_map.find("Diode_888")->second.diode_fq ;
          
          Display_Diode(String(di)  , String(Diode_map.find("Diode_888")->second.diode_p_min) ,String( Diode_map.find("Diode_888")->second.diode_p_max) );
          display.clear();
          delay(100);
          /*temp_pwmchan = TC1_PWMChannel ;
          ledcWrite( temp_pwmchan, di );*/
          break ;
          
          case 1:// imposta in map temperatura termostato T1
            if(TC_selection){
                tc_map.find("PWR_COOL")->second.S_temp += Touch_idx ;
                TC_selection = false ;}
                
                
            dtr.temp_stc = tc_map.find("PWR_COOL")->second.S_temp ;
            probe_cool_TC(String(display_page) , String(TCOOL) , String(dtr.temp_stc) , String(tc_map.find("PWR_COOL")->second.Fq)  );
            display.clear();
            delay(100);
          break ;
          case 2 : //imposta pwm peltier T1
            if(TC_selection){
                tc_map.find("PWR_COOL")->second.Fq += Touch_idx ;
                TC_selection = false ;}
                
            probe_cool_TC( String(display_page) , String(TCOOL) , String(dtr.temp_stc) , String(tc_map.find("PWR_COOL")->second.Fq) );
            display.clear();
            delay(100);
            //dtr.temp_rtc  = TCOOL;
            /*temp_pwmchan = TC2_PWMChannel ;
            t = tc_map.find("PWR_COOL")->second.Fq ;
            ledcWrite( temp_pwmchan, t  ); //imposta frequenza del resistore 
            delay(50);  */    
            break ;
            
            case 3 : //imposta in map temperatura termostato T2 
                if(TC_selection){
                tc_map.find("PWR_DIODE")->second.S_temp += Touch_idx ;
                TC_selection = false ;}
            //dtr.delta_temp = tc_map.find("PWR_DIODE")->second.S_temp ;
            dtr.temp_rtc = tc_map.find("PWR_DIODE")->second.S_temp ;
            probe_cool_PWM(String(display_page) , String(TCDIODE) , String(dtr.temp_rtc) , String(tc_map.find("PWR_DIODE")->second.Fq)  );
            display.clear();
            delay(100);
                
            break ;
            case 4 : //imposta pwm peltier T2
            if(TC_selection){
                tc_map.find("PWR_DIODE")->second.Fq += Touch_idx ;
                TC_selection = false ;}
                
            probe_cool_PWM(String(display_page) , String(TCDIODE) , String(dtr.temp_rtc) , String(tc_map.find("PWR_DIODE")->second.Fq)  );
            display.clear();
            delay(100);
            /*temp_pwmchan = TC3_PWMChannel ;
            t = tc_map.find("PWR_DIODE")->second.Fq ;
            ledcWrite( temp_pwmchan, t  ); //imposta frequenza del resistore 
            delay(50);      
            */
            break ;
          

        }    

      //settings hardware micro for temperature 
      //921600 |  CPUfrq. 40MHZ |  FLASH FREQ 80MHZ | FLASHMODE DIO | ,FLASH SIZE 4MB 
      //PARTITION SCHEME Defalt 4MB / PSRAM ENABLE
      
    //differenza du tempo di  goni cilco loop programma in millisecondi  
    

     //legge sonde  diodo e resistore burn 
     if(Temp_probe_ready ){
        

        //lettura sonde T1 T2
        tc_map.find("PWR_COOL")->second.Temp =   analogRead(GP_ADCH1);
        tc_map.find("PWR_DIODE")->second.Temp =  analogRead(GP_ADCH2);
        delay(100);
        
        //calcola temperatura °Celsius T1 T2
        Vo = tc_map.find("PWR_COOL")->second.Temp ;
        //R2 = (4700 * 5.0 ) -(4700 * Vo) /Vo ;//R1 *( (5.0 / (float)Vo) )-1;   
        logR2 = sqrt( pow( Vo , 1.3) );
        // T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)/3 );
        T = logR2; 
        //T = (T * 9.0)/ 5.0 + 32.0; 
        TCOOL = T ;//tc_map.find("PWR_COOL")->second.Temp ;

        Vo = tc_map.find("PWR_DIODE")->second.Temp ;
        //R2 = (4700 * 5.0 ) -(4700 * Vo) /Vo ;//R1 *( (5.0 / (float)Vo) )-1;
        logR2 = sqrt( pow( Vo , 1.3) );
        // T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)/3 );
        T = logR2; 
        TCDIODE = T ;
        Temp_probe_ready = false ;
        
        //setting temperature
        SET_INCTEMP( TCDIODE  , tc_map.find("PWR_DIODE")->second.S_temp   , inc_c ) ;
        SET_INCTEMP( TCOOL  , tc_map.find("PWR_COOL")->second.S_temp  , inc_d ) ;

        SET_PWM( TCDIODE , tc_map.find("PWR_DIODE")->second.S_temp , tc_map.find("PWR_DIODE")->second.Fq , inc_c );
        SET_PWM( TCOOL , tc_map.find("PWR_COOL")->second.S_temp , tc_map.find("PWR_COOL")->second.Fq , inc_d );
          
        
       //filter parametri temperatura max min 
        SET_MAX(tc_map.find("PWR_COOL")->second.S_temp  , 120 );
        SET_MIN(tc_map.find("PWR_COOL")->second.S_temp  , 10 );
        SET_MAX(tc_map.find("PWR_DIODE")->second.S_temp  , 120 );
        SET_MIN(tc_map.find("PWR_DIODE")->second.S_temp  , 10 );
        
        SET_MAX(tc_map.find("PWR_COOL")->second.Fq  , 1200 );
        SET_MIN(tc_map.find("PWR_COOL")->second.Fq  , 10 );
        SET_MAX(tc_map.find("PWR_DIODE")->second.Fq  , 1200 );
        SET_MIN(tc_map.find("PWR_DIODE")->second.Fq  , 10 );
        
        
        SET_MAX(Diode_map.find("Diode_888")->second.diode_fq  , 700 );
        SET_MIN(Diode_map.find("Diode_888")->second.diode_fq  , 15 );
        
        
        //Impost PWM potenze ausiliarie
        temp_pwmchan = TC1_PWMChannel ;
        t = tc_map.find("Diode_888")->second.Fq ;
        ledcWrite( temp_pwmchan, t  ); //imposta frequenza del resistore 
        delay(2);
        temp_pwmchan = TC2_PWMChannel ;
        t = tc_map.find("PWR_COOL")->second.Fq ;
        ledcWrite( temp_pwmchan, t  ); //imposta frequenza del resistore 
        delay(2);
        temp_pwmchan = TC3_PWMChannel ;
        t = tc_map.find("PWR_DIODE")->second.Fq ;
        ledcWrite( temp_pwmchan, t  ); //imposta frequenza del resistore 
        delay(2);
  
     
      } 

             
             USE_SERIAL.print( " dtr.delta_temp -> " );
             USE_SERIAL.println( dtr.delta_temp_tc      );
           
             
             USE_SERIAL.print( " dtr.delta_temp -> " );
             USE_SERIAL.println( dtr.delta_temp_td      );
             USE_SERIAL.print( "\n" );
            
             USE_SERIAL.print( " delta_w_cool -> " );
             USE_SERIAL.println( dtr.delta_w_cool );
             
             USE_SERIAL.println( "------------------" );
            USE_SERIAL.println();
            
            //legge variabile tempo 
            
             
  

    } //fine while 



}//fine loop 


void Temperature_settings(){
    
    
    
    
}





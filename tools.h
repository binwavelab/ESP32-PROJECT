//reference pin /home/alexxx/.arduino15/packages/esp32/hardware/esp32/1.0.6/libraries/Wire/src/Wire.h:
//reference pin /home/alexxx/.arduino15/packages/esp32/hardware/esp32/1.0.6/variants/esp32/pins_arduino.h 


SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SH1106Wire display(0x3c, SDA, SCL);
OLEDDisplayUi ui ( &display );
#define BL 10 

//PAGE 1
void Display_Diode(String pwm , String tem1 , String tem2 ){

      //display.setTextAlignment(TEXT_ALIGN_CENTER);
      
      //String text_tcxx = "LASER " +  temp ;
      String text_tcxx = "LAS 975 \n " + tem1 +  " - " + tem2  + "\n PWM  " +  pwm  ;
      display.clear();
      display.drawString(0 , 0 , text_tcxx );
      display.drawLogBuffer(0, 0);
      display.display();   
    
    }
//PAGE 2 3
void probe_cool_PWM(String ch , String temp , String tem , String pwm){

      //display.setTextAlignment(TEXT_ALIGN_CENTER);
      
      String text_tcxx = ch +" P1 CL "  +  temp  + "\n" + "S-> Tc° " +  tem + "\n PWM  " +  pwm;
      display.clear();
      display.drawString(0 , 0 , text_tcxx );
      display.drawLogBuffer(0, 0);
      display.display();
      //delay(2000);

    }
    
    //PAGE 4 5
    void probe_cool_TC(String ch , String temp , String tem , String pwm){

      //display.setTextAlignment(TEXT_ALIGN_CENTER);
      
      String text_tcxx = ch + " P2 CL "  +  temp  + "\n" + "S-> Tc° " +  tem + "\n PWM  "  +  pwm;
      display.clear();
      display.drawString(0 , 0 , text_tcxx );
      display.drawLogBuffer(0, 0);
      display.display();
      //delay(2000);

    }


      //aux
    void display_probe(String t1 , String t2 , String t3 ){

      String text_tcxx = "diod " + t1 + " \n "
                           "lbo " + t2 + " \n " +
                            "bbo " + t3  ;
      display.clear();
      display.drawString(0 , 0 , text_tcxx );
      display.drawLogBuffer(0, 0);
      display.display();

    }

//primitive LCD sequence script
    void print_LCD(String temp  ){

      //display.setTextAlignment(TEXT_ALIGN_CENTER);
      
      String text_tcxx = temp  ;
      display.clear();
      display.drawString(0 , 0 , text_tcxx );
      display.drawLogBuffer(0, 0);
      display.display();
      delay(300);

    }
    }

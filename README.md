# ESP32-PROJECT
Electonic ESP32 application  analogic probe lcd display PWM serial device keyboard 

LASER TEC PCI 
Programma sviluppato per dispositivo Micro ESP32 Wroom
Il programma svolge funzionalita di termostato e regolatore termico  
adatto pilotare  energia a circuiti mosfet accoppiati a celle di caldo freddo 
tramite sonda NTC .



LCD indicatore temperature potenza PWM di ogni dispositivo collegato
5 pagine per leggere e impostare  potenza Diodo  ( in termini tempo PWM )  ,
Temperatura TC1 termostato TC1 , Potenza Peltier PWM TC1  , Temperatura TC2 termostato TC2 , Potenza Peltier PWM TC2 , 
Keyboard 4 tasti  collegati ai seguenti pin 14 25 26 27 configurati come 
2 interrupt GPIO 25 26  Hendler irs di impostazione variabili nelle pagine del display visualizzato  
2 Porte input GPIO 14 17  previste per muovere le pagine del display 
2 Porte ADC Sonde TC1 TC2 termoresistenze 100k NTC 


Il software Laser TEC PCI e' stato compilato con la versione del compilatore presente in arduino 1.8.16 
  //arduino impostazioni hardware micro esp32 precompilatore 
  
  //921600 |  CPUfrq. 40MHZ |  FLASH FREQ 80MHZ | FLASHMODE DIO | ,FLASH SIZE 4MB 
  //PARTITION SCHEME Defalt 4MB / PSRAM ENABLE

  Attualmente e' in studio una versione con micro STM32F103 




  



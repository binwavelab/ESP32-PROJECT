# ESP32-PROJECT
Electonic ESP32 application  analogic probe lcd display PWM serial device keyboard 

LASER TEC PCI 

  E' un firmware o programma che svolge funzionalita di termostato e regolatore termico  
  adatto pilotare  energia a circuiti mosfet accoppiati a celle di caldo freddo 
  tramite sonda NTC .
  Programma e' sviluppato per dispositivi Micro ESP32 Wroom 
  Il circuito  e' composto da :
  una board Esp32 un LCD oled 120x64 bit 
  4 pulsanti touch 2 sonde NTC e collegamenti ai pin GPIO ai dispositivi  

Breve descrizione di funzionamento :

    LCD indicatore temperature potenza PWM di ogni dispositivo collegato
    5 pagine per leggere e impostare  potenza Diodo  ( in termini tempo PWM )  ,
    Temperatura TC1 termostato TC1 , Potenza Peltier PWM TC1  , Temperatura TC2 termostato TC2 , Potenza Peltier PWM TC2 , 
    Keyboard 4 tasti  collegati ai seguenti pin 14 25 26 27 configurati come 
    2 interrupt GPIO 25 26  Hendler irs di impostazione variabili nelle pagine del display visualizzato  
    2 Porte input GPIO 14 17  previste per muovere le pagine del display 
    2 Porte ADC Sonde TC1 TC2 termoresistenze 100k NTC 


Il software Laser TEC PCI e' stato compilato con la versione del compilatore presente in arduino 1.8.16 
    
    arduino impostazioni hardware micro esp32 precompilatore 
    
    921600 |  CPUfrq. 40MHZ |  FLASH FREQ 80MHZ | FLASHMODE DIO | ,FLASH SIZE 4MB 
    PARTITION SCHEME Defalt 4MB / PSRAM ENABLE 
    
    E' composto dai seguenti file
    
    LaserTecPci.ino    programma main loop pricipale su cui si svolge il funzioanto del dispositivo   
    init.h             variabili principali del programma
    tool.h             funzioni base del display e caricamento diplay in meoria 

    Nota che nel programma non sono ancora state rimosse alcune variabili appartenenti a versioni precedenti
    del programma 

    La semplificazione dell'andamento termico sonda NTC e' stata riprodotta con la funzione 
    elevazione del valore letto da porta analogica impostata a 1024 bit 
    
    x^1.3
    poiche' simile  alla crescita certificata dal costruttore sul datasheets
    seplifica i calcoli del calcolo che necessitano per determinare la temperatura 
    del resistore termico NTC .. 

    i quali prevedono il calcolo del log()^3 inerente a differenza del voltaggio medio resistenza al variare della temperatura 
    seguiti dal costruttore .

    Personalmente  ho ottenuto risultati ottimali ~2°C  a  120°C  
    
    
    
    



  



  



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

    Il codice per chi volesse destreggiarsi e' nel file principale main 

            //calcola temperatura °Celsius T1 T2
        Vo = tc_map.find("PWR_COOL")->second.Temp ;                              ---< il valore letto dalla sonda NTC2 e' registrao qui 
        //R2 = (4700 * 5.0 ) -(4700 * Vo) /Vo ;//R1 *( (5.0 / (float)Vo) )-1;   
        logR2 = sqrt( pow( Vo , 1.3) );                                        --->formula correzione elettrotermica curva 
        // T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)/3 );
        T = logR2; 
        //T = (T * 9.0)/ 5.0 + 32.0; 
        TCOOL = T ;//tc_map.find("PWR_COOL")->second.Temp ;

    Poiche per leggere la temeratura dal  grafico riportato nel datasheets del cosruttore , rappresentato su asse delle x , 
    e su asse y il valore corretto della resistenza NTC .
    Dovendo determinare il valore di y quindi cominciamo semplificare il calcolo  effettuando un calcolo inverso della funzione 
    quindi la potenza di x^1.3 . Applicando la radice quadrata si invertono gli assi della funzione .
    Ovvero formulando  una formula F(X) 
    quindi  sqrt( POW ( Volt_ADC , 1.3 ) ; Ribalta completamente gli assi x con y 
    Da qui attraverso la formula del partitore di tensione tra R1 e R2 vado a cercare il valore di NTC R2
    Quindi da in cui dovremmo estrarre il valore di R2 per determinare la Temperatura ..  
    Ho ricavato la formula inversa del partitore di temsione , per semplificare , con CAT GPT , ritengo che sia corretta . 
    
    R1 4700 ,  Vo  valore in decimali di AD_converter , 5.0 Vin
    // formula base del partitore tensione  Vo = Vi * ( (R1 - R2 / R2 ) )
    R2 = (4700 * 5.0 ) -(4700 * Vo) /Vo ;   
    Questo calcolo che porta via diverse operazioni di lavoro puo essere semplificata semplicemente 
    invertendo l'ordine di collegamento tra  R1 con R2  ( schema base di collegamento  sonda NTC a ADC GPIO ) senza amplificatore esterno . 

              R1 NTC            R2 4700 ohm
      (5V) ------/\/\/\----o ---/\/\/\--------- o (GND)
                           |
                           |
                       ____|_____
                        ADC GPIO


    Come dire che il carico assorbito da R2 compensa o bilancia linearmente il carico della sonda NTC 

    Cioe' se calcolando R2 trovo valore termico attraverso la formula  sqrt(R2^1.3)
    inverendo R1 con R2 ottengo il varole termico direttamente dalla porta
    digitale GPIO_ADC 
    --> segue   sqrt(GPIO_ADC^1.3)
    Valore Termico Sonda in °C!
    
                     Fromula x^1.3     

![photo_5771367823489221289_y](https://github.com/binwavelab/ESP32-PROJECT/assets/32112692/64dba839-ceb5-4453-a11b-820c430ae270)

                    formula sqrt(x^1.3) 

![photo_5771367823489221290_y](https://github.com/binwavelab/ESP32-PROJECT/assets/32112692/47f462a8-4828-4f6e-9381-461aead253cf)


    
    
    
    
    



  



  



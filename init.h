#define INIT_h

//File dedicato a variabili programma TC

#define SET_DIR( DA , DB , DR ) if ( DA < DB ) DR = 1 ; else DR = -1 ;
#define HYPOT2(x,y)  (sq(x)+sq(y))

#define SET_TO(x,y)  if( x > y  ) x = y-1 ;
#define HYPOT(x,y)  sqrt(HYPOT2(x,y))

#define SIGN(a) ((a>0)-(a<0))
#define WITHIN(V,L,H) ((V) >= (L) && (V) <= (H))
#define M_VH(V,L ) ( (V) < (L) ) 
#define M_VLX(V ) ( (V) > (1) )
#define M_VLY(V ) ( (V) > (1) )
#define V_TO_END(a,d) (((a < d) a++) && 1)
//#define V_TO_INIT(a,d) (a > d) a--
#define SBI(A,B) (A |= (1 << (B)))

#define SET_MAX( a , b  ) if ( a > b ) a = b-1 ;
#define SET_MIN( a , b  ) if ( a < b ) a = b+1 ;

#define SET_INCTEMP( a , b  , c ) c = (int)( abs(a - b) * 10 ) 

#define SET_PWM( a , b  , c , d ) if ( a > b ) c +=d  ; else c -= d ;



struct Button {
  const uint32_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};


//cella termica 
struct Data_temp{
      int idx ;         //indice cella
      int Temp ;        //temperatura sonda                     temp_probe
      int S_temp ;      //temperatura di riferimento            temp_set
      int Fq ;          //frequenza                             fq_set
} ;


struct Diode_888{
      int idx ;             //indice cella
      int diode_fq ;        //frequenza pwm
      int diode_p_max ;      //Potenza massima  
      int diode_p_min ;     //potenza di soglia
      int diode_intemp ;   //temperatua interna 
      float diode_lithling  ; //sonda potenza luminosa 
    
};


volatile int temp_indx = 0 ;
//int * ptemp_indx = &temp_indx ;
String SEL_TC_xxx    = "";       //temperature selector for display page and select   
String SEL_TC_xxx_lp = "";		//temperature selector in loop control 
volatile bool TC_selection = false ;  // keybord interrupt  
volatile bool Temp_probe_ready = false ;
volatile int temp_tcdio = 0;
volatile int temp_tcres = 0;

//setting PWM Register 
const int TC1_PWMFreq = 2700; /* 5 KHz */
const int TC1_PWMChannel = 2;
const int TC1_PWMResolution = 12;
const int TC1_MAX_DUTY_CYCLE = (int)(pow(2, TC1_PWMResolution) - 1);

const int TC2_PWMFreq = 2700; /* 10 KHz */
const int TC2_PWMChannel = 4;
const int TC2_PWMResolution = 12;
const int TC2_MAX_DUTY_CYCLE = (int)(pow(2, TC2_PWMResolution) - 1);


const int TC3_PWMFreq = 2700; /* 5 KHz */
const int TC3_PWMChannel = 6;
const int TC3_PWMResolution = 12;
const int TC3_MAX_DUTY_CYCLE = (int)(pow(2, TC3_PWMResolution) - 1);

const int TC4_PWMFreq = 2700; /* 10 KHz */
const int TC4_PWMChannel = 8;
const int TC4_PWMResolution = 12;
const int TC4_MAX_DUTY_CYCLE = (int)(pow(2, TC4_PWMResolution) - 1);


const int ADC_RESOLUTION = 4095; /* 12-bit */



//system arg loop temperatura 
int temp_loop = 0 ;

volatile int LBO_tc_setting ;
 

int  TCOOL , TCDIODE , SET_TCLBO , SET_TCBBO , SET_TCDIO , TC_FQLBO , TC_FQBBO , TC_FQDIO ;

//contatore tempo 
static long int lastTime = 0; //tempo del contatore lettura sonda termica Diodo 
long int warmTime = 0 ; //tempo di riscaldamento 

//potensa 60 watt per secondo 

//parametri globali per display and touch relazione 
int Touch_idx = 0;
int display_page = 0 ;
int touch_select = 0;
int temp_pwmchan  = 0 ;
int chan = 1 ;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 


//calcolo ntc variabile
int ThermistorPin = 0;
int Vo;
float R1 = 44000;
float  R2, T;
double logR2 ;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int inc_c = 10 ;
int inc_d = 10 ;



//Parametri globali sonde TC e PWM ;
struct data_temperature{
  int temp_fq;
  int temp_stc ;
  int temp_rtc ;
  int temp_rtc_old ; 
  int delta_temp_tc ;
  int delta_temp_td ;
  int tmp_c = 0 ;
  int wt_dec = 0 ;
  double delta_w_cool  = 1.002 ;
  double warm_watt = 1.002 ; 
  double wtd = 1.002 ;
  bool cooling_power_available = false ;
  bool continuing_cooling = false ;
  
} dtr ;




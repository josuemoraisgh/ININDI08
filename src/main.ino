
//////////////////////////////Funções das Trheads///////////////////////////////////
#include <Arduino.h>
#include <math.h>
#include "digitalFilter.h"
#define NUMTHREADS 2 //Deve-se sempre declarar o NUMTHREADS antes de "inindThread.h"
#include "inindThread.h"

#define pinANALOG A5      //Configura o pino de leitura do LUXÍMETRO
#define pinPWM 6          //Configura o pino de Saida do PWM
#define ANALOG_INTERVAL 1 // analog read interval (milliseconds)
#define PWM_INTERVAL 1    // analog read interval (milliseconds)
#define FREQ1 (10/(1000*PWM_INTERVAL)) //Em Hz/(1000*PWM_INTERVAL)
#define FREQ2 (6/(1000*PWM_INTERVAL))  //Em Hz/(1000*PWM_INTERVAL)


unsigned long count = 0;
byte time = 0;
unsigned int amplitude = 127;


#define FILTER_ORDER1 57
DigitalFilter filter1;
const double filter_taps1[FILTER_ORDER1] = {
  0.0001592,
  0.000165,
  0.0001823,
  0.0002108,
  0.0002502,
  0.0003,
  0.0003596,
  0.0004282,
  0.0005051,
  0.0005891,
  0.0006794,
  0.0007747,
  0.0008738,
  0.0009755,
  0.0010786,
  0.0011817,
  0.0012835,
  0.0013828,
  0.0014782,
  0.0015686,
  0.0016529,
  0.00173,
  0.0017989,
  0.0018587,
  0.0019087,
  0.0019483,
  0.0019769,
  0.0019942,
  0.002,
  0.0019942,
  0.0019769,
  0.0019483,
  0.0019087,
  0.0018587,
  0.0017989,
  0.00173,
  0.0016529,
  0.0015686,
  0.0014782,
  0.0013828,
  0.0012835,
  0.0011817,
  0.0010786,
  0.0009755,
  0.0008738,
  0.0007747,
  0.0006794,
  0.0005891,
  0.0005051,
  0.0004282,
  0.0003596,
  0.0003,
  0.0002502,
  0.0002108,
  0.0001823,
  0.000165,
  0.0001592
};

void analogReadFunc()                      // Faz a leitura do sinal Analógico
{ 
  const int analog_Value = analogRead(pinANALOG) - 512;
  DigitalFilter_put(&filter1, analog_Value);
  count += ANALOG_INTERVAL;
  Serial.print(">amp:");
  Serial.print(count);  
  Serial.print(":");  
  Serial.print(DigitalFilter_get(&filter1));
  Serial.println("§Volts|g");
}

void pwmFunc()                            //Faz a leitura e escrita da serial
{ 
  //analogWrite(pinPWM,100*sin(2*PI*FREQ1*time)+27*sin(2*PI*FREQ2*time)+127);
  analogWrite(pinPWM,127*sin(2*PI*FREQ1*time)+127.5);
  if(++time==(1/FREQ1)) time=0;
}

void setup()                              // Código de configuração
{
  Serial.begin(9600);
  pinMode(pinANALOG, INPUT);
  pinMode(pinPWM, OUTPUT);
  DigitalFilter_init(&filter1,FILTER_ORDER1,filter_taps1);    
  threadSetup(analogReadFunc,ANALOG_INTERVAL,pwmFunc,PWM_INTERVAL);//parametros:funcão,intervalo,funcão,intervalo,...
}

void loop(){}

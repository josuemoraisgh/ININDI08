
//////////////////////////////Funções das Trheads///////////////////////////////////
#include <Arduino.h>
#include <inindThread.h>
#include <digitalFilter.h>
#include <math.h>

#define pinANALOG A5                        //Configura o pino de leitura do LUXÍMETRO
#define pinPWM 6                            //Configura o pino de Saida do PWM
#define PERIODO 100                         //Em us 100ms

byte count = 0;
unsigned int amplitude = 127;
byte bLeitura[4] = {0, 0, 0, 0};

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

void analogReadFunc()                      // Faz a leitura do sinal Analogico
{ 
  DigitalFilter_put(&filter1,abs(analogRead(pinANALOG)-512));
  Serial.println(DigitalFilter_get(&filter1));//Escreve em RMS na serial
}

void pwmFunc()                            //Faz a leitura e escrita da serial
{ 
  //analogWrite(pinPWM,100*sin(2*PI*count/PERIODO)+27*sin(2*PI*count/16)+127);
  analogWrite(pinPWM,127*sin(2*PI*count/PERIODO)+127.5);
  if(++count==PERIODO) count=0;
}

void setup()                              // Codigo de configuração
{
  Serial.begin(9600);
  pinMode(pinANALOG, INPUT);
  pinMode(pinPWM, OUTPUT);
  DigitalFilter_init(&filter1,FILTER_ORDER1,filter_taps1);    
  threadSetup(analogReadFunc,1,pwmFunc,1,NULL);//parametros:funcão,intervalo,funcão,intervalo,...,NULL  
}

void loop(){}

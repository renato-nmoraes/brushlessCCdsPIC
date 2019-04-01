#include <ADC.h>
#include <math.h>

#pin_select U1TX = PIN_B9
#pin_select U1RX = PIN_B8
#use rs232(UART1,stream = ccm, baud=115200,parity=N,xmit=PIN_B9,rcv=PIN_B8,bits=8, stop=1) 



#word OVDCON=getenv("SFR:OVDCON")
#word TRISB=getenv("SFR:TRISB")

void zera_pwm();
int duty = 0;

//VARIAVEIS METODO 1
//unsigned long int unaux=0;
//float Tensao=0, Vel=0;
float un=0.0, un1=0.0, un2=0.0, en=0.0, en1=0.0, en2=0.0;
int Referencia = 140, H, led;
float ref = Referencia*95/248, Digital, unaux, Digital1;
int count=0, value=0, flagcount = 0, oi=40, i, running_motor =0;


//VARIAVEIS METODO 2
//float vetor_velocidade[1001], VelRAD;
//int running_motor = 0, contador = 0, i=0;


///////////////////////////////////////////////////////////////////////////////


#INT_TIMER1 Level=7
void TIMER_isr()
{
   disable_interrupts(INT_TIMER1);
   clear_interrupt(INT_TIMER1);
   

   /*VelRAD = contador*628.31853/3; //Número de pulsos contados *1000*2*pi/300 para 1ms
   //VelRAD = contador*62.831853/3; //Número de pulsos contados*100*2*pi/300 para 10ms
   
   //OBTENÇÃO DA RESPOSTA AO DEGRAU
   
   if(running_motor==1) //SE O MOTOR ESTIVER COMEÇADO A GIRAR
   {
      if(i<1001)
      {
         vetor_velocidade[i] = VelRAD;  //CADA VALOR OBTIDO EM CADA MS VAI PRA UMA POSIÇÃO DO VETOR
         VelRAD = 0;
         i++;
      } 
      contador = 0; //RESETA O CONTADOR
   }*/
   
///////////////////////////////////////////////////////////////////////////////////
   
   //TESTE TROCA DE DUTY EM X VEZES A VELOCIDADE DE INTERRUPÇÃO DO TIMER
   /*if(count==0 && flagcount ==0)
   {
     duty = 4000;
     flagcount = 1;
     count =30;
     output_high(pin_b4);
  
   }
    
   if(count==0 && flagcount ==1)
   {
     duty = 8000;
     flagcount = 0;
     count = 30;
     output_low(pin_b4);
   }
   else
   {
   count--;
   }*/
   
 
   //TESTE TROCA DE DUTY CYCLE NA VELOCIDADE DA INTERRUPÇÃO DO TIMER
   /*if(led==0)
   {
      output_high(pin_b4);
      led=1;
      duty =4000;
   }else{
   output_low(pin_b4);
   led=0;
   duty=8000;
   }*/
   /*Digital = 0;
   Digital1 = 0;
   //CONTAS DO ADC PARA CONTROLE
   for(i=0;i<49;i++)
   {
   Digital = read_adc();
   Digital1 = Digital1 + Digital;
   }*/
   if(running_motor==1)
   {
  // 
   if(oi==0)
   {
      ref = 230*95/248;
   }else
   {   
   oi--;
   }
   
   Digital = read_adc();
   //Tensao = Digital*0.003193359;
   //H = 50*Digital*0.003193359;
   H = Digital*0.12038;
   
   
   
   //CONTROLADOR
   en = ref-H;
   //en = ref-read_adc()*0.097751;
   //un = en*0.009362 - en1*0.01827 + en2*0.008916 + un1*1.931 - un2*0.931;   //1ms
   //un = en*0.005237 - en1*0.01012 + en2*0.004888 + un1*1.905 - un2*0.9048;  //1ms_2
   //un = en*0.008865 - en1*0.00414 + en2*0.0005839 + un1*0.7179 + un2*0.2821;  //50ms
   if(en==0)
   {
      en=0.000001;
   }
   //un = 0.004824*en - 0.004771*en1 + un1; //1ms_3
   //un = 0.005064*en - 0.004531*en1 + un1; //10ms_3
   //un = 0.00613*en - 0.003465*en1 + un1; //50ms_3
   //un = 0.007463*en - 0.002132*en1 + un1; //100ms_3
   //un = 0.009699*en - 0.009638*en1 + un1; //1ms_corrigido
   un = 0.01118*en - 0.008158*en1 + un1; //50ms_corrigido
  // en2 = en1;
   en1 = en;
  // un2 = un1;
   un1 = un;
   
   //TESTE CONTROLE ON-OFF EM TORNO DE UM SET POINT
   /*if(en>0)
   {
      duty = 8000;
   }
   else
   {
      duty = 4000;
   }*/
   
   //ATRIBUI UN AO DUTY
   un = un*10000;
   
   if(un>9500)
   {
     //unaux = 9500;
     un = 9500;
   }
   if(un<0)
   {
      un = 0;
   }
   
   
   duty = (unsigned int)un;
   }
   //VALORES ENVIADOS PELA SERIAL PARA TESTE
   
   /*   if(oi!=0)
   {
      printf(" %f," en);
      oi--;
   }*/
   /*if(oi!=0){
   printf("\n\r\n\rDUTY: ");
     printf("%Lu", duty);
     printf("\n\r\n\rADC: ");
     printf("\n\r %f", Digital);
     printf("\n\r\n\rERRO: ");
     printf("%f", en);
     printf("\n\r\n\rUN: ");
     printf(" %f \n\r\n\r", un);
     oi--;
   }*/
     enable_interrupts(INT_TIMER1);
   }

#INT_CNI
void CNI_isr()                //CN INTERRUPT SERVICE ROUTINE
{
     unsigned int Hall;
     
     //contador++;
     
     //disable_interrupts(INT_TIMER1);
     disable_interrupts(INTR_CN_PIN|PIN_B1);
     disable_interrupts(INTR_CN_PIN|PIN_B2);
     disable_interrupts(INTR_CN_PIN|PIN_B3);
     
     Hall = input_b();
     Hall = Hall&0x000E;
     Hall = Hall>>1;
     //duty = 2.5*3700;
     delay_us(250);
     
     running_motor = 1;
     
     //value = read_adc();
     /*printf("Valor Hall:\n");
     printf("\n %2u", duty);*/
     switch(Hall)
     {
                 case(0x0000):
                              zera_pwm();
                              break;
                 case(0x0001):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                             // CNIE = 1;
                              break;

                 case(0x0002):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                             // CNIE = 1;
                              break;
                 case(0x0003):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                             // CNIE = 1;
                              break;
                 case(0x0004):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE,2,2);
                             // CNIE = 1;
                              break;
                 case(0x0005):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                             // CNIE = 1;
                              break;
                 case(0x0006):
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_pwm_duty(1,1,duty); 
                              set_motor_pwm_duty(1,2,duty); 
                              set_motor_pwm_duty(1,3,duty);
                              set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_1,2,2);
                              set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
                              set_motor_unit(1,3,MPWM_ENABLE,2,2);
                             // CNIE = 1;
                              break;
                 case(0x0007):
                              zera_pwm();                      
                              //CNIE = 1;
                              break;
     }
     //CNIE = 1;
     //enable_interrupts(INT_TIMER1);
     enable_interrupts(INTR_CN_PIN|PIN_B1);
     enable_interrupts(INTR_CN_PIN|PIN_B2);
     enable_interrupts(INTR_CN_PIN|PIN_B3);
}

void zera_pwm()
{
    set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
    set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
    set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_H_0|MPWM_FORCE_L_0,2,2);
}

void inicializa_oscilador(){

   PLLPRE_1 = 0;
   PLLPRE_2 = 0;
   PLLPRE_3 = 0;
   PLLPRE_4 = 0;
   PLLPRE_5 = 0;
   PLLDIV_1 = 0;
   PLLDIV_2 = 1;
   PLLDIV_3 = 1;
   PLLDIV_4 = 1;
   PLLDIV_5 = 0;
   PLLDIV_6 = 0;
   PLLDIV_7 = 1;
   PLLDIV_8 = 0;
   PLLDIV_9 = 0;
   PLLPOST_1 = 0;
   PLLPOST_2 = 0;

}

void inicializa_perifericos_variaveis(){

   setup_spi( FALSE );
   setup_spi2( FALSE );
   setup_qei(QEI_MODE_X4 | QEI_SWAP_AB, QEI_NO_ERROR_INTS| QEI_FILTER_DIV_64 | QEI_IDX_WHEN_A0_B0,1000); //QEI_RESET_WHEN_IDX_PULSE
   
   //INICIALIZA ADC
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(sAN2|VSS_VDD);
   set_adc_channel(2);
   
   //INICIALIZA TIMER1
   
   //setup_timer1(TMR_INTERNAL|TMR_DIV_BY_8, 0x1388); //1ms
   //setup_timer1(TMR_INTERNAL|TMR_DIV_BY_8, 0xC350); //10ms
   setup_timer1(TMR_INTERNAL|TMR_DIV_BY_64, 0x7A12); //50ms
   //setup_timer1(TMR_INTERNAL|TMR_DIV_BY_64, 0xF424); //100ms
   //set_timer1(0xFFFF); 
   
  
   //INICIALIZA PWM
   setup_motor_pwm(1,MPWM_FREE_RUN,1,1,5000);
                                              
   //set_motor_unit(1,1,MPWM_ENABLE | MPWM_FORCE_L_0,10,10);
   
   set_motor_unit(1,1,MPWM_ENABLE|MPWM_FORCE_L_0|MPWM_FORCE_H_0,10,10); 
   
   set_motor_unit(1,2,MPWM_ENABLE|MPWM_FORCE_L_0|MPWM_FORCE_H_0,10,10); 
   
   set_motor_unit(1,3,MPWM_ENABLE|MPWM_FORCE_L_0|MPWM_FORCE_H_0,10,10);
   
   set_tris_b(0x001F);                                        
   
   
   set_motor_pwm_duty(1,1,duty);  //Set pin pair 1 to a 50% duty cycle.
   set_motor_pwm_duty(1,2,duty);  //Set pin pair 2 to a 25% duty cycle.
   set_motor_pwm_duty(1,3,duty);  //Set pin pair 3 to a 50% duty cycle.


   disable_interrupts(intr_global);
   disable_interrupts(int_timer5);
   disable_interrupts(INT_QEI);
}

void inicializa_interrupcao()
{
  // CN2 = 1;                           //CN2, CN3 E CN4 ENTRADA HALL E PINOS DE INTERRUPÇÃO
  // CN3 = 1;
   //CN4 = 1;
   
   //CNIF = 0;                           //RESETA FLAG DE INTERRUPÇÃO CN
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INTR_CN_PIN|PIN_B1);
   enable_interrupts(INTR_CN_PIN|PIN_B2);
   enable_interrupts(INTR_CN_PIN|PIN_B3);
   enable_interrupts(INTR_GLOBAL);
   
   //CNIE = 1;                           //ATIVA INTERRUPÇÃO CN
}

void main()
{
   inicializa_oscilador();
   
   inicializa_perifericos_variaveis();
   
   inicializa_interrupcao();
   
   printf("Teste");
   
   while(true);
   
   /*CNI_isr(); //Primeira comutação do motor
   
   while(i<1001); //Enquanto o vetor de velocidade não estiver cheio, espera
   disable_interrupts(INTR_GLOBAL); //Depois de cheio, desliga interrupções
   running_motor=0; //Desativa flag do motor girado
   
   for(i=0; i<1001; i++) //Envia para a serial cada valor de velocidade obtido
   {
      printf("\n\r %f", vetor_velocidade[i]);      
   }*/
   
}

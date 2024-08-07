#include <18f4550.h>
#fuses hs,nowdt,noprotect,nolvp,NOMCLR

void main()
{

int delay_reg1;// declaração de variáveis
int delay_reg2;// declaração de variáveis
int delay_reg3;

#asm

goto inicio // vai para label inicio
Delay100ms:
  movlw 0x83 //w=131d
  movwf delay_reg1 // reg1=131d
  movlw 0xFF // w=255d
  movwf delay_reg2 // reg2=255d
loop1:
  DECFSZ delay_reg1 // decrementa e pula se igual a zero
  goto loop2
  goto exit
loop2:
  DECFSZ delay_reg2 // decrementa e pula se igual a zero
  goto loop2
  goto loop1
exit:
  RETURN

inicio:
   movlw 0x00 // 00h= pullup habilitado;80h=pullup desabilitado.
   movwf 0xFF1 //intcon2=00h
   movlw 0xFF //w=11111111b
   movwf 0xF93 //trisbB=11111111b
   movlw 0x00 //w=00000000b
   movwf 0xF95 // trisD=00000000b
   movlw 0xE6 //w=11100110b
   movwf 0xFD3 // Oscilador 4Mhz Sem cristal e capacitor $$$$

//início da condição XOR (ou exclusivo)
loopA: // testa condição B2 = 0 (botão pressionado) e B3 = 0(botão pressionado)
   btfsc 0xf81.2 //testa o pino 2 da porta b e pula a próxima linha se 0
   goto loopB    // Se B2 = 1, irá para o loopB para confirmar a condição B0 = 1 e B1 = 1
   btfsc 0xf81.3 //testa o pino 3 da porta b e pula se 0
   goto loopSemaforo    // B3 = 1
   goto loopA           // B3 = 0
   // Testa a condição B2 = 0 e B3 = 0 e caso verdade, reinicia o loop

loopB: // Testa a condição B3 = 1 e caso verdade, reinicia o loop 
   btfss 0xf81.3 //testa o pino 3 da porta b e pula se 1
   goto loopSemaforo    // B3 = 0
   goto loopA           // B3 = 1
//fim da condição XOR


//Caso B2 = 1 e B3 = 0 ou B2 = 0 e B3 = 1, o código proceguirá para a próxima fase
loopSemaforo:
// Para o semáforo em verde
   loopGreen:
   // Via 1
      bcf  0xf83.2 //desliga o pino D2 ou o pino do vermelho
      bsf  0xf83.0 //Liga o pino D0 ou o pino do verde
   // Via 2
      bcf  0xf83.4 //desliga o pino D4 ou o pino do amarelo
      bsf  0xf83.5 //Liga o pino D5 ou o pino do vermelho
      //Coloca um delay de 8s (80x0,1s)
      movlw 0x50 // Passa para w = 80d
      movwf delay_reg3 // Atribui 80 decimal ao delay_reg3
      
      loopDelayGreen:
         decfsnz delay_reg3; //Caso delay_reg3 seja diferente de zero, pula a próxima linha
         goto loopYellow
         call Delay100ms // vai pra subrotina Delay100ms
      goto loopDelayGreen // volta a repetir o loopDelay até que delay_reg3 seja 0


// Para o semáforo em amarelo
   loopYellow:
   // Via 1
      bcf  0xf83.0 //desliga o pino D0 ou o pino do verde
      bsf  0xf83.1 //Liga o pino D1 ou o pino do amarelo
   // Via 2 
      // Mantem o pino D5 ou o LED vermelho ligado 
      //Coloca um delay de 3s (30x0,1s)
      movlw 0x1E // Passa para w = 30d
      movwf delay_reg3 // Atribui 30 decimal ao delay_reg3
   
   loopDelayYellow:
      decfsnz delay_reg3; //Caso delay_reg3 seja diferente de zero, pula a próxima linha
      goto loopRed1
      call Delay100ms // vai pra subrotina Delay100ms
   goto loopDelayYellow // volta a repetir o loopDelay até que delay_reg3 seja 0

   
// Para o semáforo em vermelho
   loopRed1: //parte 1
   // Via 1   
      bcf  0xf83.1 //desliga o pino D1 ou o pino do amarelo
      bsf  0xf83.2 //Liga o pino D2 ou o pino vermelho
   // Via 2   
      bcf  0xf83.5 //desliga o pino D5 ou o pino do vermelho
      bsf  0xf83.3 //Liga o pino D3 ou o pino verde
      //Coloca um delay de 8s (80x0,1s)
      movlw 0x50 // Passa para w = 80d
      movwf delay_reg3 // Atribui 80 decimal ao delay_reg3
      
      loopDelayRed1:
         decfsnz delay_reg3; //Caso delay_reg3 seja diferente de zero, pula a próxima linha
         goto loopRed2
         call Delay100ms // vai pra subrotina Delay100ms
      goto loopDelayRed1 // volta a repetir o loopDelay até que delay_reg3 seja 0


   loopRed2: //parte 2
   // Via 1
      // Mantem o D2 ou o pino do vermelho ligado
   // Via 2 
      bcf  0xf83.3 //desliga o pino D3 ou o pino do verde
      bsf  0xf83.4 //Liga o pino D4 ou o pino amarelo
      //Coloca um delay de 3s (30x0,1s)
      movlw 0x1E // Passa para w = 30d
      movwf delay_reg3 // Atribui 30 decimal ao delay_reg3
   
   loopDelayRed2:
      decfsnz delay_reg3; //Caso delay_reg3 seja diferente de zero, pula a próxima linha
      goto loopGreen
      call Delay100ms // vai pra subrotina Delay100ms
   goto loopDelayRed2 // volta a repetir o loopDelay até que delay_reg3 seja 0

#endasm
}



#include <Wire.h>
#include <Keyboard.h>
#define botoes A0 //Leitura dos botões da linha
#define botoes2 A1 //Leitura dos botões da coluna
#define botao1 A5 //Botão space
#define botao2 A4 //Botão backspace

int valor = 0; //Valor para leitura do A0
float x = 0.0; //Valos para transformar o valor em tensão
int valor2 = 0; //Valor para leitura de A1
float y = 0.0; //Valor para transformar o valor2 em tensão
bool botaoPressionado = false; //Flag para verificar se botão está pressionado
bool bttn1 = false;
bool bttn2 = false;
 
int linhaComando = 0; //Valor do botão da linha que foi pressionado
int colunaComando = 0; //Valor do botão da coluna que foi pressionado
//Quando os dois valores forem diferentes de zero, a letra será escrita;


void setup()
{
  delay(500); //Delay de meio segundo
  Wire.begin(); //Começando conexão I2C
  pinMode(botoes, INPUT); //Definindo pino A0 para a variavel Botões
  pinMode(botoes2, INPUT); //Definindo pino A1 para a variavel Botões2 
  pinMode(botao1, INPUT); //Definindo pino A2 para a variavel Botao (space)
  pinMode(botao2, INPUT); //Definindo pino A3 para a variavel Botao2 (backspace)
  Serial.begin(9600); //Inicia o Serial
  Keyboard.begin(); //Inicia a biblioteca do Teclado;
  
}

void loop() { //Main
 //Wire.beginTransmission(0x10);
 Serial.println(digitalRead(botao1)); //Verificando conexão do botao space
 Serial.println(digitalRead(botao2)); //Verificando conexão do botão backspace
 
 valor = analogRead(botoes); //Lê o valor de A0, que refere-se aos botoes das linhas
 x = (valor * 5.0)/1023; //Converte o valor para representar a tensão que está passando (volts);
 
 valor2 = analogRead(botoes2); //Faz a mesma coisa para os botoes da coluna
 y = (valor2 *5.0)/1023; //Mesma coisa
  
 delay(100); //Delay de 1 decimo de segundo
 //Serial.print("Linha:");
 //Serial.println(x);
 //Serial.print("Coluna: ");
 //Serial.println(y);  
 
 Serial.println(x);
 Serial.println(y);
 
 linha(x); //Chama a função linha para verificar qual botão foi apertado de acordo com o valor de X (varia de 0.0v a 5.0v)
 coluna(y); //Chama a função coluna para verificar qual botão foi apertado de acordo com o valor de Y (varia de 0.0v a 5.0v)



}




//Metodos

void enviarDados(int numero, int flag){ //Método para envias os dados para o outro arduíno. O primeiro número é o valor que vai ser enviado (de 0 a 8) e o segundo é a flag para dizer se é a coluna ou a linha que vai acender);
 Wire.beginTransmission(0x10); //Começa a transmissão no endereço 0x10
 Wire.write(flag); //Diz se é a linha ou coluna, dependendo do valor (vai de 0 a 2, 0 é nulo, 1 linha, 2 coluna);
 Wire.write(numero); //Valor para definir qual linha ou coluna será acessa (vai de 0 a 8);
 Wire.endTransmission();    //Termina a transmissão

 //obs.: o método recebe o numero primeiro e a flag depois, porém envia a flag primeiro e o numero depois;
 
 Serial.println(flag);
 Serial.println(numero);
}

float linha(float v){ //Método para verificar qual botão foi pressionado
 if(v > 4.8){ //Valor para botões soltos;
   botaoPressionado = false; //Coloca a flag do botão como falso (Não pressionado)
 }
  
 if(v < 4.8 && v > 4.2){ //Botão 1 pressionado
   botaoPressionado = true; //Muda a flag para true (Pressionado);
   enviarDados(1, 1); //Envia os dados 1 para valor e 1 para a flag (linha)
   
   //Caso o botão seja apertado uma vez, ele acenderá uma linha, se for pressionado novamente ele irá apagar aquela linha.
   //Além disso, preciso salvar o valor da tecla que foi pressionada para posteriormente escrever a linha correspondente a aqueles indices;
   
   if(linhaComando == 1){ //Caso essa tecla tenha sido apertada anteriormente
      linhaComando = 0; //Muda o valor da linhaComando para 0;
   }else{ //Caso não tenha sido apertado
      linhaComando = 1; //Muda o valor da linhaComando para 1; 
   }
      Serial.println(linhaComando);

   while(botaoPressionado){ //Loop para impedir que o botão continue enviando os dados varias vezes;
      delay(100);
      valor = analogRead(botoes); //Verifica se o botão continua pressionado atráves da leitura do valor;
      v = (valor * 5.0)/1023;
      if(v > 4.8){ //Se o valor for superior a 4.8, significa que o botão está solto
        botaoPressionado = false; //Muda a flag para false e consegue sair do loop;
      Serial.println("botao1");
      }  
   }
   enviarDados(0,0); //Envia o valor 0,0 para o outro arduino.
 }
  
 if(v < 4.2 && v > 3.8){
   botaoPressionado = true;
   enviarDados(2, 1);
   if(linhaComando == 2){
      linhaComando = 0;
   }else{
      linhaComando = 2;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("botao2");
      }  
   }
   enviarDados(0,0);
 }
  
 if(v < 3.8 && v > 3.2){
   botaoPressionado = true;
   enviarDados(3, 1);
   if(linhaComando == 3){
      linhaComando = 0;
   }else{
      linhaComando = 3;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("botao3");
      }  
   }
   enviarDados(0,0);
 }
  
 if(v < 3.2 && v > 2.8){
   botaoPressionado = true;
   enviarDados(4, 1);
   if(linhaComando == 4){
      linhaComando = 0;
   }else{
      linhaComando = 4;
   }
      Serial.println(linhaComando);

   Serial.println(linhaComando);
   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("botao4");
      }  
   }
   enviarDados(0,0);
 }
 
 if(v < 2.8 && v > 2.2){
   botaoPressionado = true;
   enviarDados(5, 1);
   if(linhaComando == 5){
      linhaComando = 0;
   }else{
      linhaComando = 5;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("Botao5");
      }  
   }
   enviarDados(0,0);
 }
  
 if(v < 2.2 && v > 1.8){
   botaoPressionado = true;
   enviarDados(6, 1);
   if(linhaComando == 6){
      linhaComando = 0;
   }else{
      linhaComando = 6;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("Botao6");
      }  
   } 
   enviarDados(0,0);
 }
  
 if(v < 1.8 && v > 1.2){
    
   botaoPressionado = true;
   enviarDados(7, 1);
   if(linhaComando == 7){
      linhaComando = 0;
   }else{
      linhaComando = 7;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("Botao7");    
      }  
   }
   enviarDados(0,0);
   
 }
  
 if(v < 1.2){
   botaoPressionado = true;
   enviarDados(8, 1);
   if(linhaComando == 8){
      linhaComando = 0;
   }else{
      linhaComando = 8;
   }
      Serial.println(linhaComando);

   while(botaoPressionado){
      delay(100);
      valor = analogRead(botoes);
      v = (valor * 5.0)/1023;
      if(v > 4.8){
        botaoPressionado = false;
        Serial.println("Botao8");
      }  
   }
    enviarDados(0,0);
 }
}

float coluna(float v2){  //Método para verificar qual botão foi pressionado, só que agora funciona para as colunas
 if(v2 > 4.8){
   botaoPressionado = false;
   //Serial.println("Solto"); 
 }
  
 if(v2 < 4.8 && v2 > 4.2){
   botaoPressionado = true;
   
   enviarDados(8, 2);
   
   if(colunaComando == 8){
      colunaComando = 0;

   }else {
      colunaComando = 8;
   }
   

      Serial.println(colunaComando);

   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
        Serial.println("botao1");
      }  
   }
   enviarDados(0,0);  
 }
  
 if(v2 < 4.2 && v2 > 3.8){
   botaoPressionado = true;
   enviarDados(7, 2);
   if(colunaComando == 7){
      colunaComando = 0;
   }else {
      colunaComando = 7;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
      Serial.println("botao2");
      }  
   }  
   enviarDados(0,0);
   }
  
 if(v2 < 3.8 && v2 > 3.2){
  botaoPressionado = true;
   enviarDados(6, 2);
   if(colunaComando == 6){
      colunaComando = 0;
   }else {
      colunaComando = 6;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
        Serial.println("botao3");
      }  
   }
   enviarDados(0,0);

 }
  
 if(v2 < 3.2 && v2 > 2.8){
  botaoPressionado = true;
   enviarDados(5, 2);
   if(colunaComando == 5){
      colunaComando = 0;
   }else {
      colunaComando = 5;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
      Serial.println("botao4");
      }  
   }
   enviarDados(0,0);
   
 }
 
 if(v2 < 2.8 && v2 > 2.2){
  botaoPressionado = true;
   enviarDados(4, 2);
   if(colunaComando == 4){
      colunaComando = 0;
   }else {
      colunaComando = 4;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
        Serial.println("Botao5");
      }  
   }  
   enviarDados(0,0);
 }
  
 if(v2 < 2.2 && v2 > 1.8){
   botaoPressionado = true;
   enviarDados(3, 2);
   if(colunaComando == 3){
      colunaComando = 0;
   }else {
      colunaComando = 3;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
      Serial.println("Botao6");
      }  
   } 
   enviarDados(0,0);
   
 }
  
 if(v2 < 1.8 && v2 > 1.2){
  botaoPressionado = true;
   enviarDados(2, 2);
   if(colunaComando == 2){
      colunaComando = 0;
   }else {
      colunaComando = 2;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
        Serial.println("Botao7");
      }  
   }  
   enviarDados(0,0);
 }
  
 if(v2 < 1.2){
  botaoPressionado = true;
   enviarDados(1, 2);
   if(colunaComando == 1){
      colunaComando = 0;
   }else {
      colunaComando = 1;
   }
   Serial.println(colunaComando);
   while(botaoPressionado){
      delay(100);
      valor2 = analogRead(botoes2);
      v2 = (valor2 * 5.0)/1023;
      if(v2 > 4.8){
        botaoPressionado = false;
      Serial.println("Botao8"); 
      }  
   }  
   enviarDados(0,0);
   
 }
  
  if(linhaComando != 0 && colunaComando != 0){ //Verifica se LinhaComando e ColunaComandos são diferentes de zero
    mostrarLetra(); //Escreve a letra;
  }
}


void mostrarLetra(){ //Método que escreve a letra de acordo com os valores que foram inseridos na leitura dos botões;
  if(linhaComando == 8){ //Se a linha for a primeira
    switch(colunaComando){ //Switch para verificar qual coluna foi selecionada
      case 1:            
        Keyboard.println("A");
        break;
      case 2:
        Keyboard.println("B");
        break;
      case 3:
        Keyboard.println("C");
        break;
      case 4:
        Keyboard.println("D");
        break;
      case 5:
        Keyboard.println("E");
        break;
      case 6:
        Keyboard.println("F");
        break;
      case 7:
        Keyboard.println("G");
        break;
      case 8:
        Keyboard.println("H");
        break;

    }
  } 
  if(linhaComando == 7){
    switch(colunaComando){
      case 1:
      Keyboard.println("I");
      break;
      case 2:
      Keyboard.println("J");
      break;
      case 3:
      Keyboard.println("K");
      break;
      case 4:
      Keyboard.println("L");
      break;
      case 5:
      Keyboard.println("M");
      break;
      case 6:
      Keyboard.println("N");
      break;
      case 7:
      Keyboard.println("O");
      break;
      case 8:
      Keyboard.println("P");
      break;

    }
  } 
  if(linhaComando == 6){
    switch(colunaComando){
      case 1:
      Keyboard.println("Q");
      break;
      case 2:
      Keyboard.println("R");
      break;
      case 3:
      Keyboard.println("S");
      break;
      case 4:
      Keyboard.println("T");
      break;
      case 5:
      Keyboard.println("U");
      break;
      case 6:
      Keyboard.println("V");
      break;
      case 7:
      Keyboard.println("W");
      break;
    case 8:
      Keyboard.println("X");
      break;
      }
  } 
  if(linhaComando == 5){
    switch(colunaComando){
      case 1:
      Keyboard.println("Y");
      break;
      case 2:
      Keyboard.println("Z");
      break;
      case 3:
      Keyboard.println("!");
      break;
      case 4:
      Keyboard.println("?");
      break;
      case 5:
      Keyboard.println("Z");
      break;
      case 6:
      Keyboard.println("?");
      break;
      case 7:
      Keyboard.println("!");
      break;
      case 8:
      Keyboard.println("@");
    break;
      
    }
  }
  if(linhaComando == 4){
    switch(colunaComando){
      case 1:
      Keyboard.println("4,1");
      break;
      case 2:
      Keyboard.println("4,2");
      break;
      case 3:
      Keyboard.println("4,3");
      break;
      case 4:
      Keyboard.println("4,4");
      break;
      case 5:
      Keyboard.println("4,5");
      break;
      case 6:
      Keyboard.println("4,6");
      break;
      case 7:
      Keyboard.println("4,7");
      break;
      case 8:
      Keyboard.println("4,8");
      break;

    }
  } 
  if(linhaComando == 3){
    switch(colunaComando){
      case 1:
      Keyboard.println("3,1");
      break;
      case 2:
      Keyboard.println("3,2");
      break;
      case 3:
      Keyboard.println("3,3");
      break;
      case 4:
      Keyboard.println("3,4");
      break;
      case 5:
      Keyboard.println("3,5");
      break;
      case 6:
      Keyboard.println("3,6");
      break;
      case 7:
      Keyboard.println("3,7");
      break;
      case 8:
      Keyboard.println("3,8");
      break;

    }
  } 
  
  if(linhaComando == 2){
    switch(colunaComando){
      case 1:
      Keyboard.println("2,1");
      break;
      case 2:
      Keyboard.println("2,2");
      break;
      case 3:
      Keyboard.println("2,3");
      break;
      case 4:
      Keyboard.println("2,4");
      break;
      case 5:
      Keyboard.println("2,5");
      break;
      case 6:
      Keyboard.println("2,6");
      break;
      case 7:
      Serial.println("2,7");
      break;
      case 8:
      Keyboard.println("2,8");
      break;

    }
  } 
  
  if(linhaComando == 1){
    switch(colunaComando){
      case 1:
      Keyboard.println("1,1");
      break;
      case 2:
      Keyboard.println("1,2");
      break;
      case 3:
      Keyboard.println("1,3");
      break;
      case 4:
      Keyboard.println("1,4");
      break;
      case 5:
      Keyboard.println("1,5");
      break;
      case 6:
      Keyboard.println("1,6");
      break;
      case 7:
      Keyboard.println("1,7");
      break;
      case 8:
      Keyboard.println("1,8");
      break;

    }
  } 
  //Zerando as variaveis;
  linhaComando = 0;
  colunaComando = 0;
  linhaComando = 0;
  colunaComando = 0;
  linhaComando = 0;
  colunaComando = 0;
  return;
}

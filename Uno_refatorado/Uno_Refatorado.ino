#include <Wire.h>

int valor = 0; //Valor que será recebido pelo arduíno pelo I2C
int aux = 0; //Flag que vai verificar se o valor é da linha ou da coluna
int primeiroValor = 0;  /*Variaveis para salva os valores dos indices de cada linha e coluna */
int segundoValor = 0;
int primeiroAux = 0; /*Variaveis para salvar as flags de linha e coluna*/
int segundoAux = 0;
bool flag = false; /*flag*/

// anodes
int row[] = {17, 16, 15, 14, 2, 3, 4, 5}; /*portas que vão para os positivos dos leds (linhas)*/
/* Indice   [ 0,  1,  2,  3, 4, 5, 6, 7]
 *  
 */
// cathodes
int col[] = {13, 12, 11, 10, 9, 8, 7, 6}; /*portas que irão para os negativos dos leds (colunas)*/
/*  Indice  [ 0,  1,  2,  3, 4, 5, 6, 7]
 *           
 */
void setup()
{
  Wire.begin(0x10); //Começa a conexão com o outro arduíno de endereço 0x10 
  Wire.onReceive(recebe); //Quando receber, manda os dados para o metodo recebe()
  Serial.begin(9600);// Inicia o Serial

  for (int i=0;i<8;i++) //Defini as portas como saídas para os leds
  {
    pinMode(row[i], OUTPUT); //linhas
    pinMode(col[i], OUTPUT); //colunas
  } 
  allOff(); //desliga todos os leds
  
  ligar(); //Método para fazer a animação de ligar o teclado
  
}


void loop()
{
  Serial.print("Valor: ");
  Serial.println(valor);
  Serial.print("Aux: ");
  Serial.println(aux);
 

  
  if(flag){
    Serial.println("Flag: true");
  }
  else{
    Serial.println("Flag: false");
  }

  
  /*PRIMEIRO CASO: É o primeiro valor que está chegando: 
   *DadosEnviados (1, 1) 1 = flag, 1 = valor;
      aux = 1
      valor = 1
      valorPrimeiro = 0
      auxPrimeiro = 0*/
   if(aux != 0 && valor != 0 && flag == false && primeiroValor == 0 && primeiroAux == 0){ 
    //Se: aux e valor forem diferentes de zero e valorTempo e auxTemp não tiverem sido iniciados ainda (pois é a primeira vez)

    /*É necessário fazer isso pois:
     * As variaveis aux e valor vão receber os dados novamente nelas.
     * Logo os seus valores irão mudar
    */
    primeiroAux = aux; //Coloca o inteiro de aux em auxTemp
    primeiroValor = valor; //Colocar o inteiro de valor em ValorTemp

  
    
    flag = true; //Muda flag para true
  }
  /*SEGUNDO CASO: ValorRecebido ex.:(1,1), flag = 1, valor = 1
  
    O valor que vai ser recebido é o mesmo do anterior.
    O mesmo botão foi pressionado para apagar o led que foi acesso anteriormente.
    aux = 1;
    valor = 1;
  */
  else if(aux != 0 && valor == primeiroValor && primeiroAux == aux){
    /* Se o aux for diferente de zero e o valor e o aux forem iguais aos valores salvos na variaveis, 
     *  significa que o mesmo botão foi pressionado
    */
    primeiroValor = 0; /*Zera as variaveis e muda a flag para false*/
    primeiroAux = 0;
    flag = false;
  }


  /* TERCEIRO CASO: O valor do aux é o mesmo, porém o valor é diferente
     Significa que um botão diferente foi apertado, porém foi um botão do mesmo conjunto (linha ou coluna)
     por exemplo (1, 3);
     Aux = 1 (linha)
     Valor = 3 (terceira linha)

     Logo o que deverá acontecer é, seguindo a lógica dos exemplos
     A linha 1 se apagar e a linha 3 acender;
  */
  if(primeiroAux == aux && primeiroValor != valor){
    /* Se o aux for igual ao primeiroAux e o valor for diferente*/
    primeiroValor = valor; //Salva o novo valor na variavel
  }

  /*QUARTO CASO: O valor de aux e de valor são diferentes dos salvos nas variaveis
    Logo: outro botão de outro conjunto foi pressionado
    Ex.: (2,3) 
    aux = 2 (coluna)
    valor = 3 (terceira coluna)
  */
  if(primeiroAux != aux && primeiroAux != 0){
   /* Se o primeiroAux for diferente de 0 e diferente do anterior
    *  Salva os novos valores nas variaveis para o segundo valos e aux
    */
    segundoValor = valor;
    segundoAux = aux;
  }

  acenderMatriz(); //Método que acende a matriz, ele fica verificando todo loop caso algum valor tenha mudado, já que são variaveis globais

  Serial.print("ValorPrimeiro: ");
  Serial.println(valorPrimeiro);
  Serial.print("AuxPrimeiro: ");
  Serial.println(auxPrimeiro);
  Serial.println("----------------------------------");
  Serial.print("ValorSegundo: ");
  Serial.println(valorSegundo);
  Serial.print("AuxSegundo: ");
  Serial.println(auxSegundo);

  Serial.println("----------------------------------");
  delay(0);
}

void ligar(){/*Métood que faz a animação de ligar do teclado*/

  for(int i = 0; i < 8; i++){
    digitalWrite(row[i], HIGH);
    digitalWrite(col[i], LOW);
    delay(100);
  }
  allOff();
  
  
}

void acenderMatriz(){ //Método que acende dependendo do valor que as variaveis recebem
  /* CASO 1:
   *  Se o primeiroAux = 1, significa que o valor de valorPrimeiro é diferente de 0
      Ele deve acender a linha correspondentem a linha de leds
   */
 if(primeiroAux == 1 && primeiroValor != 0){ 
  /* Necessário fazer teste para ligar fila inteira de leds
   * 
   */
//Versão original
//   for (int i=0; i < 8; i++){
//      digitalWrite(row[valorPrimeiro-1], HIGH);
//      digitalWrite(col[i], LOW);
//      delay(10);
//      }
//    allOff();

    //Versão modificada para acender fila inteira de leds
    digitalWrite(row[valorPrimeiro-1], HIGH);
    for (int i=0; i < 8; i++){
      digitalWrite(col[i], LOW);
    }
  }

  /* CASO 2:
   *  Se o primeiroAux = 2, significa que o valor de valorPrimeiro é diferente de 0
      Ele deve acender a coluna correspondentem a coluna de leds
   */

//Versão original
//  if(auxPrimeiro == 2){
//    for (int i=0; i < 8; i++){
//      digitalWrite(row[i], HIGH);
//      digitalWrite(col[valorPrimeiro-1], LOW);
//      delay(10);
//    }
//    allOff();



//Versão modificada para acender linhas de leds
  if(auxPrimeiro == 2){
    digitalWrite(col[valorPrimeiro-1], LOW);
    for (int i=0; i < 8; i++){
      digitalWrite(row[i], HIGH);
    }

  }

  
   /* CASO 3:
   *  Se o primeiroAux = 2, significa que o valor de valorPrimeiro é diferente de 0
      Ele deve acender a coluna correspondentem a coluna de leds
   */
  if(primeiroValor != 0 && segundoValor != 0){
    if(primeiroAux == 1){
      for(int i = 0; i < 6; i++){
        digitalWrite(row[primeiroValor-1], HIGH);
        digitalWrite(col[segundoValor-1], LOW);
        delay(150);
        allOff();    
                
      }     
      primeiroAux = 0;
      primeiroValor = 0;
      segundoAux = 0;
      segundoValor = 0;
      flag = false;
    }
    
    if(primeiroAux == 2){
      for(int i = 0; i < 5; i++){
            digitalWrite(row[segundoValor-1], HIGH);
            digitalWrite(col[primeiroValor-1], LOW);
            delay(10);
            allOff();
      }      
      primeiroAux = 0;
      primeiroValor = 0;
      segundoAux = 0;
      segundoValor = 0;
      flag = false;
    }
  }

}

void allOff() //Método que desliga todos os leds;
{
  for (int i=0;i<8;i++)
  {
    digitalWrite(row[i], LOW);
    digitalWrite(col[i], HIGH);
    delay(0);
  }
}

void recebe(int numBytes){ //Método que recebe os dados enviados do outro arduíno
  
  if(Wire.available()){ //Se tiverem dados a serem recebidos
    aux = Wire.read(); //Aux vai receber a flag (se é linha ou coluna)
    valor = Wire.read(); //valor recebe valor
  }
}

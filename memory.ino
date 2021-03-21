//pinos das leds
#define pinLedR 2
#define pinLedG 3
#define pinLedB 4
#define pinLedGO 5

//pinos dos botoes
#define pinBtnR 8
#define pinBtnG 9
#define pinBtnB 10

//pino do buzz
#define pinBuzz 11

//quantidade de jogadas
#define MAX_COLORS 5

//tempo que a led ficara acessa
#define DELAY_LED 500

//variaveis auxiliares
int actualNumber = 0;
int countRead = 0;
int sortColors[MAX_COLORS];
boolean generatingColor;
boolean selectingColor;

void setup()
{
  //pino nao utilizado para gerar randomicos
  randomSeed(analogRead(A0));

  generatingColor = true;
  selectingColor = false;
  
  actualNumber = 0;
  countRead = 0;
    
  pinMode(pinLedR, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedG, OUTPUT);
  pinMode(pinLedGO, OUTPUT);
  
  pinMode(pinBuzz, OUTPUT);
  
  pinMode(pinBtnR, INPUT);
  pinMode(pinBtnB, INPUT);
  pinMode(pinBtnG, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if(generatingColor){//sistema define a cor na posicao atual
    sortColors[actualNumber] = getPinColor(getRandom());

    blinkSortColors();

    waitColor();
  }else if(selectingColor){//aguarda o input do jogador
    readInput();
  }
}

//piscar as leds da sequencia
void blinkSortColors(){
  delay(DELAY_LED);
  
  for(int i = 0; i <= actualNumber; i ++){
    blinkLed(sortColors[i]);
  }
}

//piscar apenas uma led
void blinkLed(int pinLed){
  setLed(pinLed, true);
  delay(DELAY_LED);
    
  setLed(pinLed, false);
  delay(DELAY_LED);
}

//ativar/desativar uma led
void setLed(int pinLed, boolean isActive){
  digitalWrite(pinLed, isActive);
}

//gerar um numero randomico para definir uma cor
//0 = vermelho
//1 = verde
//2 = azul
int getRandom(){
  int rnd = random(9000);
  
  if(rnd <= 3000) return 0;
  else if(rnd <= 6000) return 1;
  else return 2;
}

//leitura dos botoes pressionados pelo usuario
void readInput(){
  int ledDelay = 200;
  
  if(digitalRead(pinBtnR)){
    checkColor(countRead, pinLedR);
    delay(ledDelay);
  }else if(digitalRead(pinBtnG)){
    checkColor(countRead, pinLedG);
    delay(ledDelay);
  }else if(digitalRead(pinBtnB)){
    checkColor(countRead, pinLedB);
    delay(ledDelay);
  }
}

//verifica se o jogador acertou ou errou a sequencia a cada jogada
void checkColor(int position, int pinLed){
  blinkLed(pinLed);
  
  if(sortColors[position] == pinLed){
    countRead ++;
    
    if(countRead == MAX_COLORS){
      win();
    }else if(countRead == actualNumber){
      waitSort();
    }    
  }else{
    lose();
  }
}

//retorna o pino da led relacionada a cada cor
int getPinColor(int number){
  int pinToReturn;
  
  switch(number){
    case 0:
      pinToReturn = pinLedR;
      break;
    case 1:
      pinToReturn = pinLedB;
      break;
    case 2:
      pinToReturn = pinLedG;
      break;
  }
  
  return pinToReturn;
}

//reseta as variaveis para que seja a vez do jogador
void waitColor(){
  setLed(pinLedGO, true);
  
  generatingColor = false;

  selectingColor = true;
  
  actualNumber ++;
}

//reseta as variaveis para o sistema definir uma nova cor
void waitSort(){
  setLed(pinLedGO, false);
  
  generatingColor = true;

  selectingColor = false;
  
  countRead = 0;
}

//se o jogador ganhar, as leds irao piscar com um bip sonoro
void win(){
  int delayLed = 200;
  
  while(true){
    tone(pinBuzz, 2000);   
  
    setLed(pinLedR, true);
    setLed(pinLedB, true);
    setLed(pinLedG, true);
    setLed(pinLedGO, true);

    delay(delayLed);

    noTone(pinBuzz);

    setLed(pinLedR, false);
    setLed(pinLedB, false);
    setLed(pinLedG, false);
    setLed(pinLedGO, false);

    delay(delayLed);
  }
}

//se o jogador perder, as leds ficaram acesas com um unico bip sonoro
void lose(){
  tone(pinBuzz, 1500);
  
  delay(200);
  
  noTone(pinBuzz);
  
  while(true){
    setLed(pinLedR, true);
    setLed(pinLedB, true);
    setLed(pinLedG, true);
    setLed(pinLedGO, true);
  }
}

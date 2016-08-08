boolean debug = true;
int led = 13;//Pino de LED Simulando Bomba.... Em producao utilizar outro pino digital
int softwareResetPin = 12;
int buzzerPin = 9;
int intervaloDesligado = 60 * 60 * 1000;//3600000; //milisegundos 1000 x 60seg x 60min
int intervaloLigado = 60 * 15 *1000;//900000; //milisegundos 1000 x 60seg x 15min
unsigned long tempoAnterior = 0; 
int bombaStatus = HIGH; //Inicia com a bomba ligada
int bombaPreviousStatus = LOW;
unsigned long tempoAtual =0;
unsigned long tempoTotal =0;

long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second


// Bootloader Setup
void setup() { 

  digitalWrite(softwareResetPin, HIGH);
  pinMode(buzzerPin, OUTPUT); // Pino do Piezo Buzzer
  pinMode(led, OUTPUT);    //Pino do Rele (Ou LED em Modo Teste)
  pinMode(softwareResetPin, OUTPUT);  // Pino do RESET via Software
  delay(200);

  // Inicializa porta Serial / Bluetooth
  Serial.begin(9600);//initialize Serial Port

  Serial.println("\nIniciando Sistema!");

  //beepLiga();
}

void printDigits(byte digits){

  Serial.print("");
  if(digits < 10){
    Serial.print('0');
  }
  Serial.print(digits,DEC);
}

void beepLiga(){
  delay(200);
  tone(buzzerPin, 1260, 100);
  delay(150);
  tone(buzzerPin, 1260, 100);
  delay(150);
  tone(buzzerPin, 1260, 100);
  delay(150);
}

void beepDesliga(){
  tone(buzzerPin, 650, 200);
  delay(300);
  tone(buzzerPin, 650, 200);
  delay(300);
}

void printDebug(char* text){
  if(debug == true){
    Serial.print(text);
  }
}

void printTempo(int hours, int minutes, int seconds){
  Serial.println("");
  printDigits(hours);  
  Serial.print(":");
  printDigits(minutes);
  Serial.print(":");
  printDigits(seconds);
}



void controleBomba(){

  tempoAtual = millis();

  int days = tempoAtual / day ;                                //number of days
  int hours = (tempoAtual % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((tempoAtual % day) % hour) / minute ;         //and so on...
  int seconds = (((tempoAtual % day) % hour) % minute) / second;
  
  printTempo(hours, minutes, seconds);

  Serial.print(" - Bomba Status: ");
  Serial.print(digitalRead(led));
  Serial.print(" - Bomba Status Anterior: ");
  Serial.print(bombaPreviousStatus);

 
  if((bombaStatus != bombaPreviousStatus) && (bombaStatus == HIGH)){
    printDebug(" - Bomba Ligada !");
  }
  
  
  if(minutes >= 15){    //Se ultrapassou o tempo de manter bomba ligada, desliga
    
    if(bombaStatus == HIGH){
        Serial.print(" - Desligando Bomba !");
        bombaPreviousStatus = bombaStatus;
        bombaStatus = LOW;
     }
     
    if(hours >= 1){     //Se estiver em execucao mais do que xx horas (Ciclo de Hardware Reset)
      beepDesliga();
      reset();
    }

  }
  else{    
    bombaPreviousStatus = bombaStatus;
    bombaStatus = HIGH;
  }    

  digitalWrite(led, bombaStatus);     //Chaveia Bomba
  
   
  
  // Serial.println("");
  
  delay(1000);
}


void reset(){
  Serial.println("Reiniciando o sistema");
  digitalWrite(softwareResetPin, LOW);
}

void loop() {    
  controleBomba();
}


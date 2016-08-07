int led = 13;//Pino de LED Simulando Bomba.... Em producao utilizar outro pino digital
int softwareResetPin = 12;
int buzzerPin = 9;
int intervaloDesligado = 60 * 60 * 1000;//3600000; //milisegundos 1000 x 60seg x 60min
int intervaloLigado = 60 * 15 *1000;//900000; //milisegundos 1000 x 60seg x 15min
unsigned long tempoAnterior = 0; 
int bombaStatus = HIGH; //Inicia com a bomba ligada
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
  
  Serial.print("Iniciando Sistema!");
  
  beepLiga();
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

void controleBomba(){
  
  tempoAtual = millis();
  //Serial.println(tempoAtual);
  
  int days = tempoAtual / day ;                                //number of days
  int hours = (tempoAtual % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((tempoAtual % day) % hour) / minute ;         //and so on...
  int seconds = (((tempoAtual % day) % hour) % minute) / second;

   // digital clock display of current time
   //Serial.print(days,DEC);  
   printDigits(hours);  
   print(":");
   printDigits(minutes);
   print(":");
   printDigits(seconds);
   Serial.println();
  
  //Se ultrapassou o tempo de manter bomba ligada, desliga
  if(minutes >= 15){
    bombaStatus = LOW;
    Serial.print("Desligando Bomba ! ");
    //Serial.println(tempoAtual);
    
    if(hours >= 1){ //Se estiver em execucao mais do que a soma dos tempos ligado e aguardando
      //Ultrapassou o tempo de espera
      Serial.print("Reset - Reiniciando Ciclo");
      //Serial.println(tempoAtual);
      beepDesliga();
      reset();
    }
    
  }else{
    bombaStatus = HIGH;
    Serial.print("Bomba Ligada!");
    //Serial.println(tempoAtual);
  }    
  
  digitalWrite(led, bombaStatus); //Liga Bomba
}
  
void reset(){
    Serial.println("Reiniciando o sistema");
    delay(200);
    digitalWrite(softwareResetPin, LOW);
}

void loop() {  
  //digitalWrite(led, bombaStatus);   // turn the LED on (HIGH is the voltage level)
  controleBomba();
}

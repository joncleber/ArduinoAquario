
// Carrega a biblioteca do LCD
#include <LiquidCrystal.h>
#include <DS1307.h>

//Modulo RTC DS1307 ligado as portas A4 e A5 do Arduino 
DS1307 rtc(A4, A5);
#define pin A0 // Pino analogico para ligacao do LM35
#define aquecedor 13
#define Botao_Mais 7
#define Botao_Menos 8
#define LDR A1
#define Luz 6

int valorldr = 0;
//temperatura que usuario quer
int Temp_user = 23;

// Variaveis que armazenam a temperatura em Celsius e Fahrenheit
int tempc = 0; 
//int tempf=0;
int samples[8]; // Array para precisão na medição

// Variáveis que guardam a temperatura máxima e mínima
int maxtemp = -100,mintemp = 100; 
int i;

// Inicializa o LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
   //Aciona o relogio
  rtc.halt(false);
   
  pinMode(aquecedor, OUTPUT);
  pinMode(Luz, OUTPUT);
  pinMode(Botao_Mais, INPUT);
  pinMode(Botao_Menos, INPUT);
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  //rtc.setDOW(THURSDAY);      //Define o dia da semana
  //rtc.setTime(17, 34, 0);     //Define o horario
  //rtc.setDate(14, 6, 2018);   //Define o dia, mes e ano
  
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);

  // Define o LCD com 20 colunas e 4 linhas
  lcd.begin(20, 4);
  
  // Mostra informacoes no display
  lcd.setCursor(8,0);
  lcd.print("Aquario Auto");
  //lcd.setCursor(2,1);
  //lcd.print("Display LCD 20x4");
  //lcd.setCursor(1,3);
  //lcd.print("arduinoecia.com.br");
}
void temp_aqua()
{
  lcd.setCursor(0,1);
  lcd.print("Temp Aquario :");
  lcd.setCursor(16,1);
  lcd.print(tempc,DEC);
  if(digitalRead(aquecedor) == 0)
  {
    lcd.setCursor(19,1);
    lcd.print("L");
  }
  else
  {
    lcd.setCursor(19,1);
    lcd.print("D");
  }
}
void mostra_Hora()
{
  lcd.setCursor(0,3);
  lcd.print("Horas :");
  lcd.setCursor(8,3);
  lcd.print(rtc.getTimeStr());
}
void mostra_temp_user()
{
  lcd.setCursor(0,2);
  lcd.print("Temp User    :");
  lcd.setCursor(16,2);
  lcd.print(Temp_user,DEC);
}
void mostra_luz()
{
  lcd.setCursor(0,0);
  //lcd.
  lcd.print(valorldr);
}

void loop()
{
   for(i = 0;i<=7;i++){ // Loop que faz a leitura da temperatura 8 vezes
    samples[i] = ( 5.0 * analogRead(pin) * 100.0) / 1024.0;
    //A cada leitura, incrementa o valor da variavel tempc
    tempc = tempc + samples[i]; 
    delay(50);
  } 

    // Divide a variavel tempc por 8, para obter precisão na medição
  tempc = tempc/8.0; 
  //Converte a temperatura em Fahrenheit e armazena na variável tempf
  //tempf = (tempc * 9)/ 5 + 32; 
  //Armazena a temperatura máxima na variável maxtemp
  if(tempc > maxtemp) {maxtemp = tempc;} 
  //Armazena a temperatura máxima na vari[avel mintemp
  if(tempc < mintemp) {mintemp = tempc;} 

  valorldr = analogRead(LDR);
  //int valormap = map(valorldr,0,1023,0,100);

  temp_aqua();
  mostra_Hora();
  if(digitalRead(Botao_Mais) == HIGH)
  {
    Temp_user ++;
  }
  else if(digitalRead(Botao_Menos) == HIGH)
  {
    Temp_user --;
  }
  mostra_temp_user();


  if(Temp_user !=  tempc)
  {
      if(Temp_user > tempc)
      {
        
        digitalWrite(aquecedor, 0);
      }
      else if(Temp_user < tempc)
      {
        digitalWrite(aquecedor, 1);
      }
      delay(100);
  }
  if(valorldr < 50)
  {
    digitalWrite(Luz, 0);
  }
  else if(valorldr > 50)
  {
    digitalWrite(Luz, 1);
    
  }
  mostra_luz();
  
  

  delay(50);


}

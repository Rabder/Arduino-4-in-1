#include <Wire.h>      // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>    // libreria para LCD por I2C
#include <Keypad.h> 
#include <DHT.h>
#include <DHT_U.h>

int x;        // variable para almacenar valor leido del eje X
int y;        // variable para almacenar valor leido del eje y
int BUTTON = 10;    // pulsador incorporado pin digital 10
int SW;       // variable para almacenar valor leido del pulsador
int first;
int count;
String options[] = {"Calculator", "Integer/Binary", "Temperature", "Music"};
bool next1;
int SENSOR = 10;     // pin DATA de DHT22 a pin digital 2
int TEMPERATURA;
int HUMEDAD;


 


bool selection(int x);

DHT dht(SENSOR, DHT22);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // pines RS, E, D4, D5, D6, D7 de modulo 1602A
LiquidCrystal_I2C lcd2 (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7


const byte rows = 4;     // define numero de filas
const byte columns = 4;    // define numero de columnas
char keys[rows][columns] = {    // define la distribucion de teclas
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[rows] = {32,34,36,38};   // pines correspondientes a las filas
byte pinesColumnas[columns] = {40,42,44,46}; // pines correspondientes a las columnas

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, rows, columns);  // crea objeto

char TECLA;       // almacena la tecla presionada


void setup()
{
  pinMode(BUTTON, INPUT);   // pulsador como entrada
  pinMode(8, OUTPUT); // Buzzer setup
  lcd.begin(16, 2);
  Serial.begin(9600);
  dht.begin();
  lcd2.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd2.setBacklight(HIGH);   // habilita iluminacion posterior de LCD
  lcd2.begin(20, 4);     // 16 columnas por 2 lineas para LCD 1602A
  lcd2.clear();      // limpia pantalla
  // entradas analogicas no requieren inicializacion

  lcd2.setCursor(0, 0);    // ubica cursor en columna 0 y linea 0
  first = 0;
  for (int i = 0; i < 4; i++)
  {
    lcd2.setCursor(0, i);
    if (i == 0)
    {
      lcd2.print(options[i] + " <--");
    }
    else
    {
      lcd2.print(options[i]);
    }
  }
  lcd2.setCursor(15, 2);
  lcd2.print("#=ENT");
  lcd2.setCursor(15, 3);
  lcd2.print("*=ESC");
}




void calculator()
{
  int x1 = analogRead(A0);
    int y1 = analogRead(A1);
    char CLAV1[5];
    char START1[1];
    int index1 = 0;
    int counter1 = 0;
    char CLAV2[5];
    char START2[1];
    int index2 = 0;
    int counter2 = 0;

    lcd2.clear();
    lcd2.setCursor(0, 0);

    lcd2.print("Calculator");
    delay(2000);

   
    lcd2.clear();

    lcd2.setCursor(0, 0);
    lcd2.print("First number: ");
    lcd2.setCursor(0, 1);
    while(counter1 < 5)
    {
      char cuskey1 = teclado.getKey();
      if (cuskey1 == '#' or index1 == 4) 
      {
        cuskey1 = false;
        for (int i=0;i<5;i++)
        {
          Serial.print(CLAV1[4]);
        }
        CLAV1[4] = '\0';
        delay(1000);
        lcd2.print(" ");
        break;
      } 
      else if (cuskey1 != '#' and cuskey1)
      {
          lcd2.print(cuskey1);
          CLAV1[index1] = cuskey1;
          Serial.print(CLAV1[index1]);
          Serial.print("to CLAV1");
          Serial.print("\n");
          index1++;
          counter1++;
      }
   }

   lcd2.clear();
   lcd2.setCursor(0, 0);
   lcd2.print("Second number: ");
   lcd2.setCursor(0, 1);
   while(counter2 < 5)
   {
      char cuskey2 = teclado.getKey(); 
      if (cuskey2 != '#' and cuskey2)
      {
          lcd2.print(cuskey2);
          CLAV2[index2] = cuskey2;
          Serial.print(CLAV2[index2]);
          Serial.print("to CLAV2");
          Serial.print("\n");
          index2++;
          counter2++;
      }
      if (cuskey2 == '#' or index2 == 4) 
      {
        delay(1000);
        Serial.print(CLAV2);
        lcd2.print(" ");
        break;
      }

  }

   delay(1000);
   lcd2.clear();
   long num1 = atol(CLAV1);
   long num2 = atol(CLAV2);

   
   //num2 = (num2 - num1) / 100;
   Serial.print(num1);
   Serial.print("\n");
   Serial.print(num2);
   Serial.print("\n");
   Serial.print("\n");
   Serial.print("\n");
   lcd2.setCursor(0, 0);
   lcd2.print("+, -, *, /: ");
   lcd2.setCursor(0, 1);

   while (true)
   {
    char op = teclado.getKey();
    if (op)
    {
      lcd2.print(op);
      delay(1000);
      lcd2.clear();
      lcd2.setCursor(0, 1);
      if (op == 'A')
      {
        long sum = num1+num2;
        lcd2.print(sum);
        delay(5000);
        lcd2.clear();
      }
      if (op == 'B')
      {
        long sum = num1-num2;
        lcd2.print(sum);
        delay(5000);
        lcd2.clear();
      }
      if (op == 'C')
      {
        long sum = num1*num2;
        lcd2.print(sum);
        delay(5000);
        lcd2.clear();
      }
      if (op == 'D')
      {
        lcd2.print((num1*1.0)/(num2*1.0));
        delay(5000);
        lcd2.clear();
      }
    break;
  }
 }
 lcd2.clear();
 lcd2.setCursor(0,0);
 lcd2.print("Restart?");
 lcd2.setCursor(0, 1);
 while (true)
 {
  char op = teclado.getKey();
  if (op)
  {
    if (op == '*')
    {
    lcd2.clear();
    lcd2.print(op);
    }
    else if (op == '#')
    {
    lcd2.print(op);
    delay(500);
    lcd2.clear();
    counter1 = 0;
    counter2 = 0;
    index1 = 0;
    index2 = 0;
    selection(0);
    }
    break;
  }
 }
}

void intbin()
{
  
  long pow2[32];
  pow2[0] = 1;
  pow2[1] = 2;
  for (int i = 2; i < 31; i++)
  {
    pow2[i] = 2 * pow2[i-1];
    if (pow2[i] < 0)
    {
      pow2[i] = pow2[i] * -1;
    }
    Serial.print('\n');
    Serial.print(pow2[i]);
    Serial.print('\n');
  }

  char bin[32];
  
  char index[9];
  int counter = 0;
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Int/Binary");
  delay(2000);
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Integer: ");
  lcd2.setCursor(0, 1);

  while(counter < 9)
    {
      char cuskey1 = teclado.getKey(); 
      if (cuskey1 != '#' and cuskey1)
      {
          lcd2.print(cuskey1);
          index[counter] = cuskey1;
          counter++;
      }
  
    if (cuskey1 == '#' or counter == 9) 
    {
      delay(500);
      lcd2.print(" ");
      break;
    }
   }


   for (int i = 0; i < 31; i++)
   {
    bin[i] = '0';
   }
   long num = atol(index);
   Serial.print('\n');
   Serial.print(num);
   Serial.print('\n');
   int first;
   bool first1 = false;
   for (int i = 30; i >= 0; i--)
   {
    if (num == 0)
    {
      break;
    }
    else if (num  - pow2[i] >= 0)
    {
      Serial.print('\n');
      Serial.print(pow2[i]);
      Serial.print(" is smaller than ");
      Serial.print(num);
      Serial.print('\n');
      num = num - pow2[i];
      bin[i] = '1';
      if (first1 == false)
      {
        first = i;
        first1 = true;
      }
    }
    Serial.print(bin);
   }


   
   lcd2.clear();
   lcd2.setCursor(0,1);
   for (int i = first; i >= 0; i--)
   {
    lcd2.print(bin[i]);
   }
   delay(5000);
 lcd2.clear();
 lcd2.setCursor(0,0);
 lcd2.print("Restart?");
 lcd2.setCursor(0, 1);
 while (true)
 {
  char op = teclado.getKey();
  if (op)
  {
    if (op == '*')
    {
    lcd2.clear();
    lcd2.print(op);
    }
    else if (op == '#')
    {
    lcd2.print(op);
    delay(500);
    lcd2.clear();
    selection(1);
    }
    break;
  }
 }
}


void timetemp()
{
  TEMPERATURA = dht.readTemperature() * 0.83;
  HUMEDAD = dht.readHumidity();
  int check = 0;
  char TECLA1;
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Temperature: ");
  lcd2.print(TEMPERATURA);
  lcd2.print(" C");
  lcd2.setCursor(0,1);
  lcd2.print("Humidity: ");
  lcd2.print(HUMEDAD);
  lcd2.print(" %");
  while (check < 1)
  {
    TECLA1 = teclado.getKey();
    if (TECLA1)
    {
      check++;
    }
  }
  lcd2.clear();
}



void tictactoe()
{
  lcd2.clear();
  char typed[1];
  int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};
  lcd2.setCursor(0, 0);
  lcd2.print("Use the keypad to ");
  lcd2.setCursor(0, 1);
  lcd2.print("play a note");
  while (true)
  {
    char cuskey1 = teclado.getKey();
    if (cuskey1 == '*')
    {
      lcd2.clear();
      break;
    }
    if (cuskey1 != '#' and cuskey1)
    {
      typed[0] = cuskey1;
      long noet = atol(typed);
      tone(8, notes[noet-1], 3000);
    }
  }
}





bool selection(int x)
{
  if (options[x] == "Calculator")
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calculator");
    calculator();
    return true;
  }
  else if (options[x] == "Integer/Binary")
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Int/Bin");
    intbin();
    return true;
  }
  else if (options[x] == "Temperature")
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature");
    timetemp();
    return true;
  }
  else if (options[x] == "Music")
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Music");
    tictactoe();
    return true;
  }
}






void menu ()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Main Menu");
    if (TECLA and TECLA == '#')
    {
        if (selection(first) == true)
        {
        }
      }
  for (int i = 0; i < 4; i++)
  {
    lcd2.setCursor(0, i);
    if (i == 0)
    {
      lcd2.print(options[i]);
    }
    else
    {
      lcd2.print(options[i]);
    }
  }
  TECLA = teclado.getKey();
  
  if (x > 460 and x < 700)
  {
    if (y > -5 and y < 80)
    {
     // Serial.print("arriba");
     // Serial.print("\n");
      delay(500);
      if (first > 0)
      {
      first--;
      lcd2.clear();
      
      for (int i = 0; i < 4; i++)
      {
        lcd2.setCursor(0, i);
          if (first-i == 4 || i == first)
          {
            lcd2.print(options[first] + " <--");
          }
          else if (first >= 4)
            {
              lcd2.print(options[i+4]);
            }
            else
            {
              lcd2.print(options[i]);
            }
        }
      }
    }
    else if (y > 800 and y < 1100)
    {
    //  Serial.print("abajo");
     // Serial.print("\n");
      delay(500);
      if (first < 3 || next1 == true)
      {
        first++;
        lcd2.clear();
      
        for (int i = 0; i < 4; i++)
        {
          lcd2.setCursor(0, i);
            if (first-i == 4 || i == first)
            {
              lcd2.print(options[first] + " <--");
            }
            else if (first >= 4)
            {
              lcd2.print(options[i+4]);
            }
            else
            {
              lcd2.print(options[i]);
            }
        }
      }
   }
 }
}





void loop()
{
    x = analogRead(A0);
    y = analogRead(A1);
    menu();
}

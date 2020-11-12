#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9;
int numberOfHorizontalDisplays = 1; // количество матриц по-горизонтали
int numberOfVerticalDisplays = 1; // количество матриц по-вертикали

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

volatile int state = 0;
volatile int time = 300;
volatile bool skip = false;

const byte hearth[4][8] = {
    0b01101100,
    0b11111110,
    0b11111110,
    0b11111110,
    0b01111100,
    0b00111000,
    0b00010000,
    0b00000000,

    0b00110110,
    0b01111111,
    0b01111111,
    0b01111111,
    0b00111110,
    0b00011100,
    0b00001000,
    0b00000000,

    0b00000000,
    0b00110110,
    0b01111111,
    0b01111111,
    0b01111111,
    0b00111110,
    0b00011100,
    0b00001000,

    0b00000000,
    0b01101100,
    0b11111110,
    0b11111110,
    0b11111110,
    0b01111100,
    0b00111000,
    0b00010000
};

const byte box[4][8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,


    0b00000000,
    0b00000000,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00000000,
    0b00000000,

    0b00000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00000000,

    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111
};

const byte smile[8] = 
{
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};
String newYearGreetingMes = "HAPPY NEW YEAR 2021! ";
String loveMes = "I LOVE YOU ";
String toDasha = "From Andrew to Dasha) ";
int wait = 300;
int spacer = 1; // расстояние между буквами
int width = 5 + spacer; // размер шрифта
int it = 0;

bool printStr(String str, int delayTime)
{
      bool ret = true;
      for ( int i = 0 ; i < str.length(); i++ ) {
        if (!skip)
        {
        matrix.fillScreen(LOW);
        matrix.drawChar(0, 0, str[i], HIGH, LOW, 1);
        matrix.write();
        delay(delayTime);
        }
        else
        {
          skip = false;
          ret = false;
          break;
        }
    }
    return ret;
}

bool printMatrix(const byte data[8])
{
    bool ret = true;
    matrix.fillScreen(LOW); // очистка матрицы
    for ( int y = 0; y < 8; y++ ) 
    {
        for ( int x = 0; x < 8; x++ ) 
        {
            if (!skip)
            {
            matrix.drawPixel(x, y, data[y] & (1<<x));
            }
            else
            {
              skip = false;
              ret = false;
              break;
            }
        }
    }
    matrix.write();
    return ret;
}

bool heathAnimation(const byte hearthLocal[4][ 8], int delayTime)
{
    bool ret = true;
    for (int i = 0; i < 4; i++)
    {
        if (!skip)
        {
        printMatrix(hearthLocal[i]);
        delay(delayTime);
        }
        else
        {
          skip = false;
          ret = false;
          break;
        }
    }
    return ret;
}

int squareAnimation(const byte squareLocal[4][ 8], int delayTime)
{
      if (delayTime < 10)
      delayTime = 300;
      for (int i = 0; i < 8 * int(300/delayTime); i++)
      {
        if (!skip)
        {
        printMatrix(squareLocal[i%4]);
        delay(delayTime);
        }
        else
        {
          skip = false;
          delayTime = 24;
          break;
        }
      }
      delayTime = delayTime - 25;
      return delayTime;
}

bool printRunningStr(String str, int delayTime)
{
        bool ret = true;
        for ( int i = 0 ; i < width * str.length() + matrix.width() - 1 - spacer; i++ ) 
        {
          if(!skip)
          {
          matrix.fillScreen(LOW);
  
          int letter = i / width;
          int x = (matrix.width() - 1) - i % width;
          int y = (matrix.height() - 8) / 2; // center the text vertically
  
          while ( x + width - spacer >= 0 && letter >= 0 ) {
              if ( letter < str.length() ) {
                  matrix.drawChar(x, y, str[letter], HIGH, LOW, 1);
              }
              letter--;
              x -= width;
              
          }

          matrix.write();
          delay(delayTime);
        }
        else
        {
          skip = false;
          ret = false;
          break;
        }
    }
    return ret;
}

void setup() 
{
    matrix.setIntensity(10); // яркость от 0 до 15
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(0, isr, CHANGE);
}

volatile uint32_t debounce;

void isr()
{
    if (millis() - debounce >= 150 && digitalRead(2)) {
    debounce = millis();
      if (state < 7)
    state++;
    else
    state = 0;
  }
  skip = true;
}

void runQueue()
{
  switch(state){
    case 0:
      printRunningStr(newYearGreetingMes,50);
      delay(50);
      break;
    case 1:
      printMatrix(smile);
      delay(50);
      break;    
    case 2:
      printRunningStr("Merry Christamas!^^", 70);
      delay(50);
      break;      
    case 3:
      heathAnimation(hearth, 200);
      break; 
    case 4:
      printRunningStr(loveMes, 50);
      break;   
    case 5:
      time = squareAnimation(box, time);
      if (time == -1)
      time = 300;
      break;
    case 6:
      printStr(toDasha, 300);
      break;    
    case 7:
      if (!printRunningStr(newYearGreetingMes,50)) break;
      delay(wait);
      if (!printMatrix(smile)) break;
      delay(wait+500);
      if (!printRunningStr("Merry Christamas!^^", 70)) break;
      delay(wait);
      if (squareAnimation(box, 200) == -1) break;
      delay(wait);
      if (!printRunningStr(loveMes, 50)) break;
      if (!heathAnimation(hearth, 200)) break;
      if (!printStr(toDasha, 300)) break;
      break;
  }   
  
}
void loop() 
{

  runQueue();
  /*it++;*/
}

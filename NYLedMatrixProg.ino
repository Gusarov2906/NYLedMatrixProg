#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9;
int numberOfHorizontalDisplays = 1; // количество матриц по-горизонтали
int numberOfVerticalDisplays = 1; // количество матриц по-вертикали

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

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
String tape = "HELLO WORLD!";
int wait = 100;
int spacer = 1; // расстояние между буквами
int width = 5 + spacer; // размер шрифта
int it = 0;

void printStr(String str, int delayTime)
{
      for ( int i = 0 ; i < str.length(); i++ ) {
        matrix.fillScreen(LOW);
        matrix.drawChar(0, 0, str[i], HIGH, LOW, 1);
        matrix.write();
        delay(delayTime);
    }
}

void printMatrix(const byte data[8])
{
    matrix.fillScreen(LOW); // очистка матрицы
    for ( int y = 0; y < 8; y++ ) 
    {
        for ( int x = 0; x < 8; x++ ) 
        {
            matrix.drawPixel(x, y, data[y] & (1<<x));
        }
    }
    matrix.write();
}

void heathAnimation1(const byte hearthLocal[4][8], int delayTime)
{
      for (int i = 0; i < 4; i++)
    {
        printMatrix(hearthLocal[i]);
        delay(delayTime);
    }
}

void heathAnimation2(const byte data[8], int delayTime)
{
    matrix.fillScreen(LOW); // очистка матрицы
    for ( int y = 0; y < 8; y++ ) 
    {
        for ( int x = 0; x < 8; x++ ) 
        {
            matrix.drawPixel(x, y, data[y] & (1<<x));
        }
    }
    matrix.write();
}

void printRunningStr(String str, int delayTime)
{
        for ( int i = 0 ; i < width * str.length() + matrix.width() - 1 - spacer; i++ ) 
        {
          matrix.fillScreen(LOW);
  
          int letter = i / width;
          int x = (matrix.width() - 1) - i % width;
          int y = (matrix.height() - 8) / 2; // center the text vertically
  
          while ( x + width - spacer >= 0 && letter >= 0 ) {
              if ( letter < tape.length() ) {
                  matrix.drawChar(x, y, str[letter], HIGH, LOW, 1);
              }
              letter--;
              x -= width;
        }

        matrix.write();
        delay(delayTime);
    }
}

void setup() 
{
    matrix.setIntensity(10); // яркость от 0 до 15
}

void runQueue(int state)
{
  switch(state){
    case 0:
      printStr(tape,200);
      delay(wait);
      break;
    case 1:
      printRunningStr(tape,50);
      delay(wait);
      break;    
    case 2:
      heathAnimation1(hearth, 200);
      delay(wait);
      break;      
  }
}
void loop() 
{
  runQueue(it);
  it++;
}

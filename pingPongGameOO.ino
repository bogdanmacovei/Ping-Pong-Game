#include "LedControl.h"
#include <LiquidCrystal.h>

const int rs = 13, en = 9, d4 = 5, d5 = 3, d6 = 1, d7 = 0;
const int joyStick1X = A4, joyStick1Y = A5, joyStick1Button = 7;
const int joyStick2X = A0, joyStick2Y = A1, joyStick2Button = 4;

const int buttonControl = 2;
bool playGame = false;

LiquidCrystal lcd (rs, en, d4, d5, d6, d7);
LedControl lc = LedControl(12, 11, 10, 1);

class pingPongGame {
    private:
      int paddle1A, paddle1B, paddle1C; /// prima paleta
      int paddle2A, paddle2B, paddle2C; /// a doua paleta
      int xBall, yBall;                 /// mingea
      int unitX, unitY;                 /// versorii deplasarii pe x si y
      int player1, player2;             /// scorul
      int delayTime;                    /// delay-ul pentru cresterea vitezei
  
    public:
      /// contructor plictisitor care sa admita valori implicite
      pingPongGame (int _paddle1A = 3, int _paddle1B = 4, int _paddle1C = 5, int _paddle2A = 3, int _paddle2B = 4, int _paddle2C = 5,
                    int _xBall = 4, int _yBall = 0, int _unitX = 1, int _unitY = 1, int _player1 = 0, int _player2 = 0, int _delayTime = 100) {
  
                      paddle1A = _paddle1A, paddle1B = _paddle1B, paddle1C = _paddle1C, paddle2A = _paddle2A, paddle2B = _paddle2B, paddle2C = _paddle2C;
                      xBall = _xBall, yBall = _yBall, unitX = _unitX, unitY = _unitY, player1 = _player1, player2 = _player2, delayTime = _delayTime;
                    }
  
      int getPaddle1A () const {
        return paddle1A;
      }
      
      int getPaddle2A () const {
        return paddle2A;
      }

      /// aprinde/stinge (in functie de mode) toate LED-urile corespunzatoare celor doua palete
      void setPaddle (int, int, int, int, int, int, bool);
  
      /// actualizeaza noile pozitii
      void modifyPaddles (int, int, int, int, int, int);
  
      void writeScore (int, int);

      /// schimbarea vizuala
      void currentAction ();

      /// schimbarea in cod
      void currentLogic ();
  
      void reset ();
      void generalReset ();
  
      /// interpretez valorile unui joyStick
      int xyDirection (int);
      
      /// iau decizia de deplasare pe matrice
      int xyTransform (int);
};

void pingPongGame :: setPaddle (int _paddle1A, int _paddle1B, int _paddle1C, int _paddle2A, int _paddle2B, int _paddle2C, bool mode) {
    lc.setLed (0, 0, _paddle1A, mode);
    lc.setLed (0, 0, _paddle1B, mode);
    lc.setLed (0, 0, _paddle1C, mode);
    lc.setLed (0, 7, _paddle2A, mode);
    lc.setLed (0, 7, _paddle2B, mode);
    lc.setLed (0, 7, _paddle2C, mode);
}

void pingPongGame :: modifyPaddles (int newPaddle1A, int newPaddle1B, int newPaddle1C, int newPaddle2A, int newPaddle2B, int newPaddle2C) {
    paddle1A = newPaddle1A, paddle1B = newPaddle1B, paddle1C = newPaddle1C;
    paddle2A = newPaddle2A, paddle2B = newPaddle2B, paddle2C = newPaddle2C;
}

void pingPongGame :: writeScore (int _player1, int _player2) {
    lcd.clear ();
    
    lcd.print("Player 1: ");
    lcd.print (_player1);
    
    lcd.setCursor(0, 1);
    
    lcd.print ("Player 2: ");
    lcd.print (_player2);
}

void pingPongGame :: reset () {
    if (player1 == 10 || player2 == 10) {
      /// cel putin un jucator are 10p, deci a castigat
      generalReset ();
    } else {
      paddle1A = paddle2A = 3, paddle1B = paddle2B = 4, paddle1C = paddle2C = 5;
      xBall = random (2, 5), yBall = random (0, 2);
      unitX = unitY = 1;
      if (delayTime >= 50) {
        delayTime -= 5;
      }
      writeScore (player1, player2);
    }
}

void pingPongGame :: generalReset () {
    playGame = false;
    paddle1A = paddle2A = 3, paddle1B = paddle2B = 4, paddle1C = paddle2C = 5;
    xBall = random (2, 5), yBall = random (0, 2);
    unitX = unitY = 1;
    delayTime = 100;
    lcd.clear ();
    
    if (player1 == 10) {
      lcd.print ("Player1 este");
      lcd.setCursor (0, 1);
      lcd.print ("castigator");
    } else {
      lcd.print ("Player2 este");
      lcd.setCursor (0, 1);
      lcd.print ("castigator");
    }
    
    player1 = 0;
    player2 = 0;
}

int pingPongGame :: xyDirection (int arg) {
    if (arg > 700) {
      return 1;
    }
    
    if (arg < 400) {
      return -1;
    }
    
    return 0;
}

int pingPongGame :: xyTransform (int arg) {
    if (arg >= 6) {
      return 5;
    }
    
    if (arg <= 0) {
      return 0;
    }
    
    return arg;
}

void pingPongGame :: currentAction () {
    lc.setLed (0, xBall, yBall, true);
    setPaddle (paddle1A, paddle1B, paddle1C, paddle2A, paddle2B, paddle2C, true);
  
    delay (delayTime);
  
    lc.setLed (0, xBall, yBall, false);
    setPaddle (paddle1A, paddle1B, paddle1C, paddle2A, paddle2B, paddle2C, false);

    xBall = xBall + unitX;
    yBall = yBall + unitY;
}

void pingPongGame :: currentLogic () {
    if (xBall == 7) { 
      // caz in care as putea fi pe o paleta
      if (yBall == paddle2C) {
        unitX = -unitX;
        unitY = 1;
      } else if (yBall == paddle2A) {
        unitX = -unitX;
        unitY = -1;
      } else if (yBall == paddle2B) {
        unitY = 0;
        unitX = -1;
      } else {
        /// a pierdut player2
        player1++;
        reset ();
      }
    }
  
    else if (xBall == 0) {
      // caz in care as putea fi pe cealalta paleta
      if (yBall == paddle1A) {
        unitX = -unitX;
        unitY = -1;
      } else if (yBall == paddle1C) {
        unitX = -unitX;
        unitY = 1;
      } else if (yBall == paddle1B) {
        unitY = 0;
        unitX = 1;
      } else {
        // a pierdut player1
        player2++;
        reset ();
      }
    }
  
    else if (xBall >= 1 && xBall <= 6) {
      if (yBall == 0 || yBall == 7) {
        unitY = -unitY;
      }
    }
}

pingPongGame pingPongObj;
  
void setup() {
    lc.shutdown (0, false);
    lc.setIntensity (0, 8);
    lc.clearDisplay (0);
    
    lcd.begin(16, 2);
    pingPongObj.writeScore (0, 0);
    analogWrite (8, 30);
}

void loop() {
    if (digitalRead (buttonControl) == LOW) {
      playGame = true;
    }
  
    if (playGame) {
      pingPongObj.currentAction ();
      pingPongObj.currentLogic ();
    
      int valX1 = analogRead (joyStick1X);
      int valX2 = analogRead (joyStick2X);
    
      int localPaddle1A = pingPongObj.getPaddle1A ();
      int localPaddle2A = pingPongObj.getPaddle2A ();
      
      localPaddle1A = pingPongObj.xyTransform ((localPaddle1A + pingPongObj.xyDirection (valX1)));
      localPaddle2A = pingPongObj.xyTransform ((localPaddle2A + pingPongObj.xyDirection (valX2)));
    
      pingPongObj.modifyPaddles (localPaddle1A, localPaddle1A + 1, localPaddle1A + 2, 
                                 localPaddle2A, localPaddle2A + 1, localPaddle2A + 2);
    }
}


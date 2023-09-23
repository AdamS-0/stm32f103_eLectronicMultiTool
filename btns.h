// +-----------------+
// | Buttons section |
// +-----------------+
#define TIME_BTN_PRESS_DELAY 200UL // [ms]

class Button {
  private:
    unsigned long ulLastTime;
    bool bPressed = false;
    bool defState = LOW;
  public:
    int pin;
    
    Button( int _pin, bool _defState = HIGH ) {
      this->pin = _pin;
      this->defState = _defState;
      pinMode( this->pin, this->defState ? INPUT_PULLUP : INPUT_PULLDOWN );
    }

    void interrupt() {
      if( millis() - this->ulLastTime < TIME_BTN_PRESS_DELAY ) return;
      this->ulLastTime = millis();
      
      this->bPressed = true;
    }

    // clears the state of the button
    bool wasPressed() {
      bool oldState = this->bPressed;
      this->bPressed = false;
      return oldState;
    }

    bool isPressed() {
      return digitalRead( this->pin ) != this->defState;
    }
};

Button btnA(PIN_BTN_A), btnB(PIN_BTN_B), btnEnc(PIN_ENC_BTN);

void INT_PIN_BTN_A() { btnA.interrupt(); exitCode = true; }
void INT_PIN_BTN_B() { btnB.interrupt(); exitCodeSub = true; }
void INT_PIN_BTN_ENC() { btnEnc.interrupt(); }

void btns_bootup() {
  
  attachInterrupt( digitalPinToInterrupt( btnEnc.pin ), INT_PIN_BTN_ENC, FALLING );
  attachInterrupt( digitalPinToInterrupt( btnA.pin ), INT_PIN_BTN_A, FALLING );
  attachInterrupt( digitalPinToInterrupt( btnB.pin ), INT_PIN_BTN_B, FALLING );
}

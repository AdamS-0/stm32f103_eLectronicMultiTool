// +-----------------+
// | Buttons section |
// +-----------------+
#define TIME_BTN_PRESS_DELAY 200UL // [ms]
void int_btn_A(), int_btn_B(), int_btn_C();
unsigned long ul_last_btn_A, ul_last_btn_B, ul_last_btn_C, ul_lastEncBtn;
bool b_btn_A = false, b_btn_B = false, b_btn_C = false, b_encBtn = false;


void int_btn_A() {
  if( millis() - ul_last_btn_A < TIME_BTN_PRESS_DELAY ) return;
  ul_last_btn_A = millis();
  
  b_btn_A = true;
  exitCode = true;
}

void int_btn_B() {
  if( millis() - ul_last_btn_B < TIME_BTN_PRESS_DELAY ) return;
  ul_last_btn_B = millis();
  
  b_btn_B = true;
  exitCodeSub = true;
}

void int_btn_C() {
  if( millis() - ul_last_btn_C < TIME_BTN_PRESS_DELAY ) return;
  ul_last_btn_C = millis();
  
  b_btn_C = true;
  modifyCode = true;
}


void int_encBtn() {
  if( millis() - ul_lastEncBtn < TIME_BTN_PRESS_DELAY ) return;
  ul_lastEncBtn = millis();
  
  b_encBtn = true;
}

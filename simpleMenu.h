// file: simpleMenu.h

#define DSP_setCursor   display.setCursor
#define DSP_setTextSize display.setTextSize
#define DSP_print       display.print

#define HIDE_SELECTOR         1
#define SHOW_BIG_SELECT       2
#define SHOW_NUM_LINE         4
#define SHOW_NUM_LINE_SELECT  8

class Menu {
  private:
    int len = 0;
    int8_t textSize = 1;
    int8_t fontHeight = 8;
    String *names;
   
   public:
    int id = 0;

    Menu(String *_names, int _length) {
      this->names = _names;
      this->len = _length;
    }
    
    void update( int8_t dV = 0 ) {
      int8_t minId = 0, maxId = this->len;
    
      if( dV == 0 ) return;
    
      int8_t newId = this->id + dV;
      if( newId < 0 ) newId = 0;
      else if( newId >= this->len ) newId = this->len - 1;
    
      this->id = newId;
    }

    void draw( char _selector = '>', int _x = 0, int _y = 0, int _dY = 0,
      int8_t elemens2Show = 5, uint8_t state = SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT ) {
      
      if( elemens2Show <= 0 ) return;
      int minId = 0, maxId = this->len;
      minId = max( this->id - elemens2Show/2, 0 );
      maxId = min( minId + elemens2Show, this->len );
      minId = min( minId, maxId - elemens2Show );
      minId = minId < 0 ? 0 : minId;
    
      if( !( state & SHOW_BIG_SELECT ) ) DSP_setTextSize( this->textSize );
      bool isCurId = 0;
    
      for(uint8_t i = minId; i < maxId; i++) {
        isCurId = i == this->id;
        DSP_setCursor( _x, _y );
        if( state & SHOW_BIG_SELECT )                     DSP_setTextSize( this->textSize + isCurId );
        if( _selector > 0 && !(state & HIDE_SELECTOR) )   DSP_print( ( isCurId ? (String)_selector : " " ) );
        if( state & SHOW_NUM_LINE  ) {
          if( !isCurId || (state & SHOW_NUM_LINE_SELECT) ) DSP_print( String(i + 1) + " " );
        }
        
        DSP_print( this->names[i] );
        _y += _dY +
          ( this->textSize + ( isCurId && (state & SHOW_BIG_SELECT) ) ) * this->fontHeight;
      }
    }
    
};

// dV - e.g. input from encoder
//void updateMenu( Menu &m, int8_t dV = 0 ) {
//  int8_t minId = 0, maxId = m.Len;
//
//  if( dV == 0 ) return;
//
//  int8_t newId = m.Id + dV;
//  if( newId < 0 ) newId = 0;
//  else if( newId >= m.Len ) newId = m.Len - 1;
//
//  m.Id = newId;
//}

//drawMenu( Menu, _selector('>'), _x(0), _y(0), _dY(0), elemens2Show(5),
//          state(SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT)[
//            HIDE_SELECTOR, SHOW_BIG_SELECT, SHOW_NUM_LINE, SHOW_NUM_LINE_SELECT
//          ] )
//void drawMenu( Menu &m, char _selector = '>', int _x = 0, int _y = 0, int _dY = 0,
//  int8_t elemens2Show = 5, uint8_t state = SHOW_NUM_LINE | SHOW_NUM_LINE_SELECT ) {
//  
//  if( elemens2Show <= 0 ) return;
//  int minId = 0, maxId = m.Len;
//  minId = max( m.Id - elemens2Show/2, 0 );
//  maxId = min( minId + elemens2Show, m.Len );
//  minId = min( minId, maxId - elemens2Show );
//  minId = minId < 0 ? 0 : minId;
//
//  if( !( state & SHOW_BIG_SELECT ) ) DSP_setTextSize( m.TextSize );
//  bool isCurId = 0;
//
//  for(uint8_t i = minId; i < maxId; i++) {
//    isCurId = i == m.Id;
//    DSP_setCursor( _x, _y );
//    if( state & SHOW_BIG_SELECT )                     DSP_setTextSize( m.TextSize + isCurId );
//    if( _selector > 0 && !(state & HIDE_SELECTOR) )   DSP_print( ( isCurId ? (String)_selector : " " ) );
//    if( state & SHOW_NUM_LINE  ) {
//      if( !isCurId || (state & SHOW_NUM_LINE_SELECT) ) DSP_print( String(i + 1) + " " );
//    }
//    
//    DSP_print( m.names[i] );
//    _y += _dY +
//      ( m.TextSize + ( isCurId && (state & SHOW_BIG_SELECT) ) ) * m.FontHeight;
//  }
//}

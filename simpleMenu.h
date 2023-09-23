// file: simpleMenu.h

#define DSP_setCursor   display.setCursor
#define DSP_setTextSize display.setTextSize
#define DSP_print       display.print

template< typename T, size_t N > size_t ArraySize (T (&) [N]){ return N; }

struct Menu {
  int Len = 0;
  int8_t TextSize = 1;
  int8_t FontHeight = 8;
  int Id = 0;
  String *names;
};

// dV - e.g. input from encoder
void updateMenu( Menu &m, int8_t dV = 0 ) {
  int8_t minId = 0, maxId = m.Len;

  if( dV == 0 ) return;

  int8_t newId = m.Id + dV;
  if( newId < 0 ) newId = 0;
  else if( newId >= m.Len ) newId = m.Len - 1;

  m.Id = newId;
}

void drawMenu( Menu &m, char _selector = '>', int _x = 0, int _y = 0, int _dY = 0,
  int8_t elemens2Show = 5, bool showBigSelected = false ) {
  
  if( elemens2Show <= 0 ) return;
  int minId = 0, maxId = m.Len;
  minId = max( m.Id - elemens2Show/2, 0 );
  maxId = min( minId + elemens2Show, m.Len );
  minId = min( minId, maxId - elemens2Show );
  minId = minId < 0 ? 0 : minId;

  if( !showBigSelected ) DSP_setTextSize( m.TextSize );
  bool isCurId = 0;

  for(uint8_t i = minId; i < maxId; i++) {
    isCurId = i == m.Id;
    DSP_setCursor( _x, _y );
    if( showBigSelected ) DSP_setTextSize( m.TextSize + isCurId );
    if( _selector > 0 ) DSP_print( ( isCurId ? (String)_selector : " " ) );
    if( !isCurId )  DSP_print( String(i + 1) + " " );
    DSP_print( m.names[i] );
    _y += ( isCurId == 0 )*_dY + ( m.TextSize + isCurId * showBigSelected ) * m.FontHeight;
  }
}

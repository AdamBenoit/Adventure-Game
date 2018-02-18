#include <LiquidCrystal.h>
#include <LCDMenuLib2.h>

	#define _LCDML_DISP_cols					20
	#define _LCDML_DISP_rows					4 
	#define _LCDML_DISP_cfg_cursor				0x7E   // cursor Symbol
	#define _LCDML_DISP_cfg_scrollbar			1 // enable a scrollbar
	// liquid crystal needs (rs, e, dat4, dat5, dat6, dat7)
	LiquidCrystal lcd(32, 33, 37, 36, 35, 34);

	const uint8_t scroll_bar[5][8] = {
		{ B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001 }, // scrollbar top
		{ B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001 }, // scroll state 1 
		{ B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001 }, // scroll state 2
		{ B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001 }, // scroll state 3
		{ B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111 }  // scrollbar bottom
	};



	void lcdml_menu_display();
	void lcdml_menu_clear();
	void lcdml_menu_control();

	LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL); // root menu element (do not change)
	LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);



	LCDML_add(0, LCDML_0, 1, "Information", mFunc_information);       // this menu function can be found on "LCDML_display_menuFunction" tab
	LCDML_add(1, LCDML_0, 2, "Time info", mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
	LCDML_add(2, LCDML_0, 3, "Programme", NULL);                    // NULL = no menu function    
	LCDML_add(3, LCDML_0_3, 1, "Program 1", NULL);                    // NULL = no menu function  
	LCDML_add(4, LCDML_0_3_1, 1, "P1 dummy", NULL);                    // NULL = no menu function  
	LCDML_add(5, LCDML_0_3_1, 2, "P1 Settings", NULL);                    // NULL = no menu function  
	LCDML_add(6, LCDML_0_3_1_2, 1, "Warm", NULL);                    // NULL = no menu function  
	LCDML_add(7, LCDML_0_3_1_2, 2, "Cold", NULL);                    // NULL = no menu function  
	LCDML_add(8, LCDML_0_3_1_2, 3, "Back", mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab 
	LCDML_add(9, LCDML_0_3_1, 3, "Back", mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
	LCDML_add(10, LCDML_0_3, 2, "Program 2", mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
	LCDML_add(11, LCDML_0_3, 3, "Back", mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
	LCDML_add(12, LCDML_0, 4, "special", NULL);                    // NULL = no menu function 
	LCDML_add(13, LCDML_0_4, 1, "go to Root", mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab 
	LCDML_add(14, LCDML_0_4, 2, "jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab 
	LCDML_add(15, LCDML_0_4, 3, "Back", mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab   
	LCDML_addAdvanced(16, LCDML_0, 5, NULL, "Parameter", NULL, 0, _LCDML_TYPE_default);                    // NULL = no menu function
	LCDML_addAdvanced(17, LCDML_0_5, 1, NULL, "Parameter 1", mFunc_para, 10, _LCDML_TYPE_default);                    // NULL = no menu function
	LCDML_addAdvanced(18, LCDML_0_5, 2, NULL, "Parameter 2", mFunc_para, 20, _LCDML_TYPE_default);                    // NULL = no menu function
	LCDML_addAdvanced(19, LCDML_0_5, 3, NULL, "Parameter 3", mFunc_para, 30, _LCDML_TYPE_default);                    // NULL = no menu function
	LCDML_addAdvanced(20, LCDML_0, 6, NULL, "", mDyn_para, 0, _LCDML_TYPE_dynParam); // NULL = no menu function
	LCDML_addAdvanced(21, LCDML_0, 7, COND_hide, "screensaver", mFunc_screensaver, 0, _LCDML_TYPE_default); // this menu function can be found on "LCDML_display_menuFunction" tab 


#define _LCDML_DISP_cnt    21    

																											// create menu
	LCDML_createMenu(_LCDML_DISP_cnt);

	void setup()
	{
		// serial init; only be needed if serial control is used     
		Serial.begin(9600);                // start serial    
		Serial.println(F(_LCDML_VERSION)); // only for examples

										   // LCD Begin
		lcd.begin(_LCDML_DISP_cols, _LCDML_DISP_rows);
		// set special chars for scrollbar
		lcd.createChar(0, (uint8_t*)scroll_bar[0]);
		lcd.createChar(1, (uint8_t*)scroll_bar[1]);
		lcd.createChar(2, (uint8_t*)scroll_bar[2]);
		lcd.createChar(3, (uint8_t*)scroll_bar[3]);
		lcd.createChar(4, (uint8_t*)scroll_bar[4]);

		// LCDMenuLib Setup
		LCDML_setup(_LCDML_DISP_cnt);

		// Some settings which can be used 

		// Enable Menü Rollover 
		LCDML.MENU_enRollover();

		// Enable Screensaver (screensaver menu function, time to activate in ms)
		LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 secounds 
													   //LCDML.SCREEN_disable();

													   // Some needfull methods 

													   // You can jump to a menu function from anywhere with 
													   //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name          
}

void loop()
{

	LCDML.loop();
}
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib2.h
# endif

boolean COND_hide()  // hide a menu element
					 // ********************************************************************* 
{
	return false;  // hidden
}

#define _LCDML_CONTROL_cfg      0

// therory:
// "#if" is a preprocessor directive and no error, look here:
// (english) https://en.wikipedia.org/wiki/C_preprocessor
// (german)  https://de.wikipedia.org/wiki/C-Pr%C3%A4prozessor


// *********************************************************************
// *************** (0) CONTROL OVER SERIAL INTERFACE *******************
// *********************************************************************
#if(_LCDML_CONTROL_cfg == 0)
// settings
# define _LCDML_CONTROL_serial_enter           'e'
# define _LCDML_CONTROL_serial_up              'w'
# define _LCDML_CONTROL_serial_down            's'
# define _LCDML_CONTROL_serial_left            'a'
# define _LCDML_CONTROL_serial_right           'd'
# define _LCDML_CONTROL_serial_quit            'q'
// *********************************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
	}

	// check if new serial input is available  
	if (Serial.available()) {
		// read one char from input buffer   
		switch (Serial.read())
		{
		case _LCDML_CONTROL_serial_enter:  LCDML.BT_enter(); break;
		case _LCDML_CONTROL_serial_up:     LCDML.BT_up();    break;
		case _LCDML_CONTROL_serial_down:   LCDML.BT_down();  break;
		case _LCDML_CONTROL_serial_left:   LCDML.BT_left();  break;
		case _LCDML_CONTROL_serial_right:  LCDML.BT_right(); break;
		case _LCDML_CONTROL_serial_quit:   LCDML.BT_quit();  break;
		default: break;
		}
	}
}

// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (1) CONTROL OVER ONE ANALOG PIN *********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 1)

unsigned long g_LCDML_DISP_press_time = 0;
// settings
#define _LCDML_CONTROL_analog_pin              0
// when you did not use a button set the value to zero
#define _LCDML_CONTROL_analog_enter_min        850     // Button Enter
#define _LCDML_CONTROL_analog_enter_max        920  
#define _LCDML_CONTROL_analog_up_min           520     // Button Up
#define _LCDML_CONTROL_analog_up_max           590   
#define _LCDML_CONTROL_analog_down_min         700     // Button Down
#define _LCDML_CONTROL_analog_down_max         770   
#define _LCDML_CONTROL_analog_back_min         950     // Button Back
#define _LCDML_CONTROL_analog_back_max         1020   
#define _LCDML_CONTROL_analog_left_min         430     // Button Left
#define _LCDML_CONTROL_analog_left_max         500   
#define _LCDML_CONTROL_analog_right_min        610     // Button Right
#define _LCDML_CONTROL_analog_right_max        680
// *********************************************************************

void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
	}
	// check debounce timer  
	if ((millis() - g_LCDML_DISP_press_time) >= 200) {
		g_LCDML_DISP_press_time = millis(); // reset debounce timer

		uint16_t value = analogRead(_LCDML_CONTROL_analog_pin);  // analogpin for keypad

		if (value >= _LCDML_CONTROL_analog_enter_min && value <= _LCDML_CONTROL_analog_enter_max) { LCDML.BT_enter(); }
		if (value >= _LCDML_CONTROL_analog_up_min    && value <= _LCDML_CONTROL_analog_up_max) { LCDML.BT_up(); }
		if (value >= _LCDML_CONTROL_analog_down_min  && value <= _LCDML_CONTROL_analog_down_max) { LCDML.BT_down(); }
		if (value >= _LCDML_CONTROL_analog_left_min  && value <= _LCDML_CONTROL_analog_left_max) { LCDML.BT_left(); }
		if (value >= _LCDML_CONTROL_analog_right_min && value <= _LCDML_CONTROL_analog_right_max) { LCDML.BT_right(); }
		if (value >= _LCDML_CONTROL_analog_back_min  && value <= _LCDML_CONTROL_analog_back_max) { LCDML.BT_quit(); }
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (2) CONTROL OVER DIGITAL PINS ***********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 2)
// settings
unsigned long g_LCDML_DISP_press_time = 0;

#define _LCDML_CONTROL_digital_low_active      0    // 0 = high active (pulldown) button, 1 = low active (pullup)
// http://playground.arduino.cc/CommonTopics/PullUpDownResistor
#define _LCDML_CONTROL_digital_enable_quit     1
#define _LCDML_CONTROL_digital_enable_lr       1
#define _LCDML_CONTROL_digital_enter           8    
#define _LCDML_CONTROL_digital_up              9
#define _LCDML_CONTROL_digital_down            10
#define _LCDML_CONTROL_digital_quit            11
#define _LCDML_CONTROL_digital_left            12
#define _LCDML_CONTROL_digital_right           13
// *********************************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
		// init buttons
		pinMode(_LCDML_CONTROL_digital_enter, INPUT_PULLUP);
		pinMode(_LCDML_CONTROL_digital_up, INPUT_PULLUP);
		pinMode(_LCDML_CONTROL_digital_down, INPUT_PULLUP);
# if(_LCDML_CONTROL_digital_enable_quit == 1)
		pinMode(_LCDML_CONTROL_digital_quit, INPUT_PULLUP);
# endif
# if(_LCDML_CONTROL_digital_enable_lr == 1)
		pinMode(_LCDML_CONTROL_digital_left, INPUT_PULLUP);
		pinMode(_LCDML_CONTROL_digital_right, INPUT_PULLUP);
# endif
	}

#if(_LCDML_CONTROL_digital_low_active == 1)
#  define _LCDML_CONTROL_digital_a !
#else
#  define _LCDML_CONTROL_digital_a
#endif

	uint8_t but_stat = 0x00;

	bitWrite(but_stat, 0, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_enter)));
	bitWrite(but_stat, 1, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_up)));
	bitWrite(but_stat, 2, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_down)));
#if(_LCDML_CONTROL_digital_enable_quit == 1)
	bitWrite(but_stat, 3, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_quit)));
#endif
#if(_LCDML_CONTROL_digital_enable_lr == 1)
	bitWrite(but_stat, 4, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_left)));
	bitWrite(but_stat, 5, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_right)));
#endif  

	if (but_stat > 0) {
		if ((millis() - g_LCDML_DISP_press_time) >= 200) {
			g_LCDML_DISP_press_time = millis(); // reset press time

			if (bitRead(but_stat, 0)) { LCDML.BT_enter(); }
			if (bitRead(but_stat, 1)) { LCDML.BT_up(); }
			if (bitRead(but_stat, 2)) { LCDML.BT_down(); }
			if (bitRead(but_stat, 3)) { LCDML.BT_quit(); }
			if (bitRead(but_stat, 4)) { LCDML.BT_left(); }
			if (bitRead(but_stat, 5)) { LCDML.BT_right(); }
		}
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (3) CONTROL WITH ENCODER ****************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 3)
// settings
#define _LCDML_CONTROL_encoder_pin_a           10 // pin encoder b
#define _LCDML_CONTROL_encoder_pin_b           11 // pin encoder a
#define _LCDML_CONTROL_encoder_pin_button      12 // pin taster
#define _LCDML_CONTROL_encoder_high_active     0  // (0 = low active (pullup), 1 = high active (pulldown)) button
// // http://playground.arduino.cc/CommonTopics/PullUpDownResistor
// global defines
unsigned long g_LCDML_DISP_press_time = 0;
uint8_t  g_LCDML_CONTROL_encoder_t_prev = 0;
uint8_t  g_LCDML_CONTROL_encoder_a_prev = 0;
// *********************************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
		// set encoder update intervall time 
		//LCDML_BACK_dynamic_setLoopTime(LCDML_BACKEND_control, 1000UL);  // 1000us 

		// init pins  
		pinMode(_LCDML_CONTROL_encoder_pin_a, INPUT_PULLUP);
		pinMode(_LCDML_CONTROL_encoder_pin_b, INPUT_PULLUP);
		pinMode(_LCDML_CONTROL_encoder_pin_button, INPUT_PULLUP);
	}

	// read encoder status
	unsigned char a = digitalRead(_LCDML_CONTROL_encoder_pin_a);
	unsigned char b = digitalRead(_LCDML_CONTROL_encoder_pin_b);
	unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);

	// change button status if high and low active are switched
	if (_LCDML_CONTROL_encoder_high_active == 1) {
		t != t;
	}

	// check encoder status and set control menu
	if (!a && g_LCDML_CONTROL_encoder_a_prev) {
		g_LCDML_CONTROL_encoder_t_prev = 1;

		if (!b) { LCDML.BT_up(); }
		else { LCDML.BT_down(); }
	}
	else {
		// check button press time for enter
		if ((millis() - g_LCDML_DISP_press_time) >= 200) {
			g_LCDML_DISP_press_time = millis(); // reset button press time

												// press button once
			if (!t && g_LCDML_CONTROL_encoder_t_prev == 0) {
				LCDML.BT_enter();
			}
			else {
				g_LCDML_CONTROL_encoder_t_prev = 0;
			}
		}
	}
	g_LCDML_CONTROL_encoder_a_prev = a;  // set new encoder status 

}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (4) CONTROL WITH A KEYPAD ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 4)
// include
// more information under http://playground.arduino.cc/Main/KeypadTutorial
#include <Keypad.h>
// settings
#define _LCDML_CONTROL_keypad_rows 4 // Four rows
#define _LCDML_CONTROL_keypad_cols 3 // Three columns
// global vars
char keys[_LCDML_CONTROL_keypad_rows][_LCDML_CONTROL_keypad_cols] = {
	{ '1','2','3' },
	{ '4','5','6' },
	{ '7','8','9' },
	{ '#','0','*' }
};
byte rowPins[_LCDML_CONTROL_keypad_rows] = { 9, 8, 7, 6 };  // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[_LCDML_CONTROL_keypad_cols] = { 12, 11, 10 };  // Create the Keypad
															// objects
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, _LCDML_CONTROL_keypad_rows, _LCDML_CONTROL_keypad_cols);
// *********************************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
	}
	char key = kpd.getKey();
	if (key)  // Check for a valid key.
	{
		switch (key)
		{
		case '#': LCDML.BT_enter(); break;
		case '2': LCDML.BT_up();    break;
		case '8': LCDML.BT_down();  break;
		case '4': LCDML.BT_left();  break;
		case '6': LCDML.BT_right(); break;
		case '*': LCDML.BT_quit();  break;
		default: break;
		}
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************


// *********************************************************************
// *************** (5) CONTROL WITH IR REMOTE ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 5)
// ir include (this lib have to be installed) 
#include <IRremote.h>
// ir global vars
int RECV_PIN = 11;
// ir objects
IRrecv irrecv(RECV_PIN);
decode_results results;

// *********************************************************************
// change in this function the ir values to your values
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
		irrecv.enableIRIn(); // Start the receiver
	}

	if (irrecv.decode(&results))
	{
		// comment this line out, to check the correct code 
		//Serial.println(results.value, HEX);

		// in this switch case you have to change the value 0x...1 to the correct ir code
		switch (results.value)
		{
		case 0x00000001: LCDML.BT_enter(); break;
		case 0x00000002: LCDML.BT_up();    break;
		case 0x00000003: LCDML.BT_down();  break;
		case 0x00000004: LCDML.BT_left();  break;
		case 0x00000005: LCDML.BT_right(); break;
		case 0x00000006: LCDML.BT_quit();  break;
		default: break;
		}
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************



// *********************************************************************
// *************** (6) CONTROL OVER JOYSTICK ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 6)
unsigned long g_LCDML_DISP_press_time = 0;
// settings
#define _LCDML_CONTROL_analog_pinx A0
#define _LCDML_CONTROL_analog_piny A1
#define _LCDML_CONTROL_digitalread 33 //don't work with u8glib

// when you did not use a button set the value to zero
#define _LCDML_CONTROL_analog_up_min 612 // Button Up
#define _LCDML_CONTROL_analog_up_max 1023
#define _LCDML_CONTROL_analog_down_min 0 // Button Down
#define _LCDML_CONTROL_analog_down_max 412
#define _LCDML_CONTROL_analog_left_min 612 // Button Left
#define _LCDML_CONTROL_analog_left_max 1023
#define _LCDML_CONTROL_analog_right_min 0 // Button Right
#define _LCDML_CONTROL_analog_right_max 412
// *********************************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
		pinMode(_LCDML_CONTROL_digitalread, INPUT);
	}
	// check debounce timer
	if ((millis() - g_LCDML_DISP_press_time) >= 200) {
		g_LCDML_DISP_press_time = millis(); // reset debounce timer

		uint16_t valuex = analogRead(_LCDML_CONTROL_analog_pinx);  // analogpinx
		uint16_t valuey = analogRead(_LCDML_CONTROL_analog_piny);  // analogpinx
		uint16_t valuee = digitalRead(_LCDML_CONTROL_digitalread);  //digitalpinenter


		if (valuey >= _LCDML_CONTROL_analog_up_min && valuey <= _LCDML_CONTROL_analog_up_max) { LCDML.BT_up(); }        // up
		if (valuey >= _LCDML_CONTROL_analog_down_min && valuey <= _LCDML_CONTROL_analog_down_max) { LCDML.BT_down(); }    // down
		if (valuex >= _LCDML_CONTROL_analog_left_min && valuex <= _LCDML_CONTROL_analog_left_max) { LCDML.BT_left(); }     // left
		if (valuex >= _LCDML_CONTROL_analog_right_min && valuex <= _LCDML_CONTROL_analog_right_max) { LCDML.BT_right(); }    // right

		if (valuee == true) { LCDML.BT_enter(); }    // enter

													 // back buttons have to be included as menuitem 
													 // lock at the examle LCDML_back_button
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************

// *********************************************************************
// *************** (7) CONTROL OVER PCF8574 ****************************
// *********************************************************************

#elif(_LCDML_CONTROL_cfg == 7)
unsigned long g_LCDML_DISP_press_time = 0;
#define PCF8574_1 0x26 // I2C Adresse für die Taster

#define PCF8574_Pin0 254
#define PCF8574_Pin1 253
#define PCF8574_Pin2 251
#define PCF8574_Pin3 247
#define PCF8574_Pin4 239
#define PCF8574_Pin5 223
#define PCF8574_Pin6 191
#define PCF8574_Pin7 127

// Hier die PCF8574 Pins angeben
#define _LCDML_CONTROL_PCF8574_enable_quit    0
#define _LCDML_CONTROL_PCF8574_enable_lr      0
#define _LCDML_CONTROL_PCF8574_enter          PCF8574_Pin1
#define _LCDML_CONTROL_PCF8574_up             PCF8574_Pin2
#define _LCDML_CONTROL_PCF8574_down           PCF8574_Pin0
#define _LCDML_CONTROL_PCF8574_left           PCF8574_Pin2
#define _LCDML_CONTROL_PCF8574l_right         PCF8574_Pin2
#define _LCDML_CONTROL_PCF8574_quit           PCF8574_Pin2
// **********************************************************
void lcdml_menu_control(void)
{
	// If something must init, put in in the setup condition
	if (LCDML.BT_setup()) {
		// runs only once
	}

	if ((millis() - g_LCDML_DISP_press_time) >= 200) {
		g_LCDML_DISP_press_time = millis(); // reset press time

		Wire.write(0xff); // Alle Pins als Eingang?
		Wire.requestFrom(PCF8574_1, 1);
		if (Wire.available()) {
			switch (Wire.read())
			{
			case _LCDML_CONTROL_PCF8574_enter:  LCDML.BT_enter(); break;
			case _LCDML_CONTROL_PCF8574_up:     LCDML.BT_up();    break;
			case _LCDML_CONTROL_PCF8574_down:   LCDML.BT_down();  break;
#if(_LCDML_CONTROL_PCF8574_enable_quit == 1)
			case _LCDML_CONTROL_PCF8574_left:   LCDML.BT_left();  break;
#endif
#if(_LCDML_CONTROL_PCF8574_enable_lr   == 1)
			case _LCDML_CONTROL_PCF8574l_right: LCDML.BT_right(); break;
			case _LCDML_CONTROL_PCF8574_quit:   LCDML.BT_quit();  break;
#endif 
			default: break;
			}
		}
	}
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************


#else
#error _LCDML_CONTROL_cfg is not defined or not in range
#endif

/* ===================================================================== *
*                                                                       *
* Dynamic content                                                       *
*                                                                       *
* ===================================================================== *
*/


uint8_t g_dynParam = 100; // when this value comes from an eeprom, load it in setup 
						  // at the moment here is no setup function (todo) 
void mDyn_para(uint8_t line)
// *********************************************************************
{
	// check if this function is active (cursor stands on this line)     
	if (line == LCDML.MENU_getCursorPos())
	{
		// make only an action when the cursor stands on this menuitem
		//check Button
		if (LCDML.BT_checkAny()) {
			if (LCDML.BT_checkEnter()) {
				// dosomething for example save the data or something else             
				LCDML.BT_resetEnter();
			}
			if (LCDML.BT_checkLeft()) {
				g_dynParam++;
				LCDML.BT_resetLeft();
			}
			if (LCDML.BT_checkRight()) {
				g_dynParam--;
				LCDML.BT_resetRight();
			}
		}
	}

	char buf[20];
	sprintf(buf, "dynValue: %d", g_dynParam);

	// use the line from function parameters
	lcd.setCursor(1, line);
	lcd.print(buf);

}

// =====================================================================
//
// Output function
//
// =====================================================================

/* ******************************************************************** */
void lcdml_menu_clear()
/* ******************************************************************** */
{
	lcd.clear();
	lcd.setCursor(0, 0);
}

/* ******************************************************************** */
void lcdml_menu_display()
/* ******************************************************************** */
{
	// update content
	// ***************
	if (LCDML.DISP_checkMenuUpdate()) {
		// clear menu
		// ***************
		LCDML.DISP_clear();

		// decalration of some variables
		// ***************    
		// content variable
		char content_text[_LCDML_DISP_cols];  // save the content text of every menu element 
											  // menu element object
		LCDMenuLib2_menu *tmp;
		// some limit values
		uint8_t i = LCDML.MENU_getScroll();
		uint8_t maxi = _LCDML_DISP_rows + i;
		uint8_t n = 0;

		// check if this element has children
		if (tmp = LCDML.MENU_getObj()->getChild(LCDML.MENU_getScroll()))
		{
			// loop to display lines        
			do
			{
				// check if a menu element has a condetion and if the condetion be true               
				if (tmp->checkCondetion())
				{
					// check the type off a menu element
					if (tmp->checkType_menu() == true)
					{
						// display normal content
						LCDML_getContent(content_text, tmp->getID());
						lcd.setCursor(1, n);
						lcd.print(content_text);
					}
					else
					{
						if (tmp->checkType_dynParam()) {
							tmp->callback(n);
						}
					}
					// increment some values 
					i++;
					n++;
				}
				// try to go to the next sibling and check the number of displayed rows           
			} while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
		}
	}

	if (LCDML.DISP_checkMenuCursorUpdate())
	{
		// init vars
		uint8_t n_max = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
		uint8_t scrollbar_min = 0;
		uint8_t scrollbar_max = LCDML.MENU_getChilds();
		uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
		uint8_t scroll_pos = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);


		// display rows
		for (uint8_t n = 0; n < n_max; n++)
		{
			//set cursor
			lcd.setCursor(0, n);

			//set cursor char
			if (n == LCDML.MENU_getCursorPos()) {
				lcd.write(_LCDML_DISP_cfg_cursor);
			}
			else {
				lcd.write(' ');
			}

			// delete or reset scrollbar
			if (_LCDML_DISP_cfg_scrollbar == 1) {
				if (scrollbar_max > n_max) {
					lcd.setCursor((_LCDML_DISP_cols - 1), n);
					lcd.write((uint8_t)0);
				}
				else {
					lcd.setCursor((_LCDML_DISP_cols - 1), n);
					lcd.print(' ');
				}
			}
		}

		// display scrollbar
		if (_LCDML_DISP_cfg_scrollbar == 1) {
			if (scrollbar_max > n_max) {
				//set scroll position
				if (scrollbar_cur_pos == scrollbar_min) {
					// min pos
					lcd.setCursor((_LCDML_DISP_cols - 1), 0);
					lcd.write((uint8_t)1);
				}
				else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
					// max pos
					lcd.setCursor((_LCDML_DISP_cols - 1), (n_max - 1));
					lcd.write((uint8_t)4);
				}
				else {
					// between
					lcd.setCursor((_LCDML_DISP_cols - 1), scroll_pos / n_max);
					lcd.write((uint8_t)(scroll_pos % n_max) + 1);
				}
			}
		}
	}
}

/* ===================================================================== *
*                                                                       *
* Menu Callback Function                                                *
*                                                                       *
* ===================================================================== *
*
* EXAMPLE CODE:
// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
if(LCDML.FUNC_setup())          // ****** SETUP *********
{
// setup
// is called only if it is started
// starts a trigger event for the loop function every 100 millisecounds
LCDML.FUNC_setLoopInterval(100);
}
if(LCDML.FUNC_loop())           // ****** LOOP *********
{
// loop
// is called when it is triggert
// - with LCDML_DISP_triggerMenu( millisecounds )
// - with every button status change
// check if any button is presed (enter, up, down, left, right)
if(LCDML.BT_checkAny()) {
LCDML.FUNC_goBackToMenu();
}
}
if(LCDML.FUNC_close())      // ****** STABLE END *********
{
// loop end
// you can here reset some global vars or delete it
}
}

* ===================================================================== *
*/


// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// setup function 
		lcd.setCursor(0, 0);
		lcd.print(F("To close this"));
		lcd.setCursor(0, 1);
		lcd.print(F("function press"));
		lcd.setCursor(0, 2);
		lcd.print(F("any button or use"));
		lcd.setCursor(0, 3);
		lcd.print(F("back button"));
	}

	if (LCDML.FUNC_loop())           // ****** LOOP ********* 
	{
		// loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
		// the quit button works in every DISP function without any checks; it starts the loop_end function   
		if (LCDML.BT_checkAny()) { // check if any button is presed (enter, up, down, left, right)
								   // LCDML_goToMenu stops a running menu function and goes to the menu
			LCDML.FUNC_goBackToMenu();
		}
	}

	if (LCDML.FUNC_close())      // ****** STABLE END ********* 
	{
		// you can here reset some global vars or do nothing 
	}
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (globale variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP ********* 
	{
		lcd.print(F("20 sec wait")); // print some content on first row    
		g_func_timer_info = 20;       // reset and set timer    
		LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 millisecounds

		LCDML.TIMER_msReset(g_timer_1);
	}


	if (LCDML.FUNC_loop())           // ****** LOOP ********* 
	{
		// loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
		// the quit button works in every DISP function without any checks; it starts the loop_end function 

		// reset screensaver timer
		LCDML.SCREEN_resetTimer();

		// this function is called every 100 millisecounds

		// this method checks every 1000 millisecounds if it is called
		if (LCDML.TIMER_ms(g_timer_1, 1000)) {
			g_func_timer_info--;                // increment the value every secound
			lcd.setCursor(0, 0);                // set cursor pos
			lcd.print(F("  "));
			lcd.setCursor(0, 0);                // set cursor pos
			lcd.print(g_func_timer_info);       // print the time counter value   
		}

		// this function can only be ended when quit button is pressed or the time is over
		// check if the function ends normaly
		if (g_func_timer_info <= 0)
		{
			// leave this function
			LCDML.FUNC_goBackToMenu();
		}
	}

	if (LCDML.FUNC_close())      // ****** STABLE END ********* 
	{
		// you can here reset some global vars or do nothing
	}
}


// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
void mFunc_p2(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// setup function
		// print lcd content
		// print lcd content
		lcd.setCursor(0, 0);
		lcd.print(F("press left or up"));
		lcd.setCursor(0, 1);
		lcd.print(F("count: 0 of 3"));
		// Reset Button Value
		g_button_value = 0;

		// Disable the screensaver for this function until it is closed
		LCDML.FUNC_disableScreensaver();

	}

	if (LCDML.FUNC_loop())           // ****** LOOP *********
	{
		// the quit button works in every DISP function without any checks; it starts the loop_end function  
		if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
		{
			if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
			{
				LCDML.BT_resetLeft(); // reset the left button
				LCDML.BT_resetUp(); // reset the left button
				g_button_value++;

				// update lcd content
				// update lcd content
				lcd.setCursor(7, 1); // set cursor   
				lcd.print(g_button_value); // print change content       
			}
		}

		// check if button count is three
		if (g_button_value >= 3) {
			LCDML.FUNC_goBackToMenu();      // leave this function  
		}
	}

	if (LCDML.FUNC_close())     // ****** STABLE END *********
	{
		// you can here reset some global vars or do nothing
	}
}



// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// update lcd content
		lcd.setCursor(0, 0); // set cursor   
		lcd.print("screensaver"); // print change content
		lcd.setCursor(0, 1); // set cursor
		lcd.print("press any key");
		lcd.setCursor(0, 2); // set cursor
		lcd.print("to leave it");
		LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 millisecounds
	}

	if (LCDML.FUNC_loop())
	{
		if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
		{
			LCDML.FUNC_goBackToMenu();  // leave this function   
		}
	}

	if (LCDML.FUNC_close())
	{
		// The screensaver go to the root menu
		LCDML.MENU_goRoot();
	}
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// end function and go an layer back
		LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
	}
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// go to root and display menu
		LCDML.MENU_goRoot();
	}
}

// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{
		// Jump to Initscreen
		if (!LCDML.OTHER_jumpToFunc(mFunc_timer_info))
		{
			// function not found or not callable
			LCDML.MENU_goRoot();
		}
	}
}


// *********************************************************************
void mFunc_para(uint8_t param)
// *********************************************************************
{
	if (LCDML.FUNC_setup())          // ****** SETUP *********
	{

		char buf[20];
		sprintf(buf, "parameter: %d", param);

		lcd.setCursor(0, 0);
		lcd.print(buf);
		lcd.setCursor(0, 1);
		lcd.print(F("press any key"));
		lcd.setCursor(0, 2);
		lcd.print(F("to leave it"));

		LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 millisecounds
	}

	if (LCDML.FUNC_loop())          // ****** LOOP *********
	{
		// For example
		switch (param)
		{
		case 10:
			// do something
			break;

		case 20:
			// do something
			break;

		case 30:
			// do something
			break;

		default:
			// do nothing
			break;
		}


		if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
		{
			LCDML.FUNC_goBackToMenu();  // leave this function   
		}
	}

	if (LCDML.FUNC_close())        // ****** STABLE END *********
	{
		// you can here reset some global vars or do nothing
	}
}

// Demo of a Keypad LCD Shield
// Common design sold by DFRobot, various vendors on eBay etc

#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>

// this is factory config shield initialisation
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// below is for shield which has been modified (author's shield)
//LiquidCrystal lcd(8,3,4,5,6,7); 

// initialise the keypad
DFR_LCD_Keypad keypad(A0, &lcd);
byte heart[8] = {
	0b00000,
	0b01010,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00000
};
byte guy[8] = {
	0b01110,
	0b01010,
	0b01110,
	0b00100,
	0b11111,
	0b00100,
	0b01010,
	0b10001
};
byte guyL[8] = {
	0b11100,
	0b10100,
	0b11100,
	0b00100,
	0b11111,
	0b00100,
	0b01010,
	0b10001
};
byte guyR[8] = {
	0b00111,
	0b00101,
	0b00111,
	0b00100,
	0b11111,
	0b00100,
	0b01010,
	0b10001
};
byte door[8] = {
	0b01110,
	0b11111,
	0b11111,
	0b11111,
	0b11101,
	0b11111,
	0b11111,
	0b11111
};
byte exitKey[8] = {
	0b00000,
	0b01110,
	0b01010,
	0b01110,
	0b00100,
	0b00100,
	0b00110,
	0b00000
};
byte enemy[8] = { 
	0b01010,
	0b01110,
	0b01110,
	0b00100,
	0b11111,
	0b00100,
	0b01010,
	0b10001
};
byte gold[8] = {
	0b00000,
	0b00100,
	0b01110,
	0b01010,
	0b01010,
	0b01110,
	0b00100,
	0b00000
};

// 1 - Wall
// 2 - Start Door 1
// 3 - End Door 1
// 4 - Enemy 2
// 5 - Gold 3
// 6 - Key 4                                                                                                                                                                                                                                                                                                        h

int map1[14][14] = {
	{1,1,1,1,1,2,1,1,1,1,1,1,1,1},
	{1,1,1,0,1,0,1,0,0,0,0,5,0,1},
	{1,0,0,0,1,0,1,1,4,1,1,1,0,1},
	{1,0,1,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,1,1,1,1,1,1,1,1,1,1,0,1},
	{1,0,4,1,0,0,4,0,0,0,0,0,0,1},
	{1,1,1,1,0,1,1,1,1,1,1,1,0,1},
	{1,6,5,1,0,0,0,0,4,0,0,1,1,1},
	{1,0,0,4,0,1,0,0,1,5,5,1,5,1},
	{1,0,1,1,0,1,4,1,1,1,1,1,0,1},
	{1,0,1,0,0,1,0,0,0,0,0,0,0,1},
	{1,0,4,0,1,1,0,5,1,0,1,1,1,1},
	{1,5,1,0,0,0,0,5,1,0,0,0,0,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

String splashScreen1 = "  Test Game 1  ";
String splashScreen2 = "  Adam Benoit   ";
String WinScreen1 = "   You Won!!!  ";

int last_key, 
	key, 
	charCol, 
	charRow, 
	colMax, 
	rowMax, 
	curChar, 
	bkgPos, 
	lives, 
	score,
	visibleRow,
	hudCounter,
	kills;

bool hasKey, hasWon;

void setup()
{
	colMax = 16;
	rowMax = 2;
	curChar = 5;
	charCol = 5;
	charRow = 1;
	visibleRow = 0;
	lives = 3;
	score = 0;
	lcd.flush();
	hasKey = false;
	hasWon = false;
	hudCounter = 0;
	kills = 0;

	lcd.begin(rowMax, colMax);
	lcd.clear();

	lcd.createChar(0, heart);
	lcd.createChar(1, door);
	lcd.createChar(2, enemy);
	lcd.createChar(3, gold);
	lcd.createChar(4, exitKey);
	lcd.createChar(5, guy);
	lcd.createChar(6, guyL);
	lcd.createChar(7, guyR);

	splash();
	//win();
	//showMap();
}

void showMap()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	for (int r = 0; r < 14; r++)
	{
		int* row0[8] = { map1[r] };
		int* row1[8] = { map1[r + 1] };
		for (int i = 0; i < 14; i++)
		{
			lcd.setCursor(i, 0);
			printTile(row0[0][i]);
			lcd.setCursor(i, 1);
			printTile(row1[0][i]);
		}
		delay(1000);
		lcd.clear();
	}
}

void drawMap()
{
	int* row0[8] = { map1[visibleRow] };
	int* row1[8] = { map1[visibleRow + 1] };
	for (int i = 0; i < 14; i++)
	{
		lcd.setCursor(i, 0);
		printTile(row0[0][i]);
		lcd.setCursor(i, 1);
		printTile(row1[0][i]);
	}
}

void printTile(int id)
{
	switch (id)
	{
		default:
		case 0:
			lcd.print((char)254);
			break;
		case 1:
			lcd.print((char)255);
			break;
		case 2:
			lcd.write(byte(1));
			break;
		case 3:
			lcd.write(byte(1));
			break;
		case 4:
			lcd.write(byte(2));
			break;
		case 5:
			lcd.write(byte(3));
			break;
		case 6:
			lcd.write(byte(4));
			break;
	}
}

bool collision()
{
	int tile;
	int r = visibleRow + charRow;
	int c = charCol;
	switch (key) {
	case KEY_RIGHT:
		c++;
		break;
	case KEY_UP:
		r--;
		break;
	case KEY_DOWN:
		r++;
		break;
	case KEY_LEFT:		
		c--;
		break;
	case KEY_NONE:
	default:
		return false;
	}
	tile = map1[r][c];

	switch (tile)
	{
	case 0:
		return true;
	case 1:
	case 2:
			return false;
	case 3:
		if (hasKey)
		{
			hasWon = true;
			return true;
		}
		else
		{
			return false;
		}
	case 4: 
		lives--;
		charCol = 5;
		charRow = 1;
		return false;
	case 5:
		score++;
		map1[r][c] = 0;
		return true;
	case 6:
		hasKey = true;
		map1[r][c] = 0;
	default:
		return false;
	}
}

void moveChar()
{
	int r = visibleRow + charRow;
	int c = charCol;
	switch (key) {
	case KEY_LEFT:
		if (collision())
		{
			charCol--;
			curChar = 6;
		}
		break;
	case KEY_RIGHT:
		if (collision())
		{
			charCol++;
			curChar = 7;
		}
				break;
	case KEY_UP:
		if (collision())
		{
			if (charRow == 0)
			{
				charRow = 1;
			}
			/*else
			{
				charRow = 0;
			}*/
			visibleRow--;
			curChar = 5;
		}
		break;
	case KEY_DOWN:
		if (collision())
		{
			if (charRow == 1)
			{
				charRow = 0;
			}
			visibleRow++;
			curChar = 5;
		}
		break;
	case KEY_SELECT:
		if (map1[r+1][c] == 4)
		{
			map1[r+1][c] = 0;
			kills++;
		}
		else if (map1[r-1][c] == 4)
		{
			map1[r-1][c] = 0;
			kills++;
		}
		else if (map1[r][c+1] == 4)
		{
			map1[r][c+1] = 0;
			kills++;
		}
		else if ( map1[r][c-1] == 4)
		{
			map1[r][c-1] = 0;
			kills++;
		}

		break;
	case KEY_NONE:
	default:
		break;
	}
	lcd.setCursor(charCol, charRow);
	lcd.write(byte(curChar));
}

void loop()
{
	if (!hasWon)
	{
		last_key = keypad.get_last_key();
		key = keypad.read_key();
		hud();

		if (key != last_key) {

			lcd.clear();

			drawMap();
			moveChar();
		}
		delay(10); // optional, but recommended if you are not debouncing the keys
	}
	else
	{
		win();
	}
}

void splash() {
	lcd.setCursor(0, 0);
	lcd.print(splashScreen1);
	lcd.setCursor(0, 1);
	lcd.print(splashScreen2);
	delay(1500);
}

void win()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(WinScreen1);
	lcd.setCursor(3, 1);
	lcd.print("$");
	lcd.setCursor(4, 1);
	lcd.print(score);
	lcd.setCursor(7, 1);

	lcd.write(byte(2));
	lcd.setCursor(8, 1);
	lcd.print(kills);
	lcd.setCursor(11, 1);
	lcd.write(byte(5));
	lcd.setCursor(12, 1);
	lcd.print(lives);
	delay(5500);
	setup();
}

void hud() {
	if (hudCounter < 200)
	{
		lcd.setCursor(colMax - 2, 0);
		lcd.write(byte(0));
		lcd.setCursor(colMax - 1, 0);
		lcd.print(lives);

		lcd.setCursor(colMax - 2, 1);
		lcd.print("$");
		lcd.setCursor(colMax - 1, 1);
		lcd.print(score);
	}
	else if (hudCounter >= 200 && hudCounter < 400)
	{
		lcd.setCursor(colMax - 2, 0);
		lcd.write(byte(2));
		lcd.setCursor(colMax - 1, 0);
		lcd.print(kills);

		lcd.setCursor(colMax - 2, 1);
		lcd.write(byte(4));
		lcd.setCursor(colMax - 1, 1);
		lcd.print(hasKey ? 1 : 0);
	}
	else
	{
		hudCounter = 0;
	}


	hudCounter++;
}
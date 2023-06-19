#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <time.h>

#define NUM0	6
#define NUM1	21
#define NUM2	22
#define NUM3	23
#define NUM4	24
#define NUM5	25
#define NUM6	26
#define NUM7	27
#define NUM8	28
#define NUM9	29

#define SWITCH_CNT	10
#define SIZE		3

// Define some device parameters
#define I2C_ADDR   0x27 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0b00000100 // Enable bit

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);
int fd;  // seen by all subroutines

int switch_arr[SWITCH_CNT] = {NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9};

int result = -1;

int switch_num(void) {
	int result = -1;
	for(int i=0; i<SWITCH_CNT; i++) {
		if(digitalRead(switch_arr[i]) == HIGH) {
			result = i;
			break;
		}
	} 

	return result;
}

void UpDownGame(int answer) {
	int digits[2];

	for(int i=0; i<2; i++) {
		digits[i] = -1;
	}

	while(1) {
		
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Press Button!");
		lcdLoc(LINE2);
		for(int i=0; i<2; i++) {
			//printf("Push the button!\n");
			while(digits[i] == -1) {
				digits[i] = switch_num();
			}
			typeInt(digits[i]);
			//printf("%d\n", digits[i]);
			delay(1000);
		}
		
		int result = digits[0] * 10 + digits[1];
		printf("Result : %02d\n", result);

		if (result == 0) return;
		else {
			if(result > answer) {
				ClrLcd();
				lcdLoc(LINE1);
				typeln("DOWN!");
				delay(2000);
				//printf("DOWN!\n");
			} else if (result < answer) {
				ClrLcd();
				lcdLoc(LINE1);
				typeln("UP!");
				delay(2000);
				//printf("UP!\n");
			} else {
				ClrLcd();
				lcdLoc(LINE1);
				typeln("Correct!!");
				delay(2000);
				//printf("Correct!!\n");
				return;
			}
		}

		for(int i=0; i<2; i++) {
			digits[i] = -1;
		}

		//delay(500);
	}

}

void BaseBallGame(int * answer) {
	int strike = 0, ball = 0, out = SIZE;
	int user[SIZE];
	int cnt = 0;

	do {
		strike = 0;
		ball = 0;
		out = SIZE;
		
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Press Button!");
		lcdLoc(LINE2);

		for(int i=0; i<SIZE; i++) {
			//printf("Push the button!\n");
			while(switch_num() == -1) {}
			user[i] = switch_num();
			typeInt(user[i]);
			//printf("%d\n", user[i]);
			delay(1000);
		}

		for(int i=0; i<SIZE; i++) {
			for(int j=0; j<SIZE; j++) {
				if(answer[i] == user[j]) {
					if(i == j) {
						strike += 1;
						out -= 1;
					} else {
						
						ball += 1;
						out -= 1;
					}
				}
			}
		}
		
		cnt++;
		ClrLcd();
		lcdLoc(LINE1);
		typeln("<result "); typeInt(cnt); typeln(">");
		lcdLoc(LINE2);
		typeln("Strike : "); typeInt(strike);
		delay(2000);
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Ball : "); typeInt(ball);
		lcdLoc(LINE2);
		typeln("Out : "); typeInt(out);
		delay(2000);
		//printf("<result %d> strike : %d, ball : %d, out : %d\n\n", cnt, strike, ball, out);		

	} while (strike != SIZE);

	ClrLcd();
	lcdLoc(LINE1);
	typeln("Success!! ");
	delay(2000);
//	printf("Success!\n");
}

void setup(void) {
	wiringPiSetup();

	for (int i=0; i<SWITCH_CNT; i++) {
		pinMode(switch_arr[i], INPUT);
		pullUpDnControl(switch_arr[i], PUD_DOWN);
	}
	
	fd = wiringPiI2CSetup(I2C_ADDR);

	lcd_init(); // setup LCD
}

int main(void) {
	setup();

	srand((unsigned int)time(NULL));

	lcdLoc(LINE1);
	typeln("~~ MINI GAME ~~");
	lcdLoc(LINE2);
	typeln("Welcome !!");
	delay(2000);

	while(1) {
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Pick a game!");
		delay(2000);
		
		ClrLcd();
		lcdLoc(LINE1);
		typeln("UP&DOWN GAME: 0");
		lcdLoc(LINE2);
		typeln("BaseBall Game: 1");
//		delay(2000);
		
		while(switch_num() == -1) {}
		
		if (switch_num() == 0) {
			ClrLcd();
			lcdLoc(LINE1);
			typeln("~ UP&DOWN GAME ~");
			lcdLoc(LINE2);
			typeln("Game Start!");
			delay(1000);

			int answer = (rand() & 98) + 1;
			
			printf("answer : %d\n", answer);
			UpDownGame(answer);

		} else if (switch_num() == 1) {
			ClrLcd();
			lcdLoc(LINE1);
			typeln("~BaseBall GAME~");
			lcdLoc(LINE2);
			typeln("Game Start!");
			delay(1000);
			
			int answer[SIZE];

			for(int i=0; i<SIZE; i++) {
				answer[i] = rand() % 10;
				for (int j=0; j<i; j++) {
					if(answer[j] == answer[i]) {
						answer[i] = rand() % 10;
					}
				}
				printf("%d", answer[i]);
			}
			printf("\n");

			BaseBallGame(answer);

		} else {
			ClrLcd();
			lcdLoc(LINE1);
			typeln("Not Options !");
			lcdLoc(LINE2);
			typeln("Choose again !!");
			delay(2000);
			continue;
		}
		
		ClrLcd();
		lcdLoc(LINE1);
		typeln("Play More?");
		lcdLoc(LINE2);
		typeln("Yes: 0, No: 1");
//		delay(1500);

		while(switch_num() == -1) {}
		if (switch_num() == 1) {
			break;
		}


	}

	ClrLcd();
	lcdLoc(LINE1);
	typeln("Bye Bye!!");
	//printf("Bye!\n");

	return 0;
}


// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

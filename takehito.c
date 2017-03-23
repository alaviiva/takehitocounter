// Takehito Koyasu Counterthingymaschine.

// Version 5.7,12
// This monstrosity was created by the following people:
// Markus Hiienkari, Johan Strandman, Sampo Vartiala, Ville Sinkkonen
// This software is distributed under the DWFYWSL (Do-Whatever-The-Fuck-You-Want-Software license)
// It can not be distributed under any other license than DWFYWSL.
// By opening this file you accept the terms and conditions.

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

/* 
PB0
PD7
PD6
PD5
PB7
PB6
PD4
PD3
PD2
PB1
PB2
PB3
PB4
PB5
PC5
PC0
PC1
PC2
PC3
PC4
*/

#define F_CPU 1000000UL 
//#define SCROLL_DELAY 1 /* ms */
#define SCROLL_DELAY 100 /* ms */
//#define SCROLL_STEP() _delay_ms(SCROLL_DELAY);
#define SCROLL_STEP() _delay_us(SCROLL_DELAY);

#define	BTN_BIT		(1<<1)

#define COL0_PORT PORTC
#define COL1_PORT PORTB
#define COL2_PORT PORTB
#define COL3_PORT PORTB
#define COL4_PORT PORTB

#define COL5_PORT PORTB
#define COL6_PORT PORTD
#define COL7_PORT PORTD
#define COL8_PORT PORTD
#define COL9_PORT PORTB

#define COL0_BIT (1<<5)  // PB5  
#define COL1_BIT (1<<5)  // PB5
#define COL2_BIT (1<<4)  // PB4
#define COL3_BIT (1<<3)  // PB3
#define COL4_BIT (1<<2)  // PC2

#define COL5_BIT (1<<1)  // PB1
#define COL6_BIT (1<<2)  // PD2
#define COL7_BIT (1<<3)  // PD3
#define COL8_BIT (1<<4)  // PD4
#define COL9_BIT (1<<6)  // PB6



#define ROW0_PORT PORTC 
#define ROW1_PORT PORTC
#define ROW2_PORT PORTC
#define ROW3_PORT PORTC
#define ROW4_PORT PORTC

#define ROW5_PORT PORTB
#define ROW6_PORT PORTD
#define ROW7_PORT PORTD
#define ROW8_PORT PORTD
#define ROW9_PORT PORTB

#define ROW0_BIT (1<<0)  // PC0
#define ROW1_BIT (1<<1)  // PC1
#define ROW2_BIT (1<<2)  // PC2
#define ROW3_BIT (1<<3)  // PC3
#define ROW4_BIT (1<<4)  // PC4

#define ROW5_BIT (1<<7)  // PB7
#define ROW6_BIT (1<<5)  // PD5
#define ROW7_BIT (1<<6)  // PD6
#define ROW8_BIT (1<<7)  // PD7
#define ROW9_BIT (1<<0)  // PB0

// strings stored in flash
const char str_0[] PROGMEM = "DAT BAT";
const char str_1[] PROGMEM = "ILPALAZZO";
const char str_2[] PROGMEM = "HARUHI'S \"MOM\"";
const char str_3[] PROGMEM = "OLIVIER";
const char str_4[] PROGMEM = "ZECHS";     
const char str_5[] PROGMEM = "THE FOOL";
const char str_6[] PROGMEM = "MU LA FLAGA";
const char str_7[] PROGMEM = "REZO";
const char str_8[] PROGMEM = "HOTOHORI";
const char str_9[] PROGMEM = "TAKAHASHI RYOUSUKE";
const char str_10[] PROGMEM = "KEIKI";
const char str_11[] PROGMEM = "KUROYANAGI";
const char str_12[] PROGMEM = "HONDA SHIGEHARU";
const char str_13[] PROGMEM = "JADE CURTISS";
const char str_14[] PROGMEM = "KRAFT";
const char str_15[] PROGMEM = "DIO";
const char str_16[] PROGMEM = "ABURATSUBO";
const char str_17[] PROGMEM = "KIRYUU";
const char str_18[] PROGMEM = "THE OTHER EINBERG";
const char str_19[] PROGMEM = "KAYAMA YUUICHI";
const char str_20[] PROGMEM = "EXCALIBUR";
const char str_21[] PROGMEM = "THAT DORSSIAN POPE"; 
const char str_22[] PROGMEM = "  SORSA%DSM WAS HERE  }}}DESUDESU}DESUDESU}DESUDESU}DESUDESU}DESUDESUDESU_";
const char str_23[] PROGMEM = "  TAKEHITO-COUNTER (2014-2016), BROUGHT TO YOU BY MURSU, SORSA%DSM, ARB AND MARQS^   AND { STILL{ NEEDS{ MOAR {DESU _";
const char str_24[] PROGMEM = "  TEAM TAKEHITO-COUNTER: PRO{{{{{{{{D}}}}}}}}}}UCING QUA{}{}{{{{{L{{IT}}Y}}}}} CO{{{DE}}} SINC{{}}E 2014}}...._";
const char str_25[] PROGMEM = "  THEY SEE MEE ROLLIN..._";
const char str_26[] PROGMEM = "  RURI-CHAAAAN NOT}I}C}E }}M}EEEEEEEEEEEEEEEEE!!!!!!!!_";
// note to self: you might want to look into PSTR() 

const char* const str_table[] PROGMEM = {
	str_0, str_1, str_2, str_3, str_4, str_5, str_6, str_7, str_8, str_9, 
	str_10, str_11, str_12, str_13, str_14, str_15, str_16, str_17, str_18, str_19, 
	str_20, str_21, str_22, str_23, str_24, str_25, str_26 
};

//fishy number used in magic
unsigned char turska = 0;

char buffer[150];

// get string to buffer (called buffer)
void get_str(int i) {
	// worse than hitler
	strcpy_P(buffer, (char*) pgm_read_word(&(str_table[i])));
}


// This is a bright idea...let's use ASCII!
char letter[59+4][5] = {
					{0x00, 0x00, 0x00, 0x00, 0x00}, //NBSP
					{0x00, 0x00, 0x17, 0x00, 0x00}, //!
					{0x00, 0x03, 0x00, 0x03, 0x00}, //"
					{0x0A, 0x1F, 0x0A, 0x1F, 0x0A}, //#
					{0x00, 0x1C, 0x14, 0x1C, 0x00}, //$ Nope, o
					{0x10, 0x10, 0x10, 0x10, 0x00}, //% Nope, _ 
					{0x00, 0x00, 0x00, 0x00, 0x00}, //& Nope
					{0x00, 0x03, 0x00, 0x00, 0x00}, //'
					{0x00, 0x00, 0x0E, 0x11, 0x00}, //(
					{0x00, 0x00, 0x11, 0x0E, 0x00}, //)
					{0x00, 0x00, 0x00, 0x00, 0x00}, //* nope
					{0x04, 0x04, 0x1F, 0x04, 0x04}, //+
					{0x00, 0x00, 0x10, 0x08, 0x00}, //,
					{0x00, 0x04, 0x04, 0x04, 0x00}, //-
					{0x00, 0x00, 0x00, 0x10, 0x00}, //.
					{0x00, 0x18, 0x04, 0x03, 0x00}, ///
					{0x0E, 0x11, 0x11, 0x0E, 0x00}, //0
					{0x00, 0x12, 0x1F, 0x10, 0x00}, //1
					{0x12, 0x19, 0x15, 0x12, 0x00}, //2
					{0x11, 0x15, 0x15, 0x1F, 0x00}, //3
					{0x07, 0x04, 0x04, 0x1F, 0x00}, //4
					{0x17, 0x15, 0x15, 0x0D, 0x00}, //5
					{0x1F, 0x15, 0x15, 0x1D, 0x00}, //6
					{0x11, 0x09, 0x05, 0x03, 0x00}, //7
					{0x1B, 0x15, 0x15, 0x1B, 0x00}, //8
					{0x17, 0x15, 0x15, 0x1F, 0x00}, //9
					{0x00, 0x00, 0x00, 0x12, 0x00}, //:
					{0x00, 0x00, 0x10, 0x0A, 0x00}, //;
					{0x00, 0x04, 0x0A, 0x11, 0x00}, //<
					{0x00, 0x0A, 0x0A, 0x0A, 0x00}, //=
					{0x00, 0x11, 0x0A, 0x04, 0x00}, //>
					{0x00, 0x15, 0x05, 0x07, 0x00}, //?
					{0x00, 0x00, 0x00, 0x00, 0x00}, //@ nope, also magic
					{0x1F, 0x05, 0x05, 0x1F, 0x00}, //A
					{0x1F, 0X15, 0x15, 0x0E, 0x00}, //B
					{0x1F, 0x11, 0x11, 0x11, 0x00}, //C
					{0x1F, 0x11, 0x11, 0x0E, 0x00}, //D
					{0x1f, 0x15, 0x15, 0x11, 0x00}, //E
					{0x1F, 0x05, 0x05, 0x01, 0x00}, //F
					{0x1F, 0x11, 0x15, 0x1D, 0x00}, //G
					{0x1F, 0x04, 0x04, 0x1F, 0x00}, //H
					{0x00, 0x11, 0x1F, 0x11, 0x00}, //I
					{0x19, 0x11, 0x1F, 0x01, 0x00}, //J
					{0x1F, 0x04, 0x0A, 0x11, 0x00}, //K
					{0x1F, 0x10, 0x10, 0x10, 0x00}, //L
					{0x1F, 0x02, 0x04, 0x02, 0x1F}, //M
					{0x1F, 0x02, 0x04, 0x1F, 0x00}, //N
					{0x1F, 0x11, 0x11, 0x1F, 0x00}, //O
					{0x1F, 0x05, 0x05, 0x07, 0x00}, //P
					{0x1F, 0x11, 0x19, 0x1F ,0x00}, //Q
					{0x1F, 0x05, 0x0D, 0x17, 0x00}, //R
					{0x17, 0x15, 0x15, 0x1D, 0x00}, //S
					{0x01, 0x01, 0x1F, 0x01, 0x01}, //T
					{0x1F, 0x10, 0x10, 0x1F, 0x00}, //U
					{0x1F, 0x08, 0x04, 0x03, 0x00}, //V
					{0x1F, 0x08, 0x04, 0x08, 0x1F}, //W
					{0x11, 0x0A, 0x04, 0x0A, 0x11}, //X
					{0x01, 0x02, 0x1C, 0x02, 0x01}, //Y
					{0x11, 0x19, 0x15, 0x13, 0x00}, //Z
					{0x00, 0x1F, 0x11, 0x11, 0x00}, //[ 
					{0x00, 0x03, 0x04, 0x18, 0x00}, /// 
					{0x00, 0x11, 0x11, 0x1F, 0x00}, //] 
					{0x00, 0x02, 0x01, 0x02, 0x00} //^
				   };






void io_setup () {
	DDRB = 0xff;	// 8
	DDRC = 0x3f;	// 6
	DDRD = 0xfc;	// 6
	
	PORTB = 0xff;   // 0b11111111
	PORTC = 0x3f;   // 0b00111111
	PORTD = 0xfc;   // 0b11111100 
	
	_delay_ms(1000);
	
	COL0_PORT &= ~COL0_BIT;
	COL1_PORT &= ~COL1_BIT;
	COL2_PORT &= ~COL2_BIT;
	COL3_PORT &= ~COL3_BIT;
	COL4_PORT &= ~COL4_BIT;
	COL5_PORT &= ~COL5_BIT;
	COL6_PORT &= ~COL6_BIT;
	COL7_PORT &= ~COL7_BIT;
	COL8_PORT &= ~COL8_BIT;
	COL9_PORT &= ~COL9_BIT;
}

void activate_col(int x, int matrix) {
	
	if (matrix == 1) {
	
		switch (x) {
			case 0:
				COL0_PORT |= COL0_BIT;
				break;
			case 1:
				COL1_PORT |= COL1_BIT;
				break;
			case 2:
				COL2_PORT |= COL2_BIT;
				break;
			case 3:
				COL3_PORT |= COL3_BIT;
				break;	
			case 4:
				COL4_PORT |= COL4_BIT;
				break;
		}
	} else {
		switch (x) {
			case 0:
				COL5_PORT |= COL5_BIT;
				break;
			case 1:
				COL6_PORT |= COL6_BIT;
				break;
			case 2:
				COL7_PORT |= COL7_BIT;
				break;
			case 3:
				COL8_PORT |= COL8_BIT;
				break;	
			case 4:
				COL9_PORT |= COL9_BIT;
				break;
		}
	}
}

/*
void enable_all_rows() {
	PORTB = 0xff;
	PORTC = 0x3f;
	PORTD = 0xfc;
	//disable all cols
	COL0_PORT &= ~COL0_BIT;
	COL1_PORT &= ~COL1_BIT;
	COL2_PORT &= ~COL2_BIT;
	COL3_PORT &= ~COL3_BIT;
	COL4_PORT &= ~COL4_BIT;
	COL5_PORT &= ~COL5_BIT;
	COL6_PORT &= ~COL6_BIT;
	COL7_PORT &= ~COL7_BIT;
	COL8_PORT &= ~COL8_BIT;
	COL9_PORT &= ~COL9_BIT;
}
void enable_all_cols() {
	PORTB = 0xff;
	PORTC = 0x3f;
	PORTD = 0xfc;
	//disable all rows
	ROW0_PORT &= ~ROW0_BIT;
	ROW1_PORT &= ~ROW1_BIT;
	ROW2_PORT &= ~ROW2_BIT;
	ROW3_PORT &= ~ROW3_BIT;
	ROW4_PORT &= ~ROW4_BIT;
	ROW5_PORT &= ~ROW5_BIT;
	ROW6_PORT &= ~ROW6_BIT;
	ROW7_PORT &= ~ROW7_BIT;
	ROW8_PORT &= ~ROW8_BIT;
	ROW9_PORT &= ~ROW9_BIT;
}
*/

void disable_all_damn_shit_crap() {
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
}



void update_matrices(char row1, char col1, char row2, char col2) {
	// row1 = five bits for row in matrix 1
	// row2 = five bits for row in matrix 2
	// col1 = column in matrix 1
	// col2 = column in matrix 2
	
	// Set values for given column on matrix 1

	
	if ( row1 & (0x01 << 0) ) {
		ROW0_PORT |= ROW0_BIT;
	}
	if ( row1 & (0x01 << 1) ) {
		ROW1_PORT |= ROW1_BIT;
		}
	if ( row1 & (0x01 << 2) ) {
		ROW2_PORT |= ROW2_BIT;
	}
	if ( row1 & (0x01 << 3) ) {
		ROW3_PORT |= ROW3_BIT;
	}
	if ( row1 & (0x01 << 4) ) {
		ROW4_PORT |= ROW4_BIT;
	}
	
	
	// Set values for given column on matrix 2
	if ( row2 & (0x01 << 0) ) {
		ROW5_PORT |= ROW5_BIT;
	}
	if ( row2 & (0x01 << 1) ) {
		ROW6_PORT |= ROW6_BIT;
		}
	if ( row2 & (0x01 << 2) ) {
		ROW7_PORT |= ROW7_BIT;
	}
	if ( row2 & (0x01 << 3) ) {
		ROW8_PORT |= ROW8_BIT;
	}
	if ( row2 & (0x01 << 4) ) {
		ROW9_PORT |= ROW9_BIT;
	}

	activate_col(col1, 1);
	activate_col(col2, 2);
			
	// Sleep for a while
	SCROLL_STEP();
		
	// Disable all goddammit	
	disable_all_damn_shit_crap();
}

void write_character(char chr1, char chr2, int time) {
	for (int phase = 0; phase < time; phase++){
		for (int col = 0; col < 5; col++){
			update_matrices(letter[chr1 - 32][col], col, letter[chr2 - 32][col], col);
		}
	}
}

void write_scroll_character(char chr1, char chr2, char chr3, int time) {
	
	char data[17];
	
	for (int p = 0; p < 17; p++){
		if (p < 5){
			data[p] = letter[chr1 - 32][p];
		}else if (p == 5) {
			data[p] = 0x00;
		}else if (p < 11){
			data[p] = letter[chr2 - 32][(p - 1) % 5];
		}else if (p == 11) {
			data[p] = 0x00;
		}else {
			data[p] = letter[chr3 - 32][(p - 2) % 5];
		}
		
	}
	for (int shift = 0; shift < 6; shift++){
		for (int phase = 0; phase < time; phase++){
			for (int col = 0; col < 5; col++){
				update_matrices(data[col + shift], col, data[col + 5 + shift], col);
			}
		}
	}
}

// scroll text
void scroll_text(const char *text, int speed) {
	int cursor1 = 0;
	int cursor2 = 1;
	int cursor3 = 2;
    while(1) {	
    	// change speed with { and } 
		while(text[cursor3] == '{' || text[cursor3] == '}'){
			if (text[cursor3] == '}'){
				speed -= 10;
				if (speed < 1){
					speed = 10;
                }
                cursor3++;             
            }if (text[cursor3] == '{'){
                speed += 10;
                cursor3++;
            }
        }
        while (text[cursor2] == '{' || text[cursor2] == '}'){
            cursor2++;
        }
        while (text[cursor1] == '{' || text[cursor1] == '}'){
            cursor1++;
        }
        
		write_scroll_character(text[cursor1], text[cursor2], text[cursor3], speed);
		
		cursor1++;
		cursor2++;
		cursor3++;
		// sorsa_DSM was here
		if ( text[cursor1] == '_'){
			break; 
		}
		if ( text[cursor2] == '_'){
			cursor2 = 0; 
		}
		if ( text[cursor3] == '_'){
			cursor3 = 0; 
		}						
	}
}

// this is where magic happens
void magic(char ch1, char ch2, int p9) {
	// TODO this could use only one buffer, 150 more bytes for our evil deeds
	char kala[150] = "";
	char k[] = "@@@@@";
	if (p9) {
		k[0] = ch1;
		k[1] = ch2;
	} else {
		k[0] = ch1;
	}
	strcat(kala, k);
	strcat(kala, PSTR("   KALA  "));
	get_str((turska % 5) + 22);
	strcat(kala, buffer);
	scroll_text(kala, 80);
}


int main() {
	io_setup();
	disable_all_damn_shit_crap();
	
	char chr1 = '0';
	char chr2 = '0';
	
	unsigned long time = 0; //...'cause why not?
	int time_div = 0;
	//scroll_text("  KALA}}KEITTOON}} TULEE}} KALAA}} JA}} KEITTOA_", 100);
	const char jee[] = "  DAYS SINCE TAKEHITO KOYASU :_"; //"  ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 !\"#$%&'()*=,-./:;<=>?@[\\]__"; //
	const char resetjee[] = " EVERYWHERE!!! ^^_";
        if (resetjee[0] == 0){}	
	//int shift = 0;
	int plus9 = 0;
	int r = 0; // index for name
	
	//magic(chr1, chr2, plus9);
	
	while (1) {	
	 
		if (!(PIND & BTN_BIT)) {
			char reset = 0;
			int timer = 0;
			get_str(r);
            char out[127];
            strcpy(out, "  ");
            strcat(out, buffer);
            strcat(out, ", ");
            strcat(out, buffer);
            strcat(out, resetjee);
            r++;
            r = r % 22; // plz no overflow

			while (!(PIND & BTN_BIT)){
				timer++;
				_delay_ms(10);
				
				if (timer > 150){
					
					scroll_text(out, 80);
					time = 0;
					chr1 = '0';
					//this is good. vahennetaan "kirjaimesta" at 32 ja sijoitetann siihen binaaridataa...
					letter['@' - 32][4] = letter['@' - 32][3] = letter['@' - 32][2] = letter['@' - 32][1] = 0x00;  // count4
					reset = 2;
					plus9 = 0;
					chr2 = '0';
				}
			}
		    if (reset == 0) {
		    	scroll_text(jee, 80);
				// funktion parametreina space...
				write_scroll_character(' ', ' ', chr1, 80);
				if (!plus9) {
					write_scroll_character(' ', chr1, '@', 80);
				} else {
					write_scroll_character(' ', chr2, chr1, 80);
				}			
					
			}
			
		} else {

		    if (!plus9) {
		    	// Magic number 893 = 47 * 19  893 = one second-ish. Maybe
				write_character(chr1, '@', 47);
			}
			else {
				write_character(chr2, chr1, 47);
			}
			
			time_div++;
			if (time_div == 19){
				time++;
                turska++;
                
				letter['@' - 32][4] = (time & 0x1F); 		   // count1
				letter['@' - 32][3] = ( (time >> 5)  & 0x1F);  // count2
				letter['@' - 32][2] = ( (time >> 10) & 0x1F);  // count3
				letter['@' - 32][1] = ( (time >> 15) & 0x1F);  // count4
				
				//14400
				if (time % 14400 == 0) {
					magic(chr1, chr2, plus9);
				}
				
				//86400 s = 24 h
				if (time == 86320){
				  //TODO set @ to 0 here? 
					time = 0;
					chr1++; 
					scroll_text(jee, 80);
				}
				if (chr1 > '9'){
					chr1 = '0';
					chr2++;
					if (chr2 > '9') {
						time = 86401;
						chr1 = '=';
						chr2 = 'D';
					}
					plus9 = 1;
					 
				}
				time_div = 0;
			}		
		}
	}
}


/*
				MINUTE COUNTER
				if (count == 0x1F){
					count = 0x00;
					count2++;
				}
				if (count2 == 0x01){
					if (count == 0x1C){
						count = 0;
						count2 = 0;
					}
				}
*/

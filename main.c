
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#define byte unsigned char
#define true 1
#define false 0

#define MISO_PIN_INPUT   DDRB &= ~(1 << DDB1)
#define MISO_PIN_READ   (PINB & (1 << PINB1))

#define MOSI_PIN_OUTPUT   DDRB |= (1 << DDB0)
#define MOSI_PIN_HIGH   PORTB |= (1 << PORTB0)
#define MOSI_PIN_LOW   PORTB &= ~(1 << PORTB0)

#define SCK_PIN_OUTPUT   DDRB |= (1 << DDB2)
#define SCK_PIN_HIGH   PORTB |= (1 << PORTB2)
#define SCK_PIN_LOW   PORTB &= ~(1 << PORTB2)

#define SRAM_CS_PIN_OUTPUT   DDRB |= (1 << DDB3)
#define SRAM_CS_PIN_HIGH   PORTB |= (1 << PORTB3)
#define SRAM_CS_PIN_LOW   PORTB &= ~(1 << PORTB3)

#define EEPROM_CS_PIN_OUTPUT   DDRB |= (1 << DDB4)
#define EEPROM_CS_PIN_HIGH   PORTB |= (1 << PORTB4)
#define EEPROM_CS_PIN_LOW   PORTB &= ~(1 << PORTB4)

#define DISPLAY_CS_PIN_OUTPUT   DDRA |= (1 << DDA2)
#define DISPLAY_CS_PIN_HIGH   PORTA |= (1 << PORTA2)
#define DISPLAY_CS_PIN_LOW   PORTA &= ~(1 << PORTA2)

#define DISPLAY_MODE_PIN_OUTPUT   DDRA |= (1 << DDA1)
#define DISPLAY_MODE_PIN_HIGH   PORTA |= (1 << PORTA1)
#define DISPLAY_MODE_PIN_LOW   PORTA &= ~(1 << PORTA1)

#define DISPLAY_RESET_PIN_OUTPUT   DDRA |= (1 << DDA0)
#define DISPLAY_RESET_PIN_HIGH   PORTA |= (1 << PORTA0)
#define DISPLAY_RESET_PIN_LOW   PORTA &= ~(1 << PORTA0)

#define LEFT_BUTTON_PIN_OUTPUT   DDRA |= (1 << DDA7)
#define LEFT_BUTTON_PIN_INPUT   DDRA &= ~(1 << DDA7)
#define LEFT_BUTTON_PIN_LOW   PORTA &= ~(1 << PORTA7)

#define RIGHT_BUTTON_PIN_OUTPUT   DDRA |= (1 << DDA6)
#define RIGHT_BUTTON_PIN_INPUT   DDRA &= ~(1 << DDA6)
#define RIGHT_BUTTON_PIN_LOW   PORTA &= ~(1 << PORTA6)

#define UP_BUTTON_PIN_OUTPUT   DDRA |= (1 << DDA5)
#define UP_BUTTON_PIN_INPUT   DDRA &= ~(1 << DDA5)
#define UP_BUTTON_PIN_LOW   PORTA &= ~(1 << PORTA5)

#define DOWN_BUTTON_PIN_OUTPUT   DDRB |= (1 << DDB6)
#define DOWN_BUTTON_PIN_INPUT   DDRB &= ~(1 << DDB6)
#define DOWN_BUTTON_PIN_LOW   PORTB &= ~(1 << PORTB6)

#define RETURN_BUTTON_PIN_OUTPUT   DDRB |= (1 << DDB5)
#define RETURN_BUTTON_PIN_INPUT   DDRB &= ~(1 << DDB5)
#define RETURN_BUTTON_PIN_LOW   PORTB &= ~(1 << PORTB5)

#define ESCAPE_BUTTON_PIN_OUTPUT   DDRA |= (1 << DDA4)
#define ESCAPE_BUTTON_PIN_INPUT   DDRA &= ~(1 << DDA4)
#define ESCAPE_BUTTON_PIN_LOW   PORTA &= ~(1 << PORTA4)

#define BUTTON_OUTPUT_PIN_INPUT   DDRA &= ~(1 << DDA3)
#define BUTTON_OUTPUT_PIN_READ   (PINA & (1 << PINA3))

#define SPI_DELAY 5

#define DISPLAY_WIDTH 16

#define LEFT_BUTTON_MASK 0x80
#define RIGHT_BUTTON_MASK 0x40
#define UP_BUTTON_MASK 0x20
#define DOWN_BUTTON_MASK 0x10
#define RETURN_BUTTON_MASK 0x08
#define ESCAPE_BUTTON_MASK 0x04

#define MAXIMUM_FILE_NAME_LENGTH 16
#define FILE_DATA_OFFSET MAXIMUM_FILE_NAME_LENGTH + 1
#define FILE_ENTRY_SIZE 4096
#define NUMBER_OF_FILE_ENTRY_POSITIONS 32
#define EMPTY_FILE_ENTRY_INDICATOR 0xFF
#define FILE_BUFFER_SIZE 100

#define SPECIAL_CHARACTER_LENGTH 6
#define NUMBER_OF_SPECIAL_CHARACTERS 2
#define TOTAL_NUMBER_OF_CHARACTERS NUMBER_OF_SPECIAL_CHARACTERS + sizeof(CHARACTER_SET) - 1
#define SPACE_CHARACTER_INDEX 0
#define DELETE_CHARACTER_INDEX 1
#define EDIT_CURSOR_CHARACTER '_'

#define LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET 0
#define LITERAL_ARGUMENT_ADDRESS_LIST_SIZE 20
#define STACK_OFFSET LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + LITERAL_ARGUMENT_ADDRESS_LIST_SIZE
#define SCOPE_RETURN_ADDRESS_OFFSET 0
#define SCOPE_PREVIOUS_SCOPE_ADDRESS_OFFSET 4
#define SCOPE_SIZE_OFFSET 6
#define SCOPE_VARIABLE_LIST_OFFSET 8
#define NUMBER_OF_SCOPE_VARIABLES 26
#define SCOPE_FLOW_DATA_OFFSET SCOPE_VARIABLE_LIST_OFFSET + NUMBER_OF_SCOPE_VARIABLES * 2

#define HEAP_START_ADDRESS 32000
#define HEAP_ENTRY_SIZE 8
#define HEAP_ENTRY_TYPE_OFFSET 0
#define HEAP_ENTRY_REFERENCE_COUNT_OFFSET 2
#define HEAP_ENTRY_DATA_OFFSET 4
#define HEAP_ENTRY_LINK_OFFSET 6

#define EMPTY_HEAP_ENTRY_TYPE 0
#define INTEGER_HEAP_ENTRY_TYPE 1
#define LIST_HEAP_ENTRY_TYPE 2

#define INTERPRET_FLOW_DATA -1
#define IGNORE_FLOW_DATA -2

const short RANDOM_DATA_LIST_1[] PROGMEM = {26300, 12613, 26904, 8022, 30794, 31703, 25650, 2068, 26336, 26781, 16264, 19980, 15295, 31750, 3123, 32465, 4086, 14700, 31978};
const short RANDOM_DATA_LIST_2[] PROGMEM = {29646, 3873, 6645, 27385, 11518, 9321, 2002, 31546, 5100, 12871, 15150, 10975, 23235, 16316, 10161, 745, 27271, 26236, 7635, 9953, 15108, 30539, 16157, 16197, 20820, 21735, 24581, 14531, 21504, 21949, 27284};

const byte DISPLAY_INITIALIZATION_COMMANDS[] PROGMEM = {0x39, 0x14, 0x55, 0x6D, 0x7F, 0x38, 0x0C, 0x01, 0x06};
const byte CHARACTER_SET[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?!;:'\"()=<>+-*/%\x9C|&";
const byte SPECIAL_CHARACTER_SET[] PROGMEM = "SPACE DELETE";

const byte MESSAGE_1[] PROGMEM = "SAVED";
const byte MESSAGE_2[] PROGMEM = "FILE NAME?";
const byte MESSAGE_3[] PROGMEM = "CREATED";
const byte MESSAGE_4[] PROGMEM = "ARE YOU SURE?";
const byte MESSAGE_5[] PROGMEM = "DELETED";
const byte MESSAGE_6[] PROGMEM = "RENAMED";
const byte MESSAGE_7[] PROGMEM = "WELCOME";
const byte MESSAGE_8[] PROGMEM = "NO FILES";
//const byte MESSAGE_9[] PROGMEM = "GO AWAY!";
const byte SELECTION_ITEM_1[] PROGMEM = "INSERT";
const byte SELECTION_ITEM_2[] PROGMEM = "DELETE";
const byte SELECTION_ITEM_3[] PROGMEM = "EDIT";
const byte SELECTION_ITEM_4[] PROGMEM = "SAVE";
const byte SELECTION_ITEM_5[] PROGMEM = "QUIT";
const byte SELECTION_ITEM_6[] PROGMEM = "CREATE";
const byte SELECTION_ITEM_7[] PROGMEM = "SELECT";
const byte SELECTION_ITEM_8[] PROGMEM = "EDIT";
const byte SELECTION_ITEM_9[] PROGMEM = "RUN";
const byte SELECTION_ITEM_10[] PROGMEM = "DELETE";
const byte SELECTION_ITEM_11[] PROGMEM = "RENAME";
const byte SELECTION_ITEM_12[] PROGMEM = "CANCEL";
const byte SELECTION_ITEM_13[] PROGMEM = "YES DELETE";
const byte * const SELECTION_MENU_1[] PROGMEM = {SELECTION_ITEM_1, SELECTION_ITEM_2, SELECTION_ITEM_3};
const byte * const SELECTION_MENU_2[] PROGMEM = {SELECTION_ITEM_4, SELECTION_ITEM_5};
const byte * const SELECTION_MENU_3[] PROGMEM = {SELECTION_ITEM_6, SELECTION_ITEM_7};
const byte * const SELECTION_MENU_4[] PROGMEM = {SELECTION_ITEM_8, SELECTION_ITEM_9, SELECTION_ITEM_10, SELECTION_ITEM_11};
const byte * const SELECTION_MENU_5[] PROGMEM = {SELECTION_ITEM_12, SELECTION_ITEM_13};

const byte BUILT_IN_FUNCTION_NAME_LIST[] PROGMEM = "= + - * / % == > ! \x9C | & << >> IF END WHL BRK RET RAND STR INT LEN TRUNC GET SET PRINT INPUT ";

short randomNumber = 0;
byte randomNumberState1 = 0;
byte randomNumberState2 = 0;

long commandAddress;
short scopeAddress;
short heapSize = 0;
byte isIgnoringCommands;
short argumentPointerAddressList[10];
byte hasStoppedExecution;

// I wrote this because rand takes up more room.
// The RNG does not need to be extremely robust.
static short generateRandomNumber()
{
	randomNumber <<= 1;
	if (randomNumber & 0x8000)
	{
		randomNumber &= 0x7FFF;
		randomNumber |= 0x0001;
	}
	randomNumber ^= pgm_read_word(RANDOM_DATA_LIST_1 + randomNumberState1);
	randomNumber ^= pgm_read_word(RANDOM_DATA_LIST_2 + randomNumberState2);
	randomNumberState1 += 1;
	randomNumberState2 += 1;
	if (randomNumberState1 >= sizeof(RANDOM_DATA_LIST_1))
	{
		randomNumberState1 = 0;
	}
	if (randomNumberState2 >= sizeof(RANDOM_DATA_LIST_2))
	{
		randomNumberState2 = 0;
	}
	return randomNumber;
}

static short getTextLength(byte *text)
{
	short output = 0;
	while (true)
	{
		byte tempCharacter = *text;
		if (tempCharacter == 0)
		{
			return output;
		}
		text += 1;
		output += 1;
	}
}

static byte equalText(byte *text1, byte *text2)
{
	while (true)
	{
		byte tempCharacter1 = *text1;
		byte tempCharacter2 = *text2;
		if (tempCharacter1 != tempCharacter2)
		{
			return false;
		}
		if (tempCharacter1 == 0)
		{
			return true;
		}
		text1 += 1;
		text2 += 1;
	}
}

static byte receiveSpiByte()
{
	byte output = 0;
	byte tempMask = 0x80;
	byte tempCount = 0;
	while (tempCount < 8)
	{
		SCK_PIN_LOW;
		_delay_us(SPI_DELAY);
		SCK_PIN_HIGH;
		_delay_us(SPI_DELAY);
		if (MISO_PIN_READ)
		{
			output |= tempMask;
		}
		tempMask >>= 1;
		tempCount += 1;
	}
	SCK_PIN_LOW;
	_delay_us(SPI_DELAY);
	return output;
}

static void sendSpiByte(byte value)
{
	byte tempMask = 0x80;
	byte tempCount = 0;
	while (tempCount < 8)
	{
		SCK_PIN_LOW;
		if (value & tempMask)
		{
			MOSI_PIN_HIGH;
		} else {
			MOSI_PIN_LOW;
		}
		_delay_us(SPI_DELAY);
		SCK_PIN_HIGH;
		_delay_us(SPI_DELAY);
		tempMask >>= 1;
		tempCount += 1;
	}
	SCK_PIN_LOW;
	_delay_us(SPI_DELAY);
}

static void sendDisplayCommand(byte command)
{
	DISPLAY_MODE_PIN_LOW;
	DISPLAY_CS_PIN_LOW;
	sendSpiByte(command);
	DISPLAY_CS_PIN_HIGH;
	_delay_ms(2);
}

static void sendDisplayCharacter(byte character)
{
	DISPLAY_MODE_PIN_HIGH;
	DISPLAY_CS_PIN_LOW;
	sendSpiByte(character);
	DISPLAY_CS_PIN_HIGH;
	_delay_us(50);
}

static void clearDisplay()
{
	sendDisplayCommand(0x01);
}

static void setDisplayPos(byte posX, byte posY)
{
	sendDisplayCommand(0x80 | posX | (posY * 0x40));
}

static void clearDisplayRow(byte posY)
{
	setDisplayPos(0, posY);
	byte tempCount = 0;
	while (tempCount < DISPLAY_WIDTH)
	{
		sendDisplayCharacter(' ');
		tempCount += 1;
	}
}

static void readSramData(byte *data, short amount, short address)
{
	SRAM_CS_PIN_LOW;
	sendSpiByte(0x03);
	sendSpiByte((address & 0xFF00) >> 8);
	sendSpiByte(address & 0x00FF);
	short tempCount = 0;
	while (tempCount < amount)
	{
		*data = receiveSpiByte();
		data += 1;
		tempCount += 1;
	}
	SRAM_CS_PIN_HIGH;
	_delay_us(10);
}

static void writeSramData(short address, byte *data, short amount)
{
	SRAM_CS_PIN_LOW;
	sendSpiByte(0x02);
	sendSpiByte((address & 0xFF00) >> 8);
	sendSpiByte(address & 0x00FF);
	short tempCount = 0;
	while (tempCount < amount)
	{
		sendSpiByte(*data);
		data += 1;
		tempCount += 1;
	}
	SRAM_CS_PIN_HIGH;
	_delay_us(10);
}

static byte readSramByte(short address)
{
	byte output;
	readSramData(&output, 1, address);
	return output;
}

static short readSramShort(short address)
{
	short output;
	readSramData((byte *)&output, 2, address);
	return output;
}

static long readSramLong(short address)
{
	long output;
	readSramData((byte *)&output, 4, address);
	return output;
}

static void writeSramByte(short address, byte value)
{
	writeSramData(address, &value, 1);
}

static void writeSramShort(short address, short value)
{
	writeSramData(address, (byte *)&value, 2);
}

static void writeSramLong(short address, long value)
{
	writeSramData(address, (byte *)&value, 4);
}

static void readEepromData(byte *data, short amount, long address)
{
	EEPROM_CS_PIN_LOW;
	sendSpiByte(0x03);
	sendSpiByte((address & 0x00FF0000) >> 16);
	sendSpiByte((address & 0x0000FF00) >> 8);
	sendSpiByte(address & 0x000000FF);
	short tempCount = 0;
	while (tempCount < amount)
	{
		*data = receiveSpiByte();
		data += 1;
		tempCount += 1;
	}
	EEPROM_CS_PIN_HIGH;
	_delay_us(10);
}

// Note: Page write only works within 256 byte boundaries.
static void writeEepromPage(long address, byte *data, short amount)
{
	EEPROM_CS_PIN_LOW;
	sendSpiByte(0x06);
	EEPROM_CS_PIN_HIGH;
	_delay_ms(1);
	EEPROM_CS_PIN_LOW;
	sendSpiByte(0x02);
	sendSpiByte((address & 0x00FF0000) >> 16);
	sendSpiByte((address & 0x0000FF00) >> 8);
	sendSpiByte(address & 0x000000FF);
	short tempCount = 0;
	while (tempCount < amount)
	{
		sendSpiByte(*data);
		data += 1;
		tempCount += 1;
	}
	EEPROM_CS_PIN_HIGH;
	_delay_ms(10);
}

static void writeEepromData(long address, byte *data, short amount)
{
	short tempCount = 0;
	while (tempCount < amount)
	{
		long tempEndAddress = (address + 256) & 0xFFFFFF00;
		short tempAmount = tempEndAddress - address;
		short tempAmount2 = amount - tempCount;
		if (tempAmount > tempAmount2)
		{
			tempAmount = tempAmount2;
		}
		writeEepromPage(address, data, tempAmount);
		address = tempEndAddress;
		data += tempAmount;
		tempCount += tempAmount;
	}
}

static byte readEepromByte(long address)
{
	byte output;
	readEepromData(&output, 1, address);
	return output;
}

static byte readButton(byte mask)
{
	_delay_ms(1);
	if (!BUTTON_OUTPUT_PIN_READ)
	{
		return mask;
	}
	return 0;
}

static byte readButtons()
{
	byte output = 0;
	LEFT_BUTTON_PIN_OUTPUT;
	output |= readButton(LEFT_BUTTON_MASK);
	LEFT_BUTTON_PIN_INPUT;
	RIGHT_BUTTON_PIN_OUTPUT;
	output |= readButton(RIGHT_BUTTON_MASK);
	RIGHT_BUTTON_PIN_INPUT;
	UP_BUTTON_PIN_OUTPUT;
	output |= readButton(UP_BUTTON_MASK);
	UP_BUTTON_PIN_INPUT;
	DOWN_BUTTON_PIN_OUTPUT;
	output |= readButton(DOWN_BUTTON_MASK);
	DOWN_BUTTON_PIN_INPUT;
	RETURN_BUTTON_PIN_OUTPUT;
	output |= readButton(RETURN_BUTTON_MASK);
	RETURN_BUTTON_PIN_INPUT;
	ESCAPE_BUTTON_PIN_OUTPUT;
	output |= readButton(ESCAPE_BUTTON_MASK);
	ESCAPE_BUTTON_PIN_INPUT;
	return output;
}

// Destination should have size at least MAXIMUM_FILE_NAME_LENGTH + 1.
static void getFileName(byte *destination, byte index)
{
	long tempAddress = index * (long)FILE_ENTRY_SIZE;
	readEepromData(destination, MAXIMUM_FILE_NAME_LENGTH + 1, tempAddress);
}

static void displayText(byte *message);
static byte promptButton();

static void displayAvailableMemory() {
	/*
	int size = 512;
	byte *buf;
	
	while ((buf = (byte *) malloc(--size)) == NULL)
	{
		
	}
	
	free(buf);
	 */
	
	extern int __heap_start, *__brkval;
	int v;
	int size = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	
	byte tempText[5];
	itoa(size, (char *)tempText, 10);
	displayText(tempText);
	promptButton();
}

static void __attribute__ ((noinline)) loadFile(byte index)
{
	long tempStartAddress = index * (long)FILE_ENTRY_SIZE + FILE_DATA_OFFSET;
	short tempOffset = 0;
	short tempEndOffset = FILE_ENTRY_SIZE - FILE_DATA_OFFSET;
	while (tempOffset < tempEndOffset)
	{
		short tempAmount = FILE_BUFFER_SIZE;
		if (tempOffset + tempAmount > tempEndOffset)
		{
			tempAmount = tempEndOffset - tempOffset;
		}
		byte tempBuffer[FILE_BUFFER_SIZE];
		readEepromData(tempBuffer, tempAmount, tempStartAddress + tempOffset);
		writeSramData(tempOffset, tempBuffer, tempAmount);
		tempOffset += tempAmount;
	}
}

static void __attribute__ ((noinline)) saveFile(byte index)
{
	long tempStartAddress = index * (long)FILE_ENTRY_SIZE + FILE_DATA_OFFSET;
	short tempOffset = 0;
	short tempEndOffset = FILE_ENTRY_SIZE - FILE_DATA_OFFSET;
	while (tempOffset < tempEndOffset)
	{
		short tempAmount = FILE_BUFFER_SIZE;
		if (tempOffset + tempAmount > tempEndOffset)
		{
			tempAmount = tempEndOffset - tempOffset;
		}
		byte tempBuffer[FILE_BUFFER_SIZE];
		readSramData(tempBuffer, tempAmount, tempOffset);
		writeEepromData(tempStartAddress + tempOffset, tempBuffer, tempAmount);
		tempOffset += tempAmount;
	}
}

static byte promptButton()
{
	byte output = 0;
	while (!output)
	{
		output = readButtons();
	}
	byte tempValue = output;
	while (tempValue)
	{
		tempValue = readButtons();
	}
	return output;
}

static void displayText(byte *text)
{
	clearDisplay();
	setDisplayPos(0, 0);
	byte index = 0;
	while (true)
	{
		byte tempCharacter = text[index];
		if (tempCharacter == 0)
		{
			break;
		}
		if (index == DISPLAY_WIDTH)
		{
			setDisplayPos(0, 1);
		}
		sendDisplayCharacter(tempCharacter);
		index += 1;
	}
}

static void displayProgmemText(const byte *text)
{
	clearDisplay();
	setDisplayPos(0, 0);
	byte index = 0;
	while (true)
	{
		byte tempCharacter = pgm_read_byte(text + index);
		if (tempCharacter == 0)
		{
			break;
		}
		if (index == DISPLAY_WIDTH)
		{
			setDisplayPos(0, 1);
		}
		sendDisplayCharacter(tempCharacter);
		index += 1;
	}
}

// Returns 255 if escape was pressed.
static byte promptProgmemSelection(const byte * const *textList, byte amount)
{
	byte output = 0;
	while (true)
	{
		displayProgmemText(pgm_read_word(textList + output));
		byte tempButtons = promptButton();
		if (tempButtons & (LEFT_BUTTON_MASK | UP_BUTTON_MASK))
		{
			if (output)
			{
				output -= 1;
			} else {
				output = amount - 1;
			}
		}
		if (tempButtons & (RIGHT_BUTTON_MASK | DOWN_BUTTON_MASK))
		{
			output += 1;
			if (output >= amount)
			{
				output = 0;
			}
		}
		if (tempButtons & RETURN_BUTTON_MASK)
		{
			return output;
		}
		if (tempButtons & ESCAPE_BUTTON_MASK)
		{
			return 255;
		}
	}
}

static void editTextLine(byte *text)
{
	// 0 = Navigate.
	// 1 = Edit.
	byte tempEditState = 0;
	short tempCursorIndex = 0;
	short tempCharacterIndex = 0;
	while (true)
	{
		clearDisplay();
		setDisplayPos(0, 0);
		short tempLength = getTextLength(text);
		short tempStartIndex;
		if (tempCursorIndex < DISPLAY_WIDTH / 2)
		{
			tempStartIndex = 0;
		} else
		{
			tempStartIndex = tempCursorIndex - DISPLAY_WIDTH / 2;
		}
		byte hasFoundLastCharacter = false;
		byte tempCount = 0;
		byte tempOffset = 0;
		while (tempOffset < DISPLAY_WIDTH)
		{
			short tempIndex = tempStartIndex + tempOffset;
			if (tempIndex == tempCursorIndex)
			{
				sendDisplayCharacter(EDIT_CURSOR_CHARACTER);
				tempCount += 1;
			}
			if (tempCount < DISPLAY_WIDTH)
			{
				byte tempCharacter = text[tempIndex];
				if (tempCharacter == 0)
				{
					hasFoundLastCharacter = true;
				}
				if (!hasFoundLastCharacter)
				{
					sendDisplayCharacter(tempCharacter);
					tempCount += 1;
				}
			}
			tempOffset += 1;
		}
		if (tempEditState == 0)
		{
			byte tempButtons = promptButton();
			if (tempButtons & LEFT_BUTTON_MASK)
			{
				tempCursorIndex -= 1;
			}
			if (tempButtons & RIGHT_BUTTON_MASK)
			{
				tempCursorIndex += 1;
			}
			if (tempButtons & UP_BUTTON_MASK)
			{
				tempCursorIndex -= 8;
			}
			if (tempButtons & DOWN_BUTTON_MASK)
			{
				tempCursorIndex += 8;
			}
			if (tempCursorIndex < 0)
			{
				tempCursorIndex = 0;
			}
			if (tempCursorIndex > tempLength)
			{
				tempCursorIndex = tempLength;
			}
			if (tempButtons & RETURN_BUTTON_MASK)
			{
				tempEditState = 1;
			}
			if (tempButtons & ESCAPE_BUTTON_MASK)
			{
				return;
			}
		} else if (tempEditState == 1)
		{
			while (true)
			{
				clearDisplayRow(1);
				setDisplayPos(0, 1);
				if (tempCharacterIndex < NUMBER_OF_SPECIAL_CHARACTERS)
				{
					byte tempOffset = 0;
					while (tempOffset < SPECIAL_CHARACTER_LENGTH)
					{
						byte tempCharacter = pgm_read_byte(SPECIAL_CHARACTER_SET + tempCharacterIndex * SPECIAL_CHARACTER_LENGTH + tempOffset);
						sendDisplayCharacter(tempCharacter);
						tempOffset += 1;
					}
				} else {
					byte tempCharacter = pgm_read_byte(CHARACTER_SET + tempCharacterIndex - NUMBER_OF_SPECIAL_CHARACTERS);
					sendDisplayCharacter(tempCharacter);
				}
				byte tempButtons = promptButton();
				if (tempButtons & LEFT_BUTTON_MASK)
				{
					tempCharacterIndex -= 1;
				}
				if (tempButtons & RIGHT_BUTTON_MASK)
				{
					tempCharacterIndex += 1;
				}
				if (tempButtons & UP_BUTTON_MASK)
				{
					tempCharacterIndex -= 8;
				}
				if (tempButtons & DOWN_BUTTON_MASK)
				{
					tempCharacterIndex += 8;
				}
				if (tempCharacterIndex < 0)
				{
					tempCharacterIndex += TOTAL_NUMBER_OF_CHARACTERS;
				}
				if (tempCharacterIndex >= TOTAL_NUMBER_OF_CHARACTERS)
				{
					tempCharacterIndex -= TOTAL_NUMBER_OF_CHARACTERS;
				}
				if (tempButtons & RETURN_BUTTON_MASK)
				{
					if (tempCharacterIndex == DELETE_CHARACTER_INDEX) {
						if (tempCursorIndex > 0)
						{
							short index = tempCursorIndex;
							while (index <= tempLength)
							{
								byte tempCharacter = text[index];
								text[index - 1] = tempCharacter;
								index += 1;
							}
							tempCursorIndex -= 1;
						}
					} else {
						byte tempCharacter;
						if (tempCharacterIndex == SPACE_CHARACTER_INDEX)
						{
							tempCharacter = ' ';
						} else {
							tempCharacter = pgm_read_byte(CHARACTER_SET + tempCharacterIndex - NUMBER_OF_SPECIAL_CHARACTERS);
						}
						short index = tempLength;
						while (index >= tempCursorIndex)
						{
							byte tempCharacter2 = text[index];
							text[index + 1] = tempCharacter2;
							index -= 1;
						}
						text[tempCursorIndex] = tempCharacter;
						tempCursorIndex += 1;
					}
					break;
				}
				if (tempButtons & ESCAPE_BUTTON_MASK)
				{
					tempEditState = 0;
					break;
				}
			}
		}
	}
}

static short findPreviousLine(short lineIndex)
{
	lineIndex -= 2;
	while (lineIndex >= 0)
	{
		byte tempCharacter = readSramByte(lineIndex);
		if (tempCharacter == '\n')
		{
			return lineIndex + 1;
		}
		lineIndex -= 1;
	}
	return 0;
}

static short findNextLine(short lineIndex)
{
	short startLineIndex = lineIndex;
	while (true)
	{
		byte tempCharacter = readSramByte(lineIndex);
		if (tempCharacter == '\n')
		{
			return lineIndex + 1;
		}
		if (tempCharacter == 0)
		{
			return startLineIndex;
		}
		lineIndex += 1;
	}
}

static void moveLines(short startIndex, short endIndex)
{
	short tempOffset = 0;
	if (endIndex > startIndex)
	{
		while (true)
		{
			byte tempCharacter = readSramByte(startIndex + tempOffset);
			if (tempCharacter == 0)
			{
				break;
			}
			tempOffset += 1;
		}
		while (tempOffset >= 0)
		{
			byte tempCharacter = readSramByte(startIndex + tempOffset);
			writeSramByte(endIndex + tempOffset, tempCharacter);
			tempOffset -= 1;
		}
	}
	if (endIndex < startIndex)
	{
		while (true)
		{
			byte tempCharacter = readSramByte(startIndex + tempOffset);
			writeSramByte(endIndex + tempOffset, tempCharacter);
			if (tempCharacter == 0)
			{
				break;
			}
			tempOffset += 1;
		}
	}
}

static void __attribute__ ((noinline)) editLoadedFile(byte fileIndex)
{
	short tempLineIndex = 0;
	while (true)
	{
		clearDisplay();
		setDisplayPos(0, 0);
		byte tempOffset = 0;
		while (tempOffset < DISPLAY_WIDTH * 2)
		{
			byte tempCharacter = readSramByte(tempLineIndex + tempOffset);
			if (tempCharacter == '\n' || tempCharacter == 0)
			{
				break;
			}
			if (tempOffset == DISPLAY_WIDTH)
			{
				setDisplayPos(0, 1);
			}
			sendDisplayCharacter(tempCharacter);
			tempOffset += 1;
		}
		byte tempButtons = promptButton();
		if (tempButtons & LEFT_BUTTON_MASK)
		{
			tempLineIndex = findPreviousLine(tempLineIndex);
		}
		if (tempButtons & RIGHT_BUTTON_MASK)
		{
			tempLineIndex = findNextLine(tempLineIndex);
		}
		if (tempButtons & UP_BUTTON_MASK)
		{
			int tempCount = 0;
			while (tempCount < 8)
			{
				tempLineIndex = findPreviousLine(tempLineIndex);
				tempCount += 1;
			}
		}
		if (tempButtons & DOWN_BUTTON_MASK)
		{
			int tempCount = 0;
			while (tempCount < 8)
			{
				tempLineIndex = findNextLine(tempLineIndex);
				tempCount += 1;
			}
		}
		if (tempButtons & RETURN_BUTTON_MASK)
		{
			byte tempResult = promptProgmemSelection(SELECTION_MENU_1, 3);
			// Insert.
			if (tempResult == 0)
			{
				byte tempBuffer[256];
				tempBuffer[0] = 0;
				editTextLine(tempBuffer);
				byte tempLength = getTextLength(tempBuffer);
				tempBuffer[tempLength] = '\n';
				tempLength += 1;
				moveLines(tempLineIndex, tempLineIndex + tempLength);
				writeSramData(tempLineIndex, tempBuffer, tempLength);
				tempLineIndex = findNextLine(tempLineIndex);
			}
			// Delete.
			if (tempResult == 1)
			{
				short tempNextLineIndex = findNextLine(tempLineIndex);
				moveLines(tempNextLineIndex, tempLineIndex);
				tempLineIndex = findPreviousLine(tempLineIndex);
			}
			// Edit.
			if (tempResult == 2)
			{
				byte tempBuffer[256];
				byte tempOffset = 0;
				byte tempLength = 0;
				while (true)
				{
					byte tempCharacter = readSramByte(tempLineIndex + tempOffset);
					if (tempCharacter == 0)
					{
						break;
					} else {
						tempLength += 1;
					}
					if (tempCharacter == '\n')
					{
						break;
					}
					tempBuffer[tempOffset] = tempCharacter;
					tempOffset += 1;
				}
				tempBuffer[tempOffset] = 0;
				editTextLine(tempBuffer);
				byte tempLength2 = getTextLength(tempBuffer);
				tempBuffer[tempLength2] = '\n';
				tempLength2 += 1;
				moveLines(tempLineIndex + tempLength, tempLineIndex + tempLength2);
				writeSramData(tempLineIndex, tempBuffer, tempLength2);
			}
		}
		if (tempButtons & ESCAPE_BUTTON_MASK)
		{
			byte tempResult = promptProgmemSelection(SELECTION_MENU_2, 2);
			// Save.
			if (tempResult == 0)
			{
				saveFile(fileIndex);
				displayProgmemText(MESSAGE_1);
				promptButton();
			}
			// Quit.
			if (tempResult == 1)
			{
				return;
			}
		}
	}
}

static void editFile(byte fileIndex)
{
	loadFile(fileIndex);
	editLoadedFile(fileIndex);
}

static byte getNumberOfFiles()
{
	byte output = 0;
	long tempAddress = 0;
	byte index = 0;
	while (index < NUMBER_OF_FILE_ENTRY_POSITIONS)
	{
		byte tempValue = readEepromByte(tempAddress);
		if (tempValue != EMPTY_FILE_ENTRY_INDICATOR)
		{
			output += 1;
		}
		tempAddress += FILE_ENTRY_SIZE;
		index += 1;
	}
	return output;
}

static byte getFileIndexByNumber(byte number)
{
	byte output = 0;
	long tempAddress = 0;
	byte index = 0;
	while (index < NUMBER_OF_FILE_ENTRY_POSITIONS)
	{
		byte tempValue = readEepromByte(tempAddress);
		if (tempValue != EMPTY_FILE_ENTRY_INDICATOR)
		{
			if (number < 1)
			{
				return output;
			}
			number -= 1;
		}
		tempAddress += FILE_ENTRY_SIZE;
		output += 1;
		index += 1;
	}
	return 255;
}

static void initializeScopeVariables()
{
	byte index = 0;
	while (index < NUMBER_OF_SCOPE_VARIABLES)
	{
		short tempAddress = scopeAddress + SCOPE_VARIABLE_LIST_OFFSET + index * 2;
		writeSramShort(tempAddress, 0);
		index += 1;
	}
}

// Returns 255 if the function could not be found.
static byte findBuiltInFunction(byte *name)
{
	byte output = 0;
	short index = 0;
	while (index < sizeof(BUILT_IN_FUNCTION_NAME_LIST))
	{
		byte hasFoundDifference = false;
		byte tempOffset = 0;
		while (true)
		{
			byte tempCharacter = pgm_read_byte(BUILT_IN_FUNCTION_NAME_LIST + index);
			index += 1;
			if (!hasFoundDifference)
			{
				byte tempCharacter2 = *(name + tempOffset);
				tempOffset += 1;
				if (tempCharacter == ' ')
				{
					if (tempCharacter2 == 0)
					{
						return output;
					}
					break;
				} else {
					if (tempCharacter != tempCharacter2)
					{
						hasFoundDifference = true;
					}
				}
			} else {
				if (tempCharacter == ' ')
				{
					break;
				}
			}
		}
		output += 1;
	}
	return 255;
}

static void setHeapEntryReference(short referenceAddress, short reference);

static short getHeapEntryType(short address)
{
	return readSramShort(address + HEAP_ENTRY_TYPE_OFFSET);
}

static short getHeapEntryData(short address)
{
	return readSramShort(address + HEAP_ENTRY_DATA_OFFSET);
}

static short getHeapEntryLink(short address)
{
	return readSramShort(address + HEAP_ENTRY_LINK_OFFSET);
}

static void setHeapEntryType(short address, short type)
{
	writeSramShort(address + HEAP_ENTRY_TYPE_OFFSET, type);
}

static void setHeapEntryData(short address, short data)
{
	short tempAddress = address + HEAP_ENTRY_DATA_OFFSET;
	if (getHeapEntryType(address) == LIST_HEAP_ENTRY_TYPE)
	{
		setHeapEntryReference(tempAddress, data);
	} else {
		writeSramShort(tempAddress, data);
	}
}

static void setHeapEntryLink(short address, short link)
{
	setHeapEntryReference(address + HEAP_ENTRY_LINK_OFFSET, link);
}

static void changeHeapEntryReferenceCount(short address, short offset)
{
	short tempCount = readSramShort(address + HEAP_ENTRY_REFERENCE_COUNT_OFFSET);
	tempCount += offset;
	if (tempCount < 1)
	{
		short tempType = getHeapEntryType(address);
		if (tempType == LIST_HEAP_ENTRY_TYPE)
		{
			// All Heap entries in the list chain should have a reference count of 1.
			// We use a loop instead of a recursion to avoid memory overhead.
			while (true)
			{
				short tempNextAddress = getHeapEntryLink(address);
				// Using setHeapEntryLink would cause recursion.
				setHeapEntryData(address, 0);
				setHeapEntryType(address, EMPTY_HEAP_ENTRY_TYPE);
				if (!tempNextAddress)
				{
					break;
				}
				address = tempNextAddress;
			}
		} else {
			setHeapEntryType(address, EMPTY_HEAP_ENTRY_TYPE);
		}
	} else {
		writeSramShort(address + HEAP_ENTRY_REFERENCE_COUNT_OFFSET, tempCount);
	}
}

static short allocateHeapEntry(short type)
{
	short tempOffset = 0;
	byte hasFoundEmptyEntry = false;
	while (tempOffset < heapSize)
	{
		short tempType = readSramShort(HEAP_START_ADDRESS - tempOffset);
		if (tempType == EMPTY_HEAP_ENTRY_TYPE)
		{
			hasFoundEmptyEntry = true;
			break;
		}
		tempOffset += HEAP_ENTRY_SIZE;
	}
	if (!hasFoundEmptyEntry)
	{
		heapSize += HEAP_ENTRY_SIZE;
	}
	short tempAddress = HEAP_START_ADDRESS - tempOffset;
	writeSramShort(tempAddress + HEAP_ENTRY_TYPE_OFFSET, type);
	writeSramShort(tempAddress + HEAP_ENTRY_REFERENCE_COUNT_OFFSET, 0);
	writeSramShort(tempAddress + HEAP_ENTRY_DATA_OFFSET, 0);
	writeSramShort(tempAddress + HEAP_ENTRY_LINK_OFFSET, 0);
	return tempAddress;
}

static short allocateInteger(short value)
{
	short output = allocateHeapEntry(INTEGER_HEAP_ENTRY_TYPE);
	setHeapEntryData(output, value);
	return output;
}

static short allocateList(short pointer)
{
	short output = allocateHeapEntry(LIST_HEAP_ENTRY_TYPE);
	setHeapEntryData(output, pointer);
	return output;
}

static short allocateText(byte *text)
{
	short output = 0;
	short tempPointer = 0;
	while (true)
	{
		byte tempCharacter = *text;
		short tempPointer2 = allocateInteger(tempCharacter);
		short tempPointer3 = allocateList(tempPointer2);
		if (tempPointer == 0)
		{
			output = tempPointer3;
		} else {
			setHeapEntryLink(tempPointer, tempPointer3);
		}
		tempPointer = tempPointer3;
		if (tempCharacter == 0)
		{
			return output;
		}
		text += 1;
	}
}

static void getTextFromHeapEntry(byte *destination, short pointer)
{
	while (true)
	{
		short tempPointer = getHeapEntryData(pointer);
		byte tempCharacter = getHeapEntryData(tempPointer);
		*destination = tempCharacter;
		if (tempCharacter == 0)
		{
			break;
		}
		pointer = getHeapEntryLink(pointer);
		destination += 1;
	}
}

// Use this function for automatic heap maintainence.
static void setHeapEntryReference(short referenceAddress, short reference)
{
	short tempOldReference = readSramShort(referenceAddress);
	if (tempOldReference != EMPTY_HEAP_ENTRY_TYPE)
	{
		changeHeapEntryReferenceCount(tempOldReference, -1);
	}
	writeSramShort(referenceAddress, reference);
	if (reference != EMPTY_HEAP_ENTRY_TYPE)
	{
		changeHeapEntryReferenceCount(reference, 1);
	}
}

static short convertEepromTextToInt(long address, short *tempOffset)
{
	byte tempBuffer[20];
	byte index = 0;
	while (true)
	{
		byte tempCharacter = readEepromByte(address + *tempOffset);
		if ((tempCharacter < '0' || tempCharacter > '9') && tempCharacter != '-')
		{
			tempBuffer[index] = 0;
			break;
		}
		tempBuffer[index] = tempCharacter;
		*tempOffset += 1;
		index += 1;
	}
	return atoi((char *)tempBuffer);
}

// Returns a new commandOffset.
static short parseArgumentTerm(short commandOffset, byte argumentIndex)
{
	byte tempCharacter = readEepromByte(commandAddress + commandOffset);
	if ((tempCharacter >= '0' && tempCharacter <= '9') || tempCharacter == '-')
	{
		short tempNumber = convertEepromTextToInt(commandAddress, &commandOffset);
		short tempPointer = allocateInteger(tempNumber);
		short tempPointerAddress = LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + argumentIndex * 2;
		setHeapEntryReference(tempPointerAddress, tempPointer);
		argumentPointerAddressList[argumentIndex] = tempPointerAddress;
	}
	if (tempCharacter >= 'A' && tempCharacter <= 'Z')
	{
		commandOffset += 1;
		short tempPointerAddress = scopeAddress + SCOPE_VARIABLE_LIST_OFFSET + (tempCharacter - 'A') * 2;
		argumentPointerAddressList[argumentIndex] = tempPointerAddress;
	}
	if (tempCharacter == '(')
	{
		short tempStartPointer = 0;
		short tempPointer = 0;
		commandOffset += 1;
		while (true)
		{
			byte tempCharacter = readEepromByte(commandAddress + commandOffset);
			if (tempCharacter == ')')
			{
				break;
			}
			if (tempCharacter == ' ')
			{
				commandOffset += 1;
			}
			commandOffset = parseArgumentTerm(commandOffset, argumentIndex);
			short tempPointerAddress = argumentPointerAddressList[argumentIndex];
			short tempPointer2 = readSramShort(tempPointerAddress);
			short tempPointer3 = allocateList(tempPointer2);
			if (tempPointer == 0)
			{
				tempStartPointer = tempPointer3;
			} else {
				setHeapEntryLink(tempPointer, tempPointer3);
			}
			tempPointer = tempPointer3;
		}
		if (tempStartPointer == 0)
		{
			tempStartPointer = allocateList(0);
		}
		short tempPointerAddress = LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + argumentIndex * 2;
		setHeapEntryReference(tempPointerAddress, tempStartPointer);
		argumentPointerAddressList[argumentIndex] = tempPointerAddress;
	}
	if (tempCharacter == '"')
	{
		short tempStartPointer = 0;
		short tempPointer = 0;
		commandOffset += 1;
		while (true)
		{
			byte tempCharacter = readEepromByte(commandAddress + commandOffset);
			commandOffset += 1;
			if (tempCharacter == '"')
			{
				tempCharacter = 0;
			}
			short tempPointer2 = allocateInteger(tempCharacter);
			short tempPointer3 = allocateList(tempPointer2);
			if (tempPointer == 0)
			{
				tempStartPointer = tempPointer3;
			} else {
				setHeapEntryLink(tempPointer, tempPointer3);
			}
			tempPointer = tempPointer3;
			if (tempCharacter == 0)
			{
				break;
			}
		}
		short tempPointerAddress = LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + argumentIndex * 2;
		setHeapEntryReference(tempPointerAddress, tempStartPointer);
		argumentPointerAddressList[argumentIndex] = tempPointerAddress;
	}
	return commandOffset;
}

static short getArgumentPointer(byte index)
{
	return readSramShort(argumentPointerAddressList[index]);
}

static short getArgumentInteger(byte index)
{
	return getHeapEntryData(getArgumentPointer(index));
}

static short getFlowDataAddress()
{
	return scopeAddress + readSramShort(scopeAddress + SCOPE_SIZE_OFFSET) - 4;
}

static void changeFlowDataAddress(short offset)
{
	short tempSize = readSramShort(scopeAddress + SCOPE_SIZE_OFFSET);
	tempSize += offset;
	writeSramShort(scopeAddress + SCOPE_SIZE_OFFSET, tempSize);
}

static void __attribute__ ((noinline)) executeNextCommand()
{
	
	long tempNextCommandAddress;
	short tempOffset = 0;
	while (tempOffset < LITERAL_ARGUMENT_ADDRESS_LIST_SIZE)
	{
		short tempAddress = LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + tempOffset;
		writeSramShort(tempAddress, 0);
		tempOffset += 2;
	}
	byte shouldQuitFile = false;
	byte tempCommandName[30];
	tempOffset = 0;
	while (true)
	{
		byte tempCharacter = readEepromByte(commandAddress + tempOffset);
		if (tempCharacter == 0)
		{
			shouldQuitFile = true;
			break;
		}
		if (tempCharacter == ' ' || tempCharacter == '\n')
		{
			break;
		}
		tempCommandName[tempOffset] = tempCharacter;
		tempOffset += 1;
	}
	tempCommandName[tempOffset] = 0;
	byte tempCommand = findBuiltInFunction(tempCommandName);
	if (isIgnoringCommands)
	{
		while (true)
		{
			byte tempCharacter = readEepromByte(commandAddress + tempOffset);
			if (tempCharacter == 0)
			{
				shouldQuitFile = true;
				break;
			}
			if (tempCharacter == ' ' || tempCharacter == '\n')
			{
				break;
			}
			tempCommandName[tempOffset] = tempCharacter;
			tempOffset += 1;
		}
		tempNextCommandAddress = commandAddress + tempOffset + 1;
		if (tempCommand == 14 || tempCommand == 16)
		{
			// IF.
			// WHL.
			changeFlowDataAddress(4);
			short tempAddress = getFlowDataAddress();
			writeSramLong(tempAddress, IGNORE_FLOW_DATA);
		} else if (tempCommand == 15)
		{
			// END.
			short tempAddress = getFlowDataAddress();
			long tempFlowData = readSramLong(tempAddress);
			changeFlowDataAddress(-4);
			if (tempFlowData == -1)
			{
				isIgnoringCommands = false;
			}
		}
	} else {
		byte tempArgumentIndex = 0;
		while (true)
		{
			byte tempCharacter = readEepromByte(commandAddress + tempOffset);
			if (tempCharacter != ' ')
			{
				break;
			}
			tempOffset = parseArgumentTerm(tempOffset + 1, tempArgumentIndex);
			tempArgumentIndex += 1;
		}
		byte tempNumberOfArguments = tempArgumentIndex;
		tempNextCommandAddress = commandAddress + tempOffset + 1;
		short tempValue1;
		short tempValue2;
		if (tempNumberOfArguments > 1)
		{
			tempValue1 = getArgumentInteger(1);
			if (tempNumberOfArguments > 2)
			{
				tempValue2 = getArgumentInteger(2);
			}
		}
		if (tempCommand == 0)
		{
			// =.
			setHeapEntryReference(argumentPointerAddressList[0], getArgumentPointer(1));
		} else if (tempCommand == 1)
		{
			// +.
			short tempResult = tempValue1 + tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 2)
		{
			// -.
			short tempResult = tempValue1 - tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 3)
		{
			// *.
			short tempResult = tempValue1 * tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 4)
		{
			// /.
			short tempResult = tempValue1 / tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 5)
		{
			// %.
			short tempResult = tempValue1 % tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 6)
		{
			// ==.
			short tempResult = tempValue1 == tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 7)
		{
			// >.
			short tempResult = tempValue1 > tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 8)
		{
			// !.
			short tempResult = !tempValue1;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 9)
		{
			// ~.
			short tempResult = ~tempValue1;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 10)
		{
			// |.
			short tempResult = tempValue1 | tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 11)
		{
			// &.
			short tempResult = tempValue1 & tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 12)
		{
			// <<.
			short tempResult = tempValue1 << tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 13)
		{
			// >>.
			short tempResult = tempValue1 >> tempValue2;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 14)
		{
			// IF.
			short tempValue = getArgumentInteger(0);
			if (!tempValue)
			{
				isIgnoringCommands = true;
			}
			changeFlowDataAddress(4);
			short tempAddress = getFlowDataAddress();
			writeSramLong(tempAddress, INTERPRET_FLOW_DATA);
		} else if (tempCommand == 15)
		{
			// END.
			short tempAddress = getFlowDataAddress();
			long tempFlowData = readSramLong(tempAddress);
			changeFlowDataAddress(-4);
			if (tempFlowData >= 0)
			{
				tempNextCommandAddress = tempFlowData;
				shouldQuitFile = false;
			}
		} else if (tempCommand == 16)
		{
			// WHL.
			long tempFlowData;
			short tempValue = getArgumentInteger(0);
			if (tempValue)
			{
				tempFlowData = commandAddress;
			} else {
				tempFlowData = INTERPRET_FLOW_DATA;
				isIgnoringCommands = true;
			}
			changeFlowDataAddress(4);
			short tempAddress = getFlowDataAddress();
			writeSramLong(tempAddress, tempFlowData);
		} else if (tempCommand == 17)
		{
			// BRK.
			isIgnoringCommands = true;
			short tempAddress = getFlowDataAddress();
			while (true)
			{
				long tempFlowData = readSramLong(tempAddress);
				if (tempFlowData == -1)
				{
					writeSramLong(tempAddress, -2);
				}
				if (tempFlowData >= 0)
				{
					writeSramLong(tempAddress, -1);
					break;
				}
				tempAddress -= 4;
			}
		} else if (tempCommand == 18)
		{
			// RET.
			shouldQuitFile = true;
		} else if (tempCommand == 19)
		{
			// RAND.
			short tempValue = getArgumentInteger(1);
			short tempResult = generateRandomNumber() % tempValue;
			short tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 20)
		{
			// STR.
			short tempValue = getArgumentInteger(1);
			byte tempBuffer[20];
			itoa(tempValue, (char *)tempBuffer, 10);
			short tempPointer = allocateText(tempBuffer);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 21)
		{
			// INT.
			byte tempBuffer[40];
			short tempPointer = getArgumentPointer(1);
			getTextFromHeapEntry(tempBuffer, tempPointer);
			short tempResult = atoi((char *)tempBuffer);
			tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 22)
		{
			// LEN.
			short tempResult = 0;
			short tempPointer = getArgumentPointer(1);
			while (true)
			{
				short tempPointer2 = getHeapEntryData(tempPointer);
				if (tempPointer2 == 0)
				{
					break;
				}
				tempResult += 1;
				tempPointer = getHeapEntryLink(tempPointer);
				if (tempPointer == 0)
				{
					break;
				}
			}
			tempPointer = allocateInteger(tempResult);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 23)
		{
			// TRUNC.
			short tempPointer = getArgumentPointer(0);
			short tempTargetLength = getArgumentInteger(1);
			short tempIndex = 0;
			while (tempIndex < tempTargetLength - 1)
			{
				short tempPointer2 = getHeapEntryData(tempPointer);
				if (tempPointer2 == 0)
				{
					break;
				}
				tempPointer2 = getHeapEntryLink(tempPointer);
				if (tempPointer2 == 0)
				{
					break;
				}
				tempPointer = tempPointer2;
				tempIndex += 1;
			}
			if (tempTargetLength == 0)
			{
				setHeapEntryData(tempPointer, 0);
			}
			setHeapEntryLink(tempPointer, 0);
		} else if (tempCommand == 24)
		{
			// GET.
			short tempPointer = getArgumentPointer(1);
			short tempEndIndex = getArgumentInteger(2);
			short tempIndex = 0;
			while (tempIndex < tempEndIndex)
			{
				short tempPointer2 = getHeapEntryLink(tempPointer);
				if (tempPointer2 == 0)
				{
					break;
				}
				tempPointer = tempPointer2;
				tempIndex += 1;
			}
			short tempPointer2 = getHeapEntryData(tempPointer);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer2);
		} else if (tempCommand == 25)
		{
			// SET.
			short tempPointer = getArgumentPointer(0);
			short tempEndIndex = getArgumentInteger(1);
			short tempIndex = 0;
			while (tempIndex < tempEndIndex)
			{
				short tempPointer2 = getHeapEntryLink(tempPointer);
				if (tempPointer2 == 0)
				{
					break;
				}
				tempPointer = tempPointer2;
				tempIndex += 1;
			}
			while (tempIndex < tempEndIndex)
			{
				short tempPointer2 = allocateInteger(0);
				short tempPointer3 = allocateList(tempPointer2);
				setHeapEntryLink(tempPointer, tempPointer3);
				tempPointer = tempPointer3;
				tempIndex += 1;
			}
			short tempPointer4 = getArgumentPointer(2);
			setHeapEntryData(tempPointer, tempPointer4);
		} else if (tempCommand == 26)
		{
			// PRINT.
			byte tempBuffer[50];
			short tempPointer = getArgumentPointer(0);
			getTextFromHeapEntry(tempBuffer, tempPointer);
			displayText(tempBuffer);
			byte tempButtons = promptButton();
			if (tempButtons & ESCAPE_BUTTON_MASK)
			{
				hasStoppedExecution = true;
			}
		} else if (tempCommand == 27)
		{
			// INPUT.
			byte tempBuffer[100];
			tempBuffer[0] = 0;
			editTextLine(tempBuffer);
			short tempPointer = allocateText(tempBuffer);
			setHeapEntryReference(argumentPointerAddressList[0], tempPointer);
		} else if (tempCommand == 255)
		{
			// Custom function.
			byte tempFileIndex = 0;
			while (tempFileIndex < NUMBER_OF_FILE_ENTRY_POSITIONS)
			{
				byte tempBuffer[MAXIMUM_FILE_NAME_LENGTH + 1];
				getFileName(tempBuffer, tempFileIndex);
				if (tempBuffer[0] != EMPTY_FILE_ENTRY_INDICATOR)
				{
					if (equalText(tempBuffer, tempCommandName))
					{
						short tempNextScopeAddress = scopeAddress + readSramShort(scopeAddress + SCOPE_SIZE_OFFSET);
						writeSramLong(tempNextScopeAddress + SCOPE_RETURN_ADDRESS_OFFSET, tempNextCommandAddress);
						writeSramShort(tempNextScopeAddress + SCOPE_PREVIOUS_SCOPE_ADDRESS_OFFSET, scopeAddress);
						writeSramShort(tempNextScopeAddress + SCOPE_SIZE_OFFSET, SCOPE_FLOW_DATA_OFFSET);
						scopeAddress = tempNextScopeAddress;
						initializeScopeVariables();
						byte tempIndex = 0;
						while (tempIndex < tempNumberOfArguments)
						{
							short tempPointer = getArgumentPointer(tempIndex);
							setHeapEntryReference(tempNextScopeAddress + SCOPE_VARIABLE_LIST_OFFSET + tempIndex * 2, tempPointer);
							tempIndex += 1;
						}
						tempNextCommandAddress = tempFileIndex * (long)FILE_ENTRY_SIZE + FILE_DATA_OFFSET;
						break;
					}
				}
				tempFileIndex += 1;
			}
		}
	}
	if (shouldQuitFile)
	{
		if (scopeAddress <= STACK_OFFSET)
		{
			hasStoppedExecution = true;
		} else {
			byte tempIndex = 0;
			while (tempIndex < NUMBER_OF_SCOPE_VARIABLES)
			{
				setHeapEntryReference(scopeAddress + SCOPE_VARIABLE_LIST_OFFSET + tempIndex * 2, 0);
				tempIndex += 1;
			}
			tempNextCommandAddress = readSramLong(scopeAddress + SCOPE_RETURN_ADDRESS_OFFSET);
			scopeAddress = readSramShort(scopeAddress + SCOPE_PREVIOUS_SCOPE_ADDRESS_OFFSET);
		}
	}
	commandAddress = tempNextCommandAddress;
	tempOffset = 0;
	while (tempOffset < LITERAL_ARGUMENT_ADDRESS_LIST_SIZE)
	{
		short tempAddress = LITERAL_ARGUMENT_ADDRESS_LIST_OFFSET + tempOffset;
		setHeapEntryReference(tempAddress, 0);
		tempOffset += 2;
	}
}

static void __attribute__ ((noinline)) displayFileMenu(byte fileIndex)
{
	long tempFileAddress = fileIndex * (long)FILE_ENTRY_SIZE;
	while (true)
	{
		byte tempResult = promptProgmemSelection(SELECTION_MENU_4, 4);
		// Edit.
		if (tempResult == 0)
		{
			editFile(fileIndex);
		}
		// Run.
		if (tempResult == 1)
		{
			scopeAddress = STACK_OFFSET;
			writeSramShort(scopeAddress + SCOPE_SIZE_OFFSET, SCOPE_FLOW_DATA_OFFSET);
			initializeScopeVariables();
			heapSize = 0;
			isIgnoringCommands = false;
			commandAddress = tempFileAddress + FILE_DATA_OFFSET;
			hasStoppedExecution = false;
			while (!hasStoppedExecution)
			{
				executeNextCommand();
				byte tempButtons = readButtons();
				if (tempButtons & ESCAPE_BUTTON_MASK)
				{
					while (readButtons())
					{
						
					}
					break;
				}
			}
		}
		// Delete.
		if (tempResult == 2)
		{
			displayProgmemText(MESSAGE_4);
			promptButton();
			byte tempResult = promptProgmemSelection(SELECTION_MENU_5, 2);
			// Yes delete.
			if (tempResult == 1)
			{
				byte tempValue = EMPTY_FILE_ENTRY_INDICATOR;
				writeEepromData(tempFileAddress, &tempValue, 1);
				displayProgmemText(MESSAGE_5);
				promptButton();
				return;
			}
		}
		// Rename.
		if (tempResult == 3)
		{
			displayProgmemText(MESSAGE_2);
			if (promptButton() & ESCAPE_BUTTON_MASK)
			{
				return;
			}
			byte tempBuffer[50];
			tempBuffer[0] = 0;
			editTextLine(tempBuffer);
			writeEepromData(tempFileAddress, tempBuffer, MAXIMUM_FILE_NAME_LENGTH + 1);
			displayProgmemText(MESSAGE_6);
			promptButton();
		}
		if (tempResult == 255)
		{
			return;
		}
	}
}

static void __attribute__ ((noinline)) navigateFiles()
{
	short tempFileNumber = 0;
	while (true)
	{
		byte tempNumberOfFiles = getNumberOfFiles();
		if (tempNumberOfFiles < 1)
		{
			// No files.
			displayProgmemText(MESSAGE_8);
			promptButton();
			return;
		}
		byte tempFileIndex = getFileIndexByNumber(tempFileNumber);
		{
			byte tempBuffer[MAXIMUM_FILE_NAME_LENGTH + 1];
			getFileName(tempBuffer, tempFileIndex);
			displayText(tempBuffer);
		}
		byte tempButtons = promptButton();
		if (tempButtons & LEFT_BUTTON_MASK)
		{
			tempFileNumber -= 1;
		}
		if (tempButtons & RIGHT_BUTTON_MASK)
		{
			tempFileNumber += 1;
		}
		if (tempButtons & UP_BUTTON_MASK)
		{
			tempFileNumber -= 8;
		}
		if (tempButtons & DOWN_BUTTON_MASK)
		{
			tempFileNumber += 8;
		}
		if (tempFileNumber < 0)
		{
			tempFileNumber = 0;
		}
		if (tempFileNumber >= tempNumberOfFiles)
		{
			tempFileNumber = tempNumberOfFiles - 1;
		}
		if (tempButtons & RETURN_BUTTON_MASK)
		{
			displayFileMenu(tempFileIndex);
			tempFileNumber = 0;
		}
		if (tempButtons & ESCAPE_BUTTON_MASK)
		{
			return;
		}
	}
}

static void __attribute__ ((noinline)) promptCreateFile()
{
	displayProgmemText(MESSAGE_2);
	if (promptButton() & ESCAPE_BUTTON_MASK)
	{
		return;
	}
	byte tempBuffer[50];
	tempBuffer[0] = 0;
	editTextLine(tempBuffer);
	tempBuffer[FILE_DATA_OFFSET] = 0;
	long tempFileAddress = 0;
	while (true)
	{
		byte tempValue = readEepromByte(tempFileAddress);
		if (tempValue == EMPTY_FILE_ENTRY_INDICATOR)
		{
			break;
		}
		tempFileAddress += FILE_ENTRY_SIZE;
	}
	writeEepromData(tempFileAddress, tempBuffer, FILE_DATA_OFFSET + 1);
	displayProgmemText(MESSAGE_3);
	promptButton();
}

static void displayPrimaryMenu()
{
	while (true)
	{
		byte tempResult = promptProgmemSelection(SELECTION_MENU_3, 2);
		// Create.
		if (tempResult == 0)
		{
			promptCreateFile();
		}
		// Select.
		if (tempResult == 1)
		{
			navigateFiles();
		}
		// Easter egg?
		/*
		if (tempResult == 255)
		{
			displayProgmemText(MESSAGE_9);
			promptButton();
		}
		 */
	}
}

int main(void)
{
	SCK_PIN_LOW;
	SRAM_CS_PIN_HIGH;
	EEPROM_CS_PIN_HIGH;
	DISPLAY_CS_PIN_HIGH;
	DISPLAY_RESET_PIN_HIGH;
	LEFT_BUTTON_PIN_LOW;
	RIGHT_BUTTON_PIN_LOW;
	UP_BUTTON_PIN_LOW;
	DOWN_BUTTON_PIN_LOW;
	RETURN_BUTTON_PIN_LOW;
	ESCAPE_BUTTON_PIN_LOW;
	
	MOSI_PIN_OUTPUT;
	SCK_PIN_OUTPUT;
	SRAM_CS_PIN_OUTPUT;
	EEPROM_CS_PIN_OUTPUT;
	DISPLAY_CS_PIN_OUTPUT;
	DISPLAY_MODE_PIN_OUTPUT;
	DISPLAY_RESET_PIN_OUTPUT;
	// All pins are inputs by default.
	/*
	MISO_PIN_INPUT;
	LEFT_BUTTON_PIN_INPUT;
	RIGHT_BUTTON_PIN_INPUT;
	UP_BUTTON_PIN_INPUT;
	DOWN_BUTTON_PIN_INPUT;
	RETURN_BUTTON_PIN_INPUT;
	ESCAPE_BUTTON_PIN_INPUT;
	BUTTON_OUTPUT_PIN_INPUT;
	 */
	
	DISPLAY_RESET_PIN_LOW;
	_delay_us(50);
	DISPLAY_RESET_PIN_HIGH;
	_delay_ms(100);

	byte index = 0;
	while (index < sizeof(DISPLAY_INITIALIZATION_COMMANDS))
	{
		byte tempCommand = pgm_read_byte(DISPLAY_INITIALIZATION_COMMANDS + index);
		sendDisplayCommand(tempCommand);
		index += 1;
	}
	
	// Enter SRAM sequential mode.
	SRAM_CS_PIN_LOW;
	sendSpiByte(0x01);
	sendSpiByte(0x41);
	SRAM_CS_PIN_HIGH;
	
	// Disable EEPROM write protection.
	EEPROM_CS_PIN_LOW;
	sendSpiByte(0x01);
	sendSpiByte(0x00);
	EEPROM_CS_PIN_HIGH;
	
	_delay_ms(1);
	
	displayProgmemText(MESSAGE_7);
	while (!(readButtons()))
	{
		randomNumber += 1;
	}
	
	while (readButtons())
	{
		_delay_us(50);
	}
	
	displayPrimaryMenu();
	
	return 0;
}


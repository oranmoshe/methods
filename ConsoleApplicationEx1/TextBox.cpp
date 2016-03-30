#include <windows.h>
#include <stdio.h>
#include <string>

void PrintValue();
VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
void Print(int x, int y, char ch, HANDLE h);
void PrintLabel();
void PrintTextBox();
void Handler();

HANDLE hStdin;
DWORD fdwSaveOldMode;
DWORD cNumRead, fdwMode, i;
INPUT_RECORD irInBuf[128];
char value[100];
int valLength = 0;

int main(VOID)
{
	PrintLabel();
	PrintTextBox();
	Handler();
	return 0;
}

void Handler(){
	//set cursor position example
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get the standard input handle. 

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	// Save the current input mode, to be restored on exit. 

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	// Loop to read and handle the next 100 input events. 

	while (true)
	{
		// Wait for the events. 
		if (true){

			if (!ReadConsoleInput(
				hStdin,      // input buffer handle 
				irInBuf,     // buffer to read into 
				128,         // size of read buffer 
				&cNumRead)) // number of records read 
				ErrorExit("ReadConsoleInput");

			// Dispatch the events to the appropriate handler. 

			for (i = 0; i < cNumRead; i++)
			{
				switch (irInBuf[i].EventType)
				{
				case KEY_EVENT: // keyboard input 
					KeyEventProc(irInBuf[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT: // mouse input 
					MouseEventProc(irInBuf[i].Event.MouseEvent);
					break;

				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
					ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT:  // disregard focus events 

				case MENU_EVENT:   // disregard menu events 
					break;

				default:
					ErrorExit("Unknown event type");
					break;
				}
			}

		}

	}

	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}
VOID KeyEventProc(KEY_EVENT_RECORD ker)
{

	if (ker.bKeyDown){
		// backspace
		if (ker.wVirtualKeyCode == 8 && valLength >0){
			valLength--;
			value[valLength] = '\0';
			// get positions
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			_CONSOLE_SCREEN_BUFFER_INFO  p = {};
			GetConsoleScreenBufferInfo(h, &p);
			COORD c = (p.dwCursorPosition);
			// set new position
			printf(" ");
			c = { c.X-1, c.Y };
			SetConsoleCursorPosition(h, c);
			printf(" ");
			c = { c.X , c.Y };
			SetConsoleCursorPosition(h, c);
		}
		else {
			// if enter print input
			if (ker.wVirtualKeyCode == 13){
				value[valLength] = '\0';
				PrintValue();
				value[0] = '\0';
				valLength = 0;
				PrintTextBox();
			}
			// add char
			if (ker.wVirtualKeyCode >= 32 && ker.wVirtualKeyCode <= 126 && valLength < 37){
				value[valLength++] = ker.wVirtualKeyCode;
				printf("%c", ker.wVirtualKeyCode);
			}
		}
	}
}
VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	//printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			//printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			//printf("right button press \n");
		}
		else
		{
			//printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		//printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		//printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		break;
	case MOUSE_WHEELED:
		//printf("vertical mouse wheel\n");
		break;
	default:
		//printf("unknown\n");
		break;
	}
}
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	//printf("Resize event\n");
	//printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}
void Print(int x, int y, char ch, HANDLE h){
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
	printf("%c", ch);
}
void PrintValue(){
	PrintLabel();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 8, 13 };
	SetConsoleCursorPosition(h, c);
	printf("%s", value);
	c = { 8, 10 };
	SetConsoleCursorPosition(h, c);
}
void PrintLabel(){
	int x1 = 7, x2 = 47, y = 13;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x1, y };
	char ch;
	for (int i = x1 + 1; i < x2; i++){
		c = { i, y + 1 };
		SetConsoleCursorPosition(h, c);
		ch = '_';
		printf("%c", ch);
	}
	x1 = 7, x2 = 47, y = 12;	
	for (int i = x1 + 1; i < x2; i++){
		c = { i, y + 1 };
		SetConsoleCursorPosition(h, c);
		ch = ' ';
		printf("%c", ch);
	}
}
void PrintTextBox(){
	int x1 = 7, x2 = 47, y = 9;
	// Print top line
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x1, y };
	SetConsoleCursorPosition(h, c);
	char ch = '\xDA';
	printf("%c", ch);
	for (int i = x1+1; i < x2; i++){
		c = { i, y };
		SetConsoleCursorPosition(h, c);
		ch = '\xC4';
		printf("%c", ch);
	}
	c = { x2, y };
	SetConsoleCursorPosition(h, c);
	ch = '\xBF';
	printf("%c", ch);

	// Print center
	c = { x1, y+1 };
	SetConsoleCursorPosition(h, c);
	ch = '\xB3';
	printf("%c", ch);
	for (int i = x1 + 1; i < x2; i++){
		c = { i, y+1 };
		SetConsoleCursorPosition(h, c);
		ch = ' ';
		printf("%c", ch);
	}
	ch = '\xB3';
	c = { x2, y+1 };
	SetConsoleCursorPosition(h, c);
	printf("%c", ch);
	// Print bottom line
	c = { x1, y+2 };
	SetConsoleCursorPosition(h, c);
	ch = '\xC0';
	printf("%c", ch);
	for (int i = x1 + 1; i < x2; i++){
		c = { i, y + 2 };
		SetConsoleCursorPosition(h, c);
		ch = '\xC4';
		printf("%c", ch);
	}
	c = { x2, y + 2 };
	SetConsoleCursorPosition(h, c);
	ch = '\xD9';
	printf("%c", ch);
	c = { 8, 10 };
	SetConsoleCursorPosition(h, c);
}
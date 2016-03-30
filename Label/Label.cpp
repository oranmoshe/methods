#include "Label.h"
// Constractor takes x & y position and text value.
LABEL::LABEL(int x, int y, string value) :COMPONNENT(x, y, length){
	this->value = value;
	this->length = value.length();
	this->position.X = x + 1;
	this->position.Y = y + 1;
}
//Set colors of the componnent
void LABEL::SetColors(){
	//set cursor size and visibility example
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { 10, TRUE };
	SetConsoleCursorInfo(h, &cci);
	//set foreground color
	DWORD wAttr = 0;
	SetConsoleTextAttribute(h, wAttr);
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(h, &cbi);
	DWORD wAttr2 = cbi.wAttributes | FOREGROUND_GREEN;
	SetConsoleTextAttribute(h, wAttr2);
}
// Draws the componnent
void LABEL::Draw(){
	SetColors();
	SetCursor(this->position.X, this->position.Y);	
	cout << value;
}
// Sets the cursor on given position
void LABEL::SetCursor(int x, int y){
	COORD c = { x, y };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, c);
}
// Checks if componnent clicked.
bool LABEL::IsClicked(COORD cursor){
	return false;
}
// Handle and events
void LABEL::Handle(INPUT_RECORD *inpt, int i){
}
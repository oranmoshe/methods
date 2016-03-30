#include "TextBox.h"
// Constractor takes x & y position and length of componnent.
TEXTBOX::TEXTBOX(int x, int y, int length) :COMPONNENT(x, y, length){
	this->valPtr = 0;
	this->position.X = x + 1;
	this->position.Y = y + 1;
	this->cursor.X = x + 1;
	this->cursor.Y = y + 1;
	this->length = length;
	this->focus = false;	
}
//Set colors of the componnent
void TEXTBOX::SetColors(){
	//set cursor size and visibility example
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { 10, TRUE };
	SetConsoleCursorInfo(h, &cci);
	//set foreground color
	DWORD wAttr = FOREGROUND_BLUE ;
	SetConsoleTextAttribute(h, wAttr);
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(h, &cbi);
	DWORD wAttr2 = cbi.wAttributes | BACKGROUND_RED | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(h, wAttr2);
}
// Draws the componnent
void TEXTBOX::Draw(){
	SetColors();
	int x = this->position.X-1;
	int y = this->position.Y-1;
	SetCursor(x, y);
	printf("%c", '\xDA');
	for (int i = x + 1; i < x + length + 1; i++){
		SetCursor(i, y);
		printf("%c", '\xC4');
	}
	SetCursor((x + length + 1), y);
	printf("%c", '\xBF');
	//
	SetCursor(x, y + 1);
	printf("%c", '\xB3');
	for (int i = x + 1; i < x + length + 1; i++){
		SetCursor(i, y + 1);
		printf("%c", ' ');
	}
	SetCursor((x + length + 1), y + 1);
	printf("%c", '\xB3');
	//
	SetCursor(x, y + 2);
	printf("%c", '\xC0');
	for (int i = x + 1; i < x + length + 1; i++){
		SetCursor(i, y + 2);
		printf("%c", '\xC4');
	}
	SetCursor((x + length + 1), y + 2);
	printf("%c", '\xD9');
	SetCursor(x + 1, y + 1);
}
// Prints the given char on the cursor position.
void TEXTBOX::PrintChar(char ch){
	SetCursor(cursor.X, cursor.Y); 
	printf("%c", ch);
	this->cursor.X++;
}
// Prints the given char on the available place or returns error
bool TEXTBOX::PutChar(char ch){
	if (value.length() < this->length) 
	{
		if (cursor.X == this->position.X + value.length()){
			PrintChar(ch);
			value += ch;
			return true;
		}
		else{
			int tmpX = cursor.X;
			int index = cursor.X - this->position.X - 1;
			string str(1, ch);
			value.insert(index + 1, std::string(str));
			SetCursor(cursor.X + index + 1, cursor.Y);
			for (int i = index + 1; i < value.length(); i++){
				PrintChar(value[i]);
			}
			cursor.X = tmpX + 1;
			SetCursor(cursor.X, this->position.Y);
			return true;
		}
	}
	return false;
}
// Moves the cursor one step left
void TEXTBOX::MoveLeft(){
	if (cursor.X > this->position.X)
		SetCursor(--cursor.X, cursor.Y);

}
// Moves the cursor one step right
void TEXTBOX::MoveRight(){
	if (cursor.X < value.length() + this->position.X)
		SetCursor(++cursor.X, cursor.Y);
}
// Checks if componnent clicked.
bool TEXTBOX::IsClicked(COORD cursor){
	if ((cursor.X - this->position.X) <= this->length && ((cursor.Y + 1 == this->position.Y) || (cursor.Y - 1 == this->position.Y) || (cursor.Y == this->position.Y))){
		SetColors();
		return true;
	}
	return false;
}
// Takes the cursor to the place where the input is edit on the componnent.
void TEXTBOX::FocusOnText(COORD cursor){
	this->cursor.Y = this->position.Y;
	if ((cursor.X - this->position.X) <= this->value.length()){
		this->cursor.X = cursor.X;
		SetCursor(this->cursor.X, this->cursor.Y);
	}
	else{
		this->cursor.X = this->position.X + value.length();
		SetCursor(this->cursor.X, this->cursor.Y);
	}
}
// Deletes the char left to the cursor position.
void TEXTBOX::BackSpace(){
	if (value.length()>0){
		if (cursor.X - this->position.X > 0){
			cursor.X = cursor.X--;
			SetCursor(cursor.X, cursor.Y);
			value.erase((cursor.X - this->position.X), 1);
			Clear();
			SetCursor(this->position.X, this->position.Y);
			printf("%s", value.c_str());
			SetCursor(cursor.X, this->position.Y);
		}
	}
}
// Deletes the char on the cursor position.
void TEXTBOX::Delete(){
	if (value.length() > 0){
		++this->cursor.X;
		BackSpace();
	}
}
// Clear the textbox
void TEXTBOX::Clear(){
	for (int i = this->position.X; i < this->position.X + length; i++){
		SetCursor(i, this->position.Y);
		printf("%c", ' ');
	}
}
// Handle and events
void TEXTBOX::Handle(INPUT_RECORD *inpt, int i){
	switch (inpt[i].EventType)
	{
	case KEY_EVENT: // keyboard input 
	{
						KEY_EVENT_RECORD ker = inpt[i].Event.KeyEvent;
						if (ker.bKeyDown && this->focus){
							switch (ker.wVirtualKeyCode){
							case VK_BACK:
								BackSpace();
								break;
							case VK_RIGHT:
							case VK_NUMPAD6:
								MoveRight();
								break;
							case VK_LEFT:
							case VK_NUMPAD4:
								MoveLeft();
								break;
							case VK_RETURN:
								break;
							case VK_DELETE:
								Delete();
								break;
							default:
								if ((ker.wVirtualKeyCode >= 65 && ker.wVirtualKeyCode <= 90) || (ker.wVirtualKeyCode >= 97 && ker.wVirtualKeyCode <= 122) || (ker.wVirtualKeyCode >= 48 && ker.wVirtualKeyCode <= 57) || (ker.wVirtualKeyCode == 32))
									PutChar(ker.wVirtualKeyCode);
								break;
							}
						}
	}
		break;

	case MOUSE_EVENT: // mouse input 
	{
						  MOUSE_EVENT_RECORD ker = inpt[i].Event.MouseEvent;
						  if (ker.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
							  if (IsClicked(ker.dwMousePosition)){
								  this->focus = true;
								  FocusOnText(ker.dwMousePosition);
							  }
							  else{
								  this->focus = false;
							  }
						  }
	}
		break;

	case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 	
		break;

	case FOCUS_EVENT:  // disregard focus events 

	case MENU_EVENT:   // disregard menu events 
		break;

	default:
		break;
	}
}
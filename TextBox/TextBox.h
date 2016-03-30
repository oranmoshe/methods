//TextBox.h

#ifndef __TEXTBOX_H_INCLUDED__   // if x.h hasn't been included yet...
#define __TEXTBOX_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include "Component.h"

class TEXTBOX :COMPONNENT {
	std::string value = "\0";
	int valPtr;
	bool focus;
	COORD cursor;
	COORD position;
public:
	TEXTBOX(int x, int y, int length); 
	void Draw();
	void SetColors();
	void PrintChar(char ch);
	bool PutChar(char ch);
	void BackSpace();
	void Delete();
	void Clear();
	void MoveLeft();
	void MoveRight();
	bool IsClicked(COORD cursor);
	void FocusOnText(COORD cursor);
	void Handle(INPUT_RECORD *inpt, int i);
};

#endif 
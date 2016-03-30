//ComboBox.h

#ifndef __COMBOBOX_H_INCLUDED__   // if x.h hasn't been included yet...
#define __COMBOBOX_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include "Component.h"

class COMBOBOX :COMPONNENT {
	std::string value = "\0";
	bool focus;
	vector<string> options;
	COORD cursor;
	COORD position;
public:
	COMBOBOX(int x, int y, vector<string>);
	void Draw();
	void Handle(INPUT_RECORD *inpt, int i);
	void SetColors();
	void SetCursor(int x, int y);
	bool IsClicked(COORD cursor);
	
	void PrintChar(char ch);
	void PutChar(char ch);
	void MoveLeft();
	void MoveRight();
	void FocusOnText(COORD cursor);
	void BackSpace();
	void Delete();
	void Clear();

};

#endif 
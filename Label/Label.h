//Label.h

#ifndef __LABEL_H_INCLUDED__   // if x.h hasn't been included yet...
#define __LABEL_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include "Component.h"

class LABEL :COMPONNENT {
	std::string value = "\0";	
	COORD position;	
public:
	LABEL(int x, int y, string value);
	void Draw();
	void SetColors();
	void SetCursor(int x, int y);
	virtual void Handle(INPUT_RECORD *inpt, int i);
	virtual bool IsClicked(COORD cursor);
};

#endif 
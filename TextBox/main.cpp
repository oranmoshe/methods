#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "Form.h"
using namespace std;

int main(void) {
	
	// Creation of form
	FORM * form = new FORM();

	// Creation of textboxes
	LABEL *lb1 = new LABEL(0, 0, "First Name:");
	TEXTBOX *tb2 = new TEXTBOX(0, 2, 20);
	LABEL *lb2 = new LABEL(0, 6, "Last Name:");
	TEXTBOX *tb1 = new TEXTBOX(0, 8, 15);

	// Relates componnents the form 
	form->Add((COMPONNENT*)lb1);
	form->Add((COMPONNENT*)lb2);
	form->Add((COMPONNENT*)tb1);
	form->Add((COMPONNENT*)tb2);
	// Running
	form->Run();

}


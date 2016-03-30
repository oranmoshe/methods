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
	LABEL *lb2 = new LABEL(0, 5, "Last Name:");

	// Relates componnents the form 
	form->Add((COMPONNENT*)lb1);
	form->Add((COMPONNENT*)lb2);

	// Running
	form->Run();

}


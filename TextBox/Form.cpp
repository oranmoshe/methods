#include "Form.h"
// Add componnent to the form
void FORM::Add(COMPONNENT* comp){
	comps.push_back(comp);
}
// Draw and setup the componnents
void FORM::Run(){
	// Prints Componnents.
	for (COMPONNENT *n : comps) {
		n->Draw();
	}
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
	// Loop to read and handle the next input events. 
	while (true)
	{		
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
				// Iterate and print values of vector
				for (COMPONNENT *n : comps) {
					n->Handle(irInBuf, i);
				}
				break;

			case MOUSE_EVENT: // mouse input 				
				for (COMPONNENT*n : comps) {
					n->Handle(irInBuf, i);
				}
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
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

	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
// On error 
VOID FORM::ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
	ExitProcess(0);
}

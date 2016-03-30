#include "RadioList.h"
HANDLE hStdin;
DWORD fdwSaveOldMode;
string copyString(COORD C , int len);
VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD, COORD, vector<string> myvector, int *checked, int max);
VOID MouseEventProc(MOUSE_EVENT_RECORD mer, COORD origin, vector<string> myvector, int *checked, int max);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
COORD getCursorCoord();

void RadioList::setChecked(int s){
	checked = s;
}
int RadioList::getChecked(){
	return checked;
}


RadioList::RadioList(){
	checked = 0;
	max = -1;
	max = -4;
	printf("hey! please enter coord.x:");
	cin >> coord.X;
	printf("\nplease enter coord.y:");
	cin >> coord.Y;

	fflush(stdin);

	printf("please enter strings with CR after every level, finish with 'done'\n\n");
	string str;
	
	while (str != "done")
	{
			printf("row:  ");
			getline(cin, str);
			cout << str << str.length()<< "\n";


			if (str != "done") {
				myvector.push_back(str);


				max = max < int( str.length() )? int(str.length()) : max;


			}

	
	}
	


	system("cls");


}


RadioList::~RadioList(){

	printf("BYE");

}


	
void RadioList::display(){
	
	COORD cursCoor;
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;
	DWORD wAttr = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, wAttr);
	SetConsoleCursorPosition(h, coord);

	//set cursor size and visibility example

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(h, &cci);
	COORD original = coord;
	for (int i = 0; i < myvector.size(); i++)
	{	
		
		SetConsoleCursorPosition(h, coord);

		//GetCursorPos(&cursCoor); FIND WHERE MOUSE
		cursCoor=getCursorCoord();
		//cout << "point is (" << cursCoor.X << "," << cursCoor.Y << ")";


		if (cursCoor.X == original.X &&  cursCoor.Y == original.Y)cout << '\xD9';
		if (cursCoor.Y > original.Y && cursCoor.Y< (original.Y+myvector.size()-1) ) cout << " ";
		else if (cursCoor.X == original.X && cursCoor.Y == (original.Y + myvector.size() - 1)) cout << '\xBF';

		cout << "[ ]" << myvector[i] ;
		
		cursCoor = getCursorCoord();

		 if (cursCoor.X == original.X + int(myvector[i].length()) + 4 && cursCoor.Y == original.Y){
			cursCoor.X = original.X + max + 4;
			SetConsoleCursorPosition(h, cursCoor);
			cout << '\xC0'; 
		}

		 else if (cursCoor.X == original.X + int(myvector[i].length()) + 4 && cursCoor.Y == (original.Y + myvector.size() - 1) ){
			 cursCoor.X = original.X + max + 4;
			 SetConsoleCursorPosition(h, cursCoor);
			 cout << '\xDA';
		 }




		cout << "\n";
		coord.Y += 1;

	}
	COORD whereIam = getCursorCoord();
	
	COORD firstCheck = { original.X + 2, original.Y };
	SetConsoleCursorPosition(h, firstCheck);
	cout << "X";
	SetConsoleCursorPosition(h, whereIam);

	//----------------------
	//finish to print with frames

	


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
				KeyEventProc(irInBuf[i].Event.KeyEvent, original, myvector, &checked, max);
				break;

			case MOUSE_EVENT: // mouse input 
				MouseEventProc(irInBuf[i].Event.MouseEvent, original, myvector, &checked, max);
				
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				
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

COORD getCursorCoord(){
	
	COORD c;
		
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&csbi
			))
		{
			c.X = -1;
			c.Y - 1;
		}
		else{
			c.X=csbi.dwCursorPosition.X;
			c.Y = csbi.dwCursorPosition.Y;


		}

		//cout << "in getcursorcoord::" << "(" << c.X << "," << c.Y << ")\n";
		return c;
}





void ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker, COORD origin, vector<string> myvector, int *checked, int max)
{
	string s;
	int whichRow= -1 , whichElement= -1;
	COORD c;
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(h, a);
	COORD saveWhereCursor = getCursorCoord();
	COORD whereIneed;
	int len = 0;

	if (!ker.bKeyDown) return;
	COORD ret;
	COORD source;
	COORD dest;
	COORD whereCursor;
	switch (ker.wVirtualKeyCode){

		
	case VK_SPACE:

		whereCursor = getCursorCoord();

		if (origin.Y + *(checked) == whereCursor.Y)return;

		if (whereCursor.Y >= origin.Y  && whereCursor.Y <= origin.Y + myvector.size() - 1){
			SetConsoleCursorPosition(h, { origin.X + 2, whereCursor.Y });

			GetConsoleScreenBufferInfo(h, &cbi);

			a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
			SetConsoleTextAttribute(h, a);

			cout << "X";


			//delete x from checked
			a = cbi.wAttributes & ~(BACKGROUND_BLUE | BACKGROUND_GREEN);
			SetConsoleTextAttribute(h, a);
			SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *(checked) });
			cout << " ";

			SetConsoleCursorPosition(h, whereCursor);
			*checked = whereCursor.Y - origin.Y;

		}

		else{

			return;
		}

		GetConsoleScreenBufferInfo(h, &cbi);
		a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
		SetConsoleTextAttribute(h, a);



		break;

		case VK_RETURN:
				
				whereCursor = getCursorCoord();

				if (origin.Y + *(checked) == whereCursor.Y)return;

				if (whereCursor.Y >= origin.Y  && whereCursor.Y <= origin.Y + myvector.size() - 1){
					SetConsoleCursorPosition(h, { origin.X + 2, whereCursor.Y });
					
					GetConsoleScreenBufferInfo(h, &cbi);
					
					a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
					SetConsoleTextAttribute(h, a);

					cout << "X";
					

					//delete x from checked
					a = cbi.wAttributes & ~(BACKGROUND_BLUE | BACKGROUND_GREEN);
					SetConsoleTextAttribute(h, a);
					SetConsoleCursorPosition(h, { origin.X + 2, origin.Y+ *(checked) });
					cout << " ";

					SetConsoleCursorPosition(h, whereCursor);
					*checked = whereCursor.Y - origin.Y ;
					
				}

				else{

					return;
				}

				GetConsoleScreenBufferInfo(h, &cbi);
				a= cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);

				
				
		break;


		case VK_UP:
		ret= whereCursor = getCursorCoord();
			//whereCursor.Y -= 1;
			whereCursor.X = origin.X;

			if (whereCursor.Y >= origin.Y  && whereCursor.Y <= origin.Y + myvector.size() - 1)
			{
				SetConsoleCursorPosition(h, { origin.X, whereCursor.Y });
				
				if (whereCursor.Y == origin.Y + myvector.size() - 1) // this is the last row
				{
					cout << "\xBF[ ]" + myvector[whereCursor.Y - origin.Y];


					SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
					cout << "\xDA";

				}

				else if (whereCursor.Y == origin.Y) // this is the first row
				{
					cout << "\xD9[ ]" + myvector[whereCursor.Y - origin.Y];
					SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
					cout << "\xC0";
				}

				else{
					cout << " [ ]" << myvector[whereCursor.Y - origin.Y];
				}

				SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *checked });
				if (origin.Y + *checked == whereCursor.Y) {
					GetConsoleScreenBufferInfo(h, &cbi);
					a = cbi.wAttributes & ~( BACKGROUND_BLUE | BACKGROUND_GREEN);
					SetConsoleTextAttribute(h, a);

					cout << "X";
				}
				
				SetConsoleCursorPosition(h, ret);
			}
			

			if (whereCursor.Y <= origin.Y )
			{
				whereCursor.Y = origin.Y + myvector.size()-1;
			}

			else if (whereCursor.Y > origin.Y + myvector.size() - 1)
			{
				whereCursor.Y = origin.Y;
			}
			else{
				whereCursor.Y -= 1;
			}

			SetConsoleCursorPosition(h, whereCursor);
			GetConsoleScreenBufferInfo(h, &cbi);
			
			a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
			SetConsoleTextAttribute(h, a);
		
			if (whereCursor.Y == origin.Y + myvector.size() - 1) // this is the last row
			{
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);

				cout << "\xBF";
				GetConsoleScreenBufferInfo(h, &cbi);
				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout<<"[ ]"<< myvector[whereCursor.Y - origin.Y];
					
					//---
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
			
					SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
					cout << "\xDA";
				
			}

			else if  (whereCursor.Y == origin.Y) // this is the first row
			{
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				cout << "\xD9";
					
				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout<<"[ ]"<<myvector[whereCursor.Y - origin.Y];

				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
				cout << "\xC0";
			}
			
			else{
				



				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				cout << " ";

				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "[ ]" << myvector[whereCursor.Y - origin.Y];

				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
				
			}
			 ret = getCursorCoord();

				//put X in the check row
			SetConsoleCursorPosition(h, {origin.X+2 ,  origin.Y+ *(checked)} );
			if (origin.Y + *checked == whereCursor.Y){


				GetConsoleScreenBufferInfo(h, &cbi);
				
			
				 a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "X";

			}
		

			SetConsoleCursorPosition(h, ret);

		break;



		case VK_DOWN:
			ret = whereCursor = getCursorCoord();
			
			whereCursor.X = origin.X;

			if (whereCursor.Y >= origin.Y  && whereCursor.Y <= origin.Y + myvector.size() - 1)
			{
				SetConsoleCursorPosition(h, { origin.X, whereCursor.Y });

				if (whereCursor.Y == origin.Y + myvector.size() - 1) // this is the last row
				{
					cout << "\xBF[ ]" + myvector[whereCursor.Y - origin.Y];


					SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
					cout << "\xDA";

				}

				else if (whereCursor.Y == origin.Y) // this is the first row
				{
					cout << "\xD9[ ]" + myvector[whereCursor.Y - origin.Y];
					SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
					cout << "\xC0";
				}

				else{
					cout << " [ ]" << myvector[whereCursor.Y - origin.Y];
				}

				SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *checked });
				if (origin.Y + *checked == whereCursor.Y) {
					GetConsoleScreenBufferInfo(h, &cbi);
					a = cbi.wAttributes & ~(BACKGROUND_BLUE | BACKGROUND_GREEN);
					SetConsoleTextAttribute(h, a);

					cout << "X";
				}

				SetConsoleCursorPosition(h, ret);
			}


			 if (whereCursor.Y >= origin.Y + myvector.size() - 1)
			{
				whereCursor.Y = origin.Y;
			}
			else{
				whereCursor.Y += 1;
			}

			SetConsoleCursorPosition(h, whereCursor);
			GetConsoleScreenBufferInfo(h, &cbi);

			a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
			SetConsoleTextAttribute(h, a);

			if (whereCursor.Y == origin.Y + myvector.size() - 1) // this is the last row
			{
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);

				cout << "\xBF";
				GetConsoleScreenBufferInfo(h, &cbi);
				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "[ ]" << myvector[whereCursor.Y - origin.Y];

				//---
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);

				SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
				cout << "\xDA";

			}

			else if (whereCursor.Y == origin.Y) // this is the first row
			{
				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				cout << "\xD9";

				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "[ ]" << myvector[whereCursor.Y - origin.Y];

				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });
				cout << "\xC0";
			}

			else{




				GetConsoleScreenBufferInfo(h, &cbi);
				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				cout << " ";

				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "[ ]" << myvector[whereCursor.Y - origin.Y];

				a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(h, a);
				SetConsoleCursorPosition(h, { origin.X + max + 4, whereCursor.Y });

			}
			ret = getCursorCoord();

			//put X in the check row
			SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *(checked) });
			if (origin.Y + *checked == whereCursor.Y){


				GetConsoleScreenBufferInfo(h, &cbi);

				//a=FOREGROUND_GREEN ;
				a = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(h, a);
				cout << "X";

			}


			SetConsoleCursorPosition(h, ret);

			break;


	}









	
}


string copyString(COORD C, int len){
	//string str;
	//DWORD size = 100;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, C);
	char row[100];
	DWORD num_read=0;


	 ReadConsoleOutputCharacter(h, (LPTSTR)row, len, C, (LPDWORD)num_read);

	 string str(row, num_read);

	 cout << " in copy string:" << str << "\n";




	 return str;
}


void dis(COORD origin, vector<string> myvector, int *checked, int max){
	DWORD wAttr = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, wAttr);
	SetConsoleCursorPosition(h, origin);

	//set cursor size and visibility example
	COORD cursCoor;
	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(h, &cci);
	COORD original = origin;
	for (int i = 0; i < myvector.size(); i++)
	{

		SetConsoleCursorPosition(h, origin);

		//GetCursorPos(&cursCoor); FIND WHERE MOUSE
		cursCoor = getCursorCoord();
		//cout << "point is (" << cursCoor.X << "," << cursCoor.Y << ")";


		if (cursCoor.X == original.X &&  cursCoor.Y == original.Y)cout << '\xD9';
		if (cursCoor.Y > original.Y && cursCoor.Y< (original.Y + myvector.size() - 1)) cout << " ";
		else if (cursCoor.X == original.X && cursCoor.Y == (original.Y + myvector.size() - 1)) cout << '\xBF';

		cout << "[ ]" << myvector[i];

		cursCoor = getCursorCoord();

		if (cursCoor.X == original.X + int(myvector[i].length()) + 4 && cursCoor.Y == original.Y){
			cursCoor.X = original.X + max + 4;
			SetConsoleCursorPosition(h, cursCoor);
			cout << '\xC0';
		}

		else if (cursCoor.X == original.X + int(myvector[i].length()) + 4 && cursCoor.Y == (original.Y + myvector.size() - 1)){
			cursCoor.X = original.X + max + 4;
			SetConsoleCursorPosition(h, cursCoor);
			cout << '\xDA';
		}




		cout << "\n";
		//maybe error
		origin.Y += 1;

	}
	COORD whereIam = getCursorCoord();

	COORD firstCheck = { original.X + 2, original.Y };
	SetConsoleCursorPosition(h, firstCheck);
	//cout << "X";
	SetConsoleCursorPosition(h, whereIam);

}




void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer, COORD origin, vector<string> myvector, int *checked, int max)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	POINT pt;
	pt.x = mer.dwMousePosition.X;
	pt.y = mer.dwMousePosition.Y;
	COORD whereCursor = getCursorCoord();
	DWORD a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (mer.dwEventFlags)
	{
	case 0:
		if (origin.Y + *(checked) == pt.y) return;
		if (pt.y == origin.Y + *(checked)) return;
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			dis(origin, myvector, checked, max);
			
			
			// remove higlight
			if (!(pt.x >= origin.X && pt.x <= origin.X + max+ 4 && pt.y <= origin.Y + myvector.size() - 1 && pt.y >= origin.Y)) return;

			//whereCursor = getCursorCoord();
			SetConsoleCursorPosition(h, { origin.X, pt.y });
			if (pt.y == origin.Y) //first row
			{
				cout << "\xD9[ ]" + myvector[pt.y - origin.Y];
				SetConsoleCursorPosition(h, { origin.X + max + 4, pt.y });
				cout << "\xC0";
			}
			else if (pt.y == origin.Y + myvector.size() - 1){
				cout << "\xBF[ ]" + myvector[pt.y - origin.Y];
				SetConsoleCursorPosition(h, { origin.X + max + 4, pt.y });
				cout << "\xDA";
			}
			else{
				cout << " [ ]" << myvector[pt.y - origin.Y];
			}


			SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *checked });
			cout << "X";
			//---------------
			

				if (pt.y >= origin.Y  && pt.y <= origin.Y + myvector.size() - 1)
				{
					SetConsoleCursorPosition(h, { origin.X + 2, pt.y });

					GetConsoleScreenBufferInfo(h, &cbi);

					
					 a = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
					SetConsoleTextAttribute(h, a);
					SetConsoleCursorPosition(h, { origin.X + 2, pt.y });
					cout << "X";


					//delete x from checked
					a = cbi.wAttributes & ~(BACKGROUND_BLUE | BACKGROUND_GREEN);
					SetConsoleTextAttribute(h, a);
					SetConsoleCursorPosition(h, { origin.X + 2, origin.Y + *(checked) });
					cout << " ";

					SetConsoleCursorPosition(h, { pt.x, pt.y });
					*checked = pt.y - origin.Y;

				}

				


		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			return;
		}
		else
		{
			return;
		}
		break;
	case DOUBLE_CLICK:
		
		break;
	case MOUSE_HWHEELED:
		//printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
	//	printf("mouse moved\n");
		break;
	case MOUSE_WHEELED:
	//	printf("vertical mouse wheel\n");
		break;
	default:
	//	printf("unknown\n");
		break;
	}
}
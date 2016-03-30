#include "ComboBox.h"

COMBOBOX::COMBOBOX(int x, int y, vector<string> options):COMPONNENT(x, y, length){

}
void COMBOBOX::Draw(){}
void COMBOBOX::Handle(INPUT_RECORD *inpt, int i){}
void COMBOBOX::SetColors(){}
void COMBOBOX::SetCursor(int x, int y){}
bool COMBOBOX::IsClicked(COORD cursor){ return true; }
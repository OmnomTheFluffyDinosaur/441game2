
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef MENU_H                 // Prevent multiple definitions if this 
#define MENU_H 

class Menu;

#include "graphics.h"
#include "constants.h"
#include "textDX.h"
#include <string>
#include <sstream>
#include "input.h"

namespace menuNS
{ }

// inherits from Entity class
class Menu 
{
	int selectedItem;

public:
    // constructor
	Menu() {};
	virtual void initialize(Graphics *g, Input *i) = 0;
	virtual void update() = 0;
	virtual int getSelectedItem() {return selectedItem;}
	virtual void displayMenu() = 0;
};
#endif


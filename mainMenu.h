
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef MAINMENU_H                 // Prevent multiple definitions if this 
#define MAINMENU_H 

class mainMenu;

#include "graphics.h"
#include "constants.h"
#include "textDX.h"
#include <string>
#include <sstream>
#include "input.h"
#include "menu.h"

namespace mainMenuNS
{ }

// inherits from Entity class
class mainMenu : public Menu
{
private:
   TextDX *menuItemFont;
   TextDX *menuItemFontHighlight;
   TextDX *menuHeadingFont;
   Input   *input;         // pointer to the input system
   Graphics *graphics;
   std::string menuHeading;
   std::string menuItem1;
   std::string menuItem2;
   std::string menuItem3;
   std::string menuItem4;
   D3DXVECTOR2 menuAnchor;
   int verticalOffset;
   int linePtr;
   int selectedItem;
   COLOR_ARGB highlightColor ;
   COLOR_ARGB normalColor;
   bool upDepressedLastFrame;
   bool downDepressedLastFrame;

public:
    // constructor
    mainMenu();
	void initialize(Graphics *g, Input *i);
	void update();
	int getSelectedItem() {return selectedItem;}
	void displayMenu();
};
#endif


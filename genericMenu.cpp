
#include "genericMenu.h"

genericMenu::genericMenu(std::string item1, std::string item2, std::string item3)
{
	selectedItem = -1;	//nothing return
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
	menuItem1 = item1;
	menuItem2 = item2;
}

void genericMenu::initialize(Graphics *g, Input *i)
{
	menuHeading ="Neon Sky";
	//menuItem1 = "Play";
	//menuItem2 = "Cheats";
	menuItem3 = "Back";
	highlightColor = D3DCOLOR_ARGB(255,190,  0,  0);
	normalColor = graphicsNS::BLACK;
	menuAnchor = D3DXVECTOR2(70,10);
	input = i;
	verticalOffset = 30;
	linePtr = 0;
	selectedItem = -1;
	graphics = g;
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
	menuItemFontHighlight = new TextDX();
	if(menuItemFont->initialize(graphics, 25, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuItemFontHighlight->initialize(graphics, 28, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuHeadingFont->initialize(graphics, 34, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuHeading font"));
	menuHeadingFont->setFontColor(normalColor);
	menuItemFont->setFontColor(normalColor);
	menuItemFontHighlight->setFontColor(highlightColor);
	upDepressedLastFrame = false;
	downDepressedLastFrame = false;
}

void genericMenu::update()
{
	/*if (input->wasKeyPressed(VK_UP))
	{
		linePtr--;
	}
	if (input->wasKeyPressed(VK_DOWN))
	{
		linePtr++;
	}
	if (linePtr != 3) linePtr = 3;
	//if (linePtr < 3) linePtr = 3;
	*/
	linePtr = 2;
	if (input->wasKeyPressed(VK_RETURN) || input->wasKeyPressed(VK_SPACE))
		selectedItem = linePtr;
	else selectedItem = -1;
}

void genericMenu::displayMenu()
{
//	menuHeadingFont->print(menuHeading, menuAnchor.x, menuAnchor.y);
//	if (linePtr==0)
//		menuItemFontHighlight->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
//	else
	menuItemFont->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
	int foo = 2*verticalOffset;
//	if (linePtr==1)
//		menuItemFontHighlight->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
//	else
		menuItemFont->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
	foo = 4*verticalOffset;
	//if (linePtr==2)
		menuItemFontHighlight->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);
	//else
	//	menuItemFont->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);

}
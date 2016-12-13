
#include "mainMenu.h"

mainMenu::mainMenu()
{
	selectedItem = -1;	//nothing return
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
}

void mainMenu::initialize(Graphics *g, Input *i)
{
	menuHeading ="Neon Sky";
	menuItem1 = "Play";
	menuItem2 = "Cheats";
	menuItem3 = "Credits";
	menuItem4 = "Controls";
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

void mainMenu::update()
{
	if (input->wasKeyPressed(VK_UP))
	{
		linePtr--;
	}
	if (input->wasKeyPressed(VK_DOWN))
	{
		linePtr++;
	}
	if (linePtr > 3) linePtr = 0;
	if (linePtr < 0) linePtr = 3;

	if (input->wasKeyPressed(VK_RETURN) || input->wasKeyPressed(VK_SPACE))
		selectedItem = linePtr;
	else selectedItem = -1;
}

void mainMenu::displayMenu()
{
	menuHeadingFont->print(menuHeading, menuAnchor.x, menuAnchor.y);
	if (linePtr==0)
		menuItemFontHighlight->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
	else
		menuItemFont->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
	int foo = 2*verticalOffset;
	if (linePtr==1)
		menuItemFontHighlight->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
	else
		menuItemFont->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
	foo = 3*verticalOffset;
	if (linePtr==2)
		menuItemFontHighlight->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);
	else
		menuItemFont->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);
	foo = 4*verticalOffset;
	if (linePtr==3)
		menuItemFontHighlight->print(menuItem4, menuAnchor.x, menuAnchor.y+foo);
	else
		menuItemFont->print(menuItem4, menuAnchor.x, menuAnchor.y+foo);
}
#include "menu_display.h"



cvector(struct ItemObjectList) menuItems = NULL;

char *menuText[] = {
    "display",
    "settings"};

extern void settingsSetup();
extern void dataDisplaySetup();

void menuSetup()
{
    struct ItemObjectList str1 = {menuText[0], &dataDisplaySetup};
    cvector_push_back(menuItems, str1);
    struct ItemObjectList str2 = {menuText[1], &settingsSetup};
    cvector_push_back(menuItems, str2);

    menuButtonHandler();
    drawDiplay = &menuDisplayDraw;
}

void menuButtonHandler()
{
    setButtonHandlerShort(0, menuSelectButton);
    setButtonHandlerLong(0, 0);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, 0);
    setButtonHandlerLong(3, 0);
}

void menuSelectButton()
{
    cvector_at(menuItems, selectRow)->setupDisplay();
    selectRow = 0;
    // clear vector
    for (int i = cvector_size(menuItems); 0 < i; i--)
    {
        cvector_pop_back(menuItems);
    }
    updateDisp();
}

void menuDisplayDraw()
{
    st7567_WriteString(0, 0, "menu", FontStyle_veranda_9);
    drawList(&menuItems);
}

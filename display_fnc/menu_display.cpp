#include "menu_display.h"
#include "settings_display.h"

std::vector<ItemObjectList> menuItems;

void menuSetup()
{
    menuItems.push_back(ItemObjectList());
    menuItems[0].text = "display";
    menuItems.push_back(ItemObjectList());
    menuItems[1].text = "train";

    menuItems.push_back(ItemObjectList());
    menuItems[2].text = "settings";
    menuItems[2].setupDisplay = &settingsSetup;

    menuItems.push_back(ItemObjectList());
    menuItems[3].text = "memory";

    menuItems.push_back(ItemObjectList());
    menuItems[4].text = "qwe";
    
    menuItems.push_back(ItemObjectList());
    menuItems[5].text = "eer";

    menuButtonHandler();
    drawDiplay = &menuDisplayDraw;
}

void menuSelectButton()
{
    menuItems[selectRow].setupDisplay();
    // clear vector
    for (int i = menuItems.size(); 0 < i; i--)
    {
        menuItems.pop_back();
    }
    updateDisp();
}

void menuButtonHandler()
{
    setButtonHandlerShort(0, menuSelectButton);
    setButtonHandlerLong(0, nullptr);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, nullptr);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, nullptr);
    setButtonHandlerShort(3, nullptr);
   // setButtonHandlerLong(3, nullptr);
}

void menuDisplayDraw()
{
    st7567_WriteString(0, 0, "menu", Font_7x10);
    drawList(&menuItems);
}

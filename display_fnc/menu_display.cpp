#include "menu_display.h"

// struct ItemObject menuItems[4];
std::vector<ItemObject> menuItems;

void menuSetup()
{
    menuItems.push_back(ItemObject());
    menuItems[0].text = "display";
    menuItems.push_back(ItemObject());
    menuItems[1].text = "train";
    menuItems.push_back(ItemObject());
    menuItems[2].text = "settings";
    menuItems.push_back(ItemObject());
    menuItems[3].text = "memory";

    menuButtonHandler();
    drawDiplay = &menuDisplayDraw;
}

void menuSelectButton()
{
    setButtonHandlerShort(0, nullptr);

    for (int i = menuItems.size(); 0 < i; i--)
    {
        menuItems.pop_back();
    }
}

void menuButtonHandler()
{
    setButtonHandlerShort(0, menuSelectButton);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(1, nullptr);
    setButtonHandlerLong(2, nullptr);
}

void menuDisplayDraw()
{
    st7567_WriteString(44, 0, "menu", Font_7x10);
    drawList(&menuItems);
}

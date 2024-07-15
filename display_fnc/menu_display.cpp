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

    drawDiplay = &menuDisplayDraw;
}

void menuSelectButton(){

}

void menuButtonHandler()
{
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(1, nullptr);
    setButtonHandlerLong(2, nullptr);
}

void menuDisplayDraw()
{
    st7567_WriteString(0, 0, "menu", Font_7x10);
    drawList(&menuItems);
}

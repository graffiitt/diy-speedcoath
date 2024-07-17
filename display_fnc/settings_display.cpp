#include "settings_display.h"
#include "menu_display.h"

std::vector<ItemObjectList> settingsItems;

void settingsSetup()
{
    settingsItems.push_back(ItemObjectList());
    settingsItems[0].text = "bluetooth";

    settingsButtonsSetup();
    drawDiplay = &settingsDrawDisplay;
}

void settingsDrawDisplay()
{
    st7567_WriteString(0, 0, "settings", Font_7x10);
    drawList(&settingsItems);
}

void settingsClearVector()
{
    for (int i = settingsItems.size(); 0 < i; i--)
    {
        settingsItems.pop_back();
    }
}

void settingsButtonsSetup()
{
    setButtonHandlerShort(0, nullptr);
    setButtonHandlerLong(0, nullptr);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, nullptr);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, nullptr);
    setButtonHandlerShort(3, settingsBackButton);
   // setButtonHandlerLong(3, nullptr);
}

void settingsBackButton()
{
    menuSetup();
    settingsClearVector();
    selectRow = 2;
    updateDisp();
}

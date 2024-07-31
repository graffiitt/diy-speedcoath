#include "settings_display.h"
 

cvector(struct ItemObjectList) settingsItems = NULL;

char *settingsText[] = {
    "bluetooth",
    "items display"};

void settingsSetup()
{
    struct ItemObjectList str1 = {settingsText[0], 0};
    cvector_push_back(settingsItems, str1);
    struct ItemObjectList str2 = {settingsText[1], 0};
    cvector_push_back(settingsItems, str2);

    settingsButtonHandler();
    drawDiplay = &settingsDisplayDraw;
}

void settingsButtonHandler()
{
    setButtonHandlerShort(0, settingsSelectButton);
    setButtonHandlerLong(0, 0);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, settingsBackButton);
   // setButtonHandlerLong(3, 0);
}

void settingsSelectButton()
{
    cvector_at(settingsItems, selectRow)->setupDisplay();
    selectRow = 0;
    for (int i = cvector_size(settingsItems); 0 < i; i--)
    {
        cvector_pop_back(settingsItems);
    }
    updateDisp();
}

extern void menuSetup();
void settingsBackButton()
{
    menuSetup();
    selectRow = 1;
    for (int i = cvector_size(settingsItems); 0 < i; i--)
    {
        cvector_pop_back(settingsItems);
    }
}

void settingsDisplayDraw()
{
    st7567_WriteString(0, 0, "settings", FontStyle_veranda_9);
    drawList(&settingsItems);
}

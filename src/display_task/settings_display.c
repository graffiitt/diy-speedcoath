#include "display_task/display_functions.h"

cvector(struct ItemObjectList) settingsItems = NULL;

char *settingsText[] = {
    "bluetooth connect",
    "items display"};

static void displayDraw();
// button handlers
static void selectButton();
static void backButton();

static void buttonHandler()
{
    setButtonHandlerShort(0, selectButton);
    setButtonHandlerLong(0, 0);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, backButton);
}

void bleSettingsSetup();
void setupDispItemsSetup();

void settingsSetup()
{
    struct ItemObjectList str1 = {settingsText[0], 0};
    cvector_push_back(settingsItems, str1);
    struct ItemObjectList str2 = {settingsText[1], setupDispItemsSetup};
    cvector_push_back(settingsItems, str2);

    buttonHandler();
    drawDiplay = &displayDraw;
}

void selectButton()
{
    startChangeDisplay();
    cvector_at(settingsItems, selectRow)->setupDisplay();
    for (int i = cvector_size(settingsItems); 0 < i; i--)
    {
        cvector_pop_back(settingsItems);
    }

    selectRow = 0;
    endChangeDisplay();
}

extern void menuSetup();
void backButton()
{
    startChangeDisplay();
    for (int i = cvector_size(settingsItems); 0 < i; i--)
    {
        cvector_pop_back(settingsItems);
    }
    menuSetup();
    selectRow = 1;
    endChangeDisplay();
}

void displayDraw()
{
    st7567_WriteString(0, 0, "settings", FontStyle_veranda_9);
    drawList(&settingsItems);
}

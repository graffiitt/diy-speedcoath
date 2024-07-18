#include "display_functions.h"

void drawList(const std::vector<ItemObjectList> *items)
{
    uint8_t x = 7;
    uint8_t y = 16;

    if (items->empty())
        return;

    if (selectRow > items->size()-1)
    {
        selectRow = items->size() - 1;
    }

    for (uint8_t i = 0; i < NUM_DISPLAY_ROWS; i++)
    {
        int item = selectRow > (NUM_DISPLAY_ROWS - 1) ? (selectRow - NUM_DISPLAY_ROWS + 1) + i : i;
        if (item < items->size())
        {
            st7567_WriteString(x, y, items->at(item).text.c_str(), Font_7x10);
            y += 12;
        }
    }
    y = 16 + 12 * (selectRow > (NUM_DISPLAY_ROWS - 1) ? (NUM_DISPLAY_ROWS - 1) : selectRow);
    st7567_WriteCharBuf(0, y, '>', Font_7x10);
}


void drawTime(Time *tm)
{
}

void buttonUpList()
{
    selectRow++;
    updateDisp();
}

void buttonDownList()
{
    if (selectRow > 0)
        selectRow--;
    updateDisp();
}

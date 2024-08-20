#include "display_functions.h"

void drawList(cvector(struct ItemObjectList) * items)
{
    uint8_t x = 7;
    uint8_t y = 16;

    if (cvector_size(*items) == 0)
        return;

    if (selectRow > cvector_size(*items) - 1)
    {
        selectRow = cvector_size(*items) - 1;
    }

    for (uint8_t i = 0; i < NUM_DISPLAY_ROWS; i++)
    {
        int item = selectRow > (NUM_DISPLAY_ROWS - 1) ? (selectRow - NUM_DISPLAY_ROWS + 1) + i : i;
        if (item < cvector_size(*items))
        {
            printf("st:%s\n", cvector_at(*items, i)->text);
            st7567_WriteString(x, y, cvector_at(*items, i)->text, FontStyle_veranda_9);
            y += 12;
        }
    }
    y = 16 + 12 * (selectRow > (NUM_DISPLAY_ROWS - 1) ? (NUM_DISPLAY_ROWS - 1) : selectRow);
    st7567_WriteChar(0, y, '>', FontStyle_veranda_9);
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

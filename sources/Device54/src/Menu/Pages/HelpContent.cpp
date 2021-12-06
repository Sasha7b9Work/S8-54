#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Grid.h"
#include "Display/Font/Font.h"
#include "HelpContentPages.h"
#include "HelpContent.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"



static int currentParagraph = 0;   // ���� TypePage(currentPage) == TypePage_Content, �� ��������� �� ������� ������ ����������
static const PageHelp *currentPage = &helpMain;


static const int WIDTH = 295;



static void DrawPageContent()
{
    int y = 50;
    Painter::DrawStringInCenterRect(0, y, WIDTH, 10, currentPage->titleHint[LANG]);

    int numPage = 0;

    y += 40;

    while(currentPage->pages[numPage])
    {
        Page *page = (Page *)currentPage->pages[numPage];
        const char *title = page->titleHint[LANG];
        if(currentParagraph == numPage)
        {
            Painter::DrawStringInCenterRectOnBackgroundC(0, y, WIDTH, 10, title, Color::BACK, 2, Color::FILL);
        }
        else
        {
            Painter::DrawStringInCenterRect(0, y, WIDTH, 10, title, Color::FILL);
        }
        y += 16;
        numPage++;
    }
}


static void DrawPageDescription()
{
    Painter::DrawStringInCenterRect(0, 3, WIDTH, 10, currentPage->titleHint[LANG]);
    Painter::DrawTextInRectWithTransfers(2, 15, WIDTH - 5, 240, currentPage->titleHint[2 + LANG]);
}


void HelpContent_Draw()
{
    Painter::FillRegion(Grid::Right(), 0, 319 - Grid::Right(), 20, Color::BACK);
    Painter::FillRegion(Grid::Right(), 219, 319 - Grid::Right(), 21);
    Painter::FillRegion(1, 1, WIDTH, 237);
    Painter::DrawRectangle(1, 0, WIDTH + 1, 239, Color::FILL);   /** \todo ����� ���������, ������ ���. ���������� �������� ������ ���� ������ ���� 
                                                                0, 0, �� � ����� ������ ����� ������������ ������ �� ����� */

    /*
    uint16 *addr1 = (uint16 *)(0x08000000 + (rand() % 65535));
    uint8 *addr2 = (uint8 *)(0x08000000 + (rand() % 65535));

    Painter::SetColor(Color::FILL);
    for (int i = 0; i < 10000; i++)
    {
        Painter::SetPoint((*addr1) % WIDTH, Math_LimitationInt(*addr2, 0, 239));
        addr1++;
        addr2++;
    }
    */

    if(currentPage->type == TypePage_Content)
    {
        DrawPageContent();
    }
    else if(currentPage->type == TypePage_Description)
    {
        DrawPageDescription();
    }
}


static int NumParagraphs(const PageHelp *page)
{
    int retValue = 0;
    while(page->pages[retValue])
    {
        retValue++;
    }
    return retValue;
}


void HelpContent_NextParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        CircleIncrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}


void HelpContent_PrevParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        CircleDecrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}


void HelpContent_EnterParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        currentPage = (const PageHelp *)currentPage->pages[currentParagraph];
    }
    currentParagraph = 0;
}


void HelpContent_LeaveParagraph()
{
    currentParagraph = 0;
    if(currentPage->parent)
    {
        currentPage = (const PageHelp *)currentPage->parent;
    }
}


bool HelpContent_LeaveParagraphIsActive()
{
    return currentPage->parent != 0;
}


bool HelpContent_EnterParagraphIsActive()
{
    return currentPage->type == TypePage_Content;
}

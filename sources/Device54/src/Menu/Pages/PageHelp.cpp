#include "defines.h"
#include "globals.h"
#include "PageHelp.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "HelpContent.h"
#include "Utils/CommonFunctions.h"



extern const PageBase pHelp;



static void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphNext(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    Painter::SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnHelpRegSet(int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphEnter,                                                                                   //--- ������ - ������� ---
    "�������",  "Open",
    "��������� ������ �������",
    "Opens the section of the reference",
    pHelp, HelpContent_EnterParagraphIsActive, HelpContent_EnterParagraph, DrawSB_Help_ParagraphEnter
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphLeave,                                                                                   //--- ������ - ������� ---
     "�������", "Close",
     "��������� ������ �������",
     "Closes the section of the reference",
    pHelp, HelpContent_LeaveParagraphIsActive, HelpContent_LeaveParagraph, DrawSB_Help_ParagraphLeave
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphPrev,                                                                          //--- ������ - ���������� ������ ---
    "���������� ������", "Previous section",
    "������� ���������� ������ �������",
    "To choose the previous section of the reference",
    pHelp, FuncActive, HelpContent_PrevParagraph, DrawSB_Help_ParagraphPrev
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphNext,                                                                           //--- ������ - ��������� ������ ---
    "��������� ������", "Next section",
    "������� ��������� ������ �������",
    "To choose the next section of the reference",
    pHelp, FuncActive, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext
)

static void PressSB_Help_Exit()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  sbExitHelp,                                                                                           //--- ������ - ����� ---
    pHelp, FuncActive, PressSB_Help_Exit, DrawSB_Help_ParagraphNext
)

const PageBase *PageHelp::pointer = &pHelp;

DEF_PAGE_SB(        pHelp,                                                                                                               // ������ ///
    "������", "HELP",
    "������� ������� ������",
    "To open sections of the help",
    &sbExitHelp,
    &sbHelpParagraphEnter,
    &sbHelpParagraphLeave,
    0,
    &sbHelpParagraphPrev,
    &sbHelpParagraphNext,
    PageSB_Help, &mainPage, FuncActive, FuncPress, HelpContent_Draw, OnHelpRegSet
)

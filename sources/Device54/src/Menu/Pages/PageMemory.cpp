#include "PageMemory.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "FlashDrive/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/CommonFunctions.h"


extern const PageBase pMemory;
extern const PageBase ppLast;
extern const PageBase ppInternal;
extern const PageBase ppDrive;
extern const PageBase pppDrive_Manager;
extern const PageBase pppDrive_Mask;
extern const PageBase pSetName;


static void DrawSetMask();  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawFileMask(int x, int y);
static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������

static void DrawMemoryWave(int num, bool exist);
static void SaveSignalToIntMemory();
       void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);


//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Points()
{
    return SET_PEAKDET_DIS;
}

void OnChanged_Points(bool active)
{
    // ���� ������� ������� ��������, �� �� ����� ����������� ������
    if (SET_PEAKDET_EN && !active)
    {
        Display::ShowWarning(WrongModePeackDet);
        return;
    }

    // ��������� ��������� 32� ����� ������, ���� ������� ������ �����
    if (FPGA_POINTS_32k && SET_ENABLED_B)
    {
        Display::ShowWarning(DisableChannelB);
        FPGA_ENUM_POINTS = FNP_16k;
    }

    int width = Grid::Width();

    FPGA::Reset();

    if (SET_PEAKDET_EN)
    {
        width *= 2;
    }
    if (TPOS_IS_LEFT)
    {
        SHIFT_IN_MEMORY = 0;
    }
    else if (TPOS_IS_CENTER)
    {
        SHIFT_IN_MEMORY = (int16)(NUM_BYTES_SET / 2 - width / 2);
    }
    else if (TPOS_IS_RIGHT)
    {
        SHIFT_IN_MEMORY = (int16)(NUM_BYTES_SET - width - 2);
    }

    FPGA::Reset();
    FPGA::SetTShift(SET_TSHIFT);
    FPGA::Reset();
}

pString namesLengthMemory[] = {"512", "1k", "2k", "4k", "8k", "16k", "32k"};

DEF_CHOICE_6(       cPoints,                                                                                                  //--- ������ - ����� ---
    "����� ������", "Mem length",
    "����� ���������� �������� ��� ����������� ��������. "
    "��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������.",
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases.",
    namesLengthMemory[0], namesLengthMemory[0],
    namesLengthMemory[1], namesLengthMemory[1],
    namesLengthMemory[2], namesLengthMemory[2],
    namesLengthMemory[3], namesLengthMemory[3],
    namesLengthMemory[4], namesLengthMemory[4],
    namesLengthMemory[5], namesLengthMemory[5],
    //,namesLengthMemory[6], namesLengthMemory[6],
    FPGA_ENUM_POINTS, pMemory, IsActive_Points, OnChanged_Points, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Exit()
{
    MODE_WORK = ModeWork_Dir;
    if (RUN_FPGA_BEFORE_SB)
    {
        FPGA::Start();
        RUN_FPGA_BEFORE_SB = 0;
    }
    OnPressSB_Exit();
}

DEF_SMALL_BUTTON_EXIT(  bLast_Exit,                                                                               //--- ������ - ��������� - ����� ---
    ppLast, FuncActive, OnPress_Last_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Next()
{
    CircleIncrease<int16>(&NUM_RAM_SIGNAL, 0, (int16)(Storage::NumElementsInStorage() - 1));
}

static void Draw_Last_Next(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x64');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bLast_Next,                                                                               //--- ������ - ��������� - ��������� ---
    "���������", "Next",
    "������� � ���������� �������",
    "Go to the next signal",
    ppLast, FuncActive, OnPress_Last_Next, Draw_Last_Next
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Prev()
{
    CircleDecrease<int16>(&NUM_RAM_SIGNAL, 0, (int16)(Storage::NumElementsInStorage() - 1));
}

static void Draw_Last_Prev(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_BACKSPACE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bLast_Prev,                                                                              //--- ������ - ��������� - ���������� ---
    "����������", "Previous",
    "������� � ����������� �������",
    "Go to the previous signal",
    ppLast, FuncActive, OnPress_Last_Prev, Draw_Last_Prev
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToROM()
{
    ((Page *)&ppInternal)->SetCurrentSB();
    MODE_WORK = ModeWork_ROM;   // ��������� � ������ ����������� ��
    EXIT_FROM_ROM_TO_RAM = 1;   // ���� ������� �� ������������� ��� ����, �����:
                                // 1 - �� ������� ������ ����� �� �������� "����� ��" �������� � "���������", � �� � �������� ����;
                                // 2 - ��� ����, ����� �� �������� "����� ��" �������� �� ��������� ��������� ������, � ��������� �� �������� 
                                // "���������";
                                // 3 - ����� � Data_Load() ������������� ��������������� ���������.
}

static void Draw_Last_SaveToROM(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bLast_SaveToROM,                                                                           //--- ������ - ��������� - ����� �� ---
    "����� ��", "Internal storage",
    "������� ��� ������, ����� ��������� ������ �� ���������� ������������ ����������",
    "Press this button to keep a signal in an internal memory",
    ppLast, FuncActive, OnPress_Last_SaveToROM, Draw_Last_SaveToROM
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToDrive()
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_LAST_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void Draw_Last_SaveToDrive(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        Painter::SetFont(TypeFont_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        Painter::SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON(   bLast_SaveToDrive,                                                                        //--- ������ - ��������� - ��������� ---
    "���������", "Save",
    "������ ���������� �������� ��� ������������� ������� ��. ��������� ��������� ������ �� ������ ��",
    "Click this button to save the signal on the external FLASH",
    ppLast, FuncActive, OnPress_Last_SaveToDrive, Draw_Last_SaveToDrive
)



static void OnPress_Last()
{
    NUM_RAM_SIGNAL = 0;
    RUN_FPGA_BEFORE_SB = FPGA_IS_RUNNING ? 1u : 0u;
    FPGA::Stop(false);
    MODE_WORK = ModeWork_RAM;
}

static void OnDraw_Last()
{
    char buffer[20];

    int width = 40;
    int height = 10;
    Painter::FillRegion(Grid::Right() - width, GRID_TOP, width, height, Color::BACK);
    Painter::DrawRectangle(Grid::Right() - width, GRID_TOP, width, height, Color::FILL);
    Painter::DrawText(Grid::Right() - width + 2, GRID_TOP + 1, Int2String(NUM_RAM_SIGNAL + 1, false, 3, buffer));
    Painter::DrawText(Grid::Right() - width + 17, GRID_TOP + 1, "/");
    Painter::DrawText(Grid::Right() - width + 23, GRID_TOP + 1, Int2String(Storage::NumElementsInStorage(), false, 3, buffer));
}

static void OnRegSet_Last(int angle)
{
    if (Storage::NumElementsInStorage() > 1)
    {
        Sound::RegulatorSwitchRotate();
    }
    if (Sign(angle) > 0)
    {
        OnPress_Last_Next();
    }
    else
    {
        OnPress_Last_Prev();
    }
}

DEF_PAGE_SB(        ppLast,                                                                                                  // ������ - ��������� ///
    "���������", "LATEST",
    "������� � ����� ������ � ���������� ����������� ���������",
    "Transition to an operating mode with the last received signals",
    &bLast_Exit,            // ������ - ��������� - �����
    &bLast_Next,            // ������ - ��������� - ���������
    &bLast_Prev,            // ������ - ��������� - ����������
    0,
    &bLast_SaveToROM,       // ������ - ��������� - ����� ��
    &bLast_SaveToDrive,     // ������ - ��������� - ���������
    PageSB_Memory_Last, &pMemory, FuncActive, OnPress_Last, OnDraw_Last, OnRegSet_Last
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Manager_Exit()
{
    Display::SetDrawMode(DrawMode_Auto, 0);
    OnPressSB_Exit();
}

DEF_SMALL_BUTTON_EXIT(  bDrive_Manager_Exit,                                                             //--- ������ - ����� �� - ������� - ����� ---
    pppDrive_Manager, FuncActive, OnPress_Drive_Manager_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_Tab(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_TAB);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Manager_Tab,                                                                    //--- ������ - ����� �� - ������� - Tab ---
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    pppDrive_Manager, FuncActive, FileManager::PressSB_Tab, Draw_Drive_Manager_Tab
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_LevelUp(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Manager_LevelUp,                                                  //--- ������ - ����� �� - ������� - ����� �� �������� ---
    "����� �� ��������", "Leave from directory",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    pppDrive_Manager, FuncActive, FileManager::PressSB_LevelUp, Draw_Drive_Manager_LevelUp
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Drive_Manager_LevelDown(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Manager_LevelDown,                                                  //--- ������ - ����� �� - ������� - ����� � ������� ---
    "����� � �������", "Enter in directory",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    pppDrive_Manager, FuncActive, FileManager::PressSB_LevelDown, Draw_Drive_Manager_LevelDown
)


static bool IsActive_Drive_Manager()
{
    return FDRIVE_IS_CONNECTED;
}

void OnPress_Drive_Manager()
{
    if (FDRIVE_IS_CONNECTED)
    {
        FDrive::Mount();
        Display::SetDrawMode(DrawMode_Auto, FileManager::Draw);
        FM_NEED_REDRAW = FM_REDRAW_FULL;
    }
    else
    {
        Display::ShowWarning(WarnNeedForFlashDrive);
    }
}

DEF_PAGE_SB(        pppDrive_Manager,                                                                               // ������ - ����� �� - ������� ///
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    &bDrive_Manager_Exit,       // ������ - ����� �� - ������� - �����
    &bDrive_Manager_Tab,        // ������ - ����� �� - ������� - Tab
    0,
    0,
    &bDrive_Manager_LevelUp,    // ������ - ����� �� - ������� - ����� �� ��������
    &bDrive_Manager_LevelDown,  // ������ - ����� �� - ������� - ����� � �������
    PageSB_Memory_Drive_Manager, &ppDrive, IsActive_Drive_Manager, OnPress_Drive_Manager, FuncDrawPage, FileManager::RotateRegSet
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDrive_Name,                                                                               //--- ������ - ����� �� - ��� ����� ---
    "��� �����", "File name"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "�� �����", "Mask",
    "�������",  "Manually",
    FILE_NAMING_MODE, ppDrive, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bDrive_Mask_Exit,                                                                  //--- ������ - ����� �� - ����� - ����� ---
    pppDrive_Mask, FuncActive, OnPressSB_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}

static void Draw_Drive_Mask_Delete(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Mask_Delete,                                                                  //--- ������ - ����� �� - ����� - ������� ---
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered symbols",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Delete, Draw_Drive_Mask_Delete
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Backspace()
{
    int size = (int)strlen(FILE_NAME_MASK);
    if (size > 0)
    {
        if (size > 1 && FILE_NAME_MASK[size - 2] == 0x07)
        {
            FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            FILE_NAME_MASK[size - 1] = '\0';
        }
    }
}

static void Draw_Drive_Mask_Backspace(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Mask_Backspace,                                                             //--- ������ - ����� �� - ����� - Backspace ---
    "Backspace", "Backspace",
    "������� ��������� �������� ������",
    "Deletes the last entered symbol",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Backspace, Draw_Drive_Mask_Backspace
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Drive_Mask_Insert()
{
    int index = INDEX_SYMBOL;
    int size = (int)strlen(FILE_NAME_MASK);
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        FILE_NAME_MASK[size] = symbolsAlphaBet[index][0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // ��� %nN - ��������� ������
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 && FILE_NAME_MASK[size - 1] <= 0x39) // ���� ����� ������� �����
                {
                    FILE_NAME_MASK[size] = FILE_NAME_MASK[size - 1] - 0x30;
                    FILE_NAME_MASK[size - 1] = 0x07;
                    FILE_NAME_MASK[size + 1] = '\0';
                }
            }
        }
        else
        {
            FILE_NAME_MASK[size] = (char)index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

static void Draw_Drive_Mask_Insert(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_INSERT);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bDrive_Mask_Insert,                                                                 //--- ������ - ����� �� - ����� - �������� ---
    "��������", "Insert",
    "��������� ��������� ������",
    "Inserts the chosen symbol",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Insert, Draw_Drive_Mask_Insert
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDrive_SaveAs,                                                                         //--- ������ - ����� �� - ��������� ��� ---
    "��������� ���", "Save as"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT"
    ,
    "�����������", "Image",
    "�����",       "Text",
    MODE_SAVE, ppDrive, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDrive_ModeBtnMemory,                                                                  //--- ������ - ����� �� - ��� �� ������ ---
    "��� �� ������", "Mode btn MEMORY",
    "",
    "",
    "����",       "Menu",
    "����������", "Save",
    MODE_BTN_MEMORY, ppDrive, FuncActive, FuncChangedChoice, FuncDraw
)


static bool IsActive_Drive_Mask()
{
    return FILE_NAMING_MODE_MASK;
}

static void OnPress_Drive_Mask()
{
    Display::SetAddDrawFunction(DrawSetMask);
}

static void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = GRID_TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Painter::DrawRectangle(x0, y0, width, height, Color::FILL);
    Painter::FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // ������ ������ �����������
    position = 0;
    while (index < ((int)sizeof(symbolsAlphaBet) / 4))
    {
        DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static pString strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    Painter::SetColor(Color::FILL);
    for (int i = 0; i < (int)sizeof(strings) / 4; i++)
    {
        Painter::DrawText(x0 + deltaX, y0 + 100 + deltaY * i, strings[i]);
    }
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    Painter::SetColor(Color::FILL);
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            x = Painter::DrawChar(x, y, *ch);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Painter::DrawChar(x, y, '%');
                x = Painter::DrawChar(x, y, (char)(0x30 | *(ch + 1)));
                x = Painter::DrawChar(x, y, 'N');
                ch++;
            }
            else
            {
                x = Painter::DrawText(x, y, symbolsAlphaBet[*ch + 0x40]);
            }
        }
        ch++;
    }
    Painter::FillRegion(x, y, 5, 8, Color::FLASH_10);
}

static void OnRegSet_Drive_Mask(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4);
}

DEF_PAGE_SB(        pppDrive_Mask,                                                                                    // ������ - ����� �� - ����� ///
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    &bDrive_Mask_Exit,      // ������ - ����� �� - ����� - �����
    &bDrive_Mask_Delete,    // ������ - ����� �� - ����� - �������
    0,
    0,
    &bDrive_Mask_Backspace, // ������ - ����� �� - ����� - Backspace
    &bDrive_Mask_Insert,    // ������ - ����� �� - ����� - ��������
    PageSB_Memory_Drive_Mask, &ppDrive, IsActive_Drive_Mask, OnPress_Drive_Mask, FuncDrawPage, OnRegSet_Drive_Mask
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDrive_Autoconnect,                                                                  //--- ������ - ����� �� - ��������������� ---
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FLASH_AUTOCONNECT, ppDrive, FuncActive, FuncChangedChoice, FuncDraw
)


DEF_PAGE_6(         ppDrive,                                                                                                  // ������ - ����� �� ///
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    pppDrive_Manager,       // ������ - ����� �� - �������
    cDrive_Name,            // ������ - ����� �� - ��� �����
    pppDrive_Mask,          // ������ - ����� �� - �����
    cDrive_SaveAs,          // ������ - ����� �� - ��������� ���
    cDrive_ModeBtnMemory,   // ������ - ����� �� - ��� �� ������
    cDrive_Autoconnect,     // ������ - ����� �� - ���������������
    Page_Memory_Drive, &pMemory, FuncActive, EmptyPressPage
)


DEF_PAGE_4(         pMemory,                                                                                                             // ������ ///
    "������", "MEMORY",
    "������ � ������� � ���������� �������.",
    "Working with external and internal memory.",
    cPoints,        // ������ - �����
    ppLast,         // ������ - ���������
    ppInternal,     // ������ - ����� ��
    ppDrive,        // ������ - ����� ��
    Page_Memory, &mainPage, FuncActive, EmptyPressPage
)

void Memory_SaveSignalToFlashDrive()
{
    if (FDRIVE_IS_CONNECTED)
    {
        if (FILE_NAMING_MODE_MANUAL)
        {
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SETNAME_TO = 0;
    }
}

static void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = GRID_TOP + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Painter::DrawRectangle(x0, y0, width, height, Color::FILL);
    Painter::FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = Painter::DrawText(x0 + deltaX, y0 + 65, FILE_NAME, Color::FILL);
    Painter::FillRegion(x, y0 + 65, 5, 8, Color::FLASH_10);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncForInternalExit()
{
    ((Page *)&ppLast)->SetCurrentSB();
}

static void OnPress_Internal_Exit()
{
    if (EXIT_FROM_ROM_TO_RAM)
    {
        MODE_WORK = ModeWork_RAM;
        EXIT_FROM_ROM_TO_RAM = 0;
        Menu::RunAfterUpdate(FuncForInternalExit);
    }
    else
    {
        MODE_WORK = ModeWork_Dir;
        if (RUN_FPGA_BEFORE_SB)
        {
            FPGA::Start();
            RUN_FPGA_BEFORE_SB = 0;
        }
        OnPressSB_Exit();
    }
}

DEF_SMALL_BUTTON_EXIT(  bInternal_Exit,                                                                            //--- ������ - ����� �� - ����� ---
    ppInternal, FuncActive, OnPress_Internal_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_ShowAlways()
{
    ALWAYS_SHOW_ROM_SIGNAL = ALWAYS_SHOW_ROM_SIGNAL ? 0u : 1u;
}

static void Draw_Internal_ShowAlways_Yes(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x66');
    Painter::SetFont(TypeFont_8);
}

static void Draw_Internal_ShowAlways_No(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x68');
    Painter::SetFont(TypeFont_8);
}

static void Draw_Internal_ShowAlways(int x, int y)
{
    if (ALWAYS_SHOW_ROM_SIGNAL == 0)
    {
        Draw_Internal_ShowAlways_No(x, y);
    }
    else
    {
        Draw_Internal_ShowAlways_Yes(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(   bInternal_ShowAlways,                                                      //--- ������ - ����� �� - ���������� ������ ---
    "���������� ������", "To show always",
    "��������� ������ ���������� ��������� ���������� ������ ������ ��������",
    "Allows to show always the chosen kept signal over the current",
    ppInternal, FuncActive, OnPress_Internal_ShowAlways, Draw_Internal_ShowAlways,
    Draw_Internal_ShowAlways_Yes,   {"���������� ��������� ������ �� ���������� ������ ������ ��������",
                                    "to show the chosen signal from internal memory over the current"},
    Draw_Internal_ShowAlways_No,    {"������ �� ���������� ������ ����� ������ � ������ ������ � ���������� ������������ �����������",
                                    "the signal from internal memory is visible only in an operating mode with an internal memory"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_ModeShow()
{
    CircleIncrease<int8>((int8 *)&SHOW_IN_INT, 0, 2);
}

static void Draw_Internal_ModeShow_Direct(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x6a');
    Painter::SetFont(TypeFont_8);
}


static void Draw_Internal_ModeShow_Saved(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x6c');
    Painter::SetFont(TypeFont_8);
}

static void Draw_Internal_ModeShow_Both(int x, int y)
{
    Painter::DrawText(x + 1, y + 5, "���");
    Painter::DrawVLine(x + 1, y + 2, y + 14, Color::BACK);
    Painter::DrawVLine(x + 2, y + 6, y + 11, Color::FILL);
}

static void Draw_Internal_ModeShow(int x, int y)
{
    if (SHOW_IN_INT_DIRECT)
    {
        Draw_Internal_ModeShow_Direct(x, y);
    }
    else if (SHOW_IN_INT_SAVED)
    {
        Draw_Internal_ModeShow_Saved(x, y);
    }
    else
    {
        Draw_Internal_ModeShow_Both(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_3(   bInternal_ModeShow,                                                              //--- ������ - ����� �� - ��� ������� ---
    "��� �������", "Type of a signal",
    "���������� ���������� ��� ������� ������ � ������ ����� ��",
    "Show recorded or current signal in mode Internal Memory",
    ppInternal, FuncActive, OnPress_Internal_ModeShow, Draw_Internal_ModeShow,
    Draw_Internal_ModeShow_Direct, {"�� ������� ������� ������", "on the display current signal"},
    Draw_Internal_ModeShow_Saved,  {"�� ������� ���������� ������", "on the display the kept signal"},
    Draw_Internal_ModeShow_Both,   {"�� ������� ��� �������", "on the display both signals"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_Delete()
{
    Display::FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    FLASHmem::DeleteData(NUM_ROM_SIGNAL);
    Display::FuncOnWaitStop();
}

static void Draw_Internal_Delete(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bInternal_Delete,                                                                            //--- ������ - ����� �� - ������� ---
    "�������", "Delete",
    "������� ��������� ������ �� ����������� ������������� ����������",
    "Removes the selected signal from the internal storage device",
    ppInternal, FuncActive, OnPress_Internal_Delete, Draw_Internal_Delete
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SaveSignalToIntMemory()
{
    // ������� � ��������� DS, DATA_A, DATA_B ������ �� ��� ��� ��������� ��������� ������, � ����������� �� ����, �� ������ ������ ����� � 
    // "������-����� ��"
    Reader::ReadFromRAM(EXIT_FROM_ROM_TO_RAM ? NUM_RAM_SIGNAL : 0, 0, false);

    if (DS)                                             // ���� ���� ��� ���������
    {
        FLASHmem::SaveData(NUM_ROM_SIGNAL, DS, IN_A, IN_B);   // �� ��������� ������ �� DS, DATA_A, DATA_B �� ����� NUM_ROM_SIGNAL � ����
        Display::ShowWarning(SignalIsSaved);
    }
}

static void OnPress_Internal_SaveToMemory()
{
    Display::FuncOnWaitStart(DICT(DStoredInMemory), false);
    SaveSignalToIntMemory();
    Display::FuncOnWaitStop();
}

static void Draw_Internal_SaveToMemory(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bInternal_SaveToMemory,                                                           //--- ������ - ����� �� - ��������� � ������ ---
    "��������� � ������", "Save to memory",
    "��������� ������ �� ���������� ������������ ����������",
    "To keep a signal in an internal memory",
    ppInternal, FuncActive, OnPress_Internal_SaveToMemory, Draw_Internal_SaveToMemory
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_SaveToDrive()
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_INT_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void Draw_Internal_SaveToDrive(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        Painter::SetFont(TypeFont_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        Painter::SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON(   bInternal_SaveToDrive,                                                           //--- ������ - ����� �� - ��������� �� ������ ---
    "��������� �� ������", "Save to disk",
    "��������� ������ �� ������",
    "Save signal to flash drive",
    ppInternal, FuncActive, OnPress_Internal_SaveToDrive, Draw_Internal_SaveToDrive
)


static void OnPress_Internal()
{
    MODE_WORK = ModeWork_ROM;
}

static void OnDraw_Internal()
{
    // ������ �������� ��������� ������

    bool exist[MAX_NUM_SAVED_WAVES] = {false};

    FLASHmem::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    char buffer[20];

    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Painter::FillRegion(x, y, width, 10, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : Color::BACK);
    Painter::DrawRectangle(x, y, width, 10, Color::FILL);
    Painter::SetColor(num == NUM_ROM_SIGNAL ? Color::FLASH_01 : Color::FILL);
    if (exist)
    {
        Painter::DrawText(x + 2, y + 1, Int2String(num + 1, false, 2, buffer));
    }
    else
    {
        Painter::DrawText(x + 3, y + 1, "\x88");
    }
}

static void OnRegSet_Internal(int delta)
{
    Sound::RegulatorSwitchRotate();
    if (delta < 0)
    {
        CircleDecrease<int8>(&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        CircleIncrease<int8>(&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    Painter::ResetFlash();
}

DEF_PAGE_SB(        ppInternal,                                                                                               // ������ - ����� �� ///
    "����� ��", "INT STORAGE",
    "������� � ����� ������ � ���������� �������",
    "Transition to an operating mode with internal memory",
    &bInternal_Exit,            // ������ - ����� �� - �����
    &bInternal_ShowAlways,      // ������ - ����� �� - ���������� ������
    &bInternal_ModeShow,        // ������ - ����� �� - ��� �������
    //0,
    //&bInternal_EraseAll,
    //&bInternal_Scale,         // ������ - ����� �� - �������
    &bInternal_Delete,          // ������ - ����� �� - �������
    &bInternal_SaveToMemory,    // ������ - ����� �� - ���������
    &bInternal_SaveToDrive,     // ������ - ����� �� - ��������� �� ������
    PageSB_Memory_Internal, &pMemory, FuncActive, OnPress_Internal, OnDraw_Internal, OnRegSet_Internal
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Exit()
{
    OnPressSB_Exit();
    if (EXIT_FROM_SETNAME_TO == RETURN_TO_DISABLE_MENU)
    {
        Menu::PagePointerFromName(PageSB_Memory_SetName)->ShortPressOnItem(0);
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_LAST_MEM)
    {
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_INT_MEM)
    {
    }
    EXIT_FROM_SETNAME_TO = RETURN_TO_DISABLE_MENU;
}

// ����� ��� ������ �� ������ ������� ����� ������������ �������. ������������ ������ ������ �� ����������
DEF_SMALL_BUTTON(   bSetName_Exit,                                                                                            // ��� ����� - ����� ///
    "�����", "Exit",
    "����� �� ����������",
    "Failure to save",
    pSetName, FuncActive, OnPress_SetName_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Delete()
{
    FILE_NAME[0] = '\0';
}

static void Draw_SetName_Delete(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSetName_Delete,                                                                                     //--- ��� ����� - ������� ---
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    pSetName, FuncActive, OnPress_SetName_Delete, Draw_SetName_Delete
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Backspace()
{
    int size = (int)strlen(FILE_NAME);
    if (size > 0)
    {
        FILE_NAME[size - 1] = '\0';
    }
}

static void Draw_SetName_Backspace(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSetName_Backspace,                                                                                //--- ��� ����� - Backspace ---
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    pSetName, FuncActive, OnPress_SetName_Backspace, Draw_SetName_Backspace
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Insert()
{
    int size = (int)strlen(FILE_NAME);
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = symbolsAlphaBet[INDEX_SYMBOL][0];
        FILE_NAME[size + 1] = '\0';
    }
}

static void Draw_SetName_Insert(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x26');
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSetName_Insert,                                                                                    //--- ��� ����� - �������� ---
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    pSetName, FuncActive, OnPress_SetName_Insert, Draw_SetName_Insert
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Save()
{
    if (FDRIVE_IS_CONNECTED)
    {
        OnPress_SetName_Exit();
        NEED_SAVE_TO_FLASHDRIVE = 1;
    }
}

static void Draw_SetName_Save(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        Painter::SetFont(TypeFont_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        Painter::SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON(   bSetName_Save,                                                                                     //--- ��� ����� - ��������� ---
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    pSetName, FuncActive, OnPress_SetName_Save, Draw_SetName_Save
)


static void OnRegSet_SetName(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4 - 7);
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    static pFuncVpI8I8I8 func[3] =
    {
        CircleDecrease<int8>,
        CircleDecrease<int8>,
        CircleIncrease<int8>
    };

    Painter::ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = (int8)(maxIndex - 1);
    }
    func[Sign(angle) + 1](&INDEX_SYMBOL, 0, (int8)(maxIndex - 1));
    Sound::RegulatorSwitchRotate();

}

DEF_PAGE_SB(        pSetName,                                                                         // �������� ���������� ��� ����� ����� ����� ///
    "", "",
    "",
    "",
    &bSetName_Exit,         // ���� ����� ����� - �����
    &bSetName_Delete,       // ���� ����� ����� - �������
    0,
    &bSetName_Backspace,    // ���� ����� ����� - Backspace
    &bSetName_Insert,       // ���� ����� ����� - ��������
    &bSetName_Save,         // ���� ����� ����� - ���������
    PageSB_Memory_SetName, 0, FuncActive, EmptyPressPage, FuncDrawPage, OnRegSet_SetName
)

/*

void DrawSB_MemLastSelect(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 3, y + 2, set.memory.strMemoryLast.isActiveModeSelect ? '\x2a' : '\x28');
    Painter::SetFont(TypeFont_8);
}

void PressSB_MemLastSelect()
{
    set.memory.strMemoryLast.isActiveModeSelect = !set.memory.strMemoryLast.isActiveModeSelect;
}


void DrawSB_MemExtNewFolder(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 1, y, '\x46');
    Painter::SetFont(TypeFont_8);
}

extern const PageBase pMemory;
extern const PageBase ppDrive;

*/

/*
// ������ - ����� �� - ������� �� -------------------------------------------------------------------------------------------------------------------
static const SButton bInternal_EraseAll =
{
Item_SmallButton, &ppInternal, 0,
{
"������� ��", "Erase all",
"������� ��� ������ �� ������� �������� ������, ������� ������� �����������. ��������� ��������������",
"It erases all data from the storage area, including the area inoformatsiya. The total format"
},
OnPress_Internal_EraseAll,
Draw_Internal_EraseAll,
{
{
Draw_Internal_EraseAll,
"������� ��� ������",
"Erase all data"
}
}
};

static void OnPress_Internal_EraseAll()
{
display.FuncOnWaitStart("������. ���������", "Erase. Wait", false);
FLASH_DeleteAllData();
display.FuncOnWaitStop();
}

static void Draw_Internal_EraseAll(int x, int y)
{
Painter::DrawText(x + 5, y + 5, "E");
}
*/

/*
void OnPress_Internal_Scale()
{
CircleIncreaseInt8((int8 *)&MEM_DATA_SCALE, 0, 1);
}
*/

/*
void Draw_Internal_Scale(int x, int y)
{
if (MEM_DATA_SCALE_RECALC)
{
Draw_Internal_Scale_Recalculated(x, y);
}
else
{
Draw_Internal_Scale_Original(x, y);
}
}
*/

/*
void Draw_Internal_Scale_Recalculated(int x, int y)
{
Painter::DrawText(x + 8, y + 2, DICT(DM));
Painter::SetFont(TypeFont_5);
Painter::DrawText(x + 5, y + 9, DICT(DABS));
Painter::SetFont(TypeFont_8);
}
*/

/*
void Draw_Internal_Scale_Original(int x, int y)
{
Painter::DrawText(x + 8, y + 2, DICT(DM));
Painter::SetFont(TypeFont_5);
Painter::DrawText(x + 5, y + 9, DICT(DREL));
Painter::SetFont(TypeFont_8);
}
*/

/*
DEF_SMALL_BUTTON_HINTS_2(   bInternal_Scale,                                                                     //--- ������ - ����� �� - ������� ---
"�������", "Scale",
"��������� ��� ��� ���������� ������ � ������� ����������",
"Whether or not to record the recorded signal to the current settings",
ppInternal, FuncActive, OnPress_Internal_Scale, Draw_Internal_Scale,
hintsInternal_Scale,
Draw_Internal_Scale_Recalculated,   "������ ������� � ������� ���������� ������������",
"The signal is given to the current oscilloscope settings",
Draw_Internal_Scale_Original,       "������ �������� � ����� ����, � ������� ������",
"The signal is drawn in the form in which the readout"
);
*/

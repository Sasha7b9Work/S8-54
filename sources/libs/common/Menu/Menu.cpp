// 2024/01/05 15:04:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Menu.h"
#include "Display/Grid.h"
#include "Display/DisplayTypes.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Pages/Definition.h"
#include "Menu/Pages/PageCursors.h"
#include "Menu/Pages/PageDisplay.h"
#include "Menu/Pages/PageHelp.h"
#include "Menu/Pages/PageMemory.h"
#include "Menu/Pages/PageChannels.h"
#include "Hardware/Panel.h"
#include "Utils/Math.h"
#include "stub.h"



        Key::E Menu::shortPressureButton = Key::Empty;
        Key::E Menu::longPressureButton = Key::Empty;
        Key::E Menu::pressButton = Key::Empty;
        Key::E Menu::releaseButton = Key::Empty;
PanelRegulator Menu::pressRegulator = R_Empty;
           int Menu::angleRegSet = 0;
      Control *Menu::itemUnderKey = 0;
       pFuncVV Menu::funcAterUpdate = 0;
        Key::E Menu::bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::Empty};
  const Key::E Menu::sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = { Key::F5, Key::F5, Key::F4, Key::F4, Key::F3, Key::F3, Key::F2, Key::F2, Key::F1, Key::F1 };
      Control *Menu::itemUnderButton[Key::Count] = {0};
   const char *Menu::stringForHint = 0;
      Control *Menu::itemHint = 0;



void Menu::UpdateInput()
{
    ProcessingShortPressureButton();
    ProcessingLongPressureButton();
    ProcessingRegulatorSetRotate();
    ProcessingPressButton();
    ProcessingReleaseButton();
    ProcessingRegulatorPress();
    SwitchSetLED();

    if (funcAterUpdate)
    {
        funcAterUpdate();
        funcAterUpdate = 0;
    }
}


void Menu::ShortPressureButton(Key::E button)
{
    if (!HINT_MODE_ENABLED)
    {
        if (button == Key::Memory && FDRIVE_IS_CONNECTED && MODE_BTN_MEMORY_IS_SAVE)
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }

        shortPressureButton = button;
    }
}


void Menu::LongPressureButton(Key::E button)
{
    if (!HINT_MODE_ENABLED)
    {
        longPressureButton = button;
        NEED_FINISH_DRAW = 1;
    }
}


void Menu::ProcessButtonForHint(Key::E button)
{
    if (button == Key::Menu)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня меню. Если текущей является "
            "корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня меню. Если текущая страница "
            "последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу."
            :
        "МЕНЮ button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the Menu::\n"
            "2. At the open menu deduction of the button during 0.5s closes the Menu::\n"
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of the Menu:: If the root page is "
            "current, the menu is closed.\n"
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the Menu:: If the current page the "
            "last in the current level, happens transition to the previous level of the Menu::\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == Key::Cursors)
    {
        Menu::stringForHint = LANG_RU ? 
            "Кнопка КУРСОРЫ открывает меню курсорных измерений."
            :
            "КУРСОРЫ button to open the menu cursor measurements.";
    }
    else if (button == Key::Display)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея."
            :
            "ДИСПЛЕЙ button opens the display settings Menu::";
    }
    else if (button == Key::Memory)
    {
        Menu::stringForHint = LANG_RU ?
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск."
            :
            "1. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measures)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ИЗМЕР открывает меню автоматических измерений."
            :
            "ИЗМЕР button opens a menu of automatic measurements.";
    }
    else if (button == Key::Help)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка ПОМОЩЬ открывает меню помощи.\n"
            "2. Нажатие и удержание кнопки ПОМОЩЬ в течение 0.5с включает и отключает режим вывода подсказок."
            :
            "1. ПОМОЩЬ button opens the help Menu::\n"
            "2. Pressing and holding the ПОМОЩЬ for 0.5s enables and disables output mode hints.";
    }
    else if (button == Key::Service)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка СЕРВИС открывает меню сервисных возможностей."
            :
            "СЕРВИС button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации."
            :
            "ПУСК/СTOП button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В."
            :
            "1. КАНАЛ1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button КАНАЛ1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В."
            :
            "1. КАНАЛ2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button КАНАЛ2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с."
            :
            "1. РАЗВ button open the settings menu sweep.\n"
            "2. Pressing and holding the button РАЗВ for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую "
            "настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень "
            "синхронизации 0В."
            :
            "1. СИНХР button opens a menu settings synchronization.\n"
            "2. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the "
            "trigger level.\n"
            "3. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
            "level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}


void Menu::PressButton(Key::E button)
{
    Sound::ButtonPress();
    if (HINT_MODE_ENABLED)
    {
        ProcessButtonForHint(button);
        return;
    }

    if (!MENU_IS_SHOWN)
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;
      
        if (memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS) == 0)
        {
            SHOW_DEBUG_MENU = 1;
            Display::ShowWarning(MenuDebugEnabled);
        }
    }
    pressButton = button;
}


void Menu::ReleaseButton(Key::E button)
{
    Sound::ButtonRelease();
    if (!HINT_MODE_ENABLED)
    {
        releaseButton = button;
    }
}


void Menu::PressReg(PanelRegulator reg)
{
    if (!HINT_MODE_ENABLED)
    {
        pressRegulator = reg;
    }
}


void Menu::RotateRegSetRight()
{   
    if (!HINT_MODE_ENABLED)
    {
        angleRegSet++;
        NEED_FINISH_DRAW = 1;
    }
}


void Menu::RotateRegSetLeft()
{
    if (!HINT_MODE_ENABLED)
    {
        angleRegSet--;
        NEED_FINISH_DRAW = 1;
    }
}


void Menu::SetAutoHide(bool)
{
    if(!MENU_IS_SHOWN)
    {
        return;
    }
    if(sDisplay_TimeMenuAutoHide() == 0)
    {
        Timer::Disable(kMenuAutoHide);
    }
    else
    {
        Timer::SetAndStartOnce(kMenuAutoHide, OnTimerAutoHide, (uint)sDisplay_TimeMenuAutoHide());
    }
}


char *Menu::StringNavigation(char buffer[100])
{
    buffer[0] = 0;
    const char * titles[10] = {0};
    int numTitle = 0;
    Control *item = OpenedItem();
    if(IsMainPage(item))
    {
        return 0;
    }
    while(!IsMainPage(item))
    {
        titles[numTitle++] = item->Title();
        item = (Control *)item->keeper;
    }
    for(int i = 9; i >= 0; i--)
    {
        if(titles[i])
        {
            strcat(buffer, titles[i]);
            if(i != 0)
            {
                strcat(buffer, " - ");
            }
        }
    }
    return buffer;
}


void Menu::OnTimerAutoHide()
{
    Menu::Show(false);
}


void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::Empty)
    {
        if (shortPressureButton == Key::Memory && MODE_BTN_MEMORY_IS_SAVE && FDRIVE_IS_CONNECTED)
        {
            EXIT_FROM_SETNAME_TO = (uint)(MENU_IS_SHOWN ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU);
            Memory_SaveSignalToFlashDrive();
            shortPressureButton = Key::Empty;
            return;
        }
        NEED_FINISH_DRAW = 1;
        Menu::SetAutoHide(true);

        Key::E button = shortPressureButton;

        do
        {
            if (button == Key::Help)
            {
                Panel::Long_Help();
            }
            else if(button == Key::Menu)                                   // Если нажата кнопка МЕНЮ и мы не находимся в режме настройки измерений.
            {
                if(!MENU_IS_SHOWN)
                {
                    Menu::Show(true);
                }
                else
                {
                    if (IS_PAGE(OpenedItem()))
                    {
                        Menu::TemporaryEnableStrNavi();
                    }
                    Menu::CloseOpenedItem();
                }
            }
            else if (MENU_IS_SHOWN && Panel::IsFunctionalButton(button))       // Если меню показано и нажата функциональная клавиша
            {
                void *item = itemUnderButton[button];
                if (HINT_MODE_ENABLED)
                {
                    SetItemForHint(item);
                }
                else
                {
                    if(item)
                    {
                        ((Control *)item)->ShortPress();
                    }
                }
            }
            else                                                        // Если меню не показано.
            {
                NamePage name = ((const Page *)OpenedItem())->GetNamePage();
                if(button == Key::ChannelA && name == Page_ChannelA && MENU_IS_SHOWN)
                {
                    SET_ENABLED_A = !SET_ENABLED_A;
                    PageChannels::OnChanged_InputA(true);
                    break;
                }
                if(button == Key::ChannelB && name == Page_ChannelB && MENU_IS_SHOWN)
                {
                    SET_ENABLED_B = !SET_ENABLED_B;
                    PageChannels::OnChanged_InputB(true);
                    break;
                }

                Page *page = (Page *)PageForButton(button);
                if(page && page != (Page *)PageHelp::pointer)
                {
                    page->SetCurrent(true);
                    page->Open(true);
                    Menu::TemporaryEnableStrNavi();
                    Menu::Show(true);
                }
            }
        } while(false);

        shortPressureButton = Key::Empty;
    }
}


void Menu::ProcessingLongPressureButton()
{
    Key::E button = longPressureButton;

    if(button != Key::Empty)
    {
        Control *item = OpenedItem();
        
        Sound::ButtonRelease();
        NEED_FINISH_DRAW = 1;
        SetAutoHide(true);

        if(button == Key::Time)
        {
            FPGA::SetTShift(0);
        }
        else if(button == Key::Trig)
        {
            FPGA::SetTrigLev(TRIGSOURCE, TrigLevZero);
        }
        else if(button == Key::ChannelA)
        {
            FPGA::SetRShift(A, RShiftZero);
        }
        else if(button == Key::ChannelB)
        {
            FPGA::SetRShift(B, RShiftZero);
        }
        else if(button == Key::Menu)
        {
            if (IS_PAGE_SB(OpenedItem()))
            {
                CloseOpenedItem();
            }
            else
            {
                Show(!MENU_IS_SHOWN);
                if (NOT_PAGE(item))
                {
                    TemporaryEnableStrNavi();
                }
            }
        }
        else if(MENU_IS_SHOWN && Panel::IsFunctionalButton(button))
        {
            item = (Control *)itemUnderButton[button];
            if(item)
            {
                item->LongPress();
            }
            if (NOT_PAGE(item))
            {
                TemporaryEnableStrNavi();
            }
        }
        longPressureButton = Key::Empty;
    }
}


void Menu::ProcessingRegulatorPress()
{
    if (pressRegulator != R_Empty)
    {
        SetAutoHide(true);
        if (pressRegulator == R_Set)
        {
            Show(!MENU_IS_SHOWN);
            if (NOT_PAGE(OpenedItem()))
            {
                TemporaryEnableStrNavi();
            }
        }

        pressRegulator = R_Empty;
    }
}


void Menu::ProcessingRegulatorSetRotate()
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (MENU_IS_SHOWN || NOT_PAGE(OpenedItem()))
    {
        Control *item = CurrentItem();
        const int step = 2;
        if (IS_PAGE(OpenedItem()) && (IS_CHOICE_REG(item) || IS_GOVERNOR(item) || IS_IP(item) || IS_MAC(item)))
        {
            if (angleRegSet > step || angleRegSet < -step)
            {
                ChangeItem(item, angleRegSet);
                angleRegSet = 0;
            }
            return;
        }
        else
        {
            item = OpenedItem();
            if (Menu::IsMinimize())
            {
                RotateRegSetSB(angleRegSet);
            }
            else if (IS_PAGE(item) || IS_IP(item) || IS_MAC(item) || IS_CHOICE(item) || IS_CHOICE_REG(item) || IS_GOVERNOR(item))
            {
                if (item->ChangeOpened(angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (IS_GOVERNOR_COLOR(item))
            {
                ChangeItem(item, angleRegSet);
            }
            else if (IS_TIME(item))
            {
                angleRegSet > 0 ? ((Time *)item)->IncCurrentPosition() : ((Time *)item)->DecCurrentPosition();
            }
        }
    }

    angleRegSet = 0;
}


void Menu::ProcessingPressButton()
{
    if((pressButton >= Key::F1 && pressButton <= Key::F5) || pressButton == Key::Menu)
    {
        if (pressButton != Key::Menu)
        {
            itemUnderKey = itemUnderButton[pressButton];
        }
    }
    if (pressButton == Key::Start && !MODE_WORK_IS_RAM)
    {
        FPGA::OnPressStartStop();
    }
    pressButton = Key::Empty;
}


void Menu::ProcessingReleaseButton()
{
    if((releaseButton >= Key::F1 && releaseButton <= Key::F5) || pressButton == Key::Menu)
    {
        itemUnderKey = 0;
        releaseButton = Key::Empty;
    }
}


void Menu::TemporaryEnableStrNavi()
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // Устанавливаем признак того, что надо выводить строку навигации меню
        Timer::SetAndStartOnce(kStrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // и запускаем таймер, который его отключит
    }
}


void Menu::OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}


void Menu::ShortPress_ChoiceReg(void *choice_)
{
    Choice *choice = (Choice*)choice_;

    if(!IS_ACTIVE(choice)) 
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    } 
    else if(OpenedItem() != choice) 
    {
        choice->SetCurrent(CurrentItem() != choice);
    }
}


void Menu::ShortPress_IP(void *item)
{
    if (OpenedItem() == item)
    {
        ((IPaddress*)item)->NextPosition();
    }
}


void Menu::ShortPress_MAC(void *item)
{
    if (OpenedItem() == item)
    {
        CircleIncrease<int8>(&gCurDigit, 0, 5);
    }
}


void Menu::ChangeStateFlashDrive()
{
    if(!FDRIVE_IS_CONNECTED)
    {
        if(GetNameOpenedPage() == PageSB_Memory_Drive_Manager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        OnPress_Drive_Manager();
    }
}


void Menu::OpenItemTime()
{
    Display::ShowWarning(TimeNotSet);
    ShortPressureButton(Key::Service);
    UpdateInput();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        RotateRegSetRight();
        UpdateInput();
        Display::Update();
    }
    ShortPressureButton(Key::F4);
    UpdateInput();
    Display::Update();
}


bool Menu::NeedForFireSetLED()
{
    Control *item = OpenedItem();
    
    if (!MENU_IS_SHOWN)
    {
        return IS_CHOICE_REG(item) || IS_CHOICE(item) || IS_GOVERNOR(item);
    }

    NamePage name = GetNameOpenedPage();
    if (
            name == PageSB_Debug_SerialNumber   ||
            name == PageSB_Service_FFT_Cursors  || 
            name == PageSB_Measures_Tune        || 
            name == PageSB_Memory_Last          || 
            name == PageSB_Memory_Internal      ||
            REG_SET_ACTIVE_ON_CURSORS           ||
            (name == PageSB_Service_Function && FUNC_MODE_DRAW_IS_ENABLED)
        )
    {
        return true;
    }
    
    item = CurrentItem();
    
    if (IS_GOVERNOR(item)       ||
        IS_CHOICE_REG(item)     ||
        IS_GOVERNOR_COLOR(item))
    {
        return true;
    }
    
    item = OpenedItem();

    if (IS_CHOICE(item)  ||
        (IS_PAGE(item) && ((const Page *)OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }

    return false;
}


void Menu::SwitchSetLED()
{
    static bool first = true;
    static bool prevState = false;  // true - горит, false - не горит

    bool state = NeedForFireSetLED();

    if (first)
    {
        first = false;
        Panel::EnableLEDRegSet(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        Panel::EnableLEDRegSet(state);
        prevState = state;
    }
}


void Menu::Show(bool show)
{
    MENU_IS_SHOWN = show;
    if (show)
    {
        Menu::TemporaryEnableStrNavi();
    }
    Menu::SetAutoHide(true);
}


void Menu::Init()
{
    INIT_PAGE_DISPLAY();
}


void Menu::RunAfterUpdate(pFuncVV func)
{
    funcAterUpdate = func;
}


Control *Menu::OpenedItem()
{
    TypeItem type = Item_None;
    return (Control *)RetLastOpened((Page *)&mainPage, &type);
}


NamePage Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetNamePage();
}


void *Menu::RetLastOpened(Page *page, TypeItem *type)
{
    if (page->CurrentItemIsOpened())
    {
        int8 posActItem = page->PosCurrentItem();
        void *item = page->Item(posActItem);
        if (IS_PAGE(page->Item(posActItem)))
        {
            return RetLastOpened((Page *)item, type);
        }
        else
        {
            return item;
        }
    }
    *type = Item_Page;
    return page;
}


Control *Menu::CurrentItem()
{
    TypeItem type = Item_None;
    void *lastOpened = RetLastOpened((Page *)&mainPage, &type);
    int8 pos = ((const Page *)lastOpened)->PosCurrentItem();
    if (type == Item_Page && pos != 0x7f)
    {
        return ((const Page *)lastOpened)->Item(pos);
    }
    return (Control *)lastOpened;
}


void Menu::CloseOpenedItem()
{
    Control *item = OpenedItem();
    if (IS_PAGE(item))
    {
        if (IS_PAGE_SB(item))
        {
            //((Page *)item)->SmallButonFromPage(0)->funcOnPress();
            SMALL_BUTTON_FROM_PAGE(item, 0)->funcOnPress();
        }
        NamePage name = KEEPER(item)->name;
        ((Page *)KEEPER(item))->SetPosActItem(MENU_POS_ACT_ITEM(name) & 0x7f);
        if (item == (Control *)&mainPage)
        {
            Menu::Show(false);
        }
    }
    else
    {
        item->Open(false);
    }
}


void Menu::ChangeItem(Control *item, int delta)
{
    if (IS_CHOICE(item) || IS_CHOICE_REG(item))
    {
        ((Choice *)item)->StartChange(delta);
    }
    else if (IS_GOVERNOR(item))
    {
        Governor *governor = (Governor*)item;
        if (OpenedItem() != governor)
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (IS_GOVERNOR_COLOR(item))
    {
        ((GovernorColor *)item)->ChangeValue(delta);
    }
}


Page *Menu::PagePointerFromName(NamePage)
{
    return 0;
}


static void DrawHintItem(int x, int y, int width)
{
    if (!Menu::itemHint)
    {
        return;
    }

    pString names[Item_NumberItems][2] =
    {
        {"",            ""},        // Item_None
        {"",            ""},        // Item_Choice
        {"Кнопка",      "Button"},  // Item_Button
        {"Страница",    "Page"},    // Item_Page
        {"",            ""},        // Item_Governor
        {"",            ""},        // Item_Time
        {"",            ""},        // Item_IP
        {"",            ""},        // Item_GovernorColor
        {"",            ""},        // Item_Formula
        {"",            ""},        // Item_MAC
        {"",            ""},        // Item_ChoiceReg
        {"Кнопка",      "Button"}   // Item_SmallButton
    };
    Language lang = LANG;
    Page *item = (Page *)Menu::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    snprintf(title, SIZE, "%s \"%s\"", names[Menu::itemHint->type][lang], item->titleHint[lang]);

    if (item->type == Item_SmallButton)
    {
        y -= 9;
    }
    Painter::DrawStringInCenterRectAndBoundItC(x, y, width, 15, title, Color::BACK, Color::FILL);
    y = Painter::DrawTextInBoundedRectWithTransfers(x, y + 15, width, item->titleHint[2 + lang], Color::BACK, Color::FILL);
    if (item->type == Item_SmallButton)
    {
        ((SButton*)item)->DrawHints(x, y, width);
    }
}


int Menu::CalculateX(int layer)
{
    return MP_X - layer * GRID_DELTA / 4;
}


void Menu::Draw()
{
    if (MENU_IS_SHOWN || NOT_PAGE(OpenedItem()))
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (MENU_IS_SHOWN)
        {
            if (IS_PAGE(item))
            {
                ((Page *)item)->Draw(CalculateX(0), GRID_TOP, true);
            }
            else
            {
                ((Page *)KEEPER(item))->Draw(CalculateX(0), GRID_TOP, true);
            }
        }
        else
        {
            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(CalculateX(0), GRID_TOP, false);
                Painter::DrawVLine(CalculateX(0), GRID_TOP + 1, GRID_TOP + 34, Color::BorderMenu(false));
                Painter::DrawVLine(CalculateX(0) + 1, GRID_TOP + 1, GRID_TOP + 34);
                Painter::DrawVLine(GRID_RIGHT, GRID_TOP + 30, GRID_TOP + 40, Color::FILL);
                Painter::DrawVLine(CalculateX(0) - 1, GRID_TOP + 1, GRID_TOP + 35, Color::BACK);
                Painter::DrawHLine(GRID_TOP + 35, CalculateX(0) - 1, GRID_RIGHT - 1);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(CalculateX(0), GRID_TOP, true);
                Painter::DrawHLine(GRID_TOP, CalculateX(0) - 2, GRID_RIGHT, Color::FILL);
                Painter::DrawVLine(GRID_RIGHT, GRID_TOP, GRID_TOP + 40);
            }
        }
    }

    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;
        if (MENU_IS_SHOWN)
        {
            width = Menu::IsMinimize() ? 289 : 220;
        }
        Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
               LANG_RU ? "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
               "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с." :
               "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
               "To disable this mode, press the button HELP and hold it for 0.5s.",
               Color::BACK, Color::FILL);
        y += LANG_RU ? 49 : 40;
        if (Menu::stringForHint)
        {
            Painter::DrawTextInBoundedRectWithTransfers(x, y, width, Menu::stringForHint, Color::BACK, Color::WHITE);
        }
        else if (Menu::itemHint)
        {
            DrawHintItem(x, y, width);
        }
    }
}


void Menu::ResetItemsUnderButton()
{
    for (int i = 0; i < Key::Count; i++)
    {
        itemUnderButton[i] = 0;
    }
}


bool Menu::IsMinimize()
{
    return IS_PAGE_SB(Menu::OpenedItem());
}


void Menu::RotateRegSetSB(int angle)
{
    Page *page = (Page *)OpenedItem();
    if (page->funcRegSetSB)
    {
        page->funcRegSetSB(angle);
    }
}


const SButton *Menu::GetSmallButton(Key::E button)
{
    if (Menu::IsMinimize() && button >= Key::Menu && button <= Key::F5)
    {
        Page *page = (Page *)OpenedItem();
        SButton *sb = (SButton *)page->items[button - Key::Menu];
        return sb;
    }
    return NULL;
}


void Menu::SetItemForHint(void *item)
{
    Menu::stringForHint = 0;
    Menu::itemHint = (Control *)item;
}

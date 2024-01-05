#pragma once

/// гДЕЯЭ ID БЯЕУ ОПЕДЯРЮБКЕММШУ Б ЛЕМЧ ЯРПЮМХЖ
typedef enum
{
    Page_Main,                      // цКЮБМЮЪ ЯРПЮМХЖЮ ЛЕМЧ
    Page_Display,                   // дхяоеи
    Page_Display_Accum,             // дхяокеи - мюйнокемхе
    Page_Display_Average,           // дхяокеи - сяпедмемхе
    Page_Display_Grid,              // дхяокеи - яерйю
    Page_Display_Settings,          // дхяокеи - мюярпнийх
    Page_Display_Settings_Colors,   // дхяокеи - мюярпнийх - жберю
    Page_ChannelA,                  // йюмюк 1
    Page_ChannelB,                  // йюмюк 2
    Page_Trig,                      // яхмуп
    Page_Trig_Search,               // яхмуп - онхяй
    Page_Time,                      // пюгбепрйю
    Page_Cursors,                   // йспянпш
    PageSB_Cursors_Set,             // йспянпш - сярюмнбхрэ
    Page_Memory,                    // оюлърэ
    PageSB_Memory_Last,             // оюлърэ - онякедмхе
    PageSB_Memory_Internal,         // оюлърэ - бмсрп гс
    Page_Memory_Drive,              // оюлърэ - бмеьм гс
    PageSB_Memory_Drive_Manager,    // оюлърэ - бмеьм гс - йюрюкнц
    PageSB_Memory_Drive_Mask,       // оюлърэ - бмеьм гс - люяйю
    PageSB_Memory_SetName,          // \brief оНЪБКЪЕРЯЪ ОЕПЕД ЯНУПЮМЕМХЕЛ ТЮИКЮ МЮ ТКЕЬЙС ДКЪ ГЮДЮМХЪ ХЛЕМХ ТЮИКЮ ОПХ ЯННРБЕРЯРБСЧЫЕИ НОЖХХ 
                                    // бмеьм гс - хЛЪ ТЮИКЮ
    Page_Measures,                  // хглепемхъ
    PageSB_Measures_Tune,           // хглепемхъ - мюярпнхрэ
    Page_Service,                   // яепбхя
    Page_Service_Calibrator,        // яепбхя - йюкхапюрнп
    Page_Service_Ethernet,          // яепбхя - ETHERNET
    Page_Service_FreqMeter,         // яепбхя - вюярнрнлеп
    Page_Service_Sound,             // яепбхя - гбсй
    Page_Service_RTC,               // яепбхя - бпелъ
    Page_Service_FFT,               // яепбхя - яоейрп
    PageSB_Service_FFT_Cursors,     // яепбхя - яоейрп - йспянпш
    PageSB_Service_Function,        // яепбхя - тсмйжхъ
    PageSB_Service_Information,     // яепбхя - хмтнплюжхъ
    PageSB_Service_Recorder,        // яепбхя - пецхярпюрнп
    PageSB_Help,                    // онлныэ
    Page_Debug,                     // нркюдйю
    Page_Debug_Console,             // нркюдйю - йнмянкэ
    Page_Debug_Console_Registers,   // нркюдйю - йнмянкэ - пецхярпш
    Page_Debug_ADC,                 // нркюдйю - южо
    Page_Debug_ADC_Balance,         // нркюдйю - южо - аюкюмя
    Page_Debug_ADC_Stretch,         // нркюдйю - южо - пюяръфйю
    Page_Debug_ADC_Shift,           // нркюдйю - южо - дно ялеы
    Page_Debug_ADC_AltShift,        // нркюдйю - южо - дно ялеы оюл
    Page_Debug_Rand,                // нркюдйю - пюмд-рнп
    Page_Debug_Channels,            // нркюдйю - йюмюкш
    PageSB_Debug_Settings,          // нркюдйю - мюярпнийх
    PageSB_Debug_SerialNumber,      // нркюдйю - я/м
    Page_NumPages,
    Page_NoPage
} NamePage;

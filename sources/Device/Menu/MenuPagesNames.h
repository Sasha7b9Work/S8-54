#pragma once

/// Çäåñü ID âñåõ ïğåäñòàâëåííûõ â ìåíş ñòğàíèö
typedef enum
{
    Page_Main,                      ///< Ãëàâíàÿ ñòğàíèöà ìåíş
    Page_Display,                   ///< ÄÈÑÏÅÉ
    Page_Display_Accum,             ///< ÄÈÑÏËÅÉ - ÍÀÊÎÏËÅÍÈÅ
    Page_Display_Average,           ///< ÄÈÑÏËÅÉ - ÓÑĞÅÄÍÅÍÈÅ
    Page_Display_Grid,              ///< ÄÈÑÏËÅÉ - ÑÅÒÊÀ
    Page_Display_Settings,          ///< ÄÈÑÏËÅÉ - ÍÀÑÒĞÎÉÊÈ
    Page_Display_Settings_Colors,   ///< ÄÈÑÏËÅÉ - ÍÀÑÒĞÎÉÊÈ - ÖÂÅÒÀ
    Page_ChannelA,                  ///< ÊÀÍÀË 1
    Page_ChannelB,                  ///< ÊÀÍÀË 2
    Page_Trig,                      ///< ÑÈÍÕĞ
    Page_Trig_Search,               ///< ÑÈÍÕĞ - ÏÎÈÑÊ
    Page_Time,                      ///< ĞÀÇÂÅĞÒÊÀ
    Page_Cursors,                   ///< ÊÓĞÑÎĞÛ
    PageSB_Cursors_Set,             ///< ÊÓĞÑÎĞÛ - ÓÑÒÀÍÎÂÈÒÜ
    Page_Memory,                    ///< ÏÀÌßÒÜ
    PageSB_Memory_Last,             ///< ÏÀÌßÒÜ - ÏÎÑËÅÄÍÈÅ
    PageSB_Memory_Internal,         ///< ÏÀÌßÒÜ - ÂÍÓÒĞ ÇÓ
    Page_Memory_Drive,              ///< ÏÀÌßÒÜ - ÂÍÅØÍ ÇÓ
    PageSB_Memory_Drive_Manager,    ///< ÏÀÌßÒÜ - ÂÍÅØÍ ÇÓ - ÊÀÒÀËÎÃ
    PageSB_Memory_Drive_Mask,       ///< ÏÀÌßÒÜ - ÂÍÅØÍ ÇÓ - ÌÀÑÊÀ
    PageSB_Memory_SetName,          ///< \brief Ïîÿâëÿåòñÿ ïåğåä ñîõğàíåíèåì ôàéëà íà ôëåøêó äëÿ çàäàíèÿ èìåíè ôàéëà ïğè ñîîòâåòñòâóşùåé îïöèè 
                                    ///< ÂÍÅØÍ ÇÓ - Èìÿ ôàéëà
    Page_Measures,                  ///< ÈÇÌÅĞÅÍÈß
    PageSB_Measures_Tune,           ///< ÈÇÌÅĞÅÍÈß - ÍÀÑÒĞÎÈÒÜ
    Page_Service,                   ///< ÑÅĞÂÈÑ
    Page_Service_Calibrator,        ///< ÑÅĞÂÈÑ - ÊÀËÈÁĞÀÒÎĞ
    Page_Service_Ethernet,          ///< ÑÅĞÂÈÑ - ETHERNET
    Page_Service_FreqMeter,         ///< ÑÅĞÂÈÑ - ×ÀÑÒÎÒÎÌÅĞ
    Page_Service_Sound,             ///< ÑÅĞÂÈÑ - ÇÂÓÊ
    Page_Service_RTC,               ///< ÑÅĞÂÈÑ - ÂĞÅÌß
    Page_Service_FFT,               ///< ÑÅĞÂÈÑ - ÑÏÅÊÒĞ
    PageSB_Service_FFT_Cursors,     ///< ÑÅĞÂÈÑ - ÑÏÅÊÒĞ - ÊÓĞÑÎĞÛ
    PageSB_Service_Function,        ///< ÑÅĞÂÈÑ - ÔÓÍÊÖÈß
    PageSB_Service_Information,     ///< ÑÅĞÂÈÑ - ÈÍÔÎĞÌÀÖÈß
    PageSB_Service_Recorder,        ///< ÑÅĞÂÈÑ - ĞÅÃÈÑÒĞÀÒÎĞ
    PageSB_Help,                    ///< ÏÎÌÎÙÜ
    Page_Debug,                     ///< ÎÒËÀÄÊÀ
    Page_Debug_Console,             ///< ÎÒËÀÄÊÀ - ÊÎÍÑÎËÜ
    Page_Debug_Console_Registers,   ///< ÎÒËÀÄÊÀ - ÊÎÍÑÎËÜ - ĞÅÃÈÑÒĞÛ
    Page_Debug_ADC,                 ///< ÎÒËÀÄÊÀ - ÀÖÏ
    Page_Debug_ADC_Balance,         ///< ÎÒËÀÄÊÀ - ÀÖÏ - ÁÀËÀÍÑ
    Page_Debug_ADC_Stretch,         ///< ÎÒËÀÄÊÀ - ÀÖÏ - ĞÀÑÒßÆÊÀ
    Page_Debug_ADC_Shift,           ///< ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ
    Page_Debug_ADC_AltShift,        ///< ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ ÏÀÌ
    Page_Debug_Rand,                ///< ÎÒËÀÄÊÀ - ĞÀÍÄ-ÒÎĞ
    Page_Debug_Channels,            ///< ÎÒËÀÄÊÀ - ÊÀÍÀËÛ
    PageSB_Debug_Settings,          ///< ÎÒËÀÄÊÀ - ÍÀÑÒĞÎÉÊÈ
    PageSB_Debug_SerialNumber,      ///< ÎÒËÀÄÊÀ - Ñ/Í
    Page_NumPages,
    Page_NoPage
} NamePage;

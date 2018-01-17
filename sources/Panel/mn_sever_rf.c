//Опрос состояния кнопок и ручек.
//Состояние кнопок и ручек передается по SPI(ведущий режим) и считывается состояние светодиодов каждые 10мс 
//Исходный Sever-S (поменялись все входы кнопок и ручек, добавились 8 шт.)

#include <18F2515.h>
#include <P18F2515.INC>

#use delay(internal = 8000000)

#zero_ram

typedef enum
{
    BTN_CHAN1       = 0x81,
    BTN_SERVICE     = 0x82,
    BTN_CHAN2       = 0x83,
    BTN_DISPL       = 0x84,
    BTN_TIME        = 0x85,
    BTN_MEMORY      = 0x86,
    BTN_TRIG        = 0x87,
    BTN_START       = 0x88,
    BTN_CURSOSR     = 0x89,
    BTN_MEAS        = 0x8a,
    BTN_HELP        = 0x8c,
    BTN_MENU        = 0x8d,
    BTN_F1          = 0x8e,
    BTN_F2          = 0x8f,
    BTN_F3          = 0x90,
    BTN_F4          = 0x91,
    BTN_F5          = 0x92,
    BTN_R_RANGE1    = 0x9c,
    BTN_R_RSHIFT1   = 0x9d,
    BTN_R_RANGE2    = 0x9e,
    BTN_R_RSHIFT2   = 0x9f,
    BTN_R_TBASE     = 0xa0,
    BTN_R_TSHIFT    = 0xa1,
    BTN_R_TRIGLEV   = 0xa2,
    BTN_R_SET       = 0xa3
} Button;

#define SL0 PIN_B0
#define SL1 PIN_B1
#define SL2 PIN_B2
#define SL3 PIN_B3
#define SL4 PIN_B4
#define SL5 PIN_B5

#define sw_K1 PIN_C0
#define sw_K2 PIN_C1
#define sw_Sx PIN_C2

/// Маски для установки последовательно SL0-SL5
const char maskSL[6] = {0x3e, 0x3d, 0x3b, 0x37, 0x2f, 0x1f};
/// Маски для проверки нажатой кнопки. Сравниваются с RL0-RL7 для данного SL
//const char maskRL[6] = {0xc9, 0xc9, 0xc9, 0x59, 0x7f, 0x49};
const char maskRL[6] = {0x36, 0x36, 0x36, 0xa6, 0x80, 0xb6};


void InitHardware(void);
void PressPowerOn(void);
void PressPowerOff(void);
char PressButton(char bit, char transDataPress);
static void PutInBufferWithRuk(char ib, char forZero);
static void PutInBuffer(char ib, char forZero);
static char PressButtons(char num, char bits[], char dataPress[]);
static char RotateSwitchGovernor(char forZero);
static char RotateGovernor(char cond, char b, char forZero);
/// Обработка SL0
static void FuncSL0(void);
/// Обработка SL1
static void FuncSL1(void);
/// Обработка SL2
static void FuncSL2(void);
/// Обработка SL3
static void FuncSL3(void);
/// Обработка SL4
static void FuncSL4(void);
/// Обработка SL5
static void FuncSL5(void);
static char FindStableChange(void); // Определим, произошло ли изменение состояния органая управления

typedef void(*pFuncVV)(void);

char curStateRB = 0;
char oldStateRB[6] = {0};
char i = 0;
char ib = 0;
char transData = 0;
char recvData = 0;
char recvPowerOn = 0;   // Если 1, то принята команда включения питания
char recvPowerOff = 0;  // Если 1, то принята команда выключения питания

static char bit1 = 0;
static char bit2 = 0;
static char bit4 = 0;
static char bit5 = 0;

void main()
{
    InitHardware();

    //СОСТОЯНИЕ кнопок и ручек по ВКЛЮЧЕНИЮ:
    //----------------------------------------
    for (i = 0; i < 6; i++)
    {
        OUTPUT_B(PORTB & 0xc0 | maskSL[i]);
        oldStateRB[i] = INPUT_A();
    }

    while (TRUE)
    {
        for (i = 0; i < 6; i++)
        {
            if (FindStableChange())         // Если состояние органов управление изменилось по сравнению с предыдущим
            {
                bit1 = curStateRB & 0x02;
                bit2 = curStateRB & 0x04;
                bit4 = curStateRB & 0x10;
                bit5 = curStateRB & 0x20;

                static const pFuncVV funcSL[] = {FuncSL0, FuncSL1, FuncSL2, FuncSL3, FuncSL4, FuncSL5};

                pFuncVV func = funcSL[i];
                func();

                oldStateRB[i] = INPUT_A(); 
            }

            if (recvPowerOff == 1)
            {
                OUTPUT_LOW(PIN_C7);
                recvPowerOff = 0;
            }
        }
    }
}


void PressPowerOn(void)
{
    OUTPUT_HIGH(PIN_C7);
    recvPowerOn = 1;
}

void PressPowerOff(void)
{
    OUTPUT_HIGH(sw_K1);
    OUTPUT_HIGH(sw_K2);
    OUTPUT_HIGH(sw_Sx);
    OUTPUT_LOW(PIN_C7);
    recvPowerOn = 0;
}

#define BIT_A(bit) (INPUT_A() & (1 << bit))

char PressButton(char bit, char transDataPress)
{
    if(!(curStateRB & (1 << bit)))
    {
        transData = transDataPress;
        while(!(BIT_A(bit)))                                                // Пока нажата соответствующая кнопка
        {
            if((transDataPress == BTN_CURSOSR) && !BIT_A(2) && !BIT_A(4))   // Если нажаты кнопки КУРСОРЫ, ДИСПЛЕЙ и ПАМЯТЬ
            {
                while(transData != 0) {};
                transData = 0x24;
                return 1;
            }
        };
        while(transData != 0) {};
        transData = transDataPress - 0x80;
        return 1;
    }
    return 0;
}

#undef BIT_A

static void PutInBufferWithRuk(char ib, char forZero)
{
    if (ib == 0)
    {
        transData = forZero; // Влево
    }
    else if (ib == 1)
    {
        transData = forZero + 0x80;  // Вправо
    }
}

static void PutInBuffer(char ib, char forZero)
{
    if (ib == 0)
    {
        transData = forZero;
    }
    else if (ib == 1)
    {
        transData = forZero + 0x80;
    }
}

static char PressButtons(char num, char bits[], char dataPress[])
{
    for (char i = 0; i < num; i++)
    {
        if (PressButton(bits[i], dataPress[i]))
        {
            return 1;
        }
    }

    return 0;
}

static char RotateSwitchGovernor(char forZero)
{
    if (bit1 && bit2)
    {
        for (ib = 0; ib < 2; ib++)
        {
            if ((curStateRB & (0x02 << ib)) && (!(oldStateRB[i] & (0x02 << ib))))
            {
                PutInBufferWithRuk(ib, forZero);
                return 1;
            }
        }
    }

    if (!bit1 && !bit2)
    {
        for (ib = 0; ib < 2; ib++)
        {
            if (!(curStateRB & (0x02 << ib)) && ((oldStateRB[i] & (0x02 << ib))))
            {
                PutInBufferWithRuk(ib, forZero);
                return 1;
            }
        }
    }

    return 0;
}

static char RotateGovernor(char cond, char b, char forZero)
{
    if (cond)
    {
        for (ib = 0; ib < 2; ib++)
        {
            if ((curStateRB & (b << ib)) && (!(oldStateRB[i] & (b << ib))))
            {
                PutInBuffer(ib, forZero);
                return 1;
            }
        }
    }
    return 0;
}

static void FuncSL0(void)
{
    static char bits[] = {0, 3, 7, 6};
    // №1   №28   № 29 
    static char data[] = {BTN_CHAN1, BTN_R_RANGE1, BTN_R_RSHIFT1, BTN_MENU};

    if (!PressButtons(4, bits, data))                       // Сначала проверяем кнопки на SL0
    {
        if (!RotateSwitchGovernor(0x14))                    // ВОЛЬТ/ДЕЛ 1
        {
            RotateGovernor(bit4 && bit5, 0x10, 0x15);     // RShift1
        }
    }
}

static void FuncSL1(void)
{
    static char bits[] = {0, 3, 7, 6};
    static char data[] = {BTN_CHAN2, BTN_R_RANGE2, BTN_R_RSHIFT2, BTN_F1};
    if (!PressButtons(4, bits, data))
    {
        if (!RotateSwitchGovernor(0x16))                    // ВОЛЬТ/ДЕЛ 2
        {
            RotateGovernor(bit4 && bit5, 0x10, 0x17);     // RShift2
        }
    }
}

static void FuncSL2(void)
{
    static char bits[] = {0, 3, 7, 6};
    static char data[] = {BTN_TIME, BTN_R_TBASE, BTN_R_TSHIFT, BTN_F2};

    if (!PressButtons(4, bits, data))
    {
        if (!RotateSwitchGovernor(0x18))                    // ВРЕМЯ/ДЕЛ
        {
            RotateGovernor(bit4 && bit5, 0x10, 0x19);     // TShift
        }
    }
}

static void FuncSL3(void)
{
    static char bits[] = {0, 3, 4, 6};
    static char data[] = {BTN_TRIG, BTN_R_TRIGLEV, BTN_START, BTN_F3};

    if (!PressButtons(4, bits, data))
    {
        RotateGovernor(bit1 && bit2, 0x02, 0x1a);        // TrigLev
    }
}

static void FuncSL4(void)
{
    static char bits[] = {0, 1, 2, 3, 4, 5, 6};
    static char data[] = {BTN_CURSOSR, BTN_MEAS, BTN_DISPL, BTN_HELP, BTN_MEMORY, BTN_SERVICE, BTN_F4};

    PressButtons(7, bits, data);
}

static void FuncSL5(void)
{
    //Pit,Men "5", Сброс Установки:

    if (!(curStateRB & (1 << 0)))  //Pit
    {
        if (recvPowerOn == 0)
        {
            PressPowerOn();
            DELAY_MS(1);  //для настройки STM
            do
                curStateRB = INPUT_A();
            while (!(curStateRB & (1 << 0)));
            //отжатие произошло
        }
        else
        {
            while (transData != 0)
            {
            }
            transData = 0x8B;  //№11-наж.
        }
        return;
    }

    static char bits[] = {3, 6};
    static char data[] = {BTN_R_SET, BTN_F5};

    if (!PressButtons(2, bits, data))
    {
        RotateGovernor(bit1 && bit2, 0x02, 0x1b);        // УСТАНОВКА
    }
}

static char FindStableChange(void)
{
    OUTPUT_B(PORTB & 0xc0 | maskSL[i]);
    curStateRB = INPUT_A();

    if (oldStateRB[i] != curStateRB)
    {
        if ((curStateRB & maskRL[i]) != maskRL[i])    //если это кнопка, то отрабат. дребезг
        {
            for (int i = 0; i < 3; i++)
            {
                if (INPUT_A() != curStateRB)
                {
                    return FALSE;
                }
                DELAY_MS(1);
            }
        }
        else if(i == SL4)
        {
            curStateRB = oldStateRB[i];
            return 0;
        }
        else        // а это ручка
        {
            for (int i = 0; i < 5; i++)
            {
                if (INPUT_A() != curStateRB)
                {
                    return FALSE;
                }
                DELAY_US(100);
            }
        }
        return TRUE;
    }

    return FALSE;
}

static void InitHardware()
{
    DISABLE_INTERRUPTS(GLOBAL);     // общий запрет прерываний
    SETUP_OSCILLATOR(OSC_8MHZ);     // |OSC_INTRC

    SET_TRIS_A(0xff);   // входы
    SET_TRIS_B(0x00);   // выходы
    SET_TRIS_C(0x10);   // выходы, RC4-вход(данные от STM)

    PORTC = 0xcf;
    PORTB = 0xff;

    PressPowerOff();

    //Инициализация модуля MSSP(режим SPI)
    SSPCON1 = 0x31;
    SSPSTAT = 0;        // 0x80;
    SSPBUF = 0x55;

    //иниц. таймеров:
    //t0con=0xc8;   // 8-ми разр
    T0CON = 0x88;   // 16-ми разр счетчик
    T1CON = 0x85;
    SETUP_TIMER_1(T1_INTERNAL | T1_DIV_BY_1);

    SET_TIMER1(62286);
    //set_timer0(64536); 

    //enable_interrupts(INT_TIMER2);
    ENABLE_INTERRUPTS(INT_TIMER1);
    //enable_interrupts(INT_TIMER0);
    ENABLE_INTERRUPTS(GLOBAL);
}


#INT_TIMER1 //Приходят каждые 10ms

void Prd_kn(void)  //По SPI перед. сост. ПП и прин сообщения от STM
{
    while (!(SSPSTAT & 0x01))
    {
    }   // ждем  завершения приема
    recvData = SSPBUF;               // чтение буфера SPI
    char kod = recvData & 0x7f;

    static char buttons[4] = {0, sw_K1, sw_K2, sw_Sx};

    if ((kod > 0) && (kod < 4))         // св.: Kan1,Kan2,Sinchr,
    {
        if (recvData & (1 << 7))         // зажигаем или гасим светодиод:
        {
            OUTPUT_LOW(buttons[kod]);   // зажигаем - (0)
        }
        else
        {
            OUTPUT_HIGH(buttons[kod]);  //гасим - (1)
        }
    }
    else
    {
        if ((kod == 4) && (recvPowerOn == 1))
        {
            recvPowerOff = 1;
            PressPowerOff();
        }
    }

    SSPCON1 = SSPCON1 & 0x7f;   //сброс бита конфликта
    //Буфер готов к передаче:   

    OUTPUT_HIGH(PIN_C6);
    // spi_write(transData); //запись байта в SPI
    SSPBUF = transData;
    transData = 0;
    OUTPUT_LOW(PIN_C6);

    SET_TIMER1(45536);  //65536-(0,01/(4/8.000.000))=20000 -- (10ms)  
}
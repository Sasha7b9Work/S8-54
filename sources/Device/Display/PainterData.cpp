#include "PainterData.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Hardware/RAM.h"
#include "Menu/Pages/PageMemory.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Нарисовать данные, которые рисовались бы, если б был установлен режим ModeWork_Dir.
static void DrawData_ModeDir();
/// Нарисовать данные, которые рисовались бы, если б был установлен режим ModeWork_RAM.
static void DrawData_ModeRAM();
/// Нарисовать данные, которые рисовались бы, если б был установлен режим ModeWork_ROM.
static void DrawData_ModeROM();
/// Нарисовать данные из outA, outB.
static void DrawData(bool forAccum);
/// Нарисовать данные из outA или outB.
static void DrawChannel(Channel ch, Color color);
/// Нарисовать данные из outA или outB c выключенным пиковым детектором.
static void DrawChannel_Normal(Channel ch, int left, int bottom, float scaleY);
/// Нарисовать данные из outA или outB с включённым пиковым детектором.
static void DrawChannel_PeakDet(Channel ch, int left, int bottom, float scaleY);
/// \brief Используется в режиме пикового детектора. В in хранятся два значения, соответствующие максимальному и минимальному. 
/// Они перемещаются в out в возрастающем порядке out[0] = min, out[1] = max. Возвращает false, если точка не считана - хотя бы одно значение == 0.
static bool CalcMinMax(uint8 in[2], uint8 out[2]);
/// Возвращает true, если изогражение сигнала выходит за пределы экрана.
static bool DataBeyondTheBorders(const uint8 *data, int firstPoint, int lastPoint);
/// \brief Выоводит сообщение на экране о выходе сигнала за границы экрана.
/// delta - расстояние от края сетки, на котором находится сообщение. Если delta < 0 - выводится внизу сетки
static void DrawLimitLabel(int delta);

static void DrawChannel_Math(uint8 *dataIn);

static int FillDataP2P(uint8 *data, DataSettings **ds);

static void DrawMarkersForMeasure(Channel ch);

static void DrawSignalLined(const uint8 *data, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX, bool calculateFiltr);

static void DrawSignalPointed(const uint8 *data, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX);

static int FillDataP2PforRecorder(int numPoints, int numPointsDS, int pointsInScreen, uint8 *src, uint8 *dest);

static int FillDataP2PforNormal(int numPoints, int numPointsDS, int pointsInScreen, uint8 *src, uint8 *dest);

static void DrawDataInRect(int width, Channel ch);

static void DrawTPos(int leftX, int rightX);

static void DrawTShift(int leftX, int rightX, int numPoints);

static int Ordinate(uint8 x, float scale);
/// Возвращает точку в экранной координате. Если точка не считана (NONE_VALUE), возвращает -1.
static void SendToDisplayDataInRect(Channel chan, int x, int *min, int *max, int width);
/// Нарисовать данные в окне памяти
static void DrawMemoryWindow();

static void IncreaseNumDrawingSignals();

#define CONVERT_DATA_TO_DISPLAY(out, inVal)                     \
    int in = inVal;                                             \
    if(in < MIN_VALUE) { in = MIN_VALUE; }                      \
    if(in > MAX_VALUE) { in = MAX_VALUE; }                      \
    out = (uint8)(maxY - (int)(((in) - MIN_VALUE) * scaleY));   \
    if(out < (uint8)minY)   { out = (uint8)minY; }              \
    if(out > (uint8)maxY)   { out = (uint8)maxY; };

/// Размещает два значения по возрастанию : val1 - меньшее, val2 - большее
#define PLACE_2_ASCENDING(v1, v2) if((v1) > (v2)) { int qwerty = v1; v1 = v2; v2 = qwerty; }


static bool interruptDrawing = false;
static Channel curCh = A;           ///< Текущий ресуемый канал.
static StructDataDrawing *dataStruct = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PainterData::InterruptDrawing()
{
    interruptDrawing = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawData()
{
    dataStruct = (StructDataDrawing *)malloc(sizeof(StructDataDrawing));

    interruptDrawing = false;

    // Нормальный режим
    if (MODE_WORK_IS_DIR)                              // Установленный режим - непосредственный
    {
        if (ALWAYS_SHOW_ROM_SIGNAL)                 // Если нужно показывать сигнал из ППЗУ
        {
            DrawData_ModeROM();
        }
        DrawData_ModeDir();
    }
    // ПАМЯТЬ - ПОСЛЕДНИЕ
    else if (MODE_WORK_IS_RAM)
    {
        DrawData_ModeRAM();
        DrawMemoryWindow();
    }
    // ПАМЯТЬ - ВНУТР ЗУ
    else
    {
        if (SHOW_IN_INT_BOTH || SHOW_IN_INT_SAVED)
        {
            DrawData_ModeROM();
            DrawMemoryWindow();
        }
        if (SHOW_IN_INT_BOTH || SHOW_IN_INT_DIRECT) // Если нужно показывать не только сохранённый сигнал
        {
            if (EXIT_FROM_ROM_TO_RAM)               // и мы перешли на страницу "ПАМЯТЬ-ВНУТР ЗУ" со страницы "ПАМЯТЬ-ПОСЛЕДНИЕ"
            {
                DrawData_ModeRAM();
                DrawMemoryWindow();
            }
            else                                    // А если перешли из нормального режим
            {
                DrawData_ModeDir();                 // То нарисуем сигнал нормального режима
                DrawMemoryWindow();
            }
        }
    }
    
    free(dataStruct);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawData_ModeDir()
{  
    Reader::ReadFromRAM(0, dataStruct, true);
    DrawMemoryWindow();

    if (MODE_ACCUM_NO_RESET && !IN_P2P_MODE && ENUM_ACCUM > ENumAccum_1)
    {
        int numAccum = NUM_ACCUM;
        int numSignalsInStorage = Storage::NumElementsWithCurrentSettings();
        if (numSignalsInStorage < numAccum)
        {
            numAccum = numSignalsInStorage;
        }

        if (ENUM_ACCUM_INF)                     // Если бесконечное накопление
        {
            numAccum = numSignalsInStorage;     // то будем выводить все сигналы
        }

        int i = 0;
        while (i < numAccum && !interruptDrawing)
        {
            Reader::ReadFromRAM(i, dataStruct, false);
            DrawData(true);
            ++i;
        }
    }
    
    if (!IN_RANDOM_MODE && !IN_P2P_MODE)
    {
        Reader::ReadMin(dataStruct);
        DrawData(true);

        Reader::ReadMax(dataStruct);
        DrawData(true);
    }

    Reader::ReadFromRAM(0, dataStruct, false);
    DrawData(false);

    IncreaseNumDrawingSignals();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void IncreaseNumDrawingSignals()
{
    static uint8 *addr = 0;

    if (addr == 0 && DS)
    {
        addr = DS->addr;
    }
    else if (DS && addr != DS->addr)
    {
        NUM_DRAWING_SIGNALS++;
        addr = DS->addr;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawData_ModeRAM()
{
    Reader::ReadFromRAM(NUM_RAM_SIGNAL, dataStruct, false);
    DrawData(false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawData_ModeROM()
{
    Reader::ReadFromROM(dataStruct);

    if (NUM_BYTES_SET == NUM_BYTES_DS)
    {
        DrawData(false);
    }
    else
    {
        pString str1[2] = {"Для просмотра содержимого", "To view the contents of the"};
        pString str2[2] = {"внутреннего ЗУ", "internal storage device"};
        pString str3[2] = {"установите длину памяти %s", "set the memory length %s"};

        int x = Grid::Left() + 10;
        int width = 260;
        int height = 60;
        int y = (Grid::ChannelBottom() + GRID_TOP) / 2 - height / 2;

        int x0 = x + 3;
        int y0 = y + 3;

        Painter::FillRegion(x, y, width, height, Color::BACK);
        Painter::DrawBigText(x0 + 2, y0 + 2, 2, str1[LANG], Color::FILL);
        Painter::DrawBigText(x0 + 2, y0 + 20, 2, str2[LANG]);
        char buffer[100];
        sprintf(buffer, (char *)str3[LANG], namesLengthMemory[ENUM_POINTS_DS]);
        Painter::DrawBigText(x0 + 2, y0 + 38, 2, buffer);

        Painter::DrawRectangle(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight(), Color::FILL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawData(bool forAccum)
{
    if (DS)
    {
        static const Channel order[NumChannels][2] = { {B, A}, {A, B} };

        Channel ch = order[LAST_AFFECTED_CH][0];

        DrawChannel(ch, forAccum ? Color::ChanAccum(ch) : Color::CHAN[ch]);

        ch = order[LAST_AFFECTED_CH][1];

        DrawChannel(ch, forAccum ? Color::ChanAccum(ch) : Color::CHAN[ch]);
    }

    Painter::DrawRectangle(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight(), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannel(Channel ch, Color color)
{
    if (!dataStruct->needDraw[ch])
    {
        return;
    }

    Painter::SetColor(color);

    int left = Grid::Left();
    int bottom = Grid::ChannelBottom();
    int top = GRID_TOP;

    float scaleY = (bottom - top) / (float)(MAX_VALUE - MIN_VALUE + 1);

    if(PEAKDET_DS)
    {
        DrawChannel_PeakDet(ch, left, bottom, scaleY);
    }
    else
    {
        DrawChannel_Normal(ch, left, bottom, scaleY);
    }

    Painter::DrawVLine(left + dataStruct->posBreak, top, bottom, Color::GRID);

    DrawMarkersForMeasure(ch);

    Painter::RunDisplay();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannel_Normal(Channel ch, int left, int bottom, float scaleY)
{
    uint8 *data = dataStruct->data[ch];

    if (DataBeyondTheBorders(data, 0, 281))
    {
        return;
    }

    float k = bottom + MIN_VALUE * scaleY;

    for(int i = 0; i < 280; ++i)                        /// \todo Последня точка не рисуется.
    {
        uint8 val = data[i];

        if(val == NONE_VALUE || data[i + 1] == NONE_VALUE)
        {
            continue;                                   // Если это значение отсутствует - переходим к следующей точке
        }

        LIMITATION(val, MIN_VALUE, MAX_VALUE);

        int y = (int)(k - val * scaleY);                //int y = bottom - (val - MIN_VALUE) * scaleY;

        int x = left + i;

        if(MODE_DRAW_SIGNAL_POINTS)
        {
            Painter::SetPoint(x, y);
        }
        else
        {
            int yNext = (int)(k - data[i + 1] * scaleY);       //int yNext = bottom - (data[i + 1] - MIN_VALUE) * scaleY;

            if(yNext < y)
            {
                Painter::DrawVLine(x, y, yNext + 1);
            }
            else if(yNext > y)
            {
                Painter::DrawVLine(x, y, yNext - 1);
            }
            else
            {
                Painter::SetPoint(x, y);
            }

        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannel_PeakDet(Channel ch, int left, int bottom, float scaleY)
{
    float k = bottom + MIN_VALUE * scaleY;

    for(int i = 0; i < 280; i++)
    {

#define Y_MIN in[0]
#define Y_MAX in[1]
#define Y_MIN_NEXT inNext[0]
#define Y_MAX_NEXT inNext[1]

        uint8 in[2];            // Здесь будут храниться отсортированные по возрастанию значения

        uint8 *data = dataStruct->data[ch];
        
        if(!CalcMinMax(data + i * 2, in))
        {
            continue;
        }
        
        uint8 inNext[2];
        if(!CalcMinMax(data + (i + 1) * 2, inNext))
        {
            Y_MIN_NEXT = Y_MIN;
            Y_MAX_NEXT = Y_MAX;
        }

        int min = (int)(k - Y_MAX * scaleY);
        int max = (int)(k - Y_MIN * scaleY);
        int minNext = (int)(k - Y_MAX_NEXT * scaleY);
        int maxNext = (int)(k - Y_MIN_NEXT * scaleY);

        int x = left + i;

        if(maxNext < min)
        {
            min = maxNext + 1;
        }
        if(minNext > max)
        {
            max = minNext - 1;
        }

        if(MODE_DRAW_SIGNAL_POINTS)
        {
            Painter::SetPoint(x, min);
            Painter::SetPoint(x, max);
        }
        else
        {
            if(min == max)
            {
                Painter::SetPoint(x, min);
            }
            else
            {
                Painter::DrawVLine(x, min, max);
            }
        }
    }

#undef Y_MIN
#undef Y_MAX
#undef Y_MIN_NEXT
#undef Y_MAX_NEXT
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool CalcMinMax(uint8 in[2], uint8 out[2])
{
    uint8 val1 = in[0];
    uint8 val2 = in[1];
    if(val1 == 0 || val2 == 0)
    {
        return false;
    }

    LIMITATION(val1, MIN_VALUE, MAX_VALUE);
    LIMITATION(val2, MIN_VALUE, MAX_VALUE);

    if(val1 < val2)
    {
        out[0] = val1;
        out[1] = val2;
    }
    else
    {
        out[0] = val2;
        out[1] = val1;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool DataBeyondTheBorders(const uint8 *data, int firstPoint, int lastPoint)
{
    if (IN_P2P_MODE)
    {
        return false;   /// \todo временно, потому что в этой функции висло иногда при переключении на поточечный вывод
    }
    int numMin = 0; // Здесь количество отсчётов, меньших или равных MIN_VALUE
    int numMax = 0; // Здесь количество отсчётов, больших или равных MAX_VALUE
    int numPoints = lastPoint - firstPoint;
    for (int i = firstPoint; i < lastPoint; i++)
    {
        if (data[i] != NONE_VALUE)
        {
            if (data[i] <= MIN_VALUE)
            {
                numMin++;
            }
            if (data[i] >= MAX_VALUE)
            {
                numMax++;
            }
        }
    }
    if (numMin >= numPoints - 1)
    {
        DrawLimitLabel(-10);
        return true;
    }
    else if (numMax >= numPoints - 1)
    {
        DrawLimitLabel(10);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawLimitLabel(int delta)
{
    int width = 150;
    int height = 20;

    Color color = Painter::GetColor();

    int x = Grid::Width() / 2 - width / 2 + Grid::Left();
    int y = 0;
    if (delta < 0)
    {
        y = Grid::FullBottom() + delta - height;
    }
    else
    {
        y = GRID_TOP + delta;
    }

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, color);
    Painter::DrawStringInCenterRect(x, y, width, height, "Сигнал за пределами экрана");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawMath()
{
    if (!FUNC_MODE_DRAW_IS_ENABLED || Storage::GetData_RAM(A, 0) == 0 || Storage::GetData_RAM(B, 0) == 0)
    {
        return;
    }

    float *dataAbsA = (float *)RAM8(DRAW_MATH_DATA_REL_A);
    float *dataAbsB = (float *)RAM8(DRAW_MATH_DATA_REL_B);

    int numPoints = NUM_BYTES_DS;

    mathFPGA.PointsRel2Voltage(OUT_A, numPoints, RANGE_DS_A, (int16)RSHIFT_DS_A, dataAbsA);
    mathFPGA.PointsRel2Voltage(OUT_B, numPoints, RANGE_DS_B, (int16)RSHIFT_DS_B, dataAbsB);

    Math::CalculateMathFunction(dataAbsA, dataAbsB, numPoints);

    uint8 points[FPGA_MAX_POINTS];

    mathFPGA.PointsVoltage2Rel(dataAbsA, numPoints, SET_RANGE_MATH, (int16)SET_RSHIFT_MATH, points);

    DrawChannel_Math(points);

    static const int WIDTH = 71;
    static const int HEIGHT = 10;
    int delta = (SHOW_STRING_NAVI_ALL && FUNC_MODE_DRAW_IS_TOGETHER) ? 10 : 0;
    Painter::DrawRectangle(Grid::Left(), Grid::MathTop() + delta, WIDTH, HEIGHT, Color::FILL);
    Painter::FillRegion(Grid::Left() + 1, Grid::MathTop() + 1 + delta, WIDTH - 2, HEIGHT - 2, Color::BACK);
    Divider divider = set.math_Divider;
    Painter::DrawText(Grid::Left() + 2, Grid::MathTop() + 1 + delta, sChannel_Range2String(SET_RANGE_MATH, divider), Color::FILL);
    Painter::DrawText(Grid::Left() + 25, Grid::MathTop() + 1 + delta, ":");
    char buffer[20];
    Painter::DrawText(Grid::Left() + 27, Grid::MathTop() + 1 + delta, sChannel_RShift2String(SET_RSHIFT_MATH, SET_RANGE_MATH, divider, buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannel_Math(uint8 *dataIn)
{
    int minY = Grid::MathTop();
    int maxY = Grid::MathBottom();

    bool calculateFiltr = true;
//    int sizeBuffer = NUM_BYTES_DS;
    
#define SIZE_BUFFER (16 * 1024)
    
    uint8 data[SIZE_BUFFER];
  
    BitSet64 points = {0};

    if (!IN_P2P_MODE ||                                // Если не находимся в режиме медленных поточечных развёрток
        (IN_P2P_MODE && TIME_MS(DS)))                  // Или в поточечном, но данные уже считаны
    {
        points = sDisplay_PointsOnDisplay();           // то находим первую и последнюю точки, выводимые на экран
    }

    if (IN_P2P_MODE &&                                 // Если находимся в режиме медленных поточечных развёрток
        TIME_MS(DS) == 0)                              // и считывание полного набора данных ещё не произошло
    {
        points.sword1 = FillDataP2P(data, &DS);
        if (points.word1 < 2)                          // Если готово меньше двух точек - выход
        {
            return;
        }
        dataIn = data;
    }

    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return;
    }

    float scaleY = (float)(maxY - minY) / (MAX_VALUE - MIN_VALUE);
    float scaleX = (float)Grid::Width() / 280.0f;

    Painter::SetColor(Color::CHAN[MathCh]);

    //    if (!DataBeyondTheBorders(dataIn, firstPoint, lastPoint))   // Если сигнал не выходит за пределы экрана
    {
        if (MODE_DRAW_SIGNAL_LINES)
        {
            DrawSignalLined(dataIn, points.sword0, points.sword1, minY, maxY, scaleY, scaleX, calculateFiltr);
        }
        else
        {
            DrawSignalPointed(dataIn, points.sword0, points.sword1, minY, maxY, scaleY, scaleX);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int FillDataP2P(uint8 *data, DataSettings **ds)
{
    int pointsInScreen = 281;
    if (SET_PEAKDET_EN)
    {
        pointsInScreen *= 2;
    }

    uint8 *dataA = 0;
    uint8 *dataB = 0;

    int numPoints = Storage::GetFrameP2P_RAM(ds, &dataA, &dataB); // Получаем фрейм поточечного вывода

    int numPointsDS = (*ds)->BytesInChannel();

    uint8 *dat[] = {dataA, dataB};

    return RECORDER_MODE ?
        FillDataP2PforRecorder(numPoints, numPointsDS, pointsInScreen, dat[curCh], data) :   // Это возвращаем, если включен режим регистратора
        FillDataP2PforNormal(numPoints, numPointsDS, pointsInScreen, dat[curCh], data);      // А это в нормальном режиме
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int FillDataP2PforRecorder(int numPoints, int numPointsDS, int pointsInScreen, uint8 *src, uint8 *dest)
{
    // Если точек меньше, чем вмещается на экран - просто копируем их в буфер
    if (numPoints <= pointsInScreen)
    {
        RAM::MemCpy16(src, dest, numPoints);
        return numPoints;
    }

    // Если точек больше, то в буфер копируем последние 281
    int allPoints = numPoints <= numPointsDS ? numPoints : numPointsDS;
    int startIndex = allPoints - pointsInScreen;
    RAM::MemCpy16(src + startIndex, dest, pointsInScreen);
    return pointsInScreen;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int FillDataP2PforNormal(int numPoints, int numPointsDS, int pointsInScreen, uint8 *src, uint8 *dest)
{
    int deltaNumPoints = numPoints - numPointsDS;
    if (deltaNumPoints < 0)
    {
        deltaNumPoints = 0;
    }

    if (numPoints > 0)
    {
        RAM::MemCpy16(src, dest, numPoints < numPointsDS ? numPoints : numPointsDS);
    }

    //    int kP2P = SET_PEAKDET_EN ? 2 : 1;

    if (numPoints > pointsInScreen)
    {
        int numScreens = numPoints / pointsInScreen;                                                        // Число полных нарисованных экранов.

#undef SIZE_BUFFER
#define SIZE_BUFFER (281 * 2)
        uint8 dataTemp[SIZE_BUFFER];

        memcpy(dataTemp, dest + (numScreens - 1) * pointsInScreen - deltaNumPoints, pointsInScreen);        // Теперь скопируем последний полный экран в буфер

        memcpy(dataTemp, dest + numScreens * pointsInScreen - deltaNumPoints, numPoints % pointsInScreen);  // Теперь скопируем остаток в начало буфера

                                                                                                            //        xP2P = Grid::Left() + ((numPoints  % pointsInScreen) / kP2P) - 1;

        memcpy(dest, dataTemp, pointsInScreen);                                                             // Теперь скопируем временный буфер в выходной
    }
    else
    {
        //        xP2P = Grid::Left() + numPoints / kP2P - 1;
    }

    return numPoints > pointsInScreen ? pointsInScreen : numPoints;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMarkersForMeasure(Channel ch)
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    int bottom = Grid::ChannelBottom();
    int left = Grid::Left();
    int right = Grid::Right();
    float scaleY = (float)(bottom - GRID_TOP) / (MAX_VALUE - MIN_VALUE);

    BitSet64 points = sDisplay_PointsOnDisplay();
    float scaleX = (float)(Grid::Right() - Grid::Left()) / (float)(points.word1 - points.word0);

    Painter::SetColor(Color::Cursors(ch));
    for (int numMarker = 0; numMarker < 2; numMarker++)
    {
        int posY = bottom - (int)(MARKER_VOLTAGE(ch, numMarker) * scaleY);
        if (posY > GRID_TOP && posY < bottom)
        {
            Painter::DrawDashedHLine(posY, left, right, 3, 2, 0);
        }

        int posX = left + (int)(MARKER_TIME(ch, numMarker) * scaleX);
        if (posX > left && posX < right)
        {
            Painter::DrawDashedVLine(posX, GRID_TOP, bottom, 3, 2, 0);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalLined(const uint8 *data, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX, bool calculateFiltr)
{
    uint8 dataCD[281];

    if (endPoint < startPoint)
    {
        return;
    }

    int gridLeft = Grid::Left();
    if (PEAKDET_DS == PeakDet_Disable)
    {
        int gridRight = Grid::Right();
        int numPoints = NUM_BYTES_DS;
        for (int i = startPoint; i < endPoint; i++)
        {

            float x0 = gridLeft + (i - startPoint) * scaleX;
            if (x0 >= gridLeft && x0 <= gridRight)
            {
                int index = i - startPoint;
                int y = calculateFiltr ? Math::CalculateFiltr(data, i, numPoints, NUM_SMOOTHING) : data[i];
                int newY = 0;
                CONVERT_DATA_TO_DISPLAY(newY, y);
                dataCD[index] = (uint8)newY;
            }
        }
    }
    else
    {
        for (int i = 1; i < 280 * 2; i += 2)
        {
            float x = gridLeft + i / 2.0f * scaleX;

            int index = startPoint * 2 + i;

            int y0 = 0, y1 = 0;
            {
                CONVERT_DATA_TO_DISPLAY(y0, data[index++]);
            }
            {
                CONVERT_DATA_TO_DISPLAY(y1, data[index++]);
            }

            PLACE_2_ASCENDING(y0, y1);

            Painter::DrawVLine((int)x, y0, y1);

            int z0 = 0;
            int z1 = 0;
            {
                CONVERT_DATA_TO_DISPLAY(z0, data[index++]);
            }
            {
                CONVERT_DATA_TO_DISPLAY(z1, data[index]);
            }

            PLACE_2_ASCENDING(z0, z1);

            if (y1 < z0)
            {
                Painter::DrawVLine((int)x, y1, z0);
            }
            else if (y0 > z1)
            {
                Painter::DrawVLine((int)(x + 1), z1, y0);
            }
        }
    }
    if (endPoint - startPoint < 281)
    {
        int numPoints = 281 - (endPoint - startPoint);
        for (int i = 0; i < numPoints + 1; i++)
        {
            int index = endPoint - startPoint + i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], 0);
        }
    }
    if (PEAKDET_DS == PeakDet_Disable)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[endPoint]);
        Painter::DrawSignal(Grid::Left(), dataCD, true);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalPointed(const uint8 *data, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX)
{
    int numPoints = NUM_BYTES_DS;

    if (scaleX == 1.0f)
    {
        uint8 dataCD[281];
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, NUM_SMOOTHING));
        }
        Painter::DrawSignal(Grid::Left(), dataCD, false);
    }
    else
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            int dat = 0;
            CONVERT_DATA_TO_DISPLAY(dat, Math::CalculateFiltr(data, i, numPoints, NUM_SMOOTHING));
            Painter::SetPoint(Grid::Left() + (int)(index * scaleX), dat);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMemoryWindow()
{
    static const int rightXses[3] = {276, 285, 247};
    int rightX = rightXses[MODE_WORK];
    if (sCursors_NecessaryDrawCursors())
    {
        rightX = 68;
    }

    if (DS)
    {
        DrawDataInRect(rightX + 2, LAST_AFFECTED_CH_IS_A ? B : A);
        DrawDataInRect(rightX + 2, LAST_AFFECTED_CH_IS_A ? A : B);
    }

    int leftX = 3;
    float scaleX = (float)(rightX - leftX + 1) / SET_POINTS_IN_CHANNEL;
    const int xVert0 = leftX + (int)(SHIFT_IN_MEMORY_IN_POINTS * scaleX);
    int width = (int)((rightX - leftX) * (282.0f / SET_POINTS_IN_CHANNEL));
    Painter::DrawRectangle(xVert0, 0, width - (FPGA_POINTS_8k ? 1 : 0), GRID_TOP - 2, Color::FILL);

    DrawTPos(leftX, rightX);

    DrawTShift(leftX, rightX, NUM_BYTES_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDataInRect(int width, Channel ch)
{
    if (!dataStruct->needDraw[ch])
    {
        return;
    }

    uint8 *data = OUT(ch);

    float elemsInColumn = NUM_BYTES_SET / (float)width;
    
#undef SIZE_BUFFER
#define SIZE_BUFFER 300
    uint8 min[SIZE_BUFFER];
    uint8 max[SIZE_BUFFER];

    if (PEAKDET_DS != PeakDet_Disable)                                 // Если пик. дет. включен
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++)
        {
            uint firstElem = (uint)(col * elemsInColumn);
            uint lastElem = (uint)(firstElem + elemsInColumn - 1);
            *iMin = data[firstElem];
            *iMax = data[firstElem];
            for (uint elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin);
                SET_MAX_IF_LARGER(data[elem], *iMax);
            }
        }
    }
    else                                                                // Если пик. дет. выключён
    {
        for (int col = 0; col < width; col++)
        {
            uint firstElem = (uint)(col * elemsInColumn);
            uint lastElem = (uint)(firstElem + elemsInColumn - 1);
            min[col] = data[firstElem];
            max[col] = data[firstElem];
            for (uint elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem], max[col]);
            }
        }
    }

    int mines[SIZE_BUFFER];     // Массив для максимальных значений в каждом столбике
    int maxes[SIZE_BUFFER];     // Массив для минимальных значений в каждом столбике

    float scale = 17.0f / (MAX_VALUE - MIN_VALUE);

    mines[0] = Ordinate(max[0], scale);
    maxes[0] = Ordinate(min[0], scale);

    for (int i = 1; i < width; i++)
    {
        maxes[i] = Ordinate((uint8)((max[i] < min[i - 1]) ? min[i - 1] : max[i]), scale);
        mines[i] = Ordinate((uint8)((min[i] > max[i - 1]) ? max[i - 1] : min[i]), scale);
    }

    //*** Теперь определим количество точек, которые нужно нарисовать
    int numPoints = 0;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] != -1 && mines[i] != -1) { numPoints++; }
    }

    numPoints--;

    //*** Теперь определим, с какой позиции выводить точки (если сигнал сжат по горизонтали, то вначале будет пустое место
    int x = 1;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] == -1 && mines[i] == -1) { ++x; }
        else { break; }
    }

    if (numPoints > 1)
    {
        int delta = x;
        if (numPoints < 256)
        {
            SendToDisplayDataInRect(ch, x, mines + delta, maxes + delta, numPoints);
        }
        else
        {
            SendToDisplayDataInRect(ch, x, mines + delta, maxes + delta, 255);
            numPoints -= 255;
            SendToDisplayDataInRect(ch, x + 255, mines + 255 + delta, maxes + 255 + delta, numPoints);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTPos(int leftX, int rightX)
{
    int x[] = {leftX, (rightX - leftX) / 2 + leftX, rightX};
    int x0 = x[TPOS];
    Painter::FillRegion(x0 - 3, 10, 6, 6, Color::BACK);
    Painter::DrawChar(x0 - 3, 10, SYMBOL_TPOS_1, Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTShift(int leftX, int rightX, int numBytes)
{
    float scale = (float)(rightX - leftX + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));
    int xShift = (int)(1.5f + (TPOS_IN_BYTES - TSHIFT_IN_POINTS) * scale) - 1;
    if (SET_PEAKDET_EN && TPOS_IS_RIGHT)
    {
        --xShift;
    }
    if (FPGA_POINTS_512)
    {
        ++xShift;                           /// \todo Костыль
    }
    LIMIT_ABOVE(xShift, rightX - 2);

    int dX01 = 1, dX02 = 2, dX11 = 3, dY11 = 7, dY12 = 6;

    if (xShift < leftX - 2)
    {
        xShift = leftX - 2;
        dX01 = 3; dX02 = 1; dY12 = 6;
    }
    else if (xShift > rightX - 1)
    {
        xShift = rightX - 2;
        dX11 = 1;
    }
    else
    {
        dY11 = 5; dY12 = 7;
    }

    Painter::FillRegion((int)xShift - 1, 1, 6, 6, Color::BACK);
    Painter::FillRegion((int)xShift, 2, 4, 4, Color::FILL);
    Painter::DrawLine((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2, Color::BACK);
    Painter::DrawLine((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/// Возвращает (-1), если точка не считана (NONE_VALUE)
static int Ordinate(uint8 x, float scale)
{
    if (x == NONE_VALUE)
    {
        return -1;
    }

    LimitationRet<uint8>((uint8)(x - MIN_VALUE), 0, (MAX_VALUE - MIN_VALUE));

    return (int)((17.0f - scale * x) + 0.5f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
// Процедура ограничивает width числом 255
//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendToDisplayDataInRect(Channel ch, int x, int *min, int *max, int width)
{
    LIMIT_ABOVE(width, 255);

#undef SIZE_BUFFER
#define SIZE_BUFFER (255 * 2)
    uint8 points[SIZE_BUFFER];

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    Painter::DrawVLineArray(x, (int)width, points, Color::CHAN[ch]);
}

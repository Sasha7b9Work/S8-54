#pragma once 
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup FileManager File Manager
 *  @{
 */

class FileManager
{
public:
    /// Вызывается один раз при обнаружении новой флешки
    static void Init();
    /// Нарисовать файловый менеджер
    static void Draw();
    
    static void PressSB_LevelUp();
    
    static void PressSB_LevelDown();
    
    static void RotateRegSet(int angle);
    
    static bool GetNameForNewFile(char name[255]);

    static void PressSB_Tab();
};

/** @}
 */

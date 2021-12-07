/*! \page page10 Изменения в стандартных библиотеках

    ff.c : 582\n
        Было
    \code
    #define INIT_NAMBUF(fs)	{ lfn = ff_memalloc((_MAX_LFN+1)*2); if (!lfn) LEAVE_FF(fs, FR_NOT_ENOUGH_CORE); (fs)->lfnbuf = lfn; }
    \endcode
        Стало
    \code
    #define INIT_NAMBUF(fs)	{ lfn = (WCHAR *)ff_memalloc((_MAX_LFN+1)*2); if (!lfn) LEAVE_FF(fs, FR_NOT_ENOUGH_CORE); (fs)->lfnbuf = lfn; }
    \endcode
        Причина\n
    Иначе не компилируется C++.\n\n
    usbh_diskio.h : 73\n
        Было
    \code
    const Diskio_drvTypeDef  USBH_Driver =
    \endcode
        Стало
    \code
    Diskio_drvTypeDef  USBH_Driver =
    \endcode

*/

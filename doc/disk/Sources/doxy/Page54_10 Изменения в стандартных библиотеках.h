/*! \page page10 ��������� � ����������� �����������

    ff.c : 582\n
        ����
    \code
    #define INIT_NAMBUF(fs)	{ lfn = ff_memalloc((_MAX_LFN+1)*2); if (!lfn) LEAVE_FF(fs, FR_NOT_ENOUGH_CORE); (fs)->lfnbuf = lfn; }
    \endcode
        �����
    \code
    #define INIT_NAMBUF(fs)	{ lfn = (WCHAR *)ff_memalloc((_MAX_LFN+1)*2); if (!lfn) LEAVE_FF(fs, FR_NOT_ENOUGH_CORE); (fs)->lfnbuf = lfn; }
    \endcode
        �������\n
    ����� �� ������������� C++.\n\n
    usbh_diskio.h : 73\n
        ����
    \code
    const Diskio_drvTypeDef  USBH_Driver =
    \endcode
        �����
    \code
    Diskio_drvTypeDef  USBH_Driver =
    \endcode

*/

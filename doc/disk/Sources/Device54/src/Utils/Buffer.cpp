#include "defines.h"
#include "Buffer.h"
#include <stdlib.h>
#include <string.h>



Buffer::Buffer(int s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        memset(data, value, static_cast<uint>(size));
    }
}


void Buffer::Free()
{
    free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(malloc(static_cast<uint>(s)));
        size = (data) ? s : 0;

        if(!data)
        {
//            LOG_ERROR("��� ������");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}

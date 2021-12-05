#pragma once

float fabsf(float);
float fabs(double);
float sqrtf(float);
float log10f(float);
float cosf(float);
float sinf(float);
bool isnan(float);
int rand();
float powf(float, float);


namespace std
{
    template <class T> class numeric_limits
    {
    public:
        static T epsilon()
        {
            return static_cast<T>(0.1f);
        }
    };
}
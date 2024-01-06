﻿#pragma once



typedef struct
{
    int x;
    int y;
    int width;
    int height;
    float fullTime;
    float passedTime;
} ProgressBar;



void ProgressBar_Draw(const ProgressBar *bar);

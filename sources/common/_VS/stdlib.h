#pragma once

void memcpy(void *, const void *, int);
void memset(void *, int, int);
void *malloc(int);
void free(void *);
int memcmp(void *, const void *, int);
#define RAND_MAX 0x7fffffff

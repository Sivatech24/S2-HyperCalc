// bigcalc.h
#ifndef BIGCALC_H
#define BIGCALC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Remove leading zeros
void stripLeadingZeros(char *str);

// Big number algorithms
void bigAdd(const char* a, const char* b, char* result);
void bigSubtract(const char* a, const char* b, char* result);
void bigMultiply(const char* a, const char* b, char* result);
void bigDivide(const char* a, int divisor, char* result);

// Main calculator function
void calculator();

#endif

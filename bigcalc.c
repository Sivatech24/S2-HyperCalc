// bigcalc.c
#include "bigcalc.h"

// Remove leading zeros
void stripLeadingZeros(char *str) {
    int i = 0;
    while (str[i] == '0' && str[i + 1]) i++;
    if (i) memmove(str, str + i, strlen(str) - i + 1);
}

void bigAdd(const char* a, const char* b, char* result) {
    int i = strlen(a) - 1, j = strlen(b) - 1, k = 0, carry = 0;
    char temp[1024] = {0};
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        temp[k++] = (sum % 10) + '0';
        carry = sum / 10;
    }
    for (int m = 0; m < k; m++) result[m] = temp[k - 1 - m];
    result[k] = '\0';
}

void bigSubtract(const char* a, const char* b, char* result) {
    int i = strlen(a) - 1, j = strlen(b) - 1, k = 0, borrow = 0;
    char temp[1024] = {0};
    while (i >= 0) {
        int diff = a[i--] - '0' - borrow;
        if (j >= 0) diff -= b[j--] - '0';
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else borrow = 0;
        temp[k++] = diff + '0';
    }
    while (k > 1 && temp[k - 1] == '0') k--;
    for (int m = 0; m < k; m++) result[m] = temp[k - 1 - m];
    result[k] = '\0';
}

void bigMultiply(const char* a, const char* b, char* result) {
    int lenA = strlen(a), lenB = strlen(b);
    int res[2048] = {0};
    for (int i = lenA - 1; i >= 0; i--)
        for (int j = lenB - 1; j >= 0; j--)
            res[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    for (int i = lenA + lenB - 1; i > 0; i--) {
        if (res[i] >= 10) {
            res[i - 1] += res[i] / 10;
            res[i] %= 10;
        }
    }
    int start = 0;
    while (start < lenA + lenB && res[start] == 0) start++;
    if (start == lenA + lenB) { strcpy(result, "0"); return; }
    for (int i = start; i < lenA + lenB; i++) result[i - start] = res[i] + '0';
    result[lenA + lenB - start] = '\0';
}

void bigDivide(const char* a, int divisor, char* result) {
    int len = strlen(a), rem = 0, k = 0;
    for (int i = 0; i < len; i++) {
        int curr = rem * 10 + (a[i] - '0');
        result[k++] = (curr / divisor) + '0';
        rem = curr % divisor;
    }
    result[k] = '\0';
    stripLeadingZeros(result);
}

void calculator() {
    char a[1024], b[1024], result[2048];
    char op;

    printf("\nCalculator - Big Integer\n");
    printf("Enter first number: ");
    scanf("%1023s", a);
    printf("Enter operator (+ - * /): ");
    scanf(" %c", &op);
    printf("Enter second number: ");
    scanf("%1023s", b);

    if (op == '+') bigAdd(a, b, result);
    else if (op == '-') bigSubtract(a, b, result);
    else if (op == '*') bigMultiply(a, b, result);
    else if (op == '/') bigDivide(a, atoi(b), result);
    else {
        printf("Invalid operator.\n");
        return;
    }

    printf("Result = %s\n", result);
}

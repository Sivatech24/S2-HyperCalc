#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Resolution data
typedef struct {
    const char* label;
    int width;
    int height;
} Resolution;

Resolution resolutions[] = {
    {"360p", 64, 20},
    {"480p", 80, 25},
    {"720p", 100, 30},
    {"1080p", 120, 35},
    {"1440p", 140, 40},
    {"2160p", 160, 45},
    {"4120p", 200, 55},
};

//

#define NUM_RESOLUTIONS (sizeof(resolutions) / sizeof(Resolution))

// ANSI color codes
#define RESET       "\033[0m"
#define BG_BLUE     "\033[44m"
#define BG_GREEN    "\033[102m"
#define FG_BLACK    "\033[30m"
#define FG_WHITE    "\033[97m"
#define FG_GREEN    "\033[32m"
#define FG_CYAN     "\033[36m"
#define FG_YELLOW   "\033[33m"
#define FG_MAGENTA  "\033[35m"
#define FG_WHITE    "\033[97m"
#define CLEAR       "\033[2J\033[H"

// Globals
int screenWidth = 80;
int screenHeight = 25;

// Helper Functions
void clearScreen() {
    printf("\033[2J\033[H");
}

void printHeader() {
    // Clear screen and show header
    printf(CLEAR);
    printf(FG_CYAN);
    printf("==================================================\n");
    printf("     Welcome to S2 HyperCalc Operating System      \n");
    printf("        by Coding Master 24             \n");
    printf("==================================================\n\n");
    printf(RESET);
}

void printTable() {
    printf(FG_YELLOW);
    printf("| Operation                   | Best Algorithm            | Time Complexity       | Use Case\n");
    printf("|----------------------------|----------------------------|-----------------------|---------------------------------------------------------------\n");
    printf(FG_WHITE);
    printf("| Addition                   | Grade School Addition      | O(n)                  | General-purpose addition of large numbers.\n");
    printf("| Subtraction                | Grade School Subtraction   | O(n)                  | General-purpose subtraction of large numbers.\n");
    printf("| Multiplication             | Karatsuba Multiplication   | O(n^1.585)            | Efficient multiplication for large numbers.\n");
    printf("| Multiplication (Very Large)| Schönhage-Strassen         | O(n log n log log n)  | Best for extremely large numbers (e.g., cryptography).\n");
    printf("| Division                   | Newton-Raphson Division    | O(n log n)            | Fast division with high precision.\n");
    printf("| Division (Very Large)      | Schönhage-Strassen         | O(n log n log log n)  | Cryptographic-level division of large numbers.\n");
    printf(RESET);
}

void drawBackground() {
    clearScreen();
    for (int i = 0; i < screenHeight; i++) {
        printf(BG_BLUE);
        for (int j = 0; j < screenWidth; j++) {
            printf(" ");
        }
        printf(RESET "\n");
    }
    printf("\033[H");
}

void centerText(const char* text, const char* color) {
    int len = strlen(text);
    int pad = (screenWidth - len) / 2;
    printf("%s", color);
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s", text);
    for (int i = 0; i < screenWidth - pad - len; i++) printf(" ");
    printf(RESET "\n");
}

void stripLeadingZeros(char* str) {
    int i = 0;
    while (str[i] == '0' && str[i + 1]) i++;
    if (i > 0) memmove(str, str + i, strlen(str + i) + 1);
}

// Big Integer Arithmetic
void bigAdd(const char* a, const char* b, char* result) {
    int i = strlen(a) - 1, j = strlen(b) - 1, carry = 0, k = 0;
    char temp[2048] = {0};

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        temp[k++] = (sum % 10) + '0';
        carry = sum / 10;
    }

    for (int m = 0; m < k; m++)
        result[m] = temp[k - 1 - m];
    result[k] = '\0';
}

void bigSubtract(const char* a, const char* b, char* result) {
    int i = strlen(a) - 1, j = strlen(b) - 1, k = 0, borrow = 0;
    char temp[2048] = {0};

    while (i >= 0) {
        int diff = a[i--] - '0' - borrow;
        if (j >= 0) diff -= b[j--] - '0';

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        temp[k++] = diff + '0';
    }

    while (k > 1 && temp[k - 1] == '0') k--;
    for (int m = 0; m < k; m++)
        result[m] = temp[k - 1 - m];
    result[k] = '\0';
}

void bigMultiply(const char* a, const char* b, char* result) {
    int lenA = strlen(a), lenB = strlen(b);
    int res[4096] = {0};

    for (int i = lenA - 1; i >= 0; i--) {
        for (int j = lenB - 1; j >= 0; j--) {
            res[i + j + 1] += (a[i] - '0') * (b[j] - '0');
        }
    }

    for (int i = lenA + lenB - 1; i > 0; i--) {
        if (res[i] >= 10) {
            res[i - 1] += res[i] / 10;
            res[i] %= 10;
        }
    }

    int start = 0;
    while (start < lenA + lenB && res[start] == 0) start++;

    if (start == lenA + lenB) {
        strcpy(result, "0");
        return;
    }

    for (int i = start; i < lenA + lenB; i++)
        result[i - start] = res[i] + '0';
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

// UI & Menu
void waitForEnter() {
    printf("\n");
    centerText("Press Enter to return to menu...", BG_BLUE FG_WHITE);
    getchar(); // Wait for Enter
    getchar(); // Flush newline
}

void drawTitleBar() {
    printf("\033[1;1H");
    centerText("S2 HyperCalc Operating System", BG_GREEN FG_BLACK);
    printf("\n");
}

void drawMenu() {
    drawBackground();
    drawTitleBar();

    const char* menu[] = {
        "[1] Home",
        "[2] Settings",
        "[3] About",
        "[4] Calculator",
        "[0] Exit"
    };

    int startY = (screenHeight - 10) / 2;
    for (int i = 0; i < startY; i++) printf("\n");

    for (int i = 0; i < 5; i++)
        centerText(menu[i], BG_BLUE FG_WHITE);

    printf("\n");
    centerText("Enter your choice: ", BG_BLUE FG_WHITE);
}

void showScreen(const char* title, const char* content) {
    drawBackground();
    centerText(title, BG_GREEN FG_BLACK);
    printf("\n\n");
    centerText(content, BG_BLUE FG_WHITE);
    waitForEnter();
}

void showSettings() {
    drawBackground();
    centerText("SETTINGS - CHOOSE RESOLUTION", BG_GREEN FG_BLACK);
    printf("\n");

    for (int i = 0; i < NUM_RESOLUTIONS; i++) {
        char line[128];
        sprintf(line, "[%d] %s (%dx%d)", i + 1, resolutions[i].label, resolutions[i].width, resolutions[i].height);
        centerText(line, BG_BLUE FG_WHITE);
    }

    printf("\n");
    centerText("Enter choice (1-7): ", BG_BLUE FG_WHITE);

    int choice;
    scanf("%d", &choice);
    getchar(); // Consume newline

    if (choice >= 1 && choice <= NUM_RESOLUTIONS) {
        screenWidth = resolutions[choice - 1].width;
        screenHeight = resolutions[choice - 1].height;
        showScreen("SETTINGS", "Resolution updated successfully.");
    } else {
        showScreen("ERROR", "Invalid resolution choice.");
    }
}

// Calculator Logic
void calculator() {
    char a[1024], b[1024], result[2048];
    char op;

    drawBackground();
    centerText("BIG INTEGER CALCULATOR", BG_GREEN FG_BLACK);
    printf("\n\n");

    printf("Enter first number: ");
    scanf("%1023s", a);

    printf("Enter operator (+ - * /): ");
    scanf(" %c", &op);

    printf("Enter second number: ");
    scanf("%1023s", b);

    if (op == '+') {
        bigAdd(a, b, result);
    } else if (op == '-') {
        bigSubtract(a, b, result);
    } else if (op == '*') {
        bigMultiply(a, b, result);
    } else if (op == '/') {
        int divisor = atoi(b);
        if (divisor == 0) {
            showScreen("ERROR", "Division by zero is undefined.");
            return;
        }
        bigDivide(a, divisor, result);
    } else {
        showScreen("ERROR", "Invalid operator.");
        return;
    }

    printf("\nResult = %s\n", result);
    waitForEnter();
}


//Home
int home() {
    printHeader();
    printTable();
    
    // Footer
    printf("\n" FG_MAGENTA);
    printf(">> S2 HyperCalc OS is optimized for large-number computation and cryptographic performance.\n");
    printf(">> Built for researchers, mathematicians, and power users.\n");
    printf(">> Powered by next-gen number theory and algorithmic precision.\n");
    printf(RESET);

    // Wait before exit
    printf("\n\nPress Enter to exit...");
    getchar();
    return 0;
}

//About

int about() {
    // Clear the screen and display the About Page
    printf(CLEAR);
    printf(FG_CYAN);
    printf("===========================================\n");
    printf("         ABOUT - S2 NUMAX OS               \n");
    printf("===========================================\n\n");
    printf(RESET);

    printf(FG_WHITE);
    printf("S2 NUMAX OS is a high-performance number-oriented\n");
    printf("operating system built to handle extremely large\n");
    printf("integer computations with unmatched precision and speed.\n\n");

    printf(FG_YELLOW);
    printf("Main Features:\n");
    printf(FG_WHITE);
    printf("  • Optimized algorithms for big-number arithmetic\n");
    printf("  • Cryptographic support for ultra-large multiplications\n");
    printf("  • Efficient and scalable core for scientific computing\n\n");

    printf(FG_YELLOW);
    printf("Supported Algorithms:\n");
    printf(FG_WHITE);
    printf("  - Grade School Addition / Subtraction\n");
    printf("  - Karatsuba Multiplication\n");
    printf("  - Schönhage-Strassen Multiplication & Division\n");
    printf("  - Newton-Raphson Division\n\n");

    printf(FG_YELLOW);
    printf("Developed by:\n");
    printf(FG_MAGENTA);
    printf("  ➤ Coding Master 24\n");
    printf("  ➤ Version: S2 NUMAX 1.0\n\n");

    printf(FG_GREEN);
    printf(">> Designed for researchers, cryptographers, and\n");
    printf("   anyone working with high-precision numerics.\n\n");

    printf(RESET);
    printf("Press Enter to return...");
    getchar();  // Pause

    return 0;
}

// Main Entry
int main() {
    char choice;

    screenWidth = resolutions[1].width;  // Default 480p
    screenHeight = resolutions[1].height;

    while (1) {
        drawMenu();
        choice = getchar();
        getchar(); // flush newline

        switch (choice) {
            case '1': home(); break;
            case '2': showSettings(); break;
            case '3': about(); break;
            case '4': calculator(); break;
            case '0':
                drawBackground();
                centerText("Exiting... Bye!", BG_GREEN FG_BLACK);
		printf("\nExiting S2 NUMAX OS... Goodbye!\n");
    		exit(0);  // Terminates the program successfully
                printf("\n");
                return 0;
            default:
                showScreen("ERROR", "Invalid choice.");
                break;
        }
    }
}

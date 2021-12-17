#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

// Structured data type handling on arrays.
//  + Data: cac cay trong dat (tuong ung voi input mang 2 chieu).
//  + LengthX: chieu dai dat (tuong ung voi input N).
//  + LengthY: chieu rong dat (tuong ung voi input M).
struct MatrixArray {
    int Data[15][15];
    int LengthX;
    int LengthY;
};

// The data structure type handles the current point of the robot.
//  + X: Toa do theo truc hoanh (tuong ung voi input X)
//  + Y: Toa do theo truc tung (tuong ung voi input Y)
struct Point {
    int X;
    int Y;
};

// Set maximum string.
#define StrMAX 226

// Console settings: Enable console result display.
#define CONSOLE_DISPLAY_ENABLE 1

// Console settings: Console display speed (ms).
#define CONSOLE_DISPLAY_SPEED 200

// Name (if file in a folder with same for this application) or path of file.
#define FILE_INPUT "ROBOT1515.INP"
#define FILE_OUTPUT "ROBOT.OUT"

// Global variable to save longest way which robot can grow.
char result[226] = "\0";

void PrintInfoProject();

void GoToXY(int x, int y);

void ChangeConsoleSize(int x, int y);

void DisplayMatrix(struct MatrixArray array, char s[StrMAX]);

void AssignAndFindWay(struct MatrixArray array, struct Point point, char result[226]);

void ChangeWay(struct MatrixArray array, struct Point point, char input[226], int way);

int main() {
    system("MODE CON: COLS=45 LINES=32");
    
    // Variables will be used to store values when reading files.
    struct MatrixArray array;
    struct Point point;
    int i, j;

    // ========================== Read file ==========================
    FILE* file;
    // Open file with read-only permission.
    file = fopen(FILE_INPUT, "r");
    // If file can't read, show error message and exit program.
    if (file == NULL) {
        PrintInfoProject();
        printf("\nKhong the doc file.\n\nNhan ENTER de thoat...\n");
        getch();
        return -1;
    }
    // Read 4 values M, N, X, Y and save them to variables.
    fscanf(file, "%d %d %d %d", &array.LengthY, &array.LengthX, &point.Y, &point.X);
    // Read the matrix M rows N columns.
    for (i = 0; i < array.LengthY; i++) {
        for (j = 0; j < array.LengthX; j++) {
            fscanf(file, "%d", &array.Data[i][j]);
        }
    }
    // Close file after reading
    fclose(file);

    //Decrease by 1 unit because the array is from 0, not 1
    point.X -= 1; point.Y -= 1;

    // For debug/consone: Show information of project.
    if (CONSOLE_DISPLAY_ENABLE == 1) {
        PrintInfoProject();
        printf("\n");
        printf("Ban do hien tai:\n");
    }

    // Find the longest path here.
    AssignAndFindWay(array, point, result);

    // For debug/console: Show longest way and exit.
    if (CONSOLE_DISPLAY_ENABLE == 1) {
        PrintInfoProject();
        printf("\nDuong dai nhat ma ROBOT co the di duoc:\n%s\n\nNhan ENTER de thoat...", result);
        getch();
    }

    // ========================== Write file ==========================
    // Open file with write permission.
    file = fopen(FILE_OUTPUT, "w");
    // If file can't write, show error message and exit program.
    if (file == NULL) {
        PrintInfoProject();
        printf("\nKhong the ghi file.\n\nNhan ENTER de thoat...\n");
        getch();
        return -1;
    }
    // Write longest way to output file.
    fputs(result, file);
    // MUST close file to avoid error.
    fclose(file);

    // Exit program.
    return 0;
}

// Plant the tree using 'point' variable and find the direction to go
void AssignAndFindWay(struct MatrixArray array, struct Point point, char result[226]) {
    // Mark current point of robot is 1.
    array.Data[point.Y][point.X] = 1;

    // For debug/console: Show current progress.
    if (CONSOLE_DISPLAY_ENABLE == 1) DisplayMatrix(array, result);

    // Check if robot can go to left.
    if (point.X - 1 >= 0) 
        if (array.Data[point.Y][point.X - 1] == 0) 
            ChangeWay(array, point, result, 0);

    // Check if robot can go to top.
    if (point.Y - 1 >= 0) 
        if (array.Data[point.Y - 1][point.X] == 0) 
            ChangeWay(array, point, result, 1);

    // Check if robot can go to right.
    if (point.X + 1 < array.LengthX) 
        if (array.Data[point.Y][point.X + 1] == 0) 
            ChangeWay(array, point, result, 2);

    // Check if robot can go to bottom.
    if (point.Y + 1 < array.LengthY) 
        if (array.Data[point.Y + 1][point.X] == 0) 
            ChangeWay(array, point, result, 3);
}

// Change way of robot
// Note: way: 0: Left; 1: Top; 2: Right; 3: Bottom
void ChangeWay(struct MatrixArray array, struct Point point, char input[226], int way) {
    char resultTemp[226]; strcpy(resultTemp, input);

    //int size = strlen(result);
    switch (way) {
        default:
            break;
        case 0:
            point.X -= 1;
            strcat(resultTemp, "T");
            break;
        case 1:
            point.Y -= 1;
            strcat(resultTemp, "B");
            break;
        case 2:
            point.X += 1;
            strcat(resultTemp, "D");
            break;
        case 3:
            point.Y += 1;
            strcat(resultTemp, "N");
            break;
    }
    
    // Compare string and (global variable) result
    if (strlen(resultTemp) >= strlen(result)) strcpy(result, resultTemp);

    // Assign and find a new way.
    AssignAndFindWay(array, point, resultTemp);
}

// Go to line Y column X.
void GoToXY(int x, int y) {
    COORD coord; coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// For debug/console: Show matrix and string to console.
void DisplayMatrix(struct MatrixArray array, char s[StrMAX]) {
    GoToXY(0, 7);

    int i, j;

    for (i = 0; i <= array.LengthY; i++) printf("=="); printf("=\n");

	// Print array data to console.
    for (i = 0; i < array.LengthY; i++) {
        printf("|");
        for (j = 0; j < array.LengthX; j++) printf(" %d", array.Data[i][j]);
        printf(" |\n");
    }

    for (i = 0; i <= array.LengthY; i++) printf("=="); printf("=\n");

    // Print string to console.
    char s2[226];
    for (i = 0; i < strlen(s); i++) s2[i] = s[i];
    for (i = strlen(s); i < 225; i++) s2[i] = ' ';
    s2[225] = '\0';
    printf("%s\n", s2);

    // Delay console.
    Sleep(CONSOLE_DISPLAY_SPEED);
}

void PrintInfoProject() {
    system("cls");
    printf("========================================\n");
    printf("|  Ten nguoi thuc hien: Que Xuan Tung  |\n");
    printf("| Lop: 19TCLC_DT3 - Lop hoc phan 19N12 |\n");
    printf("|   Ten do an: A05 - ROBOT trong cay   |\n");
    printf("========================================\n");
}

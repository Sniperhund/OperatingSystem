#include "drivers/terminal.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void TerminalClearRow(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void TerminalClear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        TerminalClearRow(i);
    }
}

void TerminalNewLine() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    TerminalClearRow(NUM_COLS - 1);
}

void TerminalPutChar(char character) {
    if (character == '\n') {
        TerminalNewLine();
        return;
    }

    if (col > NUM_COLS) {
        TerminalNewLine();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t)character,
        color: color,
    };

    col++;
}

void TerminalWrite(char* str) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        TerminalPutChar(character);
    }
}

void TerminalSetColor(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}

void TerminalDeleteLastChar() {
    if (col == 0 && row == 0) {
        // No characters to delete
        return;
    }

    if (col == 0) {
        // Move to the previous row
        row--;
        col = NUM_COLS - 1;
    } else {
        col--;
    }

    // Find the last non-space character in the previous row
    size_t prev_row = row;
    size_t prev_col = col;
    while (prev_col > 0 && buffer[prev_col + NUM_COLS * prev_row].character == ' ') {
        prev_col--;
    }

    // Set the column to the last non-space character's position
    col = prev_col;

    // Clear the character at the current position
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };
    buffer[col + NUM_COLS * row] = empty;
}


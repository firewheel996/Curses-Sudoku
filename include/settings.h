#ifndef SETTINGS_H
#define SETTINGS_H

#include <ncurses.h>
#include "difficulty.h"

// Constants for Keystroke checks
#define DEFAULT 		1
#define CANCEL 			2
#define DONE			3
#define KEY_SETTINGS 	4
#define NONE			0

// The entered number is out of bounds
#define OUT_OF_BOUNDS	-1

typedef struct {
	
	// Char to move cursor up
	int up[2];
	
	// Char to move cursor down
	int down[2];
	
	// Char to move cursor left
	int left[2];
	
	// Char to move cursor right
	int right[2];
	
	// Char to remove number
	int rmNum[2];
	
	// Char to get hint
	int hint[2];
	
	// Char to check solution
	int check[2];
	
	// Char to solve puzzle
	int solve[2];
	
	// Char for new game
	int newGame[2];
	
	// Char to quit
	int quit[2];
}KeyBindings;

extern KeyBindings * bindings;

typedef struct {
	DIFFICULTY difficulty;
	int checkOnEntry;
	int guessLimit;
	int hintLimit;
} GameSettings;

extern GameSettings * settings;

// Window for Key Bindings
extern WINDOW * wBindings;

// Window for Game Settings
extern WINDOW * wGameSettings;

void init_settings_windows(void);
void init_settings(void);

void reset_settings(void);
void reset_bindings(void);

/**
* Process Keystroke with respect to the key bindings window
* @param key The key that was pressed
* @param tempBindings Pointer to temporary key bindings
* @returns NONE if no action is necessary, DEFAULT, CANCEL, or DONE
*		if one of those "buttons" were pressed
**/
int bindingsKeystroke(int key, KeyBindings * tempBindings);

/**
* Copy Key Bindings from one struct to another
* @param master The key bindings to copy
* @param slate The struct to copy to
**/
void copyKeyBindings(KeyBindings * master, KeyBindings * slate);

/**
* Process Keystroke with respect to the game settings window
* @param key The key that was pressed
* @param tempSettings Pointer to temporary game settings
* @returns NONE if no action is necessary, DEFAULT, CANCEL, DONE,
*		or KEY_SETTINGS if the corresponding option was selected
**/
int settingsKeystroke(int key, GameSettings * tempSettings);
	
/**
* Print the 3 char representation of the key to a window
* @param window The window to print to
* @param key The Key to Print
**/
void printKey(WINDOW * window, int key);

/**
* Copy Key Bindings from 1 struct to another
* @param from The keybinding to copy from
* @param to The keybinding to copy to
**/
void copyKeyBindings(KeyBindings * from, KeyBindings * to);

void copySettings(GameSettings * from, GameSettings * to);

void cleanupSettings(void);

extern char BLANK_CHAR;

#endif // SETTINGS_H
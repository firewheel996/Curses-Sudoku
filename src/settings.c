#include "settings.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Coordinates on the screen that the windows will sit at
#define SCREEN_X	3
#define SCREEN_Y	3

#define GAME_SETTINGS_LINES		14
#define GAME_SETTINGS_COLS		42

#define GAME_KEY_SETTINGS_ROW	2
#define GAME_KEY_BINDINGS_X		12

#define DIFFICULTY_ROW			4
#define DIFFICULTY_EASY_X		15
#define DIFFICULTY_MEDIUM_X		22
#define DIFFICULTY_HARD_X		31

#define CHECK_ROW				6
#define CHECK_YES_X				19
#define CHECK_NO_X				26

#define GUESS_LIMIT_ROW			8
#define HINT_LIMIT_ROW			10
#define VALUE_X					28

#define GAME_SET_BUTTONS_ROW	12
#define DEFAULT_BUTTON_X		2
#define CANCEL_BUTTON_X			12
#define DONE_BUTTON_X			21

#define KEY_BINDING_LINES		25
#define KEY_BINDING_COLS		27
#define	KEY_1_X					20
#define KEY_2_X					24

#define FUNCTION_ROW			2
#define CURSOR_UP_ROW			4
#define CURSOR_DOWN_ROW			6
#define CURSOR_LEFT_ROW			8
#define CURSOR_RIGHT_ROW		10
#define REMOVE_NUMBER_ROW		12
#define GET_HINT_ROW			14
#define CHECK_SOLUTION_ROW		16
#define SOLVE_PUZZLE_ROW		18
#define NEW_GAME_ROW			20
#define QUIT_ROW				22
#define KEY_BINDING_BUTTONS_ROW	24

#define GREEN 			COLOR_PAIR(1)
#define BLUE			COLOR_PAIR(2)
#define USER_IN			COLOR_PAIR(3)
#define HIGHLIGHT		COLOR_PAIR(4)

// Constants for Window Starting Positions
#define KEY_BINDING_START_X		KEY_1_X
#define KEY_BINDING_START_Y		4
#define GAME_SETTINGS_START_X	12
#define GAME_SETTINGS_START_Y	2

KeyBindings * bindings;
GameSettings* settings;
WINDOW * wBindings;
WINDOW * wGameSettings;
char BLANK_CHAR = 1;

typedef struct{ 
	int x,y;
	int inputState;
	} State;
State bindingState, settingState;

void init_settings(){
	////printf("Init Settings\n");
	bindings = malloc(sizeof(KeyBindings));
	settings = malloc(sizeof(GameSettings));
	
	reset_bindings();
	reset_settings();
}

void cleanupSettings(){
	free(bindings);
	free(settings);
}


const size_t FUNCTION_NAME_LENGTH = 16;

void printKey(WINDOW * window, int key){
	
	switch(key){
		case(KEY_DC):{
			wprintw(window, "del");
			break;
		}
		case(KEY_LEFT):{
			wprintw(window," <-");
			break;
		}
		case(KEY_RIGHT):{
			wprintw(window," ->");
			break;
		}
		case KEY_UP:{
			wprintw(window," up");
			break;
		}
		case KEY_DOWN:{
			wprintw(window,"dwn");
			break;
		}
		case 1:{
			wprintw(window,"   ");
			break;
		}
		default:{
			wprintw(window," %c ",key);
			break;
		}
	}
	
}

void highlightDifficulty(DIFFICULTY diff){
	char * toPrint = "Easy";
	wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_EASY_X);
	if(diff == MEDIUM){
		wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_MEDIUM_X);
		toPrint = "Medium";
	}
	else if(diff == HARD){
		wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_HARD_X);
		toPrint = "Hard";
	}
	
	wattron(wGameSettings, A_BOLD|HIGHLIGHT);
	wprintw(wGameSettings, "%s",toPrint);
}

void unHighlightDifficulty(DIFFICULTY diff){
	char * toPrint = "Easy";
	wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_EASY_X);
	if(diff == MEDIUM){
		wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_MEDIUM_X);
		toPrint = "Medium";
	}
	else if(diff == HARD){
		wmove(wGameSettings,DIFFICULTY_ROW,DIFFICULTY_HARD_X);
		toPrint = "Hard |\n";
	}
	
	wattron(wGameSettings, A_BOLD|GREEN);
	wprintw(wGameSettings, "%s",toPrint);
}

// yes = 0 to highlight yes
// no != 0 to highlight no
void highlightCheckSolution(int yes,int no){
	
	wmove(wGameSettings,CHECK_ROW, CHECK_YES_X);
	
	if(yes == 0){
		wattron(wGameSettings,A_BOLD|HIGHLIGHT);
	}
	else{
		wattron(wGameSettings,A_BOLD|GREEN);
	}
	
	wprintw(wGameSettings,"Yes ");
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"| ");
	
	if(no == 0){
		wattron(wGameSettings,GREEN);
	}
	else{
		wattron(wGameSettings,HIGHLIGHT);
	}
	
	wprintw(wGameSettings,"No");
}
		

const char * KeyBindingTableOutline = "+-----------------+---+---+";

const char * KeyBindingRowNames[] = {"Function","Cursor Up", "Cursor Down",
		"Cursor Left", "Cursor Right", "Remove Number", "Get Hint",
		"Check Solution", "Solve Puzzle", "New Game", "Quit"};

const char * GameSettingsGridline = "+---------------------------------------+\n";
		
void init_settings_windows(){
	////printf("Init Settings Windows\n");
	
	// Make new Windows
	wBindings = newwin(KEY_BINDING_LINES, KEY_BINDING_COLS, SCREEN_Y, SCREEN_X);
	wGameSettings = newwin(GAME_SETTINGS_LINES, GAME_SETTINGS_COLS, SCREEN_Y, SCREEN_X);
	
	int row,indx;
	wattron(wBindings,A_BOLD|GREEN);
	
	// Draw the rows for the KeyBindings Window
	wprintw(wBindings,"     Key Bindings\n");
	
	for(row = 2; row <= QUIT_ROW; row += 2){
		indx = (row - 2) / 2;
		
		wattron(wBindings,A_BOLD);
		wattron(wGameSettings,A_BOLD);
		// Set Grid color to Blue
		wattron(wBindings, BLUE);
		wprintw(wBindings,"%s| ",KeyBindingTableOutline);
		
		// Set Letter color to Green
		wattron(wBindings, GREEN);
		wprintw(wBindings,"%s",KeyBindingRowNames[indx]);
		size_t nSpaces = FUNCTION_NAME_LENGTH - strlen(KeyBindingRowNames[indx]);
		while(nSpaces > 0){
			wprintw(wBindings, " ");
			nSpaces--;
		}
		
		char c1, c2;
		switch(row){
			case FUNCTION_ROW:{
				c1 = '1';
				c2 = '2';
				break;
			}
			case CURSOR_UP_ROW:{
				c1 = bindings->up[0];
				c2 = bindings->up[1];
				break;
			}
			case CURSOR_DOWN_ROW:{
				c1 = bindings->down[0];
				c2 = bindings->down[1];
				break;
			}
			case CURSOR_LEFT_ROW:{
				c1 = bindings->left[0];
				c2 = bindings->left[1];
				break;
			}
			case CURSOR_RIGHT_ROW:{
				c1 = bindings->right[0];
				c2 = bindings->right[1];
				break;
			}
			case REMOVE_NUMBER_ROW:{
				c1 = bindings->rmNum[0];
				c2 = bindings->rmNum[1];
				break;
			}
			case GET_HINT_ROW:{
				c1 = bindings->hint[0];
				c2 = bindings->hint[1];
				break;
			}
			case CHECK_SOLUTION_ROW:{
				c1 = bindings->check[0];
				c2 = bindings->check[1];
				break;
			}
			case SOLVE_PUZZLE_ROW:{
				c1 = bindings->solve[0];
				c2 = bindings->solve[1];
				break;
			}
			case NEW_GAME_ROW:{
				c1 = bindings->newGame[0];
				c2 = bindings->newGame[1];
				break;
			}
			case QUIT_ROW:{
				c1 = bindings->quit[0];
				c2 = bindings->quit[1];
				break;
			}
			default:{
				c1 = BLANK_CHAR;
				c2 = BLANK_CHAR;
				break;
			}
		}
		
		wstandend(wBindings);
		
		// Set Grid color to Blue
		wattron(wBindings, A_BOLD|BLUE);
		wprintw(wBindings,"|");
		
		// Set Letter color to Green
		wattron(wBindings, GREEN);
		printKey(wBindings,c1);
		
		// Set Grid color to Blue
		wattron(wBindings, BLUE);
		wprintw(wBindings,"|");
		
		// Set Letter color to Green
		wattron(wBindings, GREEN);
		printKey(wBindings,c2);
		
		// Set Grid color to Blue
		wattron(wBindings, BLUE);
		wprintw(wBindings,"|");
	}
	
	wattron(wBindings,BLUE);
	// Draw on the "buttons" for the key bindings window
	wprintw(wBindings,"%s| ",KeyBindingTableOutline);
	
	wattron(wBindings,GREEN);
	wprintw(wBindings,"Default ");
	
	wattron(wBindings,BLUE);
	wprintw(wBindings,"| ");
	
	wattron(wBindings,GREEN);
	wprintw(wBindings, "Cancel ");
	
	wattron(wBindings,BLUE);
	wprintw(wBindings,"| ");
	
	wattron(wBindings,GREEN);
	wprintw(wBindings,"Done ");
	
	wattron(wBindings,BLUE);
	wprintw(wBindings,"|+-------------------------+");
	
	// Draw the Game Settings Window
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"           Game Settings\n");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"%s",GameSettingsGridline);
	wprintw(wGameSettings,"|");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"           ");
	wattron(wGameSettings,HIGHLIGHT);
	wprintw(wGameSettings,"Set Key Bindings");
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"            |\n");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"%s",GameSettingsGridline);
	wprintw(wGameSettings,"|");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings," Difficulty ");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"|      |        |                |\n");
	
	unHighlightDifficulty(EASY);
	highlightDifficulty(MEDIUM);
	unHighlightDifficulty(HARD);
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"%s",GameSettingsGridline);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"Check On Entry ");
	
	wattron(wGameSettings, BLUE);
	wprintw(wGameSettings,"| ");
	
	highlightCheckSolution(settings->checkOnEntry,settings->checkOnEntry);
	
	wattron(wGameSettings, BLUE);
	wprintw(wGameSettings,"            |\n");
	wprintw(wGameSettings,"%s",GameSettingsGridline);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"Guess Limit (0 = inf) ");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"  %d",settings->guessLimit);
	
	if(settings->guessLimit < 10){
		wprintw(wGameSettings," ");
	}
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"          |\n%s",GameSettingsGridline);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"Hint Limit");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"            | ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"  %d",settings->hintLimit);
	
	if(settings->hintLimit < 10){
		wprintw(wGameSettings," ");
	}
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"          |\n%s",GameSettingsGridline);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"Default ");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings, "Cancel ");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"| ");
	
	wattron(wGameSettings,GREEN);
	wprintw(wGameSettings,"Done ");
	
	wattron(wGameSettings,BLUE);
	wprintw(wGameSettings,"              |\n%s",GameSettingsGridline);
	
	wmove(wGameSettings,GAME_SETTINGS_START_Y, GAME_SETTINGS_START_X);
	settingState.x = GAME_SETTINGS_START_X;
	settingState.y = GAME_SETTINGS_START_Y;
	settingState.inputState = 0;
	
	wmove(wBindings,KEY_BINDING_START_Y, KEY_BINDING_START_X);
	bindingState.x = KEY_BINDING_START_X;
	bindingState.y = KEY_BINDING_START_Y;
	bindingState.inputState = 0;
}

void highlightButton(WINDOW * win, int button){
	wattron(win,A_BOLD|HIGHLIGHT);
	int row;
	if(win == wBindings){
		row = KEY_BINDING_BUTTONS_ROW;
	}
	else{
		row = GAME_SET_BUTTONS_ROW;
	}
	
	if(button == DEFAULT){
		mvwprintw(win,row,DEFAULT_BUTTON_X,"Default");
	}
	else if(button == CANCEL){
		mvwprintw(win,row,CANCEL_BUTTON_X,"Cancel");
	}
	else{
		mvwprintw(win,row,DONE_BUTTON_X,"Done");
	}
}

void unHighlightButton(WINDOW * win, int button){
	wattron(win,A_BOLD|GREEN);
	int row;
	if(win == wBindings){
		row = KEY_BINDING_BUTTONS_ROW;
	}
	else{
		row = GAME_SET_BUTTONS_ROW;
	}
	
	if(button == DEFAULT){
		mvwprintw(win,row,DEFAULT_BUTTON_X,"Default");
	}
	else if(button == CANCEL){
		mvwprintw(win,row,CANCEL_BUTTON_X,"Cancel");
	}
	else{
		mvwprintw(win,row,DONE_BUTTON_X,"Done");
	}
}

int * getBindingByPos(KeyBindings * bind){
	int * result = NULL;
	
	switch(bindingState.y){
		case(CURSOR_UP_ROW):{
			result = &bind->up[0];
			break;
		}
		case(CURSOR_DOWN_ROW):{
			result = &bind->down[0];
			break;
		}
		case(CURSOR_LEFT_ROW):{
			result = &bind->left[0];
			break;
		}
		case(CURSOR_RIGHT_ROW):{
			result = &bind->right[0];
			break;
		}
		case(REMOVE_NUMBER_ROW):{
			result = &bind->rmNum[0];
			break;
		}
		case(GET_HINT_ROW):{
			result = &bind->hint[0];
			break;
		}
		case(CHECK_SOLUTION_ROW):{
			result = &bind->check[0];
			break;
		}
		case(SOLVE_PUZZLE_ROW):{
			result = &bind->solve[0];
			break;
		}
		case(NEW_GAME_ROW):{
			result = &bind->newGame[0];
			break;
		}
		case(QUIT_ROW):{
			result = &bind->quit[0];
			break;
		}
		default:{
			break;
		}
	}
	if(result != NULL && bindingState.x == KEY_2_X){
		result++;
	}
	
	return result;
}

void setKeystrokeByPos(int key, KeyBindings * binding){
	int * bind = getBindingByPos(binding);
	*bind = key;
	if(key == KEY_DC || key == KEY_DOWN){
		wattron(wBindings,HIGHLIGHT);
	}
	else{
		wattron(wBindings,USER_IN);
	}
	wmove(wBindings,bindingState.y,bindingState.x-1);
	printKey(wBindings,key);
}

void highlightCurrentKey(void){
	int * bind = getBindingByPos(bindings);
	if(*bind == KEY_DC || *bind == KEY_DOWN){
		wattron(wBindings,HIGHLIGHT);
		
		wmove(wBindings,bindingState.y, bindingState.x-1);
		printKey(wBindings,*bind);
	}
}

void unHighlightCurrentKey(KeyBindings * tempBindings){
	int * bind = getBindingByPos(bindings);
	if(*bind == KEY_DC || *bind == KEY_DOWN){
		wattroff(wBindings,GREEN);
		int * tempBind = getBindingByPos(tempBindings);
		if(*bind != *tempBind){
			wattron(wBindings,USER_IN);
		}
		wmove(wBindings,bindingState.y, bindingState.x-1);
		printKey(wBindings,*bind);
	}
}

int bindingsKeystroke(int key, KeyBindings * tempBindings){
	
	int result = NONE;
	
	if(bindingState.inputState == 0){
		if(key == bindings->up[0] || key == bindings->up[1]){
			if(bindingState.y > CURSOR_UP_ROW){
				if(bindingState.y < KEY_BINDING_BUTTONS_ROW){
					unHighlightCurrentKey(tempBindings);
				}
				else{
					bindingState.x = KEY_1_X;
				}
				bindingState.y -= 2;
				highlightCurrentKey();
			}
		}
		else if(key == bindings->down[0] || key == bindings->down[1]){
			if(bindingState.y < KEY_BINDING_BUTTONS_ROW-1){
				unHighlightCurrentKey(tempBindings);
				if(bindingState.y == QUIT_ROW){
					bindingState.x = CANCEL_BUTTON_X;
					highlightButton(wBindings,CANCEL);
				}
				else{
					highlightCurrentKey();
				}
				bindingState.y += 2;
				
			}
		}
		else if(key == bindings->left[0] || key == bindings->left[1]){
			if(bindingState.y == KEY_BINDING_BUTTONS_ROW){
				if(bindingState.x > DEFAULT_BUTTON_X){
					if(bindingState.x == CANCEL_BUTTON_X){
						// Change Button to Default
						unHighlightButton(wBindings,CANCEL);
						highlightButton(wBindings,DEFAULT);
						bindingState.x = DEFAULT_BUTTON_X;
					}
					else{
						// Change Button to Cancel
						unHighlightButton(wBindings,DONE);
						highlightButton(wBindings,CANCEL);
						bindingState.x = CANCEL_BUTTON_X;
					}
				}
				else{
					unHighlightButton(wBindings,DEFAULT);
					bindingState.x = KEY_2_X;
					bindingState.y -= 2;
					highlightCurrentKey();
				}
			}
			else{
				unHighlightCurrentKey(tempBindings);
				if(bindingState.x == KEY_1_X 
					&& bindingState.y > CURSOR_UP_ROW)
				{
					bindingState.x = KEY_2_X;
					bindingState.y -= 2;
				}
				else{
					bindingState.x = KEY_1_X;
				}
				highlightCurrentKey();
			}
		}
		else if(key == bindings->right[0] || key == bindings->right[1]){
			if(bindingState.x == KEY_2_X){
				unHighlightCurrentKey(tempBindings);
				bindingState.y += 2;
				if(bindingState.y == QUIT_ROW){
					bindingState.x = DEFAULT_BUTTON_X;
					highlightButton(wBindings,DEFAULT);
				}
				else{
					bindingState.x = KEY_1_X;
					highlightCurrentKey();
				}
			}
			else if(bindingState.y < KEY_BINDING_BUTTONS_ROW){
				unHighlightCurrentKey(tempBindings);
				bindingState.x = KEY_2_X;
				highlightCurrentKey();
			}
			else{
				if(bindingState.x == DEFAULT_BUTTON_X){
					unHighlightButton(wBindings,DEFAULT);
					highlightButton(wBindings,CANCEL);
					bindingState.x = CANCEL_BUTTON_X;
				}
				else if(bindingState.x == CANCEL_BUTTON_X){
					unHighlightButton(wBindings,CANCEL);
					highlightButton(wBindings,DONE);
					bindingState.x = DONE_BUTTON_X;
				}
			}
		}
		else if(key == bindings->rmNum[0] || key == bindings->rmNum[1]){
			setKeystrokeByPos(BLANK_CHAR,tempBindings);
		}
		else if(key == KEY_ENTER || key == 10){
			if(bindingState.y < KEY_BINDING_BUTTONS_ROW){
				bindingState.inputState = 1;
			}
			else if(bindingState.x == DEFAULT_BUTTON_X){
				result = DEFAULT;
			}
			else if(bindingState.x == CANCEL_BUTTON_X){
				result = CANCEL;
			}
			else{
				result = DONE;
			}
		}
		else if(key == 27){				// Escape Key
			result = CANCEL;
		}
	}
	else{
		setKeystrokeByPos(key,tempBindings);
		bindingState.inputState = 0;
	}
	
	wmove(wBindings,bindingState.y, bindingState.x);
	
	return result;
}

int settingsKeystroke(int key, GameSettings * tempSettings){
	int result = NONE;
	
	if(settingState.inputState == 0){
		if(key == bindings->up[0] || key == bindings->up[1]){
			if(settingState.y > GAME_KEY_SETTINGS_ROW){
				switch(settingState.y){
					case(GAME_SET_BUTTONS_ROW):{
						int button = DEFAULT;
						if(settingState.x == CANCEL_BUTTON_X){
							button = CANCEL;
						}
						else if(settingState.x == DONE_BUTTON_X){
							button = DONE;
						}
						
						unHighlightButton(wGameSettings,button);
						wattron(wGameSettings,HIGHLIGHT);
						mvwprintw(wGameSettings,HINT_LIMIT_ROW,VALUE_X,"%d",tempSettings->hintLimit);
						settingState.x = VALUE_X;
						break;
					}
					case(HINT_LIMIT_ROW):{
						wattron(wGameSettings,GREEN);
						if(tempSettings->hintLimit != settings->hintLimit){
							wattron(wGameSettings,USER_IN);
						}
						
						wprintw(wGameSettings,"%d",tempSettings->hintLimit);
						
						wattron(wGameSettings,HIGHLIGHT);
						mvwprintw(wGameSettings,GUESS_LIMIT_ROW,VALUE_X,"%d",tempSettings->guessLimit);
						break;
					}
					case(GUESS_LIMIT_ROW):{
						wattron(wGameSettings,GREEN);
						if(tempSettings->guessLimit != settings->guessLimit){
							wattron(wGameSettings,USER_IN);
						}
						wprintw(wGameSettings,"%d",tempSettings->guessLimit);
						
						settingState.x = CHECK_YES_X;
						if(tempSettings->checkOnEntry != 0){
							settingState.x = CHECK_NO_X;
						}
						break;
					}
					case(CHECK_ROW):{
						
						highlightCheckSolution(tempSettings->checkOnEntry,tempSettings->checkOnEntry);
						settingState.x = DIFFICULTY_EASY_X;
						if(tempSettings->difficulty == MEDIUM){
							settingState.x = DIFFICULTY_MEDIUM_X;
						}
						else if(tempSettings->difficulty == HARD){
							settingState.x = DIFFICULTY_HARD_X;
						}
						
						break;
					}
					case(DIFFICULTY_ROW):{
						unHighlightDifficulty(EASY);
						unHighlightDifficulty(MEDIUM);
						unHighlightDifficulty(HARD);
						highlightDifficulty(tempSettings->difficulty);
						settingState.x = GAME_KEY_BINDINGS_X;
						
						wattron(wGameSettings,HIGHLIGHT);
						mvwprintw(wGameSettings,GAME_KEY_SETTINGS_ROW,
							GAME_KEY_BINDINGS_X,"Set Key Bindings");
						break;
					}
					default:{
						break;
					}
				}
				settingState.y -= 2;
			}
		}
		else if(key == bindings->down[0] || key == bindings->down[1]){
			if(settingState.y < GAME_SET_BUTTONS_ROW){
				switch(settingState.y){
					case(GAME_KEY_SETTINGS_ROW):{
						wattron(wGameSettings,GREEN);
						mvwprintw(wGameSettings,GAME_KEY_SETTINGS_ROW,
							GAME_KEY_BINDINGS_X,"Set Key Bindings");
						
						highlightDifficulty(EASY);
						settingState.x = DIFFICULTY_EASY_X;
						break;
					}
					case(DIFFICULTY_ROW):{
						unHighlightDifficulty(EASY);
						unHighlightDifficulty(MEDIUM);
						unHighlightDifficulty(HARD);
						highlightDifficulty(tempSettings->difficulty);
						
						settingState.x = CHECK_YES_X;
						if(tempSettings->checkOnEntry != 0){
							settingState.x = CHECK_NO_X;
						}
						break;
					}
					case(CHECK_ROW):{
						highlightCheckSolution(tempSettings->checkOnEntry,tempSettings->checkOnEntry);
						settingState.x = VALUE_X;
						wattron(wGameSettings,HIGHLIGHT);
						mvwprintw(wGameSettings,GUESS_LIMIT_ROW,VALUE_X,"%d",tempSettings->guessLimit);
						break;
					}
					case(GUESS_LIMIT_ROW):{
						wattron(wGameSettings,GREEN);
						if(tempSettings->guessLimit != settings->guessLimit){
							wattron(wGameSettings,USER_IN);
						}
						wprintw(wGameSettings,"%d",tempSettings->guessLimit);
						wattron(wGameSettings,HIGHLIGHT);
						mvwprintw(wGameSettings,HINT_LIMIT_ROW,VALUE_X,"%d",tempSettings->hintLimit);
						break;
					}
					case(HINT_LIMIT_ROW):{
						wattron(wGameSettings,GREEN);
						if(tempSettings->hintLimit != settings->hintLimit){
							wattron(wGameSettings,USER_IN);
						}
						wprintw(wGameSettings,"%d",tempSettings->hintLimit);
						highlightButton(wGameSettings,CANCEL);
						settingState.x = CANCEL_BUTTON_X;
						break;
					}
					default:{
						break;
					}
				}
				settingState.y += 2;
			}
		}
		else if(key == bindings->left[0] || key == bindings->left[1]){
			switch(settingState.y){
				case(DIFFICULTY_ROW):{
					if(settingState.x == DIFFICULTY_HARD_X){
						highlightDifficulty(MEDIUM);
						if(tempSettings->difficulty != HARD){
							unHighlightDifficulty(HARD);
						}
						settingState.x = DIFFICULTY_MEDIUM_X;
					}
					else if(settingState.x = DIFFICULTY_MEDIUM_X){
						highlightDifficulty(EASY);
						if(tempSettings->difficulty != MEDIUM){
							unHighlightDifficulty(MEDIUM);
						}
						settingState.x = DIFFICULTY_EASY_X;
					}
					break;
				}
				case(CHECK_ROW):{
					if(settingState.x == CHECK_NO_X){
						highlightCheckSolution(0,tempSettings->checkOnEntry);
						settingState.x = CHECK_YES_X;
					}
					break;
				}
				case(GAME_SET_BUTTONS_ROW):{
					if(settingState.x == CANCEL_BUTTON_X){
						unHighlightButton(wGameSettings,CANCEL);
						highlightButton(wGameSettings,DEFAULT);
						settingState.x = DEFAULT_BUTTON_X;
					}
					else if(settingState.x == DONE_BUTTON_X){
						unHighlightButton(wGameSettings,DONE);
						highlightButton(wGameSettings,CANCEL);
						settingState.x = CANCEL_BUTTON_X;
					}
					break;
				}
				default:{
					break;
				}
			}
		}
		else if(key == bindings->right[0] || key == bindings->right[1]){
			switch(settingState.y){
				case(DIFFICULTY_ROW):{
					if(settingState.x == DIFFICULTY_EASY_X){
						highlightDifficulty(MEDIUM);
						if(tempSettings->difficulty != EASY){
							unHighlightDifficulty(EASY);
						}
						settingState.x = DIFFICULTY_MEDIUM_X;
					}
					else if(settingState.x == DIFFICULTY_MEDIUM_X){
						highlightDifficulty(HARD);
						if(tempSettings->difficulty != MEDIUM){
							unHighlightDifficulty(MEDIUM);
						}
						settingState.x = DIFFICULTY_HARD_X;
					}
					break;
				}
				case(CHECK_ROW):{
					if(settingState.x == CHECK_YES_X){
						highlightCheckSolution(tempSettings->checkOnEntry,1);
						settingState.x = CHECK_NO_X;
					}
					break;
				}
				case(GAME_SET_BUTTONS_ROW):{
					if(settingState.x == DEFAULT_BUTTON_X){
						unHighlightButton(wGameSettings,DEFAULT);
						highlightButton(wGameSettings,CANCEL);
						settingState.x = CANCEL_BUTTON_X;
					}
					else if(settingState.x == CANCEL_BUTTON_X){
						unHighlightButton(wGameSettings,CANCEL);
						highlightButton(wGameSettings,DONE);
						settingState.x = DONE_BUTTON_X;
					}
					break;
				}
				default:{
					break;
				}
			}
		}
		else if(key == 27){
			result = CANCEL;
		}
		else if(key == KEY_ENTER || key == 10){
			//printf("Enter Hit\n");
			switch(settingState.y){
				case(GAME_KEY_SETTINGS_ROW):{
					result = KEY_SETTINGS;
					break;
				}
				case(DIFFICULTY_ROW):{
					unHighlightDifficulty(tempSettings->difficulty);
					if(settingState.x == DIFFICULTY_EASY_X){
						tempSettings->difficulty = EASY;
					}
					else if(settingState.x == DIFFICULTY_MEDIUM_X){
						tempSettings->difficulty = MEDIUM;
					}
					else{
						tempSettings->difficulty = HARD;
					}
					highlightDifficulty(tempSettings->difficulty);
					break;
				}
				case(CHECK_ROW):{
					if(settingState.x == CHECK_NO_X){
						tempSettings->checkOnEntry = 1;
					}
					else{
						tempSettings->checkOnEntry = 0;
					}
					highlightCheckSolution(tempSettings->checkOnEntry,tempSettings->checkOnEntry);
					break;
				}
				case(GAME_SET_BUTTONS_ROW):{
					if(settingState.x == DEFAULT_BUTTON_X){
						//printf("Default Button Hit\n");
						result = DEFAULT;
					}
					else if(settingState.x == CANCEL_BUTTON_X){
						//printf("Cancel Button Hit\n");
						result = CANCEL;
					}
					else{
						//printf("Done Button Hit\n");
						result = DONE;
					}
					break;
				}
				default:{
					settingState.inputState = 1;
					break;
				}
			}
		}
	}
	else{
		int * pVal = &tempSettings->guessLimit;
		if(settingState.y == HINT_LIMIT_ROW){
			pVal = &tempSettings->hintLimit;
		}
		
		if(key == KEY_ENTER || key == 10){
			settingState.inputState = 0;
			settingState.x = VALUE_X;
			
		}
		else if('0' <= key && key <= '9'){
			int check = (*pVal * 10)+(key-'0');
			if(check < 81){
				
				if(*pVal > 0){
					*pVal *= 10;
				}
				
				*pVal += (key - '0');
				wprintw(wGameSettings,"%c",key);
				settingState.x++;
			}
		}
	}
	
	wmove(wGameSettings,settingState.y, settingState.x);
	return result;
}

void copyKeyBindings(KeyBindings * from, KeyBindings * to){
	
	to->up[0] = from->up[0];
	to->up[1] = from->up[1];
	
	to->down[0] = from->down[0];
	to->down[1] = from->down[1];
	
	to->left[0] = from->left[0];
	to->left[1] = from->left[1];
	
	to->right[0] = from->right[0];
	to->right[1] = from->right[1];
	
	to->rmNum[0] = from->rmNum[0];
	to->rmNum[1] = from->rmNum[1];
	
	to->hint[0] = from->hint[0];
	to->hint[1] = from->hint[1];
	
	to->check[0] = from->check[0];
	to->check[1] = from->check[1];
	
	to->solve[0] = from->solve[0];
	to->solve[1] = from->solve[1];
	
	to->newGame[0] = from->newGame[0];
	to->newGame[1] = from->newGame[1];
	
	to->quit[0] = from->quit[0];
	to->quit[1] = from->quit[1];
}

void copySettings(GameSettings * from, GameSettings * to){
	to->difficulty = from->difficulty;
	to->checkOnEntry = from->checkOnEntry;
	to->guessLimit = from->guessLimit;
	to->hintLimit = from->hintLimit;
}

void reset_settings(){
	// Set default Game Settings
	settings->difficulty = MEDIUM;
	settings->checkOnEntry = 1;
	settings->guessLimit = 0;
	settings->hintLimit = 20;
}

void reset_bindings(){
	// Set default Key Bindings
	bindings->up[0] = 'w';
	bindings->up[1] = 'k';
	
	bindings->down[0] = 's';
	bindings->down[1] = 'j';
	
	bindings->left[0] = 'a';
	bindings->left[1] = 'h';
	
	bindings->right[0] = 'd';
	bindings->right[1] = 'l';
	
	bindings->rmNum[0] = 'x';
	bindings->rmNum[1] = KEY_DC;
	
	bindings->hint[0] = 'H';
	bindings->hint[1] = '?';
	
	bindings->check[0] = 'c';
	bindings->check[1] = BLANK_CHAR;
	
	bindings->solve[0] = 'S';
	bindings->solve[1] = BLANK_CHAR;
	
	bindings->newGame[0] = 'n';
	bindings->newGame[1] = 'N';
	
	bindings->quit[0] = 'q';
	bindings->quit[1] = 'Q';	
}
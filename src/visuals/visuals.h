#ifndef VISUALS_H
#define VISUALS_H

#include <ncurses.h>

enum user_option_e
{
    USER_OPTION_YES,
    USER_OPTION_NO,
    USER_OPTION_EXIT
};

struct subwindow_s
{
    WINDOW* window;
    int     cursor_x;
    int     cursor_y;
};

struct visualisation_context 
{
    WINDOW*     img_window;
    WINDOW*     question_window;
    WINDOW*     scan_window;
    subwindow_s subwindow_1;
    subwindow_s subwindow_2;
    subwindow_s subwindow_3;        
    int         rows;
    int         columns;
};

enum mode_menu_e 
{
    MODE_MENU_GUESS = 0,
    MODE_MENU_DEFINITION = 1,
    MODE_MENU_COMPARE = 2,
    MODE_MENU_QUIT = 3
};

// ======================== SCREEN_VISUALISATION_FUNCTIONS ====================

void ScreenContextInit(visualisation_context* screen);
void ScreenContextDestroy(visualisation_context* screen);

// ========================== WINDOW_MEMORY_CONTROLLING =======================

WINDOW* CreateWin(int height, int width, int starty, int startx);
void ImageWindowInit(visualisation_context* screen);
void QuestionWindowInit(visualisation_context* screen);
void ScanWindowInit(visualisation_context* screen);
void SubWindow1Init(visualisation_context* screen);
void SubWindow2Init(visualisation_context* screen);
void SubWindow3Init(visualisation_context* screen);
void DestroySubwindow(subwindow_s* subwindow);
void DestroyWindow(WINDOW** local_win);

// ============================= USER_INTERACTIONS ============================

void            ShowMessage(visualisation_context* screen, const char* message, size_t message_length);
user_option_e   GiveChoice();
void            ShowAkinator(visualisation_context* screen);
void            ScanUserInput(visualisation_context* screen, char* string, size_t  max_string_count);
mode_menu_e     ShowMenu();
void AddTextToSubwindow(subwindow_s* subwindow, const char*  message, size_t message_size);

#endif // VISAULS_H
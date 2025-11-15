#ifndef VISUALS_H
#define VISUALS_H

#include <ncurses.h>

enum user_option_e
{
    USER_OPTION_YES,
    USER_OPTION_NO,
    USER_OPTION_EXIT
};


struct visualisation_context 
{
    WINDOW* img_window;
    WINDOW* question_window;
    WINDOW* scan_window;
    int     rows;
    int     columns;
};

// ======================== SCREEN_VISUALISATION_FUNCTIONS ====================

void ScreenContextInit(visualisation_context* screen);
void ScreenContextDestroy(visualisation_context* screen);

// ========================== WINDOW_MEMORY_CONTROLLING =======================

void ImageWindowInit(visualisation_context* screen);
void QuestionWindowInit(visualisation_context* screen);

void DestroyWindow(WINDOW *local_win);

#endif // VISAULS_H
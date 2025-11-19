#include "visuals.h"

#include <ncurses.h>   
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Assert.h"

static const char* AKINATOR = 
R"(--------------------------------------------
--------------------------------------------
--------------------------------------------
--------------------+++++-------------------
------------------++--+++++------+++++++++++
----------------+--+###########+++++++++++++
-----------------+##########++++++++++++++++
-----------------+#########+++++++----+-++++
----------------+###########++++-----+++-+++
-----------------+++++++++-----------+++++++
-----------------+-----#+------------+++++++
---------------++#+++####++++-++-----+++++++
--------------+++#######+-+###++-----+++++++
-------------++++#####++---+#+++----++++++++
--------------++++###+++++-+++------++++++++
--------------+++++++++++--+++------++++++++
---------------++--+++##++++---..----+++++++
-------------------------------.---.------++
--------------------+-----------------------
-------------------++++---------+-----------
-------------------##+++++++++++------------
------------------+############-------------
-------------------+###########+------------
------------------+############-------------
.-----------------+###########+-------------
..---------------+############---------.----
..---------------+###########+--------------
..---------------+###########---------------
.----------------###########+-------------..
.---------------+###########-------------...
----------------###########+------.----.....
----------------###########+-----.----......
-----..--------+##########+-----.---........
-----..-------.-+####+++#++----..-..........
-------------.-+#########+----..---.........
.........---..-##########+---..----.........)";       

WINDOW*
CreateWin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win); 
    
    return local_win;
}

// ========================== MEMORY_CONTROLLING ==============================

void 
ScreenContextInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);
 
    initscr();
    getmaxyx(stdscr , screen->rows, screen->columns);
    curs_set(0);
    refresh();
}

void 
ScreenContextDestroy(visualisation_context* screen)
{
    if (screen == NULL)
    {
        return;
    }

    DestroyWindow(&screen->img_window); 
    DestroyWindow(&screen->question_window);
    DestroyWindow(&screen->scan_window);

    DestroySubwindow(&screen->subwindow_1);
    DestroySubwindow(&screen->subwindow_2);
    DestroySubwindow(&screen->subwindow_3);

    *screen = {};

    endwin();
}

// ========================= WINDOW_MEM_CONTROLLING ===========================

void 
QuestionWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int question_width   = (int) (0.6 * screen->columns);
    const int question_height  = (int) (0.2 * screen->rows);
    const int question_start_y = (int) (0.05 * screen->rows);
    const int question_start_x =        0;

    screen->question_window = CreateWin(question_height, question_width,
                                        question_start_y, question_start_x);
}

void 
ImageWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int image_width   = (int) (0.5 * screen->columns);
    const int image_heigth  = (int) (0.5 * screen->rows);
    const int image_start_x = (int) (0.65 * screen->columns);
    const int image_start_y = (int) (0.05 * screen->rows); 
    
    screen->img_window = CreateWin(image_heigth, image_width,
                                   image_start_y, image_start_x);
}

void 
ScanWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int scan_width   = (int) (0.6 * screen->columns);
    const int scan_heigth  =        5;
    const int scan_start_x = (int) (0.0 * screen->columns);
    const int scan_start_y = (int) (0.3 * screen->rows); 

    screen->scan_window = CreateWin(scan_heigth, scan_width,
                                    scan_start_y, scan_start_x);
}

void 
SubWindow1Init(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int subwindow_1_width = (int) (0.9 * screen->columns);
    const int subwindow_1_height = (int) (0.1 * screen->rows);
    const int subwindow_1_x = 5;
    const int subwindow_1_y = 5;

    screen->subwindow_1.window = CreateWin(subwindow_1_height, subwindow_1_width, 
                                           subwindow_1_y, subwindow_1_x);  
    screen->subwindow_1.cursor_x = 1;
    screen->subwindow_1.cursor_y = 1;   
    
    wattron(screen->subwindow_1.window, A_REVERSE);
}

void 
SubWindow2Init(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int subwindow_2_width = (int) (0.9 * screen->columns);
    const int subwindow_2_height = (int) (0.3 * screen->rows);
    const int subwindow_2_x = 5;
    const int subwindow_2_y = (int) (0.3 * screen->rows);

    screen->subwindow_2.window = CreateWin(subwindow_2_height, subwindow_2_width, 
                                           subwindow_2_y, subwindow_2_x);  
    screen->subwindow_2.cursor_x = 2;
    screen->subwindow_2.cursor_y = 2;        
    
    wattron(screen->subwindow_2.window, A_ITALIC);
}

void 
SubWindow3Init(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int subwindow_3_width = (int) (0.9 * screen->columns);
    const int subwindow_3_height = (int) (0.3 * screen->rows);
    const int subwindow_3_x = 5;
    const int subwindow_3_y = (int) (0.7 * screen->rows);

    screen->subwindow_3.window = CreateWin(subwindow_3_height, subwindow_3_width, 
                                           subwindow_3_y, subwindow_3_x);  
    screen->subwindow_3.cursor_x = 2;
    screen->subwindow_3.cursor_y = 2;                                      
    
    wattron(screen->subwindow_3.window, A_ITALIC);
}

void 
DestroyWindow(WINDOW** local_win) 
{
    ASSERT(local_win != NULL);

    if (*local_win != NULL)
    {
        wclear(*local_win);
        wrefresh(*local_win);
        delwin(*local_win);
        *local_win = NULL;
    }
}

void 
DestroySubwindow(subwindow_s* subwindow)
{
    ASSERT(subwindow != NULL);

    DestroyWindow(&subwindow->window);
    subwindow->cursor_x = 0;
    subwindow->cursor_y = 0;
}

// ============================ USER_INTERACTIONS =============================

void 
AddTextToSubwindow(subwindow_s* subwindow,
                   const char*  message,
                   size_t       message_size)
{
    ASSERT(subwindow != NULL);
    ASSERT(subwindow->window != NULL);
    ASSERT(message != NULL);

    mvwprintw(subwindow->window, subwindow->cursor_y,
              subwindow->cursor_x, "%.*s", 
              (int) message_size, message);
    
    subwindow->cursor_y++;

    wrefresh(subwindow->window);
}

void 
ScanUserInput(visualisation_context* screen,  
              char*                  string,
              size_t                 max_string_count)
{
    ASSERT(screen != NULL);
    ASSERT(screen->scan_window != NULL);
    ASSERT(string != NULL);

    echo();
    mvwprintw(screen->scan_window, 1, 2, "Your input:");
    mvwgetnstr(screen->scan_window, 3, 2, string, (int) max_string_count);
    noecho();
}

user_option_e
GiveChoice()
{
    noecho();

    int user_input = 0;
    user_option_e current_state = USER_OPTION_YES;

    int yes_button_position_x = (int) (0.1 * getmaxx(stdscr));
    int yes_button_position_y = (int) (0.3 * getmaxy(stdscr));
    int no_button_position_x =  (int) (0.3 * getmaxx(stdscr));
    int no_button_position_y =  (int) (0.3 * getmaxy(stdscr));

    do
    {
        user_input = getch();

        refresh();
        switch(user_input)
        {
            case 'a':
                current_state = USER_OPTION_YES;
                attron(A_STANDOUT);
                mvprintw(yes_button_position_y, yes_button_position_x,
                         "YES");
                attroff(A_STANDOUT);
                mvprintw(no_button_position_y, no_button_position_x,
                         "NO");
                break;

            case 'd':
                current_state = USER_OPTION_NO;
                mvprintw(yes_button_position_y, yes_button_position_x,
                         "YES");
                attron(A_STANDOUT);
                mvprintw(no_button_position_y, no_button_position_x,
                         "NO");
                attroff(A_STANDOUT);
                break;

            case 'q':
                current_state = USER_OPTION_EXIT;
                break;
            default: break;
        }
        refresh();
    } while ((user_input != 'q') && (user_input != '\n'));

    mvprintw(yes_button_position_y, yes_button_position_x, "   ");
    mvprintw(no_button_position_y, no_button_position_x, "   ");

    refresh();

    return current_state;
}

void 
ShowMessage(visualisation_context* screen,
            const char*            message,
            size_t                 message_length)
{
    ASSERT(screen != NULL);
    ASSERT(screen->question_window != NULL);
    ASSERT(message != NULL);

    WINDOW* question_window = screen->question_window;

    srand((unsigned int) time(NULL));
    const unsigned int max_delay = 150000;

    const char* character_ask = "Pe'trovich asks if...";

    wclear(question_window);
    box(question_window, 0, 0);

    wattron(question_window, A_BOLD); // atrpritnw
    mvwprintw(question_window, 1, 1, "%s", character_ask);
    wattroff(question_window, A_BOLD);

    wattron(question_window, A_ITALIC);
    wmove(question_window, 2, 2);
    for (size_t letter_num = 0;
         (letter_num < message_length) && (message[letter_num] != 0);
         letter_num++)
    {
        waddch(question_window, (unsigned) message[letter_num]);
        usleep((__useconds_t)random() % max_delay);
        wrefresh(question_window);
    }
    wattroff(question_window, A_ITALIC);

    wrefresh(question_window);
}

void 
ShowAkinator(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    wclear(screen->img_window);

    wprintw(screen->img_window, "%s", AKINATOR);

    wrefresh(screen->img_window);
}

// ============================== MENU_INTERACTIONS ============================

static mode_menu_e
SwitchNextPosition(mode_menu_e current_mode)
{
    if (current_mode == MODE_MENU_COMPARE)
    {
        return MODE_MENU_GUESS;
    }

    return (mode_menu_e) (current_mode + 1); 
}

static mode_menu_e
SwitchPreviousPosition(mode_menu_e current_mode)
{
    if (current_mode == MODE_MENU_GUESS)
    {
        return MODE_MENU_COMPARE;
    }

    return (mode_menu_e) (current_mode - 1); 
}

static void
DrawButtons(mode_menu_e current_mode)
{
    int button_position_x = (int) (0.5 * getmaxx(stdscr));
    int guess_button_position_y = (int) (0.3 * getmaxy(stdscr));
    int definition_button_position_y = (int) (0.4 * getmaxy(stdscr) + 1);
    int compare_button_position_y = (int) (0.5 * getmaxy(stdscr) + 2);

    const char* guess_button_text = "Guess";
    const char* definition_button_text = "Define";
    const char* compare_button_text = "Compare";

    attroff(A_STANDOUT);
    mvprintw(guess_button_position_y, button_position_x - 1,
             "%s", guess_button_text);
    mvprintw(definition_button_position_y, button_position_x - 1, 
             "%s", definition_button_text);
    mvprintw(compare_button_position_y, button_position_x - 1, 
             "%s", compare_button_text);
    
    attron(A_STANDOUT);

    switch(current_mode)
    {
        case MODE_MENU_GUESS:
            mvprintw(guess_button_position_y, button_position_x - 1,
                     "%s", guess_button_text);
            break;

        case MODE_MENU_DEFINITION:
            mvprintw(definition_button_position_y, button_position_x - 1,
                     "%s", definition_button_text);
            break;
        
        case MODE_MENU_COMPARE:
            mvprintw(compare_button_position_y, button_position_x - 1,
                     "%s", compare_button_text);
            break;

        case MODE_MENU_QUIT:
            break;

        default: break;
    }

    attroff(A_STANDOUT);
}

mode_menu_e
ShowMenu()
{
    noecho();

    int user_input = 0;
    mode_menu_e current_mode = MODE_MENU_GUESS;

    do
    {
        DrawButtons(current_mode);

        user_input = getch();
        refresh();

        switch(user_input)
        {
            case 'w':
                current_mode = SwitchPreviousPosition(current_mode);
                break;

            case 's':
                current_mode = SwitchNextPosition(current_mode);
                break;

            case 'q':
                current_mode = MODE_MENU_QUIT;
                break;

            default: break;
        }

        refresh();
    } while ((user_input != 'q') && (user_input != '\n'));

    clear();
    refresh();

    return current_mode;
}



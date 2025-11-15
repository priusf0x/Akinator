#include "visuals.h"

#include <ncurses.h>   
#include <unistd.h>
#include <string.h>

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

static WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win); 
    
    return local_win;
}

static void PrintASCIImage(WINDOW* window, const char* asci_image);
void ReadUserInput(WINDOW* window, char* string, size_t max_string_count);

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
    ASSERT(screen != NULL);

    DestroyWindow(screen->img_window);  
    DestroyWindow(screen->question_window);
    DestroyWindow(screen->scan_window);
    *screen = {};

    endwin();
}

// ========================= WINDOW_MEM_CONTROLLING ===========================

void 
QuestionWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int columns = screen->columns;
    const int rows = screen->rows;

    const int question_width   = (int) (0.6 * columns);
    const int question_height  = (int) (0.2 * rows);
    const int question_start_y = (int) (0.05 * rows);
    const int question_start_x =        0;

    screen->question_window = create_newwin(question_height, question_width,
                                            question_start_y, question_start_x);
}

void 
ImageWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int columns = screen->columns;
    const int rows = screen->rows;

    const int image_width   = (int) (0.5 * columns);
    const int image_heigth  = (int) (0.5 * rows);
    const int image_start_x = (int) (0.65 * columns);
    const int image_start_y = (int) (0.05 * rows); 
    
    screen->img_window = create_newwin(image_heigth, image_width,
                                       image_start_y, image_start_x);
}

void 
ScanWindowInit(visualisation_context* screen)
{
    ASSERT(screen != NULL);

    const int columns = screen->columns;
    const int rows = screen->rows;

    const int scan_width   = (int) (0.6 * columns);
    const int scan_heigth  = (int) (5);
    const int scan_start_x = (int) (0.0 * columns);
    const int scan_start_y = (int) (0.3 * rows); 

    screen->scan_window = create_newwin(scan_heigth, scan_width,
                                     scan_start_y, scan_start_x);
}

void DestroyWindow(WINDOW *local_win) {
    wclear(local_win);
    wrefresh(local_win);
    delwin(local_win);
}

// ============================================================================

void 
ReadUserInput(WINDOW* window,  
              char*   string,
              size_t  max_string_count)
{
    ASSERT(window != NULL);
    ASSERT(string != NULL);

    echo();
    mvwprintw(window, 1, 2, "Your input:");
    mvwgetnstr(window, 3, 2, string, max_string_count);
    noecho();
}

user_option_e
GiveChoice()
{
    noecho();

    int user_input = getch();
    user_option_e current_state = USER_OPTION_YES;

    int yes_button_position_x = (int) (0.1 * getmaxx(stdscr));
    int yes_button_position_y = (int) (0.3 * getmaxy(stdscr));
    int no_button_position_x =  (int) (0.3 * getmaxx(stdscr));
    int no_button_position_y =  (int) (0.3 * getmaxy(stdscr));

    do
    {
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
        user_input = getch();

    } while (user_input != 'q' && user_input != '\n');

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

    const size_t delay = 100000;

    const char* character_ask = "Pe'trovich asks if...";

    wclear(question_window);
    box(question_window, 0, 0);

    wattron(question_window, A_BOLD);
    mvwprintw(question_window, 1, 1, "%s", character_ask);
    wattroff(question_window, A_BOLD);

    wattron(question_window, A_ITALIC);
    wmove(question_window, 2, 2);
    for (size_t letter_num = 0; letter_num < message_length; letter_num++)
    {
        waddch(question_window, (unsigned char) message[letter_num]);
        usleep(delay);
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

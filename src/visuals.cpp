#include "visual.h"

#include <ncurses.h>   
#include <unistd.h>
#include <string.h>

#include "Assert.h"

static WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win); 
    
    return local_win;
}

static void destroy_win(WINDOW *local_win) {
    wclear(local_win);
    wrefresh(local_win);
    delwin(local_win);
}

static void ShowMessage(WINDOW* window, const char* message, size_t message_length);
static void PrintASCIImage(WINDOW* window, const char* asci_image);
static user_option_e ProvideChoice();

int
main()
{
    initscr();
    int rows = 0, columns = 0;
    getmaxyx(stdscr ,rows, columns);
    curs_set(0);
    refresh();

    const int image_width   = (int) (0.5 * columns);
    const int image_heigth  = (int) (0.5 * rows);
    const int image_start_x = (int) (0.65 * columns);
    const int image_start_y = (int) (0.05 * rows); 
    
    WINDOW* img_win = create_newwin(image_heigth, image_width,
                                    image_start_y, image_start_x);

    PrintASCIImage(img_win, AKINATOR);
    
    refresh();

    const int question_width   = (int) (0.6 * columns);
    const int question_height  = (int) (0.2 * rows);
    const int question_start_y = (int) (0.05 * rows);
    const int question_start_x =        0;

    WINDOW* quest_win = create_newwin(question_height, question_width,
                                      question_start_y, question_start_x);

    
    ShowMessage(quest_win, "Do you see theeeeese cocks", 25);

    ProvideChoice();

    refresh();
    
    destroy_win(quest_win);  
    destroy_win(img_win);
    endwin();

    return 0;
} 

static user_option_e
ProvideChoice()
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

    return current_state;
}

static void 
ShowMessage(WINDOW*     window,
            const char* message,
            size_t      message_length)
{
    ASSERT(window != NULL);
    ASSERT(message != NULL);

    const size_t delay = 50000;

    const char* character_ask = "Pe'trovich says...";

    wclear(window);
    box(window, 0, 0);

    wattron(window, A_BOLD);
    mvwprintw(window, 1, 1, "%s", character_ask);
    wattroff(window, A_BOLD);

    wattron(window, A_ITALIC);
    wmove(window, 2, 2);
    for (size_t letter_num = 0; letter_num < message_length; letter_num++)
    {
        waddch(window, (unsigned char) message[letter_num]);
        usleep(delay);
        wrefresh(window);
    }
    wattroff(window, A_ITALIC);

    wrefresh(window);
}


static void 
PrintASCIImage(WINDOW*     window,
               const char* asci_image)
{
    ASSERT(window != NULL);
    ASSERT(asci_image != NULL);

    wclear(window);

    wprintw(window, "%s", asci_image);

    wrefresh(window);
}

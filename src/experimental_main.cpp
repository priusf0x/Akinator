#include "visuals.h"

#include <ncurses.h>

#include "state_machine.h"

enum mode_menu_e 
{
    MODE_MENU_GUESS = 0,
    MODE_MENU_DEFINITION = 1,
    MODE_MENU_COMPARE = 2,
    MODE_MENU_QUIT = 3
};

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
    int definition_button_position_y =  (int) (0.4 * getmaxy(stdscr) + 1);
    int compare_button_position_y =  (int) (0.5 * getmaxy(stdscr) + 2);

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

program_state_e
AskProgramMode()
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
}


/*
----------------------------------- SCHEDULE BUILDER -----------------------------------
-- This program was born out of necessity and a touch of curiosity.                    --
-- Have you ever found yourself sketching schedules on scraps of paper or your phone?  --
-- Me too! But this time, I decided to turn it into a coding adventure.                --
-- Inspired to have a little fun with the C language, I created this Schedule Builder. --
-- It's a simple program that takes your start date and end date and helps you plan.   --
-- Who says practicality can't be fun? Let's dive into some C magic!                   --
----------------------------------------------------------------------------------------
*/

#include <ncurses.h>
#include <stdlib.h>

#define WIDTH 10
#define HEIGHT 20

typedef struct _point{
    int x;
    int y;
}Point;

/*DMY: Date-Month-Year is used as an input to create a date*/
typedef struct _dmy{
    int day;
    int month;
    int year;
}DMY;

/*DMY *createStartDate: Creates a start date for building your preferred schedule
returns a pointer to a DMY*/
DMY *createStartDate(int day, int month, int year){
    //allocate memory because we are returning a pointer 
    DMY *dmy = malloc(sizeof(DMY));                 //remember to free
    dmy->day = day;
    dmy->month = month;
    dmy->year = year;

    return dmy;
}

/*int calculateNumOfDays: Calulates the number of days between the start date and end date
returns an int*/
int calculateNumOfDays(DMY *startDate, DMY *endDate){
    
}
/*void freeDMY: free dynamically allocated memory for the UserCreated Data-structure
has no return value*/
void freeDMY(DMY *dmy){
    free(dmy);
}

int main(void){

    initscr();
    // if (has_colors() == FALSE) {  // Check if terminal supports color
    //     endwin();
    //     printf("Your terminal does not support color\n");
    //     return 1;
    // }

    // start_color();
    // init_pair(1, COLOR_RED, COLOR_BLACK);          //blue for text and black for background

    // bkgd(COLOR_PAIR(1));  // Set the background of the entire screen
    // clear();

    int day,month,year;
    // attron(COLOR_PAIR(1));
    printw(
    "----------------------------------- SCHEDULE BUILDER -----------------------------------\n"
    "-- Because who needs paper, right? It's not like it's been working for centuries.     --\n"
    "-- Instead of jotting schedules on your phone like a normal person, I thought:        --\n"
    "-- Hey, why not overcomplicate it and write a whole program in C?                     --\n"
    "-- This masterpiece takes your start and end dates and does the heavy lifting.        --\n"
    "-- It's totally not because I was bored or looking for an excuse to mess with C.      --\n"
    "-- Anyway, enjoy this marvel of modern procrastination engineering.                   --\n"
    "----------------------------------------------------------------------------------------\n"
    );

    // attroff(COLOR_PAIR(1));
    printw("Enter your start date: <dd-mm-yyy>");

    refresh();
    getch();
    endwin();

    return 0;
}
/*
----------------------------------- SCHEDULE BUILDER -----------------------------------
-- This program was born out of necessity and a touch of curiosity.                    --
-- Have you ever found yourself sketching schedules on scraps of paper or your phone?  --
-- Me too! But this time, I decided to turn it into a coding adventure.                --
-- Inspired to have a little fun with the C language, I created this Schedule Builder. --
-- It's a simple program that takes your start date and end date and helps you plan.   --
-- Who says practicality can't be fun? Let's dive into some C magic!                   --
-- ncurses adds stdio.h, unctrl.h, stdarg.h, stddef.h so we don't need to re-include   --
----------------------------------------------------------------------------------------
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>


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

Point *createPoint(int x, int y){
    Point *p1 = malloc(sizeof(Point));
    p1->x = x;
    p1->y = y;

    return p1;
}

void freePoint(Point *point){
    free(point);
}

void printDate(DMY *dmy){
    printw("----------------------------------------------------------\n"
           "|START DATE: <%d> - <%d> - <%d>                         |\n"
           "----------------------------------------------------------\n", 
           dmy->day, dmy->month, dmy->year);
}

void printDetails(){
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
}

int main(void){

    initscr();                                       //initialize ncurses
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);                  //to get current date
    // Point *moveScreen = createPoint(5,2);
    DMY *userStartDate = createStartDate(00,00,0000);

    int day,month,year;
    // move(moveScreen->y, moveScreen->x);
    printDetails();

    printw("Today's date is %d-%d-%d on the system\n",tm->tm_mday,tm->tm_mon + 1, tm->tm_year + 1900);
    addstr("Enter your start date: <dd-mm-yyy>: ");
    scanw("%d %d %d", &userStartDate->day, &userStartDate->month, &userStartDate->year);
    printDate(userStartDate);
    //handle error if user input exceeds our expectations 
    refresh();
    getch();
    
    endwin();

    // freePoint(moveScreen);
    freeDMY(userStartDate);

    return 0;
}
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
#include <math.h>

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

typedef struct _schedule{
    DMY **days;
    int size;
    int capacity;
}Schedule;

/*createSchedule: Allocates dynamic memory on the heap to create a new schedule instance*/
Schedule *createSchedule(int capacity){
    Schedule *schedule = malloc(sizeof(Schedule));
    if(schedule == NULL){
        printw("Failed to allocate memory for schedule");
        return NULL;
    }

    schedule->capacity = capacity;
    schedule->size = 0;
    schedule->days = malloc(sizeof(DMY*) * schedule->capacity);
    if(!schedule->days){
        printw("Unable to allocate memory for days arr!");
        return NULL;
    }

    for(int i = 0; i < schedule->capacity; i++){
        schedule->days[i] == NULL;
    }

    return schedule;
}


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

/*isleapYear: returns 1 0r 0 depeneding on the year*/
bool isLeapYear(int year) {
    if (year % 400 == 0) return true; // Divisible by 400, so it's a leap year.
    else if (year % 100 == 0) return false; // Divisible by 100 but not 400, so not a leap year.
    else if (year % 4 == 0) return true; // Divisible by 4 but not 100, so it's a leap year.
    else return false; // Not divisible by 4, so not a leap year.
}

/*monthDays: returns the required number of days of the particular month*/
int monthDays(int month, int year){
    switch(month){
        case 4: case 6: case 9: case 11:
            return 30;
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 2:
            return isLeapYear(year) ? 29 : 28;          // condition ? value_if_true : value_if_false
        default:
            return 0;           //month is invalid
    }
}

/*convertYearToDay: Convert year to days*/
int convertYearToDay(int year){
    if(isLeapYear(year)) return year * 366;
    else return year * 365;
}

/*int calculateNumOfDays: Calulates the number of days between the start date and end date
returns an int*/
int calculateNumOfDays(DMY *startDate, DMY *endDate){
    int totalDayResult = 0;
    //we start date and end date
    if ((startDate->year > endDate->year) ||
        (startDate->year == endDate->year && startDate->month > endDate->month) ||
        (startDate->year == endDate->year && startDate->month == endDate->month && startDate->day > endDate->day)) {
        int tempDay = startDate->day;
        int tempMonth = startDate->month;
        int tempYear = startDate->year;

        startDate->day = endDate->day;
        startDate->month = endDate->month;
        startDate->year = endDate->year;

        endDate->day = tempDay;
        endDate->month = tempMonth;
        endDate->year = tempYear;
    }

   
    if (startDate->month == endDate->month) {
        // Same month: I just directly calculate the difference
        totalDayResult = endDate->day - startDate->day;
    } 
    else {
        // Different months in the same year
        for (int m = startDate->month; m <= endDate->month; ++m) {
            if (m == startDate->month) {
                totalDayResult += monthDays(m, startDate->year) - startDate->day;
            } 
            else if (m == endDate->month) {
                totalDayResult += endDate->day;
            } 
            else {
                totalDayResult += monthDays(m, startDate->year);
            }
        }
    }


    if(startDate->year != endDate->year){
         //days remaining in the start year
        for(int m = startDate->month; m <= 12; ++m){
            if(m == startDate->month){
                //eg if day is 15th June it first gets all the days and returns the total - 15
                totalDayResult += monthDays(m, startDate->year) - startDate->day;
            }
            else{
                //get all the days for the month if not in the start month
                totalDayResult += monthDays(m, startDate->year);
            }
        }

        //days in between years
        for(int y = startDate->year; y < endDate->year; ++y){
            totalDayResult += isLeapYear(y) ? 366 : 365;
        }

        //elapsed days in the end year
        for (int m = 1; m <= endDate->month; ++m) {
            if (m == endDate->month) {
                totalDayResult += endDate->day;
            } else {
                totalDayResult += monthDays(m, endDate->year);
            }
        }

    }
    // if (startDate->day == endDate->day &&
    //     startDate->month == endDate->month &&
    //     startDate->year == endDate->year) {
    //     return 1;  // Same day = 1 day
    // }


   
    return totalDayResult;
}

/*void freeDMY: free dynamically allocated memory for the UserCreated Data-structure
has no return value*/
void freeDMY(DMY *dmy){
    free(dmy);
}

/*createPoint: dynamically allocates memory and creates a new point*/
Point *createPoint(int x, int y){
    Point *p1 = malloc(sizeof(Point));
    p1->x = x;
    p1->y = y;

    return p1;
}

void freePoint(Point *point){
    free(point);
}

void printDate(char *arg,DMY *dmy){
    printw("--------------------------------------\n"
           "|%s: <%d> - <%d> - <%d>               \n"
           "--------------------------------------\n", 
           arg,dmy->day, dmy->month, dmy->year);
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

void printRD(int days){
    if(days <= 0){
        printw("Invalid number of days\n");
        return;  //becuase it is a void function
    }

    int weeks = days / 7;
    int remainingDays = days % 7;

    printw("------------------------\n"
           "|Weeks: %d | Days: %d  |\n"
           "------------------------\n"
           ,weeks,remainingDays);
}

/*free schedule: frees the dynamic memory allocated for schedule itself, an array of pointers 
pointing an array dates*/
void freeSchedule(Schedule *schedule) {
    if (schedule == NULL) {
        return; // Safeguard against null pointer input
    }

    // Free each element in the days array
    for (int i = 0; i < schedule->capacity; i++) {
        if (schedule->days[i]) {
            free(schedule->days[i]);  // Free individual DMY pointers
        }
    }

    // Free the array of pointers
    free(schedule->days);

    // Free the schedule structure itself
    free(schedule);
}

void addToSchedule(DMY *mydate, Schedule *schedule) {
    // Check if resizing is needed
    if (schedule->size == schedule->capacity) {
        schedule->capacity = (schedule->capacity == 0) ? 2 : schedule->capacity * 2;
        DMY **newDays = realloc(schedule->days, sizeof(DMY*) * schedule->capacity);

        if (newDays == NULL) {
            printw("Memory allocation failed, exiting program.\n");
            freeSchedule(schedule);
            exit(EXIT_FAILURE);
        }


        schedule->days = newDays;
    }

    // Add the new date to the schedule array
    schedule->days[schedule->size] = createStartDate(mydate->day, mydate->month, mydate->year);
    schedule->size++;
}

void printSchedule(Schedule *schedule){
    printw("Starting Day %d: <%d> - <%d> - <%d>\n", 1,
    schedule->days[0]->day,schedule->days[0]->month, schedule->days[0]->year);
    for(int i = 1; i < schedule->size; i++){
        if(i != schedule->size - 1) {
            printw("Day %d: <%d> - <%d> - <%d>\n",i,
            schedule->days[i]->day, schedule->days[i]->month, schedule->days[i]->year);
        }
        else printw("End Day %d: <%d> - <%d> - <%d>\n", i,
        schedule->days[i]->day, schedule->days[i]->month, schedule->days[i]->year);
    }
    
}

bool isValidDate(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    return day <= monthDays(month, year);
}


int main(void){

    initscr();                                       //initialize ncurses
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);                  //to get current date
    DMY *userStartDate = createStartDate(0,0,0);
    DMY *userEndDate = createStartDate(0,0,0);

    int totalDays = 0;
    printDetails();
    printw("Today's date is %d-%d-%d on the system\n",tm->tm_mday,tm->tm_mon + 1, tm->tm_year + 1900);
    
    //user start date
    do {
        addstr("Enter your start date: <dd-mm-yyyy>: ");
        scanw("%d %d %d", &userStartDate->day, &userStartDate->month, &userStartDate->year);
        if (!isValidDate(userStartDate->day, userStartDate->month, userStartDate->year)) {
            printw("Invalid date! Please try again.\n");
        }
    } while (!isValidDate(userStartDate->day, userStartDate->month, userStartDate->year));
    // addstr("Enter your start date: <dd-mm-yyy>: ");
    // scanw("%d %d %d", &userStartDate->day, &userStartDate->month, &userStartDate->year);
    printDate("START DATE",userStartDate);

    //user end date
    do {
        addstr("Enter your end date: <dd-mm-yyy>: ");
        scanw("%d %d %d", &userEndDate->day, &userEndDate->month, &userEndDate->year);
        if(!isValidDate(userEndDate->day, userEndDate->month, userEndDate->year)){
            printw("Invalid date! Please try again.\n");
        }
    } while(!isValidDate(userEndDate->day, userEndDate->month, userEndDate->year));
    
    
    printDate("END DATE",userEndDate);

    printw("Total Days: %d\n",totalDays = calculateNumOfDays(userStartDate, userEndDate));
    printRD(totalDays);

    Schedule *myschedule = createSchedule(totalDays);
    //to help us know the start date 
    addToSchedule(userStartDate, myschedule);
    printSchedule(myschedule);

    refresh();
    getch();
    
    endwin();

    // freePoint(moveScreen);
    freeDMY(userStartDate);
    freeDMY(userEndDate);
    freeSchedule(myschedule);

    return 0;
}

//next addition


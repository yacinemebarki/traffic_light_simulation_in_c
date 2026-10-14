#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <signal.h>
  #include <sys/time.h>
#endif

#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"
#define CYAN   "\033[36m"
#define RESET  "\033[0m"
#define BOLD   "\033[1m"

#ifdef _WIN32
HANDLE tick_event;

VOID CALLBACK timer_isr(PVOID param, BOOLEAN fired){
    SetEvent(tick_event);                 
}

void wait_tick(void){
    WaitForSingleObject(tick_event, INFINITE);
}

void start_timer(void){
    HANDLE hTimer;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | 0x0004);  

    tick_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    CreateTimerQueueTimer(&hTimer, NULL, timer_isr, NULL, 1000, 1000, WT_EXECUTEDEFAULT);
}
#else
volatile sig_atomic_t tick = 0;

void timer_isr(int sig){
    tick = 1;
}

void wait_tick(void){
    sigset_t block, old;
    sigemptyset(&block);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, &old);
    while(!tick) sigsuspend(&old);
    tick = 0;
    sigprocmask(SIG_SETMASK, &old, NULL);
}

void start_timer(void){
    struct sigaction sa;
    sa.sa_handler = timer_isr;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    struct itimerval t = {{1, 0}, {1, 0}};   
    setitimer(ITIMER_REAL, &t, NULL);
}
#endif

struct Road {
    char name[20];  
    int remain;     
    int number;     
};

void passe_car(struct Road *road_green, struct Road *road_red){
    printf("\n" BOLD "========================================\n");
    printf("           TRAFFIC LIGHT SYSTEM\n");
    printf("========================================" RESET "\n");

    for(int i = 1; i <= 60; i++){
        printf("\n" GREEN "[GREEN] " RESET "%s: %d seconds remaining\n",
               road_green->name, 60 - i);
        printf(RED "[RED]   " RESET "%s: %d seconds remaining\n",
               road_red->name, 60 - i);

        if(i % 3 == 0){
            if(road_green->remain > 0){
                printf(GREEN "  >> A car from %s has passed!\n" RESET,
                       road_green->name);
                road_green->remain--;
                printf(CYAN "  Cars remaining on %s: %d\n" RESET,
                       road_green->name, road_green->remain);
            }else{
                printf(YELLOW "  No cars remain on %s\n" RESET,
                       road_green->name);
            }
        }
        wait_tick();
    }
    int remain_yellow_time = 10;

    while(remain_yellow_time > 0){
        printf("\n" YELLOW "[YELLOW] " RESET
               "Traffic light changing in %d seconds...\n",
               remain_yellow_time);
        wait_tick();
        remain_yellow_time--;
    }

    printf("\n" BOLD "----------------------------------------\n");
    printf("          SWITCHING THE ROADS\n");
    printf("----------------------------------------" RESET "\n");
}

int main(){
    struct Road road_a = {"Road_A", 50, 50};
    struct Road road_b = {"Road_B", 40, 40};

    struct Road road_green = road_a;
    struct Road road_red = road_b;
    struct Road road;

    start_timer();

    printf(CYAN BOLD "------- PROGRAM STARTED -------\n" RESET);

    while(1){
        passe_car(&road_green, &road_red);

        road = road_green;
        road_green = road_red;
        road_red = road;

        if(road_green.remain == 0){
            road_green.remain = road_green.number;
        }
        if(road_red.remain == 0){
            road_red.remain = road_red.number;
        }
    }
}
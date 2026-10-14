# Traffic Light System

This project simulates a two-road traffic light controller in C. One road has the green light at a time, the other stays red, and the roles alternate automatically.

## What it does

- Maintains two roads: `Road_A` and `Road_B`
- Uses a timer interrupt to advance the simulation every second
- Displays green/red/yellow states with ANSI color output
- Tracks vehicles remaining on the active green road
- Repeats the cycle until the program is interrupted

## Build and run

On Linux or macOS:

```bash
gcc -Wall main.c -o main
./main
```

On Windows (MinGW / GCC):

```bash
gcc main.c -o main.exe
main.exe
```

Press `Ctrl + C` to stop the program.

## Notes

The program uses a timer-based signal mechanism on Unix-like systems (`SIGALRM` + `setitimer`) and a Windows timer queue on Windows.

This behavior is documented in the project PDF report: `traffic_light_report.pdf`.

#include <sys/time.h>

typedef struct {
    struct timeval startTime;
    struct timeval endTime;
} Timer;

Timer timer;

void startTime();
void stopTime();
void elapsedTime();


void startTime() {
	printf("Start Timer.\n");
    gettimeofday(&(timer.startTime), NULL);
}

void stopTime() {
    printf("Stop Timer.\n");
    gettimeofday(&(timer.endTime), NULL);
}

void elapsedTime() {
	float elapseTime = (float) ((timer.endTime.tv_sec - timer.startTime.tv_sec) \
                + (timer.endTime.tv_usec - timer.startTime.tv_usec)/1.0e6);
	printf("Elapsed Time: %4.2f Sec.\n",elapseTime);
}


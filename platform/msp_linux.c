
#include "msp_linux.h"
#include  <time.h>
uint32_t millis(void)
{
    uint64_t            time_ms;
    struct timespec     ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    time_ms = ((uint64_t)ts.tv_sec * (uint64_t)1000) + (ts.tv_nsec / 1000 / 1000);

    return time_ms;
}

void delay(uint32_t ms)
{
    usleep(ms*1000);
}

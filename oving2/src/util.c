/*
 * Utility functions.
 *
 */
#include <stdlib.h>
#include <time.h>

#include <sys/unistd.h>
#include <sys/fcntl.h>


/*
 * Summary:      	Suspends the current thread
 * Parameters:   	Number of miliseconds the thread should be suspended
 * Return:
 */
void SleepEx(unsigned long msec, int dummy){
  struct timespec req;
  struct timespec rem;

  req.tv_sec = msec / 1000;
  req.tv_nsec = (msec % 1000)*1000000L;

  nanosleep(&req, &rem);

  return;
}

/*
 * Summary:      	Set the socket to be non-blocking
 * Parameters:   	Socket descriptor
 * Return:			0 for OK, -1 for Error
 */
int setNonblocking(int fd) {
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

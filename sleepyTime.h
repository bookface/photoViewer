
#ifndef SLEEPYTIME_H
#define SLEEPYTIME_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>                    // just use built-in Sleep
#else
  #include <time.h>                       // for nanosleep
  inline void Sleep(int msecs) {
      int secs = msecs / 1000;
      int milliseconds = msecs - secs * 1000;
      struct timespec t;
      t.tv_sec = secs;
      t.tv_nsec = milliseconds * 1000 * 1000;
      nanosleep(&t,NULL);
  }
#endif

#endif //SLEEPYTIME_H

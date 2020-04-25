
#ifndef SLEEPYTIME_H
#define SLEEPYTIME_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>                    // just for Sleep()
  inline void Sleepy(int secs, int msecs) {
      Sleep(secs * 1000 + msecs);
  }
#else
  #include <time.h>                       // just for nanosleep
  inline void Sleepy(int secs, int msecs) {
      struct timespec t;
      t.tv_sec = secs
      t.tv_nsec = msecs * 1000 * 1000;
      nanosleep(&t,NULL);
  }
#endif

#endif //SLEEPYTIME_H

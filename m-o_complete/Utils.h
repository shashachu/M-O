#ifndef UTILS_H
#define UTILS_H

// Non-blocking delay
void simDelay(long period){
  long delayMillis = millis() + period;
  while (millis() <= delayMillis)
  {
    int x = 0; // dummy variable, does nothing
  }
}

#endif // UTILS_H

//General global functions

#include "arduino.h"
#include "miscFunctions.h"

// Function that return average of an array. 

double averageArray(int a[], int n) { 
    // Find sum of array element 
    int sum = 0; 
    for (int i=0; i<n; i++) 
       sum += a[i]; 
  
    return sum/n; 
} 

void debugPrint(int debugPrioritySetting, String routine, int priority, String info){
  String message = " ";
  
  if (Serial.available() > 0) {
    if(debugPrioritySetting >= priority){
      message = String(F("debugPrint - ")) + String(priority) + F(" - ") + String(routine) + F(" - ") + String(info) + F(" - ") + String(freeMemory());
      Serial.println(message);
    }
  }
}

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

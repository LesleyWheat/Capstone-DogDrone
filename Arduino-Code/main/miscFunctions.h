//Header for global misc functions 

#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

double averageArray(int a[], int n);
double fuzzyNum(double num, double sigma);
void debugPrint(int debugPrioritySetting, String routine, int priority, String info);
int freeMemory();

#endif // MISC_FUNC_H

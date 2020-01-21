//Creates a timer that operates on a real time check based on the arduino internal clock
//Used as an alarm or to create time based loops
//Uses milliseconds
//Could have over flow issues if runtime is too long

#ifndef REAL_TIMER_H
#define REAL_TIMER_H

class realTimer{
  private:
    //Private variables
    unsigned long accum;
    double lastCheck;
    double enable;
    unsigned long targetTime;

    //Private variables
    
  public:
    //Public variables
    
    //Public functions
    void init(double setTime);
    bool check(bool condition);
};

#endif // REAL_TIMER_H

#ifndef CUSTOM_CLOCK_H
#define CUSTOM_CLOCK_H

#include <Arduino.h>
#include <uClock.h>

class BlipClock {
public:
  // Callback types
  typedef void (*ClockCallback)();
  typedef void (*StepCallback)(uint8_t step);
  
  // Initialize with default 16 steps
  void begin(uint8_t custom_steps = 16);
  
  // Tempo control
  void setTempo(uint16_t bpm);
  uint16_t getTempo() { return _bpm; }
  
  // Step division control
  void setCustomSteps(uint8_t steps);
  uint8_t getCustomSteps() { return _custom_steps; }
  
  // Transport control
  void start();
  void stop();
  bool isRunning() { return uClock.isRunning(); }
  
  // Callback registration
  void on16PPQN(ClockCallback callback) { uClock.setOnPPQN(callback); }
  void on16Step(StepCallback callback) { _step16_callback = callback; }
  void onCustomStep(StepCallback callback) { _custom_step_callback = callback; }
  
  // Internal handlers (public for uClock callbacks)
  void handle16PPQN();
  void handleCustomStep();

private:
  uint8_t _custom_steps = 16;
  uint16_t _bpm = 120;
  uint32_t _custom_interval = 0;
  uint32_t _last_custom_step = 0;
  uint8_t _current_custom_step = 0;
  
  // Callbacks
  StepCallback _step16_callback = nullptr;
  StepCallback _custom_step_callback = nullptr;
  
  void calculateIntervals();
};

extern BlipClock blipClock;

#endif
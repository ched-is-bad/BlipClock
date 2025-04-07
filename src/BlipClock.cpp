#include "BlipClock.h"

BlipClock blipClock;

void BlipClock::begin(uint8_t custom_steps) {
  setCustomSteps(custom_steps);
  uClock.init();
  uClock.setOnPPQN([this]() { this->handle16PPQN(); });
}

void BlipClock::setTempo(uint16_t bpm) {
  _bpm = bpm;
  calculateIntervals();
  uClock.setTempo(bpm);
}

void BlipClock::setCustomSteps(uint8_t steps) {
  _custom_steps = constrain(steps, 1, 16);
  calculateIntervals();
}

void BlipClock::start() {
  _last_custom_step = millis();
  _current_custom_step = 0;
  uClock.start();
}

void BlipClock::stop() {
  uClock.stop();
}

void BlipClock::calculateIntervals() {
  // Standard 24PPQN interval (uClock's internal timing)
  float standard_interval = (60000.0 / (_bpm * 4.0));
  // Custom interval to complete cycle in same time as 16 steps
  _custom_interval = (standard_interval * 16.0) / _custom_steps;
}

void BlipClock::handle16PPQN() {
  // Handle standard 16-step clock
  static uint8_t ppqn_count = 0;
  static uint8_t current_step = 0;
  
  ppqn_count++;
  if (ppqn_count >= 6) { // 24PPQN / 4 = 6 PPQN per step (16 steps)
    ppqn_count = 0;
    current_step = (current_step + 1) % 16;
    if (_step16_callback) {
      _step16_callback(current_step);
    }
  }
  
  // Handle custom step clock
  uint32_t now = millis();
  if (now - _last_custom_step >= _custom_interval) {
    _last_custom_step = now;
    _current_custom_step = (_current_custom_step + 1) % _custom_steps;
    if (_custom_step_callback) {
      _custom_step_callback(_current_custom_step);
    }
  }
}
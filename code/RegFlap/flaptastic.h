// Split-Flap display control software.
// Uses 74HC595 shift registers for scalability.
// Elliot Baptist 2019/06/13

#ifndef FLAPTASTIC_H
#define FLAPTASTIC_H

// Includes
#include <stdint.h>
#include <SPI.h>

namespace EWNB {

  class Flaptastic {
    // Types
    public:
      // User-provided display configuration data
      struct disp_cfg_t {
        // bool microstep;
        // bool accelerate;
        uint8_t n_oe_pin;
      };
      // User-provided unit configuration data
      struct unit_cfg_t {
        uint8_t motor_level:1, home_rising:1, dir:1;
        uint8_t thresh;
        uint8_t flaps;
        int steps;
        int offset;
        int tolerance; // TODO: move to display config?
      };
    private:
      // Internal unit configuration data
      struct unit_int_cfg_t {
        uint8_t motor_level:1, home_rising:1, dir:1;
        uint8_t thresh;
        int steps;
        int offset;
        int home_start; // TODO: change back to tolerance to save memory?
        int home_end;
        unsigned long msteps_flap; // milli-steps per flap
      };
      // Internal unit state
      struct unit_state_t {
        uint8_t homed:1, prev_home:2, phase:2, out0:1, out1:1;
        int pos;
        int target;
      };
      // Constants
      static const int MAX_UNITS = 20; // TODO: allow user to change without editing this file
      // Variables
      volatile bool _idle;
      uint8_t _num_units;
      SPIClass* _spi;
      disp_cfg_t _disp_cfg;
      unit_int_cfg_t _unit_cfg[MAX_UNITS];
      volatile unit_state_t _unit_state[MAX_UNITS];

    public:
      Flaptastic();
      void init(disp_cfg_t disp_cfg, SPIClass* spi);
      bool addUnit(unit_cfg_t unit_cfg);
      void setFlap(int unit, int flap);
      void setOut(int unit, int output, bool value);
      bool step();
      bool done(int unit);
      bool allDone();
      void reset();
  };

};

#endif

#ifndef CS_PROBE_H
#define CS_PROBE_H

#include <string>
#include "probe.h"
#include "cs_probe.h"
#include "cs_utils.h"

// inherits from Probe to define a CS probe
class CSProbe:public Probe {
  public:
    // The csutils (csu) object will allow us to perform some
    // sequence to color space work
    CSProbe(std::string, CSUtils &);
    bool is_cs() {return true;}; // This is a CS probe
    std::string get_cs_five_p(void) { return cs_five_p; };

  private:
    std::string cs_five_p;
    std::string cs_three_p;
    std::string cs_ref;
    std::string cs_var;
    CSUtils     *csu;
};

#endif

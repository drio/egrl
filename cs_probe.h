#ifndef CS_PROBE_H
#define CS_PROBE_H

#include <string>
#include "probe.h"
#include "cs_probe.h"

// inherits from Probe to define a CS probe
class CSProbe:public Probe {
  public:
    CSProbe(std::string);
    // This is a CS probe
    bool is_cs() {return true;};
    std::string get_cs_five_p(void) { return cs_five_p; };

  private:
    void set_cs();
    std::string cs_five_p;
    std::string cs_three_p;
    std::string cs_ref;
    std::string cs_var;
};

#endif

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
    CSProbe(std::string, CSUtils *);
    //~CSProbe() {};
    bool is_cs() {return true;}; // This is a CS probe
    std::string get_cs_five_p(void) { return cs_five_p; };
    // Rewrite the method of Probe since it is different for a CS probe
    // We will receive a dibase not a nucleotide
    void update_counters(std::string);
    void cs_info(void);

  private:
    std::string cs_five_p;
    std::string cs_three_p;
    std::string cs_ref, cs_var, cs_o1, cs_o2;
    CSUtils     *csu; // The CSutils object contain utilities to work in color space
    void to_cs(void);
};

#endif

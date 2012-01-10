#ifndef PROBE_H
#define PROBE_H

#include <string>
#include "Util/Util.h"

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

// Encapsulates a probe

class Probe {
  public:
    Probe() {};
    Probe(std::string);
    ~Probe();

    void set_all(std::string, std::string, std::string,
                 std::string, std::string, std::string, std::string);
    bool is_cs(void)                  { return false; };
    std::string get_chrm(void)        { return chrm; };
    std::string get_coordinates(void) { return coordinates; };
    std::string get_id(void)          { return id; };
    std::string get_five_p(void)      { return five_p; };
    std::string get_three_p(void)     { return three_p; };
    std::string get_ref(void)         { return ref; };
    std::string get_var(void)         { return var; };
    int has_hits(void)                { return hits > 0;}
    void set_five_p(std::string f)    { five_p = f; };
    void set_three_p(std::string t)   { three_p = t; };
    void set_rc_p(Probe *p)           { rc = p; };
    void update_counters(std::string);
    void info(void);
    void get_counters(int *);
    void set_rc(); // Convert the current probe to a reverse complement version of it

    char visited; // To determine if we have already process this probe
    Probe *rc;    // Reverse complement version of the probe
    char is_a_rc_probe;

  protected:
    std::string chrm, coordinates, id;
    std::string five_p, three_p;
    std::string ref, var, o1, o2;
    void setup_counters(void);
    void find_other_alleles(void);
    char hits; // Tell me if the probe has any hit
    google::dense_hash_map<std::string, int> *counters;
};

#endif

#ifndef PROBE_H
#define PROBE_H

#include <string>

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
    bool is_cs(void) { return false; };
    std::string get_chrm(void) { return chrm; };
    std::string get_coordinates(void) { return coordinates; };
    std::string get_id(void) { return id; };
    std::string get_five_p(void) { return five_p; };
    std::string get_three_p(void) { return three_p; };
    void update_counters(std::string);
    void info(void);

  protected:
    std::string chrm, coordinates, id;
    std::string five_p, three_p;
    std::string ref, var, o1, o2;
    void setup_counters(void);
    void find_other_alleles(void);
    google::dense_hash_map<std::string, int> *counters;
};

#endif

#ifndef PROBE_H
#define PROBE_H

#include <string>
// Encapsulates a probe

class Probe {
  public:
    Probe() {};
    Probe(std::string);
    ~Probe() {};

    void set_all(std::string, std::string, std::string,
                 std::string, std::string, std::string, std::string);
    bool in_cs(void); // Check to see if we are dealing with a CS probe
    std::string get_chrm(void) { return chrm; };
    std::string get_coordinates(void) { return coordinates; };
    std::string get_id(void) { return id; };
    std::string get_five_p(void) { return five_p; };
    std::string get_three_p(void) { return three_p; };

  private:
    std::string chrm;
    std::string coordinates;
    std::string id;
    std::string five_p;
    std::string three_p;
    std::string ref;
    std::string var;
};

#endif

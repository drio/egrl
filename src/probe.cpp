#include <iostream>
#include <sstream>
#include <string>
#include "probe.h"

Probe::Probe(std::string line)
{
  std::stringstream ss;
  ss << line;
  ss >> chrm >> coordinates >> id >> five_p >> three_p >> ref >> var;
  counters = NULL;
  visited = 0;
  rc = NULL;
  hits = 0;
  is_a_rc_probe = 0;
}

Probe::~Probe() {
  if (counters != NULL)
    delete counters;
}

void Probe::set_all(std::string c   , std::string coor, std::string id_,
                    std::string five, std::string three,
                    std::string ref_ , std::string var_)
{
  chrm        = c;
  coordinates = coor;
  id          = id_;
  five_p      = five;
  three_p     = three;
  ref         = ref_;
  var         = var_;
}

// There is a read that matches this probe, so we want to update the
// counters with the allele we are seening (as)
void Probe::update_counters(std::string as)
{
  if (counters == NULL) {
    setup_counters();
    find_other_alleles();
    hits++;
  }
  if (as == "A" || as == "C" || as == "G" || as == "T") (*counters)[as]++;
  else (*counters)["N"]++;
}

// Ask memory for the hash that will hold the allele counters
void Probe::setup_counters(void)
{
  counters = new google::dense_hash_map<std::string, int>;
  counters->set_empty_key("-");
  (*counters)["A"] = 0; (*counters)["C"] = 0; (*counters)["G"] = 0; (*counters)["T"] = 0;
  (*counters)["N"] = 0; // For alleles seen other than ACGT
}

void Probe::find_other_alleles(void)
{
  google::dense_hash_map<std::string, int> h;
  h.set_empty_key("");
  google::dense_hash_map<std::string, int>::iterator i;

  h["A"] = 0; h["C"] = 0; h["G"] = 0; h["T"] = 0;
  h[ref] = 1; h[var] = 1;
  o1 = ""; o2 = "";
  for (i=h.begin(); i!=h.end(); i++) {
    if (i->second == 0) {
      if (o1 == "") o1 = i->first;
      else          o2 = i->first;
    }
  }
}

void Probe::info(void)
{
  std::cout << "info ..";
  std::cout << ref << "|" << (*counters)[ref] << " "
            << var << "|" << (*counters)[var] << " "
            << o1  << "|" << (*counters)[o1]  << " "
            << o2  << "|" << (*counters)[o2]
            << " visited: " << (int)visited << std::endl;
}

void Probe::get_counters(int *ac) // ac: array counters
{
  if (hits) {
    ac[0] = (*counters)[ref];
    ac[1] = (*counters)[var];
    ac[2] = (*counters)[o1];
    ac[3] = (*counters)[o2];
    ac[4] = (*counters)["N"];
  }
  else {
    ac[0] = 0;
    ac[1] = 0;
    ac[2] = 0;
    ac[3] = 0;
    ac[4] = 0;
  }

  /*
  std::cout << chrm << "," << coordinates << "," << id << ","
            << ref << "," << var << ","

            << (*counters)[ref] << "," << (*counters)[var] << ","
            << (*counters)[o1]  << "," << (*counters)[o2] << ","
            << (*counters)["N"] << ","

            << (*rc_c)[ref] << "," << (*rc_c)[var]
            << (*rc_c)[o1]  << "," << (*rc_c)[o2]
            << (*rc_c)["N"] << ","

            << (*counters)[ref] + (*rc_c)[ref] << ","
            << (*counters)[var] + (*rc_c)[var] << ","
            << (*counters)[o1] + (*rc_c)[o1] << ","
            << (*counters)[o2] + (*rc_c)[o2] << ","
            << (*counters)["N"] + (*rc_c)["N"]

            << std::endl;
  */

}

void Probe::set_rc()
{
  std::string tmp = three_p;
  tmp             = reverseComplement(five_p);
  five_p          = reverseComplement(three_p);
  three_p         = tmp;
  ref             = reverseComplement(ref);
  var             = reverseComplement(var);
  is_a_rc_probe   = 1;
}

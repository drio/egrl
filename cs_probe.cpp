#include <iostream>
#include <string>
#include "cs_probe.h"

CSProbe::CSProbe(std::string line, CSUtils *_csu) : Probe(line)
{
  csu = _csu;
  csu->to_cs(three_p, cs_three_p);
  csu->to_cs(five_p, cs_five_p);
}

// There is a CSread that matches this probe, so we want to update the
// counters with the allele we are seening. The parameter we recieve in
// the call is a dibase (db),
void CSProbe::update_counters(std::string db)
{
  if (counters == NULL) {
    setup_counters();
    find_other_alleles();
    to_cs();
  }
  if      (db == cs_ref) (*counters)[ref]++;
  else if (db == cs_var) (*counters)[var]++;
  else if (db == cs_o1)  (*counters)[o1]++;
  else if (db == cs_o2)  (*counters)[o2]++;
  else (*counters)["N"]++;
}

void CSProbe::cs_info(void)
{
  std::cout << ref << "|" << cs_ref << "|" << (*counters)[ref] << " "
            << var << "|" << cs_var << "|" << (*counters)[var] << " "
            << o1  << "|" << cs_o1  << "|" << (*counters)[o1]  << " "
            << o2  << "|" << cs_o2  << "|" << (*counters)[o2]  << std::endl;
}

// Convert all the alleles to its dibase version
void CSProbe::to_cs(void)
{
  // last base of five prime flank seq, ref(or var o1 o2) and first base of
  // three prime flanking sequence
  std::string tb;
  tb = five_p.substr(five_p.size()-1, 1) + ref + three_p.substr(0, 1);
  csu->two_colors(tb, cs_ref);
  tb = five_p.substr(five_p.size()-1, 1) + var + three_p.substr(0, 1);
  csu->two_colors(tb, cs_var);

  tb = five_p.substr(five_p.size()-1, 1) + o1 + three_p.substr(0, 1);
  csu->two_colors(tb, cs_o1);
  tb = five_p.substr(five_p.size()-1, 1) + o2 + three_p.substr(0, 1);
  csu->two_colors(tb, cs_o2);
}

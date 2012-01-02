#include <string>
#include "cs_probe.h"

CSProbe::CSProbe(std::string line, CSUtils &csu) : Probe(line) {
  csu.to_cs(three_p, cs_three_p);
  csu.to_cs(five_p, cs_five_p);
  std::string tb;
  // last base of five prime flank seq, ref(or var) and first base of
  // three prime flanking sequence
  tb = five_p.substr(five_p.size()-1, 1) + ref + three_p.substr(0, 1);
  csu.two_colors(tb, cs_ref);
  tb = five_p.substr(five_p.size()-1, 1) + var + three_p.substr(0, 1);
  csu.two_colors(tb, cs_var);
  // 3. we are going to have to setup the other two possible alleles
  //    both in SS and CS
}

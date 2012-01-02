#ifndef CS_UTILS_H
#define CS_UTILS_H

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

// Useful Color space routines
namespace cs {
  google::dense_hash_map<std::string, std::string> stcs;
  void setup_cs_table()
  {
    stcs.set_empty_key("-");
    stcs["AA"] = "0"; stcs["AC"] = "1"; stcs["AG"] = "2"; stcs["AT"] = "3";
    stcs["CA"] = "1"; stcs["CC"] = "0"; stcs["CG"] = "3"; stcs["CT"] = "2";
    stcs["GA"] = "2"; stcs["GC"] = "3"; stcs["GG"] = "0"; stcs["GT"] = "1";
    stcs["TA"] = "3"; stcs["TC"] = "2"; stcs["TG"] = "1"; stcs["TT"] = "0";
  }

  // Convert a sequence from sequence space to color space
  inline void ss_to_cs(std::string &ss, std::string &scs)
  {
    for(unsigned int i=0; i<ss.length()-1; i++)
      scs.append(stcs[ss.substr(i,2)]);
  }

  // Tell me the dibase color encoding for Y in sequence XYZ
  // TODO: raise excpetion if input.lenght() != 3
  inline void two_colors(std::string input, std::string &dibase)
  {
    dibase.append(stcs[input.substr(0,2)]);
    dibase.append(stcs[input.substr(1,2)]);
  }
}

#endif

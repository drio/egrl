#ifndef CS_UTILS_H
#define CS_UTILS_H

#include <string>

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

// Useful Color space routines
class CSUtils {
  public:
    CSUtils();
    // Convert a sequence from sequence space to color space
    inline void to_cs(std::string &ss, std::string &scs)
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

  private:
    // Here we have the di-nucleotide to color conversion table
    // stcs: sequence to color space
    google::dense_hash_map<std::string, std::string> stcs;
};

#endif

#ifndef COUNT_H
#define COUNT_H

#include "probe.h"

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

void parse_count_options(int, char **);
void count_main(int, char **);
void load_probes(google::dense_hash_map<std::string, Probe*> *);

#endif

#ifndef COUNT_H
#define COUNT_H

#include "probe.h"
#include "cs_probe.h"
#include "cs_utils.h"

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

void parse_count_options(int, char **);
void count_main(int, char **);
void load_probes(google::dense_hash_map<std::string, Probe*> *, std::istream *);
void screen_reads(google::dense_hash_map<std::string, Probe*> *, int);
void dump_results(google::dense_hash_map<std::string, Probe*> &);

void cs_load_probes(google::dense_hash_map<std::string, CSProbe*> *, std::istream *, CSUtils *);
void cs_screen_reads(google::dense_hash_map<std::string, CSProbe*> *, int);
void cs_dump_results(google::dense_hash_map<std::string, CSProbe*> &);

#endif

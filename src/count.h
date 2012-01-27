#ifndef COUNT_H
#define COUNT_H

#include "probe.h"
#include "cs_probe.h"
#include "cs_utils.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

typedef struct {
  google::dense_hash_map<std::string, Probe*> ss;
  google::dense_hash_map<std::string, CSProbe*> cs;
} ss_or_cs_probes;

typedef struct {
  int  n_reads;
  int  tid;
  std::string **buffer;
  ss_or_cs_probes *probes;
  int p_len;
  int cs_data;
  pthread_mutex_t *mutex;
  int n_threads;
} thread_data;

void parse_count_options(int, char **);
void count_main(int, char **);
void load_probes(google::dense_hash_map<std::string, Probe*> *, std::istream *);
void *screen_reads(google::dense_hash_map<std::string, Probe*> *, int);
void dump_results(google::dense_hash_map<std::string, Probe*> &);

void cs_load_probes(google::dense_hash_map<std::string, CSProbe*> *, std::istream *, CSUtils *);
void *cs_screen_reads(google::dense_hash_map<std::string, CSProbe*> *, int);
void cs_dump_results(google::dense_hash_map<std::string, CSProbe*> &);

void count_in_ss(std::istream*);
void count_in_cs(std::istream*);

int load_to_buffer(SeqReader *, std::string **);

#endif

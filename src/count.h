#ifndef COUNT_H
#define COUNT_H

#include "probe.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

#include <google/sparse_hash_map>
#include <google/dense_hash_map>

typedef google::dense_hash_map<std::string, Probe*> ss_probes;

typedef struct {
  int  n_reads;
  int  tid;
  std::string **buffer;
  ss_probes *probes;
  int p_len;
  pthread_mutex_t *mutex;
  int n_threads;
} thread_data;

void parse_count_options(int, char **);
void load_probes(ss_probes &, std::istream *);
void *screen_reads(ss_probes *, int, std::string **, int, int, pthread_mutex_t *, int);

void dump_results(google::dense_hash_map<std::string, Probe*> &);
int load_to_buffer(SeqReader *, std::string **);
void free_up_probes(ss_probes);


void count_main(int, char **);

#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>

#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include <getopt.h>

#include "config.h"
#include "count.h"
#include "probe.h"

#include "Util/Util.h"
#include "Util/SeqReader.h"
#include "Util/Timer.h"

// Getopt
#define SUBPROGRAM "count"

#define BUFFER_SIZE 1000000

static const char *COUNT_VERSION_MESSAGE =
SUBPROGRAM " Version " PACKAGE_VERSION "\n"
"Written by David Rio Deiros\n";

static const char *COUNT_USAGE =
"Usage: " PACKAGE_NAME " " SUBPROGRAM " [OPTION] --probes=STR --reads=STR\n"
"This command screens reads against the probes provided.\n"
"\n"
"      --help                           display this help and exit.\n"
"      --version                        report version.\n"
"      -v, --verbose                    display verbose output.\n"
"      -p, --probes=FILE                Probes file. Use - for standard input.\n"
"      -r, --reads=FILE                 Reads file. Use - for standard input.\n"
"      -t, --n_threads                  Number of threads [1].\n"
"\nReport bugs to " PACKAGE_BUGREPORT "\n\n";

namespace opt
{
  static unsigned int verbose    = 0;
  static std::string reads_file  = "";
  static std::string probes_file = "";
  static unsigned int n_threads  = 1;
}

// static: Only visible in this file
static const char* shortopts = "p:r:t:hcv";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
  { "verbose"    ,         no_argument,       NULL, 'v' },
  { "probes"     ,         required_argument, NULL, 'p' },
  { "reads"      ,         required_argument, NULL, 'r' },
  { "n_threads"  ,         no_argument,       NULL, 't' },
  { "help"       ,         no_argument,       NULL, OPT_HELP },
  { "version"    ,         no_argument,       NULL, OPT_VERSION },
  { NULL, 0, NULL, 0 }
};

void parse_count_options(int argc, char **argv)
{
  bool die = false;
  for (char c; (c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1;) {
    std::istringstream arg(optarg != NULL ? optarg : "");
    switch (c) {
      case 'v': opt::verbose++; break;
      case 't': arg >> opt::n_threads; break;
      case 'p': arg >> opt::probes_file; break;
      case 'r': arg >> opt::reads_file; break;
      case '?': die = true; break;
      case OPT_HELP:
        std::cout << COUNT_USAGE;
        exit(EXIT_SUCCESS);
      case OPT_VERSION:
        std::cout << COUNT_VERSION_MESSAGE;
        exit(EXIT_SUCCESS);
    }
  }

  if (optind-argc != 0) {
    std::cerr << SUBPROGRAM ": Incorrect number of parameters\n";
    die = true;
  }

  if (opt::probes_file == "") {
    std::cerr << SUBPROGRAM ": Probe file not provided.\n";
    die = true;
  }

  if (opt::reads_file == "") {
    std::cerr << SUBPROGRAM ": Reads file not provided.\n";
    die = true;
  }

  if (die) {
    std::cerr << "Try `" << SUBPROGRAM << " --help' for more information.\n";
    exit(EXIT_FAILURE);
  }
}

void load_probes(google::dense_hash_map<std::string, Probe*> &h_probes, std::istream *probes)
{
  std::string line;
  Probe *p, *rc_p;
  Timer timer("Probe loading (SS)");
  /* TODO: Confirm that the probe size is constant */
  while (getline(*probes, line)) {
    /* TODO: check if probe already there, if so, ignore, otherwise we will have memory leaks */
    p       = new Probe(line);
    rc_p    = new Probe(line);
    rc_p->set_rc();

    rc_p->set_rc_p(p);
    p->set_rc_p(rc_p);

    std::string key = p->get_five_p() + "N" + p->get_three_p();
    h_probes[key]   = p;
    key             = rc_p->get_five_p() + "N" + rc_p->get_three_p();
    h_probes[key]   = rc_p;
  }
}

void *screen_reads(ss_probes *h_probes,
                   int probe_length,
                   std::string **buffer,
                   int n_reads,
                   int tid,
                   pthread_mutex_t *mutex,
                   int n_threads)
{
  google::dense_hash_map<std::string, Probe *>::iterator p_iter;
  const int n_pos = probe_length/2;
  std::string window;
  std::string n_value(" ");

  int p,i; // pointer to buffer, pointer in read
  for (p=0; p!=n_reads; ++p) {
    if ((n_threads>1) && (p % n_threads != tid)) continue;
    for (i=0; i+probe_length<=(int) buffer[p]->length(); ++i) {
      window        = buffer[p]->substr(i, probe_length);
      n_value[0]    = window[n_pos];
      window[n_pos] = 'N';
      if ((p_iter = h_probes->find(window)) != h_probes->end()) {
        pthread_mutex_lock(mutex);
        p_iter->second->update_counters(n_value);
        pthread_mutex_unlock(mutex);
      }
    }
  }
  return 0;
}

void dump_results(google::dense_hash_map<std::string, Probe*> &h_probes)
{
  int cs1[5], cs2[5];
  Probe *op; // Original probe

  google::dense_hash_map<std::string, Probe *>::iterator p_iter;
  for (p_iter=h_probes.begin(); p_iter!=h_probes.end(); p_iter++) {
    // In op we want always the original probe, not the rc one
    op = p_iter->second->is_a_rc_probe ? p_iter->second->rc : p_iter->second;
    if (!op->visited && (op->has_hits() || op->rc->has_hits())) {
      op->get_counters(cs1);
      op->rc->get_counters(cs2);
      std::cout
        << op->get_chrm() << ","
        << op->get_coordinates() <<  ","
        << op->get_id() <<  ","
        << op->get_ref() <<  ","
        << op->get_var() <<  ","

        << cs1[0] << ","
        << cs1[1] << ","
        << cs1[2] << ","
        << cs1[3] << ","
        << cs1[4] << ","

        << cs2[0] << ","
        << cs2[1] << ","
        << cs2[2] << ","
        << cs2[3] << ","
        << cs2[4] << ","

        << cs1[0] + cs2[0] << ","
        << cs1[1] + cs2[1] << ","
        << cs1[2] + cs2[2] << ","
        << cs1[3] + cs2[3] << ","
        << cs1[4] + cs2[4]

        << std::endl;

      op->visited = 1;
      op->rc->visited = 1;
    }
  }
}

int load_to_buffer(SeqReader *reader, std::string **buffer)
{
  int n_rr = 0; // number of reads read
  SeqRecord record;
  std::string *read;
  std::cerr << ">> loading data in buffer " << std::endl;
  while(n_rr < BUFFER_SIZE && reader->get(record)) {
   read         = new std::string(record.seq.toString());
   buffer[n_rr] = read;
   n_rr++;
  }
  std::cerr << ">> data loaded in buffer " << std::endl;
  return n_rr;
}

static void *worker(void *data)
{
  thread_data *d = (thread_data*) data;
  screen_reads(d->probes, d->p_len, d->buffer,
               d->n_reads, d->tid, d->mutex, d->n_threads);
  return 0;
}

void free_up_probes(ss_probes h_probes)
{
  google::dense_hash_map<std::string, Probe *>::iterator p_iter;
  for (p_iter=h_probes.begin(); p_iter!=h_probes.end(); p_iter++) {
    delete p_iter->second;
  }
}

void count_main(int argc, char **argv)
{
  Timer *total_timer = new Timer("count_main");
  parse_count_options(argc, argv);

  std::istream* probes_stream;
  probes_stream = (opt::probes_file == "-") ?
                  &std::cin : createReader(opt::probes_file);

  /* Load probes */
  ss_probes probes;
  int probe_length;
  std::cerr << ">> Loading probes " << std::endl;
  probes.set_empty_key("-");
  load_probes(probes, probes_stream);
  std::cerr << ">> # of probes (RC included): " << probes.size() << std::endl;
  probe_length = probes.begin()->first.length();

  /* Process reads */
  std::string **buffer;
  /* request mem for buffer of reads */
  buffer = (std::string **) calloc(sizeof(std::string *), BUFFER_SIZE);
  int n_rr  = 0;  // number of reads read
  int total = 0;  // total number of reads processed
  SeqReader reader(opt::reads_file, SRF_NO_VALIDATION);
  while((n_rr = load_to_buffer(&reader, buffer)) != 0) {
    Timer *pr_timer = new Timer("Processing reads");
    std::cerr << ">> Computing screen on " << n_rr << " reads" << std::endl;
    total += n_rr;
    pthread_t *tid;
    pthread_attr_t attr;
    thread_data *data;
    unsigned int j; // to iterate over num of threads
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    data = (thread_data*) calloc(opt::n_threads, sizeof(thread_data));
    tid  = (pthread_t*) calloc(opt::n_threads, sizeof(pthread_t));
    /* create/init mutex */
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    for (j=0; j<opt::n_threads; ++j) {
      data[j].n_reads   = n_rr;
      data[j].tid       = j;
      data[j].buffer    = buffer;
      data[j].probes    = &probes;
      data[j].p_len     = probe_length;
      data[j].mutex     = &mutex;
      data[j].n_threads = opt::n_threads;
      pthread_create(&tid[j], &attr, worker, data+j);
    }
    /* join-start threads */
    for (j=0; j<opt::n_threads; ++j) pthread_join(tid[j], 0);
    std::cerr << ">> Done computing " << n_rr << " reads" << std::endl;
    std::cerr << ">> Total # of reads processed so far: " << total << std::endl;
    free(data); free(tid);
    delete pr_timer; // force dump cpu time.
  }

  dump_results(probes);
  free_up_probes(probes);
  delete probes_stream;
  for (int j=0; j<BUFFER_SIZE; j++) delete buffer[j];
  free(buffer);
  delete total_timer;
}


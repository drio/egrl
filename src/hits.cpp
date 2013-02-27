#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>

#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include <getopt.h>

#include "config.h"
#include "hits.h"
#include "count.h"
#include "probe.h"

#include "Util/Util.h"
#include "Util/SeqReader.h"
#include "Util/Timer.h"

// Getopt
#define SUBPROGRAM "hits"

#define BUFFER_SIZE 1000000

static const char *HITS_VERSION_MESSAGE =
SUBPROGRAM " Version " PACKAGE_VERSION "\n"
"Written by David Rio Deiros\n";

static const char *HITS_USAGE =
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
  static std::string reads_file  = "";
  static std::string probes_file = "";
}

// static: Only visible in this file
static const char* shortopts = "p:r:h";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
  { "verbose"    ,         no_argument,       NULL, 'v' },
  { "probes"     ,         required_argument, NULL, 'p' },
  { "reads"      ,         required_argument, NULL, 'r' },
  { "help"       ,         no_argument,       NULL, OPT_HELP },
  { "version"    ,         no_argument,       NULL, OPT_VERSION },
  { NULL, 0, NULL, 0 }
};

void parse_hits_options(int argc, char **argv)
{
  bool die = false;
  for (char c; (c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1;) {
    std::istringstream arg(optarg != NULL ? optarg : "");
    switch (c) {
      case 'p': arg >> opt::probes_file; break;
      case 'r': arg >> opt::reads_file; break;
      case '?': die = true; break;
      case OPT_HELP:
        std::cout << HITS_USAGE;
        exit(EXIT_SUCCESS);
      case OPT_VERSION:
        std::cout << HITS_VERSION_MESSAGE;
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

void hits_main(int argc, char **argv)
{
  Timer *total_timer = new Timer("count_main");
  parse_hits_options(argc, argv);

  /* Load probes */
  std::istream* probes_stream;
  probes_stream = (opt::probes_file == "-") ?
                  &std::cin : createReader(opt::probes_file);
  ss_probes probes;
  std::cerr << ">> Loading probes " << std::endl;
  probes.set_empty_key("-");
  load_probes(probes, probes_stream);
  std::cerr << ">> # of probes (RC included): " << probes.size() << std::endl;
  int probe_length;
  probe_length = probes.begin()->first.length();

  /*
   * Screen the reads to see if we hit any probes, if so report the probe id, allele seen and
   * the read id
   */
  SeqReader reader(opt::reads_file, SRF_NO_VALIDATION);
  SeqRecord record;
  std::string *read;
  ss_probes::iterator p_iter;
  int i;
  std::string window, n_value;
  int n_pos = probe_length/2;
  Probe *op; // Original probe
  uint64_t nrp = 1;
  int report_when = 100000;
  std::string timer_msg("Time spent computing reads:");
  Timer *pr_timer = new Timer(timer_msg);
  while (reader.get(record)) {
    n_value = " ";
    read    = new std::string(record.seq.toString());

    for (i=0; i+probe_length<=(int) read->length(); ++i) {
      window     = read->substr(i, probe_length);
      n_value[0] = window[n_pos];
      window[n_pos] = 'N';
      if ((p_iter = probes.find(window)) != probes.end()) {
        op = p_iter->second->is_a_rc_probe ? p_iter->second->rc : p_iter->second;
        std::cout << op->get_id() << "," << n_value << "," << record.id << std::endl;
      }
    }

    if (nrp % report_when == 0) {
     std::cerr << ">> Number of reads processed so far: " << nrp << std::endl;
     delete pr_timer;
     pr_timer = new Timer(timer_msg);
    }
    nrp++;
  }

  std::cerr << ">> Done. Total number of reads: " << nrp << std::endl;
  delete pr_timer;
  free_up_probes(probes);
  delete probes_stream;
  delete total_timer;
}


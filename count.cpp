#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include <getopt.h>

#include "config.h"
#include "count.h"
#include "probe.h"

#include "Util/Util.h"
#include "Util/SeqReader.h"

//
// Getopt
//
#define SUBPROGRAM "count"

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
"      -c, --color_space                Input is in color space.\n"
"\nReport bugs to " PACKAGE_BUGREPORT "\n\n";

namespace opt
{
  static unsigned int verbose = 0;
  static std::string reads_file = "";
  static std::string probes_file = "";
  static unsigned int cs_data = 0; // Data in color space?
}

// static: Only visible in this file
static const char* shortopts = "p:r:hcv";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
  { "verbose"    ,         no_argument,       NULL, 'v' },
  { "probes"     ,         required_argument, NULL, 'p' },
  { "reads"      ,         required_argument, NULL, 'r' },
  { "color_space",         no_argument,       NULL, 'c' },
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
      case 'c': opt::cs_data++; break;
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

void load_probes(google::dense_hash_map<std::string, Probe*> &h_probes)
{
  std::istream* probes;
  probes = (opt::probes_file == "-") ?
           &std::cin : createReader(opt::probes_file);

  std::string line;
  std::stringstream ss;
  Probe *p, *rc_p;
  // TODO: Confirm that the probe size is constant
  //
  while (getline(*probes, line)) {
    // TODO: check if probe already there, if so, ignore, otherwise we will have memory leaks
    p       = new Probe(line);
    rc_p    = new Probe(line);
    rc_p->set_rc();

    rc_p->set_rc_p(p);
    p->set_rc_p(rc_p);

    std::string key                  = p->get_five_p() + "N" + p->get_three_p();
    h_probes[key]                    = p;
    h_probes[reverseComplement(key)] = rc_p;
  }
}

void screen_reads(google::dense_hash_map<std::string, Probe*> &h_probes,
                  int probe_length)
{
  SeqReader reader(opt::reads_file, SRF_NO_VALIDATION);
  SeqRecord record;
  google::dense_hash_map<std::string, Probe *>::iterator p_iter;
  const int n_pos = probe_length/2;
  std::string read;
  std::string window;
  std::string n_value(" ");

  while(reader.get(record)) {
    read = record.seq.toString();
    for (unsigned int i=0; i+probe_length<=read.length(); i++) {
      window        = read.substr(i,probe_length);
      n_value[0]    = window[n_pos];
      window[n_pos] = 'N';
      if ((p_iter = h_probes.find(window)) != h_probes.end())
        p_iter->second->update_counters(n_value);
    }
  }
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

void count_main(int argc, char **argv)
{
  parse_count_options(argc, argv);

  // Load probes into hash
  //google::sparse_hash_map<std::string, Probe*> h_probes;
  google::dense_hash_map<std::string, Probe*> h_probes;
  h_probes.set_empty_key("-");
  std::cerr << std::endl << ">> loading probes" << std::endl;
  load_probes(h_probes);

  int probe_length = h_probes.begin()->first.length();
  std::cerr << std::endl << ">> screening reads" << std::endl;
  screen_reads(h_probes, probe_length);
  std::cerr << std::endl << ">> dumping" << std::endl;
  dump_results(h_probes);

  std::cerr << std::endl << "# of probes (RC included): " << h_probes.size() << std::endl;

  // TODO: Free probes
  /*
  std::cout << "verbose: " << opt::verbose     << std::endl;
  std::cout << "cs     : " << opt::cs_data     << std::endl;
  std::cout << "probes : " << opt::probes_file << std::endl;
  std::cout << "reads  : " << opt::reads_file  << std::endl;
  */
}


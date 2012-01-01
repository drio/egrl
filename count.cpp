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
  while (getline(*probes, line)) {
    Probe *p                         = new Probe(line);
    std::string key                  = p->get_five_p() + "N" + p->get_three_p();
    h_probes[key]                    = p;
    h_probes[reverseComplement(key)] = p;
  }
}

//1       730720  drio2   GTGTCAGTGATAGAA GGTCATGGGATCTTT C       T
//1       711153  drio1   10020220202     02010132202     C       G       30      03
void count_main(int argc, char **argv)
{
  parse_count_options(argc, argv);

  // Load probes into hash
  //google::sparse_hash_map<std::string, Probe*> h_probes;
  google::dense_hash_map<std::string, Probe*> h_probes;
  h_probes.set_empty_key("-");
  load_probes(h_probes);
  std::cerr << "# of probes (RC included): " << h_probes.size() << std::endl;
  // TODO
  // DONE: 0. Add reverse complement per each probe!
  // If in CS (at load_probes level):
  // -> data will come in sequence space always!
  // DONE. convert flaking sequence to CS (even number of colors)
  // DONE. Find two colors for Ref and Var
  //
  // 1. Extend probe to have a cs version

  // TODO: Free probes
  std::cout << "verbose: " << opt::verbose     << std::endl;
  std::cout << "cs     : " << opt::cs_data     << std::endl;
  std::cout << "probes : " << opt::probes_file << std::endl;
  std::cout << "reads  : " << opt::reads_file  << std::endl;
}


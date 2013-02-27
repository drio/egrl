#include <string>
#include <iostream>

#include "config.h"
#include "probe.h"
#include "count.h"
#include "hits.h"

static const char *EGRL_USAGE =
"Program: " PACKAGE_NAME "\n"
"Version: " PACKAGE_VERSION "\n"
"Contact: " AUTHOR "\n"
"Support: " PACKAGE_BUGREPORT "\n"
"Usage:   " PROGRAM_BIN " <command> [options]\n\n"
"Commands:\n"
" count     Screen reads using probes.\n"
" hits      Dump hits to probes as soon as you find them.\n"
"\nReport bugs to " PACKAGE_BUGREPORT "\n\n";

int main(int argc, char** argv)
{
  if(argc <= 1) {
    std::cout << EGRL_USAGE;
    return 0;
  }
  else {
    std::string command(argv[1]);
    if (command == "help" || command == "--help") {
      std::cout << EGRL_USAGE;
      return 0;
    }
    else if (command == "version" || command == "--version") {
      std::cout << EGRL_USAGE;
      return 0;
    }
    if(command == "count")
      count_main(argc - 1, argv + 1);
    else if(command == "hits")
      hits_main(argc - 1, argv + 1);
    else {
      std::cerr << "Unrecognized command: " << command << std::endl;
      return 1;
    }
  }
}

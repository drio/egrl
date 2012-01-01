#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include "config.h"
#include "probe.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

using std::cout;
using std::cin;
using std::endl;

/*
class Probe {
  public:
     Probe() {};
     Probe(std::string);
     void print() const {
       cout << "id   : " << id << endl;
       cout << "five : " << five << endl;
       cout << "three: " << three << endl;
     }
     void set_id(std::string);
     void set_five(std::string);
     void set_three(std::string);
     bool check_if_cs(void);

  private:
     std::string id;
     std::string five;
     std::string three;
};

Probe::Probe(std::string s) {
  id = "id_this"; five = s ; three = "33333";
}
void Probe::set_id(std::string s) { id = s; }
void Probe::set_five(std::string s) { five = s; }
void Probe::set_three(std::string s) { three = s; }
bool Probe::check_if_cs(void) {
  //return (five[0]=='A' || five[0]=='C' || five[0] == 'G' || five[0] == 'T');
  return true;
}
*/

void read_fasta(char **argv)
{
  // Read fasta or fastq from file or stdin
  /////////////////////////////////////
  SeqReader reader(argv[1], SRF_NO_VALIDATION);
  SeqRecord record;

  while(reader.get(record)) {
    std::cout << record.seq.toString() << std::endl;
  }
  cout << "---------------------" << endl;
}

void read_probes(char **argv)
{
  //////////////////////////////////
  std::string probes_file_name(argv[2]);
  std::istream* probes;
  probes = (probes_file_name == "-") ?
           &std::cin : createReader(probes_file_name);

  std::string line;
  std::stringstream ss;
  std::string ch, co, id, f, t, ref, var;
  //google::sparse_hash_map<std::string, Probe*> h_probes;
  google::dense_hash_map<std::string, Probe*> h_probes;
  h_probes.set_empty_key("-");
  while (getline(*probes, line)) {
    Probe *p        = new Probe(line);
    std::string key = p->get_five_p() + "N" + p->get_three_p();
    h_probes[key]   = p;
  }
  cout << "h_probes.size() : " << h_probes.size() << endl;
  //cout << "CACCAAAGGAGGAAGNCCTTGGTAGAAGATA: " << h_probes["CACCAAAGGAGGAAGNCCTTGGTAGAAGATA"]->get_id() << endl;

  if (probes_file_name != "-") delete probes;
}

void play_with_reverse()
{
  std::string ds("GAANGGT");
  std::cout << ds << std::endl;
  std::cout << reverseComplement(ds) << std::endl;
}

// Useful Color space routines
namespace cs {
  google::dense_hash_map<std::string, std::string> stcs;
  void setup_cs_table()
  {
    stcs.set_empty_key("-");
    stcs["AA"] = "0"; stcs["AC"] = "1"; stcs["AG"] = "2"; stcs["AT"] = "3";
    stcs["CA"] = "1"; stcs["CC"] = "0"; stcs["CG"] = "3"; stcs["CT"] = "2";
    stcs["GA"] = "2"; stcs["GC"] = "3"; stcs["GG"] = "0"; stcs["GT"] = "1";
    stcs["TA"] = "3"; stcs["TC"] = "2"; stcs["TG"] = "1"; stcs["TT"] = "0";
  }

  // Convert a sequence from sequence space to color space
  inline void ss_to_cs(std::string &ss, std::string &scs)
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
}

// To generate the two base encoding for ref or var:
// need flaking bases and ref or var nucleotide (in sequence space)

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./test <reads_file> <probe_file>" << endl;
    exit(EXIT_FAILURE);
  }

  read_fasta(argv);
  //read_probes(argv);
  //play_with_reverse();
  std::string input("ATGGTGGTTGTTA"), output;
  cs::setup_cs_table();
  std::cout << input << std::endl;
  std::cout << "---------\n";
  cs::ss_to_cs(input, output);
  std::cout << output << std::endl;

  std::string dibase, i("TCG");
  std::cout << i << std::endl;
  cs::two_colors(i, dibase);
  std::cout << dibase << std::endl;

  return 0;
}

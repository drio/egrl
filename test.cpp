#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <google/sparsehash/sparseconfig.h>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include "config.h"
#include "probe.h"
#include "cs_probe.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

using std::cout;
using std::cin;
using std::endl;

// NormalProbe:
// chrm, coordinate, id, five, three, ref, var
class NormalProbe {
  public:
     NormalProbe() {};
     NormalProbe(std::string);
     void print() const {
       cout << "id   : " << id << endl;
       cout << "five : " << five << endl;
     }
     void set_id(std::string);
     bool check_if_cs(void);

  protected:
     std::string id;
     std::string five;
     std::string three;
};

NormalProbe::NormalProbe(std::string s) {
  id = "id_this"; five = s ; three = "33333";
}
void NormalProbe::set_id(std::string s) { id = s; }
bool NormalProbe::check_if_cs(void) { return false; }

// _CSProbe:
// + cs_five, cs_three, cs_ref, cs_var
class _CSProbe:public NormalProbe {
  public:
    _CSProbe(std::string);
    void print() const {
      cout << "CS id   : " << id << endl;
      cout << "CS five : " << cs_five << endl;
    }
    bool check_if_cs();
  private:
    std::string cs_five;
};

_CSProbe::_CSProbe(std::string s) {
  id = "CS -- id_this"; cs_five = s ; three = "CS -- 33333";
}
bool _CSProbe::check_if_cs(void) { return true; }

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

// To generate the two base encoding for ref or var:
// need flaking bases and ref or var nucleotide (in sequence space)
int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./test <reads_file> <probe_file>" << endl;
    exit(EXIT_FAILURE);
  }

  // TODO: refactor cs-utils.. it cannot be a namespace...
  // Look into sga code to get ideas...
  // Then change CSprobe constructor
  cout << argv[0] << endl;
  cout << argc << endl;
  std::cout << "___________________________________" << std::endl;

  // Testing Probe and subclasses ..
  Probe np("1       100006955       rs4908018       TTTGTCTAAAACAAC CTTTCACTAGGCTCA C       A");
  std::cout << np.is_cs() << std::endl;
  std::cout << np.get_five_p() << std::endl;

  std::cout << "____" << std::endl;

  CSProbe csp("1       100006955       rs4908018       TTTGTCTAAAACAAC CTTTCACTAGGCTCA C       A");
  std::cout << csp.get_cs_five_p() << std::endl;

  return 0;
}

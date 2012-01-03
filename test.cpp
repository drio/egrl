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
#include "cs_utils.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

using std::cout;
using std::cin;
using std::endl;

class mother {
  public:
    mother ()
    {
      //bar = new google::dense_hash_map<std::string, int>;
      bar = NULL;
      cout << "CONST_1 mother: no parameters\n";
    }
    mother (int a) { cout << "CONST_2 mother: int parameter " << a << std::endl; }
    ~mother()      {
      cout << "DEST mother\n";
      if (bar != NULL) {
        cout << "delete!" << endl;
        delete bar;
      }
      else {
        cout << "No need to delete" << endl;
      }
    };
    void foo(void) { cout << "BASE foo" << endl;}
  protected:
    google::dense_hash_map<std::string, int> *bar;
};

class daughter : public mother {
  public:
    daughter (int a) { cout << "CONST daughter: int parameter" << a << std::endl; }
    ~daughter() { cout << "DEST -daughter\n";};
    void foo(void) { cout << "DAUGHTER foo" << endl;}
};

class son : public mother {
  public:
    son (int a) : mother (a) { cout << "cONST son: int parameter\n\n"; }
};

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

void test_probe_class()
{
  std::cout << ">> TESTING probe class" << std::endl;
  Probe np("1       100006955       rs4908018       TTTGTCTAAAACAAC CTTTCACTAGGCTCA C       A");
  std::cout << "Is in a cs probe? " << np.is_cs() << std::endl;
  std::cout << "five_p attibute: "  << np.get_five_p() << std::endl;

  std::cout << ">> TESTING Probe.update_counters()" << std::endl;
  np.update_counters("A");
  np.update_counters("C");
  np.update_counters("G");
  np.update_counters("T");
  np.update_counters("T");
  np.update_counters(".");

  np.info();

  // Notice that we need a CSUtils object to be passed in the constructor
  std::cout << ">> TESTING CS probe class" << std::endl;
  CSUtils csu;
  CSProbe csp("1       100006955       rs4908018       TTTGTCTAAAACAAC CTTTCACTAGGCTCA C       A", &csu);
  std::cout << "five_p (CS) seq: " << csp.get_cs_five_p() << std::endl;
  csp.update_counters("11"); csp.update_counters("11");
  csp.update_counters("00"); csp.update_counters("00");
  csp.update_counters("33");
  csp.update_counters("22"); csp.update_counters("22");
  csp.update_counters(".");

  csp.cs_info();

  std::cout << std::endl;
}

void test_csutils()
{
  std::cout << ">> TESTING csutils class" << std::endl;

  CSUtils csu;
  std::string tmp("ACACACAC"), out;
  std::cout << "Input sequence: " << tmp << std::endl;
  csu.to_cs(tmp, out);
  std::cout << "to_cs: " << out << std::endl;

  tmp = "ACA";
  std::string tc;
  std::cout << "Tribase: "    << tmp << std::endl;
  csu.two_colors(tmp, tc);
  std::cout << "two_colors: " << tc << std::endl;
  std::cout << std::endl;
}

void play_mom_class()
{
  daughter cynthia (0);
  cynthia.foo();
  //daughter david (1);
}

void set_others(void) {
  // Given ref and var
  // set o1 and o2 to the other possible nucleotides
  std::string ref("G"), var("T"), o1, o2;
  google::dense_hash_map<std::string, int> h;
  h.set_empty_key("");
  google::dense_hash_map<std::string, int>::iterator i;

  h["A"] = 0; h["C"] = 0; h["G"] = 0; h["T"] = 0;
  h[ref] = 1; h[var] = 1;
  o1 = ""; o2 = "";
  for (i=h.begin(); i!=h.end(); i++) {
    cout << "++ " << i->first << " / " << i->second << endl;
    if (i->second == 0) {
      if (o1 == "") o1 = i->first;
      else          o2 = i->first;
    }
  }
  cout << "ref: " << ref << endl;
  cout << "var: " << var << endl;
  cout << "o1: "  << o1  << endl;
  cout << "o2: "  << o2  << endl;
}

// To generate the two base encoding for ref or var:
// need flaking bases and ref or var nucleotide (in sequence space)
int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./test <reads_file> <probe_file>" << endl;
    exit(EXIT_FAILURE);
  }

  // To avoid warning to break compilation
  cout << argv[0] << endl; cout << argc << endl << endl;

  //play_mom_class();
  test_probe_class();
  //test_csutils();
  //set_others();

  // The screening algorithm will look like this (assuming I have already
  // anchored a window in a read):
  // . save the N value
  // . change substring to N (window)
  // . check if the probes hash has a window (substring with N)
  // . IF hit
  //    . h_probes[hit_substring_n].update_counters(N_value)
  return 0;
}

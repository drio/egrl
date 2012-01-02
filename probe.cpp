#include <iostream>
#include <sstream>
#include <string>
#include "probe.h"

Probe::Probe(std::string line)
{
  std::stringstream ss;
  ss << line;
  ss >> chrm >> coordinates >> id >> five_p >> three_p >> ref >> var;
}

void Probe::set_all(std::string c   , std::string coor, std::string id_,
                    std::string five, std::string three,
                    std::string ref_ , std::string var_)
{
  chrm        = c;
  coordinates = coor;
  id          = id_;
  five_p      = five;
  three_p     = three;
  ref         = ref_;
  var         = var_;
}


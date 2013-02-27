#ifndef HITS_H
#define HITS_H

#include "probe.h"
#include "Util/Util.h"
#include "Util/SeqReader.h"

#include <google/sparse_hash_map>
#include <google/dense_hash_map>

void parse_hits_options(int, char **);
void hits_main(int, char **);

#endif

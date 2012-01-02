#include <iostream>
#include "cs_utils.h"

CSUtils::CSUtils()
{
  stcs.set_empty_key("-");
  stcs["AA"] = "0"; stcs["AC"] = "1"; stcs["AG"] = "2"; stcs["AT"] = "3";
  stcs["CA"] = "1"; stcs["CC"] = "0"; stcs["CG"] = "3"; stcs["CT"] = "2";
  stcs["GA"] = "2"; stcs["GC"] = "3"; stcs["GG"] = "0"; stcs["GT"] = "1";
  stcs["TA"] = "3"; stcs["TC"] = "2"; stcs["TG"] = "1"; stcs["TT"] = "0";
}

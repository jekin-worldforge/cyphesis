#!/usr/bin/env python

import sys

sys.stdout.write(
"""// This file was generated by gen_buildid.py
// Do not edit

#include "common/const.h"

namespace consts {

  const char * buildTime = __TIME__;
  const char * buildDate = __DATE__;
  const int buildId = %s;
}
""" % sys.argv[1])

# tail -n 1 ChangeLog | sed "s/^.* 1\.\([0-9]*\).*$/\1/"

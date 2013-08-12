#!/usr/bin/env python
from __future__ import print_function

from math import *
def bheap(sz):
    depth = ceil(log2(sz + 2))
    d_sz = 1 << (depth - 1)
    d_idx = sz - d_sz + 1

    print('depth={} d_sz={} d_idx={}'.format(depth,d_sz,d_idx))

#!/usr/bin/python3

from bcc import BPF
import time
import sys

if len(sys.argv) <= 3:
    print("Usage: %s xxx.c func dev" % (sys.argv[0]))
    exit(0)

src_file = sys.argv[1]
func = sys.argv[2]
device = sys.argv[3]

bpf = BPF(src_file)
fn = bpf.load_func(func, BPF.XDP)
bpf.attach_xdp(device, fn, 0)

try:
    bpf.trace_print()
except KeyboardInterrupt:
    pass

bpf.remove_xdp(device, 0)


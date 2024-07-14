#!/bin/python
from bcc import BPF

# 1.define BPF program (""" is used for multi-line string).
prog = """
#include <uapi/linux/ptrace.h>
#include <uapi/linux/limits.h>
#include <linux/sched.h>
// define output data structure in C
struct data_t {
    u32 pid;
    u64 ts;
    char comm[TASK_COMM_LEN];
	int dfd;
    char fname[NAME_MAX];
	int flags;
	unsigned short mode;
};
BPF_PERF_OUTPUT(events);

// define the handler for do_sys_open.
// ctx is required, while other params depends on traced function.
int user_cb(struct pt_regs *ctx, int dfd, const char __user *filename, int flags, umode_t mode){
    struct data_t data = {};
    data.pid = bpf_get_current_pid_tgid();
    data.ts = bpf_ktime_get_ns();
    if (bpf_get_current_comm(&data.comm, sizeof(data.comm)) == 0) {
        data.dfd = dfd;
        bpf_probe_read(&data.fname, sizeof(data.fname), (void *)filename);
		data.flags = flags;
		data.mode = mode;
    }
    events.perf_submit(ctx, &data, sizeof(data));
    return 0;
}
"""

# 2.load BPF program
bpf = BPF(text=prog)
# attach the kprobe for do_sys_open, and set handler to user_cb
bpf.attach_kprobe(event="do_sys_open", fn_name="user_cb")


# 3.process event
start = 0
def print_event(cpu, data, size):
    global start
    # event's type is data_t
    event = bpf["events"].event(data)
    if start == 0:
            start = event.ts
    time_s = (float(event.ts - start)) / 1000000000
    print("%-18.9f %-16s %-6d %-8s %-8s %-8s %-16s" % (time_s, event.comm, event.pid, hex(event.dfd), hex(event.flags), hex(event.mode), event.fname))

# loop with callback to print_event
bpf["events"].open_perf_buffer(print_event)
print("%-18s %-16s %-6s %-8s %-8s %-8s %-16s" % ("TIME(s)", "COMM", "PID", "DFD", "FLAGS", "MODE", "FILE"))

# 4 start the event polling loop
while 1:
    try:
        bpf.perf_buffer_poll()

    except KeyboardInterrupt:
        exit()


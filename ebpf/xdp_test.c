#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>

/* linux/include/net/xdp.h
struct xdp_rxq_info {
	struct net_device *dev;
	u32 queue_index;
	u32 reg_state;
	struct xdp_mem_info mem;
} ____cacheline_aligned;

struct xdp_buff {
	void *data;
	void *data_end;
	void *data_meta;
	void *data_hard_start;
	unsigned long handle;
	struct xdp_rxq_info *rxq;
};
*/

int test(struct xdp_md *ctx) {
    int ipsize = 0;
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;
    struct ethhdr *eth = data;
    struct iphdr *ip;

    ipsize = sizeof(struct ethhdr);
    ip = data + ipsize;

    ipsize += sizeof(struct iphdr);
    if (data + ipsize > data_end) {
        return XDP_DROP;
    }

    if (ip->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = (void *)ip + sizeof(*ip);
        ipsize += sizeof(struct tcphdr);
        if (data + ipsize > data_end) {
            return XDP_DROP;
        }

        if (tcp->dest == ntohs(6666)) {
            bpf_trace_printk("drop tcp dst port 6666\n");
            return XDP_DROP;
        }
    }

    return XDP_PASS;
}

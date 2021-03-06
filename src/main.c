#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/random.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <linux/init.h>   


void show_ipaddr(struct iphdr *iph);
int is_dstlocal(struct iphdr *iph);
static unsigned handle_hook(
    void *priv,
    struct sk_buff *skb,
    const struct nf_hook_state *state);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TCP Hijack Kernel Module");
MODULE_AUTHOR("Kazuki Takashima");
MODULE_VERSION("0.1");

void show_ipaddr(struct iphdr *iph)
{
    int ip_addr[4];
    ip_addr[0] = (int)((iph->saddr & 0xff000000) >> 24);
    ip_addr[1] = (int)((iph->saddr & 0x00ff0000) >> 16);
    ip_addr[2] = (int)((iph->saddr & 0x0000ff00) >> 8);
    ip_addr[3] = (int)(iph->saddr & 0x000000ff);
    printk(KERN_INFO "src %d.%d.%d.%d, %d\n", ip_addr[3], ip_addr[2], ip_addr[1], ip_addr[0], is_dstlocal(iph));
    return;
}

int is_dstlocal(struct iphdr *iph)
{
    return iph->saddr == 0x100007f;
}


static unsigned handle_hook(
    void *priv,
    struct sk_buff *skb,
    const struct nf_hook_state *state)
{
    struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = tcp_hdr(skb);

    if (!iph) {
        return NF_ACCEPT;
    }

    if (iph->version != 4) {
        return NF_ACCEPT;
    }

    if (iph->protocol != IPPROTO_TCP) {
        return NF_ACCEPT;
    }
    
    if (!is_dstlocal(iph))
    {
        return NF_ACCEPT;
    }

    // This packet is tcp and its destication IP is 127.0.0.1.
    tcph = tcp_hdr(skb);
    show_ipaddr(iph);

    u16 sport, dport;
    
    tcph->dest = htons(9999);
    sport = ntohs(tcph->source);
    dport = ntohs(tcph->dest);
    printk(KERN_INFO "tcph %u->%u\n", sport, dport);

    return NF_ACCEPT;
}

static struct nf_hook_ops hook_ops = {
    .hook = handle_hook,
    .pf = PF_INET,
    .hooknum = NF_INET_LOCAL_OUT, // パケット送信をフック
    .priority = NF_IP_PRI_FILTER,
};

int init_dev_hook(void)
{
    int err;
    err = nf_register_net_hook(&init_net, &hook_ops);
    if (err < 0)
    {
        return err;
    }
    return 0;
}

void cleanup_dev_hook(void)
{
    nf_unregister_net_hook(&init_net, &hook_ops);
}

static int __init init(void)
{
    int err;
    printk(KERN_INFO "[TCPJK] Hello World.\n");
    err = init_dev_hook();
    if (err < 0)
    {
        return err;
    }
    return 0;
}

static void __exit cleanup(void)
{
    printk(KERN_INFO "[TCPJK] Bye.\n");
    cleanup_dev_hook();
}

module_init(init);
module_exit(cleanup);

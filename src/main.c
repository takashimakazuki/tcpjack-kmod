#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TCP Hijack Kernel Module");
MODULE_AUTHOR("Kazuki Takashima");
MODULE_VERSION("0.1");

static int init(void)
{
    printk(KERN_INFO "[TCPJK] Hello World.\n");
    return 0;
}

static void cleanup(void)
{
    printk(KERN_INFO "[TCPJK] Bye.\n");
}

module_init(init);
module_exit(cleanup);
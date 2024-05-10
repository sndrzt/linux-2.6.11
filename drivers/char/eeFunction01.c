#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/ip.h>

extern int(*eeFunction01)(struct sk_buff* skb);

int eeShowMessage(struct sk_buff* skb)
{
	printk("Hello Linux Kernel Module!\n");
	return 0;
}

int init_module(void)
{
	eeFunction01 = eeShowMessage;
	printk("\n---Module install ok!---");
	return 0;
}

void cleanup_module(void)
{
	eeFunction01 = 0;
	printk("\n---Module uninstall ok!---");
}

MODULE_LICENSE("GPL");


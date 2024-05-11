#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <net/ip.h>

extern int(*eeFunction02)(struct sk_buff* skb);

int DSCPmaker(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *th;

	iph = skb->nh.iph;
	if(iph->protocol == IPPROTO_TCP)
	{
		skb->h.raw = (unsigned char*)(skb->nh.raw + iph->ihl * 4);
		th = skb->h.th;

		if((tcp_flag_word(th) & TCP_FLAG_ACK))
		{
			skb->nh.iph->tos = 0xB8;
			ip_send_check(skb->nh.iph);
		}
	}
	return 0;
}

int init_module(void)
{
	eeFunction02 = DSCPmaker;
	printk("\n---eeFunction02: DSCP Maker install ok!---");
	return 0;
}

void cleanup_module(void)
{
	eeFunction02 = 0;
	printk("\n---eeFunction02: DSCP Maker uninstall ok!---");
}

MODULE_LICENSE("GPL");


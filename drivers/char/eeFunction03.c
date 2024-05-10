#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <net/ip.h>

#define u32toValue(x) ((__u32)(\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
	(((__u32)(x) & (__u32)0x0000FF00UL) << 8) | \
	(((__u32)(x) & (__u32)0x00FF0000UL) >> 8) | \
	(((__u32)(x) & (__u32)0xFF000000UL) >> 24) ))

extern int eeSynCount;
extern int(*eeFunction03)(struct sk_buff* skb);

static void IpAddressToString(unsigned long i, char *s)
{
	unsigned long j1, j2, j3,j4;
	j1 = i & 0x000000FF;
	j2 = (i & 0x0000FF00) >> 8;
	j3 = (i & 0x00FF0000) >> 16;
	j4 = (i & 0xFF000000) >> 24;
	sprintf(s, "%u.%u.%u.%u", j4, j3, j2, j1);
}

int eePacketMonitor(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *th;
	char SourceIP[50], TargetIP[50];

	iph = skb->nh.iph;
	if(iph->protocol == IPPROTO_TCP)
	{
		th = skb->h.th;

		if(th->syn)
		{
			IpAddressToString(u32toValue(skb->nh.iph->saddr), SourceIP);
			IpAddressToString(u32toValue(skb->nh.iph->daddr), TargetIP);
			if(strcmp(SourceIP, "192.168.128.130") == 0 && strcmp(TargetIP, "192.168.128.128") == 0)
			{
				eeSynCount++;
				if(eeSynCount > 30000)
					eeSynCount = 0;
				printk("%s ---> %s ===> SYN Counter = %d\n", SourceIP, TargetIP, eeSynCount);
			}
		}
	}
	return 0;
}

int init_module(void)
{
	eeFunction03 = eePacketMonitor;
	printk("\n---eeFunction03: SYN Counter install ok!---");
	return 0;
}

void cleanup_module(void)
{
	eeFunction03 = 0;
	printk("\n---eeFunction03: SYN Counter uninstall ok!---");
}

MODULE_LICENSE("GPL");


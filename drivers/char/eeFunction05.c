#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/random.h>

#define u32toValue(x) ((__u32)(\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
	(((__u32)(x) & (__u32)0x0000FF00UL) << 8) | \
	(((__u32)(x) & (__u32)0x00FF0000UL) >> 8) | \
	(((__u32)(x) & (__u32)0xFF000000UL) >> 24) ))

extern int(*eeFunction05)(struct sk_buff* skb);

static void IpAddressToString(unsigned long i, char *s)
{
	unsigned long j1, j2, j3,j4;
	j1 = i & 0x000000FF;
	j2 = (i & 0x0000FF00) >> 8;
	j3 = (i & 0x00FF0000) >> 16;
	j4 = (i & 0xFF000000) >> 24;
	sprintf(s, "%u.%u.%u.%u", j4, j3, j2, j1);
}

int eePacketDropper(struct sk_buff *skb)
{
	struct iphdr *iph;
	char TargetIP[50];

	iph = skb->nh.iph;
	unsigned short t;

	printk("eePacketDropper entered.");
	IpAddressToString(u32toValue(skb->nh.iph->daddr), TargetIP);

	printk("\nTargetIP=%s\n", TargetIP);
	if(strcmp(TargetIP, "192.168.128.130") == 0)
	{
		get_random_bytes(&t, 2);
		printk("\nrand=%d", t);

		if(t < 32768)
		{
			printk("Packet Dropped!");

			return 1;
		}
	}

	return 0;
}

int init_module(void)
{
	eeFunction05 = eePacketDropper;
	printk("\n---eeFunction05: eePacketDropper install ok!---");
	return 0;
}

void cleanup_module(void)
{
	eeFunction05 = 0;
	printk("\n---eeFunction05: eePacketDropper uninstall ok!---");
}

MODULE_LICENSE("GPL");


#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

struct person {
        struct hlist_node fa_list;
        int age;
};

struct hlist_head leaf;

static int __init testhlist_init(void){
        printk(KERN_ALERT "testhlist_init\n");
        struct person a1 = { .age = 1 };
        struct person a2 = { .age = 3 };
        struct person a3 = { .age = 2 };
        struct person a4 = { .age = 4 };

	hlist_add_head(&a1.fa_list, &leaf);
	hlist_add_head(&a2.fa_list, &leaf);
	hlist_add_head(&a3.fa_list, &leaf);
	hlist_add_head(&a4.fa_list, &leaf);

	struct person *p;
	struct hlist_node *_p;

	hlist_for_each_entry(p, _p, &leaf, fa_list) {
        	printk(KERN_ALERT "Age: %d\n", p->age);
	}

        hlist_del(&p->fa_list);

	hlist_for_each_entry(p, _p, &leaf, fa_list) {
        	printk(KERN_ALERT "Age: %d\n", p->age);
	}

        return 0;
}

static void __exit testhlist_exit(void){
        printk(KERN_ALERT "testlist exit.\n\n");
}

module_init(testhlist_init);
module_exit(testhlist_exit);
MODULE_LICENSE("Dual BSD/GPL");


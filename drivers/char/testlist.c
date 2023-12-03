#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

struct item{
	int id;
	char* name;
	struct list_head list;
};

void print_item(struct item *p){
	printk(KERN_ALERT "id = %d, name = %s\n", p->id, p->name);
}

static int __init testlist_init(void){
	struct item *a, *b, *c;
	struct item *p;

	LIST_HEAD(itm_lst);

	a = kmalloc(sizeof(*a), GFP_KERNEL); a->id = 1; a->name = "xr1";
	INIT_LIST_HEAD(&a->list);

	b = kmalloc(sizeof(*b), GFP_KERNEL); b->id = 2; b->name = "xr2";
	INIT_LIST_HEAD(&b->list);

	c = kmalloc(sizeof(*c), GFP_KERNEL); c->id = 3; c->name = "xr3";
	INIT_LIST_HEAD(&c->list);

	list_add(&a->list, &itm_lst); list_add(&b->list, &itm_lst); list_add(&c->list, &itm_lst);

	list_for_each_entry(p, &itm_lst, list){// print from 3 to 1
		print_item(p);
	}

	list_for_each_entry_reverse(p, &itm_lst, list){// print from 1 to 3
		print_item(p);
	}

	list_del(&b->list); // delete a entry b
	list_for_each_entry(p, &itm_lst, list){
		print_item(p);
	}

#if 0
	list_replace(&c->list, &b->list); // replace c with b
	list_for_each_entry(p, &itm_lst, list){
		print_item(p);
	}
#endif

	return 0;
}

static void __exit testlist_exit(void){
	printk(KERN_ALERT "testlist exit.\n\n");
}

module_init(testlist_init);
module_exit(testlist_exit);
MODULE_LICENSE("Dual BSD/GPL");


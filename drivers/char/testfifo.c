#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/time.h>
#include <linux/err.h>

struct item
{
	int id;
	char *name;
};

static void print_item(struct item *itm){
	printk(KERN_ALERT "  item id: %d, name: %s\n", itm->id, itm->name);
}

static int __init testfifo_init(void){
    struct kfifo *que;
    struct item *a, *b, *c;
    struct item *p;
    char *itm;
    int i;

    spinlock_t fifo_lock;
    spin_lock_init(&fifo_lock);
    que = kfifo_alloc(3 * sizeof(struct item), GFP_KERNEL, &fifo_lock);

    a = kmalloc(sizeof(struct item), GFP_KERNEL);
    a->id = 1; a->name = "xr1";
    kfifo_put(que, (char *)a, sizeof(struct item));

    b = kmalloc(sizeof(struct item), GFP_KERNEL);
    b->id = 2; b->name = "xr2";
    kfifo_put(que, (char *)b, sizeof(struct item));

    c = kmalloc(sizeof(struct item), GFP_KERNEL);
    c->id = 3; c->name = "xr3";
    kfifo_put(que, (char *)c, sizeof(struct item));

    itm = kmalloc(sizeof(struct item), GFP_KERNEL);

    printk(KERN_ALERT "current kfifo length: %d\n", kfifo_len(que));

    for(i = 0; i < 3; i++){
    	kfifo_get(que, itm, sizeof(struct item));
    	p = (struct item*)(itm);
    	print_item(p);
    	printk(KERN_ALERT "  current kfifo length: %d\n\n", kfifo_len(que));
    }

    printk(KERN_ALERT "current kfifo length: %d\n", kfifo_len(que));
    kfifo_free(que);
    kfree(itm);

    return 0;
}

static void __exit testfifo_exit(void){
	printk(KERN_ALERT "testfifo exit.\n\n");
}

module_init(testfifo_init);
module_exit(testfifo_exit);
MODULE_LICENSE("Dual BSD/GPL");


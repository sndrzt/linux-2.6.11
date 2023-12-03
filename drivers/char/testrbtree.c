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
	struct rb_node node;
};

void print_item(struct item *itm){
	printk(KERN_ALERT "id=%d\tname=%s\n", itm->id, itm->name);
}

static void insert_item(struct item *itm, struct rb_root *root){
	struct rb_node *parent = NULL, **new = &root->rb_node;

	int id = itm->id;

	while(*new){
		parent = *new;
		if(id < rb_entry(parent, struct item, node)->id){
			new = &parent->rb_left;
		}else if(id > rb_entry(parent, struct item, node)->id){
			new = &parent->rb_right;
		}else{
			printk(KERN_ALERT "id=%d has in the rbtree now\n", id);
			return;
		}
	}

	rb_link_node(&itm->node, parent, new);
	rb_insert_color(&itm->node, root);
}

static void remove_item(struct item *itm, struct rb_root *root){
	rb_erase(&(itm->node), root);
}

static void display_item_from_small(struct rb_node *node){
	struct item *p;

	if(node){
		display_item_from_small(node->rb_left);
		p = rb_entry(node, struct item, node);
		print_item(p);
		display_item_from_small(node->rb_right);
	}
}

static void display_item_from_big(struct rb_node *node){
	struct item *p;

	if(node){
		display_item_from_big(node->rb_right);
		p = rb_entry(node, struct item, node);
		print_item(p);
		display_item_from_big(node->rb_left);
	}
}

static int display_item(struct rb_root *root, int order){
	if(!root->rb_node){
		return 1;
	}

	if(order < 0){
		display_item_from_small(root->rb_node);
	}else{
		display_item_from_big(root->rb_node);
	}

	return 0;
}

static int testrbtree_init(void){
#define N 10
	struct rb_root root = RB_ROOT;
	struct item *itm[N];
	char tmp_name[5] = "xray";
	int i;

	for(i = 0; i < N; i++){
		itm[i] = kmalloc(sizeof(struct item), GFP_KERNEL);
		itm[i]->id = i;
		itm[i]->name = kmalloc(sizeof(char) * 6, GFP_KERNEL);
		tmp_name[3] = (char)(i + '0');
		strcpy(itm[i]->name, tmp_name);
		itm[i]->node.rb_left = NULL;
		itm[i]->node.rb_right = NULL;
	}

	for(i = 0; i < N; i++){
		printk(KERN_ALERT"id=%d   name=%s\n", itm[i]->id, itm[i]->name);
	}

	for(i = 0; i < N; i++){
		insert_item(itm[i], &root);// add N items to rb-tree
	}

	printk(KERN_ALERT "print from small to big\n");
	display_item(&root, -1);
	printk(KERN_ALERT "\nprint from big to small\n");
	display_item(&root, 1);

	remove_item(itm[8], &root);
	display_item(&root, -1);

	for(i = 0; i < N; i++){
		kfree(itm[i]->name);
		kfree(itm[i]);
	}

	return 0;
}

static void testrbtree_exit(void){
	printk(KERN_ALERT"testrbtree exit.\n");
}

module_init(testrbtree_init);
module_exit(testrbtree_exit);
MODULE_LICENSE("Dual BSD/GPL");


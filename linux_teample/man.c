#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>
#define iter_num 1000000
MODULE_LICENSE("Dual BSE/GPL");

//global
struct list_head my_list;
struct list_head garbage;
struct my_node{
	struct list_head list;
	int data;
};

struct my_node* Get_Node(struct list_head* avail){
	struct my_node* newNode;
	if(avail->next==avail){
		newNode=kmalloc(sizeof(struct my_node),GFP_KERNEL);
	//	printk("my_node create new\n");
	}
	else{
		newNode=list_entry(avail->next,struct my_node,list);
		list_del(avail->next);
	//	printk("my_node from garbage\n");
	}
	return newNode;
}
void Return_Node(struct list_head* node,struct list_head* avail){ 	
	list_add(node,avail);
	//printk("node store in garbage\n");
}
long start,last;

static inline s64 curr_get_timestamp(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return timespec_to_ns(&ts);
}
void struct_example(void)
{

	/*initialize list*/
	INIT_LIST_HEAD(&my_list);
	INIT_LIST_HEAD(&garbage);
	int i;
	start=curr_get_timestamp();
	for(i=0;i<iter_num;i++){
		struct my_node *new=Get_Node(&garbage);
		new->data=i;
		list_add(&new->list,&my_list);
	} //insert 100000

	struct my_node *cur;
	struct my_node *tmp;
	list_for_each_entry_safe(cur,tmp,&my_list,list){
		list_del(&cur->list);
		Return_Node(&cur->list,&garbage);
	} //delete all
	for(i=0;i<iter_num;i++){
		struct my_node* new=Get_Node(&garbage);
		new->data=i;
		list_add(&new->list,&my_list);
	}//reinsert 10000
	list_for_each_entry_safe(cur,tmp,&my_list,list){
		list_del(&cur->list);
		Return_Node(&cur->list,&garbage);
	}
	last=curr_get_timestamp();
	printk("insert-delete-insert time(getnode):%ldus",last-start);

	/*list_for_each_entry_safe(cur,tmp,&my_list,list){
		if(cur->data==12232){
		printk("delete-cur-value:%d\n",cur->data);
		list_del(&cur->list);
		kfree(cur);
		break;
	}
	}
	last=curr_get_timestamp();*/
	//printk("IN 100000's list. delete time:%ldus",last-start);*/
}
/*static inline s64 curr_get_timestamp(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return timespec_to_ns(&ts);
}
*/
static int __init hello_init(void)
{
	printk(KERN_NOTICE "init!\n");
	struct_example();
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_NOTICE "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);



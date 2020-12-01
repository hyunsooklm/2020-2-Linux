#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
MODULE_LICENSE("Dual BSE/GPL");
int counter;
spinlock_t counter_lock;
struct task_struct *w_t1,*w_t2,*w_t3,*w_t4;
struct list_head my_list;
int search_num=42312; //search_num
struct my_node{
	struct list_head list;
	int data;
};

long start,last;

static inline s64 curr_get_timestamp(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return timespec_to_ns(&ts);
}
/*static int writer_function(void *data)
{
	
}*/

static int search_function(void* data)
{
	struct my_node *tmp;
	struct my_node *cur;
	list_for_each_entry_safe(cur,tmp,&my_list,list){
		spin_lock(&counter_lock);
		if(cur->data==search_num){
			last=curr_get_timestamp();
			/*first find search time is here. it starts when init_function start thread_create */
			printk("search_num found!:%d",search_num);
			printk("SPinlock linked list search time: %ld ns",last-start);
		}
		spin_unlock(&counter_lock);
	}
}
static void insert_function(void* data)
{
	for(;counter<100000;counter++){
		struct my_node *new= kmalloc(sizeof(struct my_node),GFP_KERNEL);
		new->data=counter;
		list_add(&new->list,&my_list);
	}		
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
	INIT_LIST_HEAD(&my_list);
	printk(KERN_NOTICE "init!\n");
	counter=0;
	insert_function("hi");
	start=curr_get_timestamp();
	spin_lock_init(&counter_lock);
	w_t1=kthread_run(search_function,NULL,"w_thread1");
	w_t2=kthread_run(search_function,NULL,"w_thread2");
	w_t3=kthread_run(search_function,NULL,"w_thread3");
	w_t4=kthread_run(search_function,NULL,"w_thread4");
	last=curr_get_timestamp();
	printk("bye~");
	return 0;
}

static void __exit hello_exit(void)
{
	kthread_stop(w_t1);
	kthread_stop(w_t2);
	kthread_stop(w_t3);
	kthread_stop(w_t4);
	printk(KERN_NOTICE "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);



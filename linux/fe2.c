#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kthread.h>

int counter=0;
int i=0;
struct task_struct *th1,*th2,*th3,*th4;

static int writer_function(void *data)
{
	__sync_fetch_and_add(&i,1); 
	printk("counter:%d by pid:%u\n",++counter,current->pid);

}
static int __init my_init(void)
{
	printk("start\n");
	th1=kthread_run(writer_function,NULL,"writer_function");
	th2=kthread_run(writer_function,NULL,"writer_function");
	th3=kthread_run(writer_function,NULL,"writer_function");
	th4=kthread_run(writer_function,NULL,"writer_function");
	return 0;

}
static void __exit my_exit(void)
{
	kthread_stop(th1);
	kthread_stop(th2);
	kthread_stop(th3);
	kthread_stop(th4);
	printk("exit\n");
}
module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

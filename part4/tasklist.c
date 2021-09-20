#include <linux/module.h>

int tasklist_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	return 0;
}

void tasklist_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

module_init(tasklist_init);
module_exit(tasklist_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklist Module");
MODULE_AUTHOR("SGG");

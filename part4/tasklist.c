#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/task.h>

void print_process(const struct task_struct *const process, const u32 level)
{
	const bool is_root = level == 0;
	/* don't indent too far, and don't put an arrow on root/init */
	const char *const arrow = is_root ? "" : " \\_ ";
	const u32 indents = min(is_root ? 0 : level - 1, 20u);
	const u32 num_spaces = indents * 4;
	printk(KERN_INFO "%*s%s[%d] %s\n", num_spaces, "", arrow, process->pid,
	       process->comm);
}

void print_process_tree(const struct task_struct *const parent, const u32 level)
{
	const struct task_struct *child;
	print_process(parent, level);
	list_for_each_entry (child, &parent->children, sibling) {
		print_process_tree(child, level + 1);
	}
}

int tasklist_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	print_process_tree(&init_task, 0);
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
MODULE_AUTHOR("ks3343");

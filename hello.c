// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Kulish Alina <alinakulis8@gmail.com>");
MODULE_DESCRIPTION("Modified Kernel Module for Hello Messages");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of 'Hello, world!' messages to print");

struct hello_entry {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	int i;
	struct hello_entry *item;

	if (print_count == 0 || (print_count >= 5 && print_count <= 10))
		pr_warn("Warning: print_count is %u.\n", print_count);
	else if (print_count > 10)
		BUG_ON(1);

	for (i = 0; i < print_count; i++) {
		if (i == 4) {
			pr_err("Error: Simulated failure for the 5th entry.\n");
			item = NULL;
		} else {
			item = kmalloc(sizeof(*item), GFP_KERNEL);
		}

		BUG_ON(!item);

		item->time = ktime_get();
		list_add(&item->list, &hello_list);
		pr_info("Hello, kernel world! (%d/%u)\n", i + 1, print_count);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_entry *item, *tmp;

	pr_info("Cleaning up...\n");
	list_for_each_entry_safe(item, tmp, &hello_list, list) {
		pr_info("Entry time: %lld ns\n", ktime_to_ns(item->time));
		list_del(&item->list);
		kfree(item);
	}
	pr_info("Module unloaded.\n");
}

module_init(hello_init);
module_exit(hello_exit);

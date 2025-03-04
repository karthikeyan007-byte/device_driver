#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\xb1\x19\x54\xaa"
	"cdev_add\0\0\0\0"
	"\x18\x00\x00\x00\xe2\x75\x96\x16"
	"class_create\0\0\0\0"
	"\x18\x00\x00\x00\xc3\x9e\x64\x92"
	"device_create\0\0\0"
	"\x20\x00\x00\x00\x8e\x83\xd5\x92"
	"request_threaded_irq\0\0\0\0"
	"\x14\x00\x00\x00\x82\x7f\x8a\x2b"
	"kernel_kobj\0"
	"\x20\x00\x00\x00\x01\xd2\xee\xd6"
	"kobject_create_and_add\0\0"
	"\x20\x00\x00\x00\x91\x58\x0d\x2d"
	"sysfs_create_file_ns\0\0\0\0"
	"\x18\x00\x00\x00\x10\x5e\x65\x38"
	"device_destroy\0\0"
	"\x18\x00\x00\x00\xf5\x76\x81\x28"
	"class_destroy\0\0\0"
	"\x14\x00\x00\x00\x05\xb4\xd2\x39"
	"cdev_del\0\0\0\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x20\x00\x00\x00\x13\xc4\x2d\x4f"
	"sysfs_remove_file_ns\0\0\0\0"
	"\x14\x00\x00\x00\xb0\xd8\xae\x22"
	"kobject_put\0"
	"\x14\x00\x00\x00\x3b\x4a\x51\xc1"
	"free_irq\0\0\0\0"
	"\x14\x00\x00\x00\xd3\x85\x33\x2d"
	"system_wq\0\0\0"
	"\x18\x00\x00\x00\x36\xf2\xb6\xc5"
	"queue_work_on\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x10\x00\x00\x00\xfd\xf9\x3f\x3c"
	"sprintf\0"
	"\x1c\x00\x00\x00\x2b\x2f\xec\xe3"
	"alloc_chrdev_region\0"
	"\x14\x00\x00\x00\x60\x73\x77\x14"
	"cdev_init\0\0\0"
	"\x18\x00\x00\x00\xd7\xd3\x75\x6d"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "ECF18C1942EB0497B9759D4");

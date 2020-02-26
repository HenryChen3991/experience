#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x884142c7, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9c5fdb7d, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0x5c113231, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0x6baa8f01, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0x139b504, __VMLINUX_SYMBOL_STR(cpu_current_top_of_stack) },
	{ 0x8ed9f6a4, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F956683CF7DD8A2A2F4BF78");

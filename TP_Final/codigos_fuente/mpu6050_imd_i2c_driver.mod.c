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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2ecf8102, "i2c_register_driver" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x56dc061e, "misc_deregister" },
	{ 0x92997ed8, "_printk" },
	{ 0x6ce9b950, "i2c_transfer_buffer_flags" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xabe56251, "devm_kmalloc" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x94952119, "misc_register" },
	{ 0x3d9c637d, "i2c_del_driver" },
	{ 0xb347dd95, "i2c_transfer" },
	{ 0x5c8f8c65, "__current" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x2c66a812, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmse,IMD_TPF");
MODULE_ALIAS("of:N*T*Cmse,IMD_TPFC*");

MODULE_INFO(srcversion, "7AC9F1C0ADEE8FB0502AAF3");

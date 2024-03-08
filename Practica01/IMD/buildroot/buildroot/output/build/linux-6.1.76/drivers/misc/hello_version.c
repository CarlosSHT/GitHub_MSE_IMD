// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/moduleparam.h>

static char *param_author = "Created by Carlos Herrera";
module_param(param_author, charp, 0644);
MODULE_PARM_DESC(param_author, "Menciona el autor del modulo");

static uint start_secs;
static uint total_secs;
module_param(total_secs, uint, 0644);
MODULE_PARM_DESC(total_secs, "Tiempo total del modulo cargado");

static int __init helloworld_init(void)
{
	pr_info("Hola Mundo. Estas utilizando Linux %s\n", utsname()->release);
	pr_info("%s\n", param_author);
	start_secs = ktime_get_seconds();
	return 0;
}

static void __exit helloworld_exit(void)
{
	total_secs = ktime_get_seconds() - start_secs;
	pr_info("Tiempo transcurrido es %d segundos\n", total_secs);
	pr_info("Fin del mundo. Adios\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
MODULE_AUTHOR("Cosme Fulanito <cosme.fulanito@gmail.com>");
MODULE_LICENSE("GPL");

#include <linux/module.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/kernel.h>
struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

void print_pokemon(struct pokemon *p)
{
	printk(KERN_INFO "%s: National Dex No. #%d\n", p->name, p->dex_no);
}

static LIST_HEAD(pokedex);

struct pokemon new_pokemon(const char *const name, const int dex_no)
{
	struct pokemon this = {
		.name = { 0 },
		.dex_no = dex_no,
		.list = LIST_HEAD_INIT(this.list),
	};
	strncpy(this.name, name, sizeof(this.name) - 1);
	return this;
}

struct pokemon *alloc_pokemon(const char *const name, const int dex_no)
{
	struct pokemon *const this = kmalloc(sizeof(*this), GFP_KERNEL);
	*this = new_pokemon(name, dex_no);
	return this;
}

void add_pokemon(char *name, int dex_no)
{
	printk(KERN_INFO "add_pokemon\n");
	struct pokemon *const pokemon = alloc_pokemon(name, dex_no);
	print_pokemon(pokemon);
	list_add_tail(&pokemon->list, &pokedex);
}

void print_pokedex(void)
{
	printk(KERN_INFO "print_pokedex\n");
	struct pokemon *pokemon;
	list_for_each_entry (pokemon, &pokedex, list) {
		print_pokemon(pokemon);
	}
}

void delete_pokedex(void)
{
	printk(KERN_INFO "delete_pokedex\n");
	struct pokemon *pokemon;
	struct pokemon *tmp;
	list_for_each_entry_safe (pokemon, tmp, &pokedex, list) {
		list_del(&pokemon->list);
		kfree(pokemon);
	}
}

int pokedex_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init(pokedex_init);
module_exit(pokedex_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module");
MODULE_AUTHOR("ks3343");

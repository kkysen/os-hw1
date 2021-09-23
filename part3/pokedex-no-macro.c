#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

/*
 * Undefine commonly used macros -- DO NOT MODIFY
 * Please also do not use other macros and functions defined in <linux/list.h>,
 * such as INIT_LIST_HEAD, list_add, list_add_tail, list_del, list_empty,
 * or any of the internal helper functions beginning with __.
 */
#undef LIST_HEAD
#undef LIST_HEAD_INIT
#undef list_entry
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef container_of
#undef offsetof
#undef READ_ONCE
#undef WRITE_ONCE
struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

const size_t offset = (size_t) & ((struct pokemon *)NULL)->list;

void print_pokemon(struct pokemon *p)
{
	printk(KERN_INFO "%s: National Dex No. #%d\n", p->name, p->dex_no);
}

static struct list_head pokedex = {
	.next = &pokedex,
	.prev = &pokedex,
};

void add_pokemon(char *name, int dex_no)
{
	struct pokemon *pokemon_ptr;
	struct list_head *new;
	struct list_head *head;
	struct list_head *prev;
	struct list_head *next;
	struct list_head *volatile *prev_next_ptr;
	struct pokemon pokemon = {
		.name = { 0 },
		.dex_no = dex_no,
		.list = {
			.next = &pokemon.list,
			.prev = &pokemon.list,
		},
	};
	strncpy(pokemon.name, name, sizeof(pokemon.name) - 1);
	pokemon_ptr = kmalloc(sizeof(*pokemon_ptr), GFP_KERNEL);
	*pokemon_ptr = pokemon;
	new = &pokemon_ptr->list;
	head = &pokedex;
	prev = head->prev;
	next = head;
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev_next_ptr = &prev->next;
	*prev_next_ptr = new;
}

void print_pokedex(void)
{
	struct pokemon *pokemon;
	for (pokemon = (void *)pokedex.next - offset;
	     &pokemon->list != &pokedex;
	     pokemon = (void *)pokemon->list.next - offset) {
		print_pokemon(pokemon);
	}
}

void delete_pokedex(void)
{
	struct pokemon *pokemon;
	struct pokemon *tmp;
	for (pokemon = (void *)pokedex.next - offset,
	    tmp = (void *)pokemon->list.next - offset;
	     &pokemon->list != &pokedex;
	     pokemon = tmp, tmp = (void *)tmp->list.next - offset) {
		struct list_head *entry = &pokemon->list;
		struct list_head *prev = entry->prev;
		struct list_head *next = entry->next;
		struct list_head *volatile *prev_next_ptr;
		next->prev = prev;
		prev_next_ptr = &prev->next;
		*prev_next_ptr = next;
		entry->next = (void *)0x100;
		entry->prev = (void *)0x122;
		kfree(pokemon);
	}
}

int pokedex_nom_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_nom_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init(pokedex_nom_init);
module_exit(pokedex_nom_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module, without list macros");
MODULE_AUTHOR("ks3343");

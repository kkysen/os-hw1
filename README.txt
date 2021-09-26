Khyber Sen
ks3343
hw1

### part1
This part is "working", if a text file can work.

### part2
This part is working.  I also added a couple helper functions
(`new_pokemon` and `alloc_pokemon`) just to make things a bit clearer.

### part3
This part is working.  It's basically the same code as part1,
except
    1) I inlined the `new_pokemon` and `alloc_pokemon` functions
       because the instructions said we couldn't have any other functions.
    2) The list functions and macros are fully expanded,
       except the assertions in macros (like the ones from `container_of`
       and `WRITE_ONCE`) were removed since they don't result in errors anyways.
       The `WRITE_ONCE` expansions I kept, though
       (i.e. the writes through volatile pointers).

### part4
This part is working.  I didn't include the leading ` -- `, but otherwise
it's mostly the same as the example, and definitely shows the process tree.

### In-General
I also included a few extra files helpful for running and developing this homework.
For example, I included the `.clang-format` from the linux kernel repo,
which I use the format the C files so that they definitely follow
the linux kernel style guidelines.

I also included a `justfile`, which is similar to a `Makefile`.
It's run by `just` (https://github.com/casey/just),
a command-runner similar to `make` but with a bunch of extra nifty features, like:

    `just fmt`: format the C files using the `.clang-format`.
    `just make-part part<N>`: run make in the given part directory.
    `just make`: make everything
    `just run-mod part<N>`: make and run the module in a given part,
        running it with `kedr`,
        printing out the `dmesg` log from this module,
        and printing out the `kedr` stats.
    `just run-mods`: make and run all the modules
    `just compile-commands`: make everything through `bear` so it can
        generate a compilation database for better IDE intellisense.

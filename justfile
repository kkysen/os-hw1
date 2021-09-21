set shell := ["bash", "-c"]

uni := "ks3343"
n_proc := `nproc`

default:
    just --list

parallel-bash:
    printf '%s\nwait' "$(map printf '%s &\n')" | bash

list-parts:
    fd '^Makefile$' --exec-batch dirname

make-part part *args:
    cd "{{part}}" && make -j{{n_proc}} {{args}}

make-commands *args:
    just list-parts | map printf '(just make-part "%s" {{args}})\n'

make *args:
    just make-commands {{args}} | just parallel-bash

link-clang-format-style:
    ln --force ../linux/.clang-format .

fmt-commands *args:
    rg --files --type c | map printf 'clang-format {{args}} "%s"\n'

fmt-args *args: link-clang-format-style
    just fmt-commands {{args}} | just parallel-bash

fmt: (fmt-args "-i")

gitui: fmt
    GIT_EDITOR=nano gitui

log *args:
    sudo dmesg --kernel {{args}}

log-watch *args: (log "--follow-new" args)

ll *args:
    cd "{{invocation_directory()}}" && \
        exa -alF --git --sort time {{args}} \
        $(fd --hidden --type file --exclude '.git')

run-mod-priv part:
    #!/usr/bin/env bash
    cd "{{part}}"
    kedr start *.ko
    echo "running $(tput setaf 2){{part}}$(tput sgr 0):"
    insmod *.ko
    rmmod *.ko
    cd /sys/kernel/debug/kedr_leak_check
    bat info possible_leaks unallocated_frees
    kedr stop

run-mod-only part:
    sudo env "PATH=${PATH}:/usr/local/sbin:/usr/sbin:/sbin" just run-mod-priv "{{part}}"

run-mod part: (make-part part) (run-mod-only part)

run-mods: make
    # not parallel b/c modules are global, but can build in parallel
    just list-parts | map just run-mod-only

archive:
    #!/usr/bin/env bash
    dir="${PWD##*/}"
    cd .. && tar cfz "${dir}-{{uni}}.tgz" "${dir}"

set shell := ["bash", "-c"]

n_proc := `nproc`

default:
    just --list

parallel-bash:
    printf '%s\nwait' "$(map printf '%s &\n')" | bash

make-commands *args:
    fd '^Makefile$' --exec-batch dirname | map printf '(cd "%s" && make -j{{n_proc}} {{args}})\n'

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

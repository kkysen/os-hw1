set shell := ["bash", "-c"]

default:
    just --list

parallel-bash:
    printf '%s\nwait' "$(map printf '%s &\n')" | bash

make *args:
    fd '^Makefile$' --exec-batch dirname | map printf '(cd "%s" && make {{args}})\n' | just parallel-bash

link-clang-format-style:
    ln --force ../linux/.clang-format .

fmt-args *args: link-clang-format-style
    rg --files --type c | map printf 'clang-format {{args}} "%s"\n' | just parallel-bash

fmt: (fmt-args "-i")

gitui: fmt
    GIT_EDITOR=nano gitui

set shell := ["bash", "-c"]

default:
    just --list

parallel-bash:
    printf '%s\nwait' "$(map printf '%s &\n')" | bash

make *args:
    fd '^Makefile$' --exec-batch dirname | map printf '(cd "%s" && make {{args}})\n' | just parallel-bash

fmt-args *args:
    ln --force ../linux/.clang-format .
    rg --files --type c | map printf 'clang-format {{args}} "%s"\n' | just parallel-bash

fmt:
    just fmt-args -i

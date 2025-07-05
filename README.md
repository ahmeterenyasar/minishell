# minishell

## Project Overview

`minishell` is a lightweight Unix‑like shell implementation in C as part of the 42 Istanbul curriculum. The goal is to reproduce the core interactive features of **bash** while respecting the strict coding standards of the school (no external libraries beyond `readline`, secure memory management, and norm‑compliant style).

Key capabilities include:

* Prompt with command history and line‑editing (GNU Readline)
* Lexing, parsing, expansion, and execution pipeline mirroring POSIX shells
* Built‑in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
* Pipes (`|`), input/output redirections (`<`, `>`, `>>`) and here‑documents (`<<`)
* Environment‑variable expansion, including `$?` for the last exit status
* Proper signal handling for `Ctrl‑C`, `Ctrl‑\`, and `Ctrl‑D`

---

## How It Works

Internally **minishell** keeps to a clear four‑stage processing pipeline, but instead of an Abstract Syntax Tree our implementation relies on a *linked list* of `t_command` nodes:

1. **Tokenizer** – breaks the input line into lexical tokens (words, operators, quotes) while honouring escapes and contextual rules.
2. **Parser** – converts the flat token array into a linked list where each node describes one command and its metadata:

   * `argv[]` – arguments (after quote handling)
   * `redir`  – list of redirections / heredocs
   * `next`   – pointer to the following command when part of a pipeline
3. **Expander** – walks the list, applying environment‑variable expansion, parameter substitution, and quote removal, then rebuilds each node's `argv`.
4. **Executor** – iterates over the list:

   * built‑ins run directly in the parent when side effects are required (e.g. `cd`, `export`)
   * external commands fork/exec; pipes are assembled by following `next` pointers; per‑command redirections are applied just before `execve()`

[Chart will be uploaded here]

[Test scenarios will be uploaded here]

---

## Installation & Compilation

```bash
# Clone
$ git clone https://github.com/ahmeterenyasar/minishell.git
$ cd minishell

# Build (requires GNU make and clang or gcc)
$ make            # builds libft and minishell binary

# Optional clean targets
$ make clean      # remove object files
$ make fclean     # remove objects + binary
$ make re         # full rebuild
```

*Dependencies*: `readline` headers/libs (e.g. `libreadline-dev` on Debian/Ubuntu).

---

## Usage

```bash
$ ./minishell
minishell$ echo "Hello world" | tr a-z A-Z
HELLO WORLD
minishell$ export PATH=$PATH:./scripts
minishell$ ./myscript.sh < in.txt >> out.log 2>&1
minishell$ exit 0
```

The prompt reflects the exit status of the previous command (`$?`). Press `Ctrl‑D` to send EOF and exit gracefully.

---

## File Structure

```
libft/            → 42 libft (utility library)
include/          → header files
  ├── builtins.h
  ├── executor.h
  ├── expansions.h
  ├── heredoc.h
  ├── minishell.h
  ├── parser.h
  ├── redirection.h
  ├── shell_main.h
  ├── signals.h
  ├── string_utils.h
  ├── syntax_checker.h
  ├── tokenizer.h
  └── types.h
src/              → source files
  ├── builtins/
  ├── core/
  ├── environment/
  ├── executor/
  ├── expansion/
  ├── parser/
  ├── pipeline/
  ├── redirection/
  ├── signals/
  └── tokenizer/
Makefile
main.c
README.md
readline.supp     → valgrind suppression file for readline
```

---

## Flow Diagram

[Chart will be uploaded here]

---

## Status

### Known Issues 🔴
- `export a="ls -la"` and `$a` expansion issues
- `^C echo $?` sometimes not working - signal handling needs fixing
- Empty string commands (`"  "`) should give "command not found"

### In Progress 🟡
- Signal handling issues (mostly fixed, needs heredoc verification)
- Comprehensive pipe testing
- Code normalization compliance

### Completed 🟢
- Exit code handling in pipelines (refactored from special return codes to flag-based approach)
  - `minishell$ ls | exit 100` → `$? = 100`
  - `minishell$ ls | exit` → `$? = 0`
  - `minishell$ invalid_cmd | exit 50` → `$? = 50`
  - `minishell$ exit 42 | ls` → `$? = 0`
  - Normal exit behavior preserved
  - Exit with too many arguments handled correctly
- Signal handling for `^C` returns exit code 130
- Heredoc expansion with quotes fixed
- Memory management improvements
- Complex pipe handling: `cat << eof | cat << asd | cat << qwe`
- Sleep with pipes functionality
- Debugger cleanup 

---

## License

This repository currently has **no explicit license file**. Until one is added, all rights remain with the original author(s). If you wish to reuse the code, please open an issue or contact the maintainer.

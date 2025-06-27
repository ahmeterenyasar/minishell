# minishell

## Project Overview

`minishell` is a lightweight Unix‑like shell written in C as part of the 42 Istanbul curriculum. The goal is to reproduce the core interactive features of **bash** while respecting the strict coding standards of the school (no external libraries beyond `readline`, secure memory management, and norm‑compliant style).

Key capabilities include:

* Prompt with command history and line‑editing (GNU Readline)
* Lexing, parsing, expansion, and execution pipeline mirroring POSIX shells
* Built‑in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
* Pipes (`|`), input/output redirections (`<`, `>`, `>>`) and here‑documents (`<<`)
* Environment‑variable expansion, including `$?` for the last exit status
* Proper signal handling for `Ctrl‑C`, `Ctrl‑\`, and `Ctrl‑D`

---

## How It Works

Internally **minishell** keeps to a clear four‑stage processing pipeline, but instead of an Abstract Syntax Tree our implementation relies on a *linked list* of `t_command` nodes:

1. **Tokenizer** – breaks the input line into lexical tokens (words, operators, quotes) while honouring escapes and contextual rules.
2. **Parser** – converts the flat token array into a linked list where each node describes one command and its metadata:

   * `argv[]` – arguments (after quote handling)
   * `redir`  – list of redirections / heredocs
   * `next`   – pointer to the following command when part of a pipeline
3. **Expander** – walks the list, applying environment‑variable expansion, parameter substitution, and quote removal, then rebuilds each node’s `argv`.
4. **Executor** – iterates over the list:

   * built‑ins run directly in the parent when side effects are required (e.g. `cd`, `export`)
   * external commands fork/exec; pipes are assembled by following `next` pointers; per‑command redirections are applied just before `execve()`

[Chart will be uploaded here]

[Test scenarios will be uploaded here]

---

## Installation & Compilation

```bash
# Clone
$ git clone https://github.com/ahmeterenyasar/minishell.git
$ cd minishell

# Build (requires GNU make and clang or gcc)
$ make            # builds libft and minishell binary

# Optional clean targets
$ make clean      # remove object files
$ make fclean     # remove objects + binary
$ make re         # full rebuild
```

*Dependencies*: `readline` headers/libs (e.g. `libreadline-dev` on Debian/Ubuntu).

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

The prompt reflects the exit status of the previous command (`$?`). Press `Ctrl‑D` to send EOF and exit gracefully.

---

## File Structure

```
libft/            → 42 libft (utility library)
inc/              → header files
  ├── builtins.h/
  ├── debug.h/
  ├── executor.h/
  ├── expansions.h/
  ├── heredoc.h/
  ├── minishell.h/
  ├── parser.h/
  ├── redirection.h/
  ├── signals.h/
  ├── string_utils.h/
  ├── tokenizer.h/
  └── types.h/
src/              → source files
  ├── executor/
  └── parser/
Makefile
main.c
STATUS.txt
readline.supp     → valgrind suppression file for readline
```

---

## Flow Diagram


[Chart will be uploaded here]

```

## Status
🔴 heredoc -> ctrl+c (signals suan duzeldi ama function not-allowed fix it)
🔴 after ctrl+c in heredoc the extra new line problem.
    -> [slack msg] can be solved by deploying another state for the global var whether we are in heredoc or not

✅ Test  62: ❌ cat <"./test_files/file name with spaces"
✅ Test  83: ❌ echo hi >         ./outfiles/outfile01 bye 
    -> **FIXED**: Replaced printf with write in builtin functions to avoid buffering issues with redirections
✅ Test  86: ❌ ls >"./outfiles/outfile with spaces"
✅ Test  87: ❌ ls >"./outfiles/outfile""1""2""3""4""5"
✅ Test 133: ❌ $PWD
✅ Test 134: ❌ $EMPTY
✅ Test 135: ❌ $EMPTY echo hi
✅ Test 136: ❌ ./test_files/invalid_permission 
✅ Test 137: ./missing.out
✅ Test 141: ❌ ./test_files
✅ Test 142: /test_files


🟡 $? values must be checked
🟡 detailed valgrind tests needed
🟡 debugger functions needs to be cleaned
🟡 divide into norm rules
✅ single quotes heredoc should not expand
✅ 

---

## License

This repository currently has **no explicit license file**. Until one is added, all rights remain with the original author(s). If you wish to reuse the code, please open an issue or contact the maintainer.

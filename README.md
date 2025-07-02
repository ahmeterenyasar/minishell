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


## Status
🔴 signal handling issues for heredoc.
🟡 comprehense pipe test -> 
    ✅ cat << eof | cat << asd | cat << qwe  
🟡 test: sleep with pipes
🟡 debugger functions and files needs to be cleaned     
🟡 divide into norm rules

✅ Fixed: Exit code handling in pipelines (refactored from special return codes to flag-based approach)
    ✅ minishell$ ls | exit 100 → $? = 100
    ✅ minishell$ ls | exit → $? = 0
    ✅ minishell$ invalid_cmd | exit 50 → $? = 50
    ✅ minishell$ exit 42 | ls → $? = 0
    ✅ Normal exit behavior preserved
    ✅ Exit with too many arguments handled correctly

🟡 minishell$ sleep 100 | ls
    docs  include  libft  main.c  Makefile	minishell  readline.supp  README.md  src
    ^C
    minishell$ echo $?
    130
    minishell$ echo $?
    0
minishell$ exit 100 | ls
docs  include  libft  main.c  Makefile	minishell  readline.supp  README.md  src
minishell$ echo $?
0
minishell$ 


 % echo $?                                                                                  
142
ayasar@k1m32s02 ~/Desktop/minishell
 % valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
==34871== Memcheck, a memory error detector
==34871== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==34871== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==34871== Command: ./minishell
==34871== 
minishell$ exit 3244444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444
exit
==34871== 
==34871== HEAP SUMMARY:
==34871==     in use at exit: 208,175 bytes in 222 blocks
==34871==   total heap usage: 515 allocs, 293 frees, 241,965 bytes allocated
==34871== 
==34871== LEAK SUMMARY:
==34871==    definitely lost: 0 bytes in 0 blocks
==34871==    indirectly lost: 0 bytes in 0 blocks
==34871==      possibly lost: 0 bytes in 0 blocks
==34871==    still reachable: 0 bytes in 0 blocks
==34871==         suppressed: 208,175 bytes in 222 blocks
==34871== 
==34871== For lists of detected and suppressed errors, rerun with: -s
==34871== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
ayasar@k1m32s02 ~/Desktop/minishell
 % echo $?                                                                                  
28




✅ some still reachables still exist e.g., echo test | asd | echo test
✅ ^C exit value must be 130 *check exit values
✅ echo "test" << eof > output.txt readline kaynaklı definitely lost ve indirectly lost veriyor
✅ cleanup_child_inherited_memory changed -> cmd var deleted - verified safe, cmd memory properly managed by callers
✅ echo "'$US'ER'"
✅ heredoc "" expansion fixed

✅ 
🟡 
🔴 

---

## License

This repository currently has **no explicit license file**. Until one is added, all rights remain with the original author(s). If you wish to reuse the code, please open an issue or contact the maintainer.

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
🔴 some still reachables still exist e.g., echo test | asd | echo test
🟡 debugger functions and files needs to be cleaned
🟡 divide into norm rules
🟡 cleanup_child_inherited_memory changed -> cmd var deleted but check if anything affected
🟡 echo "'$US'ER'"


🟡 echo "test" << eof > output.txt readline kaynaklı definitely lost ve indirectly lost veriyor
HEAP SUMMARY:
    in use at exit: 208,765 bytes in 234 blocks
  total heap usage: 790 allocs, 556 frees, 291,470 bytes allocated

4 bytes in 2 blocks are indirectly lost in loss record 1 of 73
   at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x48ACBAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A36D5: rl_copy_text (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A4775: rl_delete_text (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A9DA0: _rl_rubout_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488AE97: _rl_dispatch_subseq (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488BCC9: readline_internal_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x4894D14: readline (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x40139E: main (in /home/ayasar/Desktop/final_v_minishell/minishell)

32 bytes in 1 blocks are indirectly lost in loss record 17 of 73
   at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x48ACBAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A37C2: rl_add_undo (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A47D6: rl_delete_text (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A9DA0: _rl_rubout_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488AE97: _rl_dispatch_subseq (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488BCC9: readline_internal_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x4894D14: readline (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x40139E: main (in /home/ayasar/Desktop/final_v_minishell/minishell)

64 bytes in 2 blocks are indirectly lost in loss record 21 of 73
   at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x48ACBAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A37C2: rl_add_undo (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A3F4C: rl_insert_text (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A5411: _rl_insert_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48AA154: rl_insert (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488AE97: _rl_dispatch_subseq (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488BCC9: readline_internal_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x4894D14: readline (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x40139E: main (in /home/ayasar/Desktop/final_v_minishell/minishell)

132 (32 direct, 100 indirect) bytes in 1 blocks are definitely lost in loss record 27 of 73
   at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x48ACBAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A37C2: rl_add_undo (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A47D6: rl_delete_text (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x48A9DA0: _rl_rubout_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488AE97: _rl_dispatch_subseq (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x488BCC9: readline_internal_char (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x4894D14: readline (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
   by 0x40139E: main (in /home/ayasar/Desktop/final_v_minishell/minishell)

LEAK SUMMARY:
   definitely lost: 32 bytes in 1 blocks
   indirectly lost: 100 bytes in 5 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 0 bytes in 0 blocks
        suppressed: 208,633 bytes in 228 blocks


✅ 
🟡 
🔴 

---

## License

This repository currently has **no explicit license file**. Until one is added, all rights remain with the original author(s). If you wish to reuse the code, please open an issue or contact the maintainer.

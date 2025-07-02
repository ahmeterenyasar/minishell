#!/bin/zsh

echo "Testing exit codes in pipelines..."

echo -e "\nTest 1: ls | exit"
echo "ls | exit" | ./minishell
echo "Expected exit code: 0, Actual: $?"

echo -e "\nTest 2: ls | exit 100"
echo "ls | exit 100" | ./minishell
echo "Expected exit code: 100, Actual: $?"

echo -e "\nTest 3: invalid_command | exit"
echo "invalid_command | exit" | ./minishell
echo "Expected exit code: 0, Actual: $?"

echo -e "\nTest 4: invalid_command | exit 50"
echo "invalid_command | exit 50" | ./minishell
echo "Expected exit code: 50, Actual: $?"

echo -e "\nTest 5: exit 42 | ls"
echo "exit 42 | ls" | ./minishell
echo "Expected exit code: 0, Actual: $?"

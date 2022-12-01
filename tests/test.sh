#!/bin/bash
# test.sh - Simple testing script for projects
# Author: Joao Gouveia

# Defining colors
RES=$(tput sgr0)
RED=$(tput setaf 1)$(tput bold)
GREEN=$(tput setaf 2)$(tput bold)
FAILED="${RED}failed$RES"
PASSED="${GREEN}passed$RES"
ERROR="${RED}Erro: a flag '$1' nao existe.$RES"

# Show usage
help() {
	[ "$1" = "" ] || printf "\t%s\n" "$ERROR"
	printf "\t%s\n" "Flags:"
	printf "\t%s\n" "-b <Caminho-para-o-executavel>: Definir o executavel a testar, em caso de omicao da flag o default eh 'a.out'."
	printf "\t%s\n" "-t <Caminho-para-a-diretoria-dos-testes>: Definir os testes, em caso de omicao da flag o default eh 'public'."
	printf "\t%s\n" "-h: Mostrar utilizacao."
	exit 0
}

# File not found
fileError() {
	printf "%s\n" "${RED}Erro: $1 nao encontrado.$RES"
	exit 1
}

# Check for flags
while getopts :b:t:h FLAG; do
	case $FLAG in
		b) BIN="$OPTARG";;
		t) TESTDIR="$OPTARG";;
		h) help;;
		?) help "$OPTARG";;
	esac
done

# Default values if a flag is not provided
[ "$BIN" = "" ] && BIN="../bin/p1"
[ "$TESTDIR" = "" ] && TESTDIR="public_tests"

# Checking if files exist
{ test -e "$BIN" && test -f "$BIN"; } || fileError "$BIN"
{ test -e "$TESTDIR" && test -d "$TESTDIR"; } || fileError "$TESTDIR"

# Getting tests from the test directory
TESTS=$(ls "$TESTDIR"/*.in)

# Creating MyOutput directory if it doesn't exist
{ test -e MyOutput && test -d MyOutput; } || mkdir MyOutput

# Executing all tests
for TEST in $TESTS; do
	TESTNAME=${TEST%.in}
	TESTNUM="${TESTNAME#"$TESTDIR"/}"
	MYOUT=MyOutput/"${TESTNUM}".myout
	./"$BIN" < "$TEST" > "$MYOUT"
	{ diff "$MYOUT" "${TESTNAME}".out > /dev/null && echo "$TESTNUM $PASSED"; } || echo "$TESTNUM $FAILED"
done

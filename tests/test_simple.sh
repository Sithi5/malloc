#!/bin/sh

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_CYAN=`tput setaf 3`
_END=`tput sgr0`


# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"

BASE_FILE_NAME=$(basename "$0" .sh)
DEFAULT_C_FILE_NAME=${BASE_FILE_NAME}_default
CUSTOM_C_FILE_NAME=${BASE_FILE_NAME}_custom

# Compile the test program
gcc -Wall -Wextra -Werror ./tests/$DEFAULT_C_FILE_NAME.c -o ./tests/$DEFAULT_C_FILE_NAME
gcc -Wall -Wextra -Werror -I ./include -L . -lft_malloc_$HOSTTYPE ./tests/$CUSTOM_C_FILE_NAME.c -o ./tests/$CUSTOM_C_FILE_NAME

# Run the test program
./tests/$DEFAULT_C_FILE_NAME > ./tests/$DEFAULT_C_FILE_NAME.log
./tests/$CUSTOM_C_FILE_NAME > ./tests/$CUSTOM_C_FILE_NAME.log

diff ./tests/$DEFAULT_C_FILE_NAME.log ./tests/$CUSTOM_C_FILE_NAME.log

rm tests/$DEFAULT_C_FILE_NAME
rm tests/$CUSTOM_C_FILE_NAME
rm tests/$DEFAULT_C_FILE_NAME.log
rm tests/$CUSTOM_C_FILE_NAME.log
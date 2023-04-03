#!/bin/sh

ITERATIONS=100
TEST_MALLOC=1
TEST_FREE=1
TEST_FREE_QUALITY=1
TEST_REALLOC=1
TEST_ERROR=1
TEST_SHOW_ALLOC_MEMORY=1
TEST_CALLOC=1
TEST_SHOW_ZONE_MEMORY_USAGE=1
SUMMARY=1
DISPLAY_ERRORS=0

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_RED=`tput setaf 1`
_CYAN=`tput setaf 3`
_END=`tput sgr0`


# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"

malloc()
{
	echo "\n${_GREEN}Test my Malloc:${_END}\n"
	valid_my_malloc=0
	i=0
	while [ $i -lt $ITERATIONS ]
	do
		test0=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test0 2>&1 | grep "Maximum" | awk '{print $NF}')`
		test1=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test1 2>&1 | grep "Maximum" | awk '{print $NF}')`
		ret=`echo "$test1 - $test0" | bc`
		if [ $ret -gt 254 -a $ret -lt 273 ]
		then
			valid_my_malloc=$(($valid_my_malloc + 1))
		else
			if [ $DISPLAY_ERRORS -eq 1 ]
			then
				echo "\n$ret"
			fi
		fi
		printf "\rValue:\t$ret\tValid : $valid_my_malloc/$ITERATIONS"
		i=$(($i + 1))
	done
	echo "\n$valid_my_malloc/$ITERATIONS passed the condition"

	echo "\n${_GREEN}Test real Malloc:${_END}\n"
	i=0
	valid_real_malloc=0
	while [ $i -lt $ITERATIONS ]
	do
		test0=`./test_get_pages_used $(/usr/bin/time -v ./test0 2>&1 | grep "Maximum" | awk '{print $NF}')`
		test1=`./test_get_pages_used $(/usr/bin/time -v ./test1 2>&1 | grep "Maximum" | awk '{print $NF}')`
		ret=`echo "$test1 - $test0" | bc`
		if [ $ret -gt 254 -a $ret -lt 273 ]
		then
			valid_real_malloc=$(($valid_real_malloc + 1))
		else
			if [ $DISPLAY_ERRORS -eq 1 ]
			then
				echo "\n$ret"
			fi
		fi
		printf "\rValue:\t$ret\tvalid : $valid_real_malloc/$ITERATIONS"
		i=$(($i + 1))
	done
	echo "\n$valid_real_malloc/$ITERATIONS passed the condition"

	echo "\n${_GREEN}Comparing both Malloc results:${_END}\n"
	echo "\n$valid_my_malloc/$ITERATIONS vs $valid_real_malloc/$ITERATIONS"
}

free()
{
	echo "\n${_GREEN}Test my Free:${_END}\n"
	valid_my_free=0
	i=0
	while [ $i -lt $ITERATIONS ]
	do
		test1=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test1 2>&1 | grep "Maximum" | awk '{print $NF}')`
		test2=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test2 2>&1 | grep "Maximum" | awk '{print $NF}')`
		ret=`echo "$test1 - $test2" | bc`
		if [ $ret -ge 0 ]
		then
			valid_my_free=$(($valid_my_free + 1))
		else
			if [ $DISPLAY_ERRORS -eq 1 ]
			then
				echo "\n$ret"
			fi
		fi
		printf "\rValue:\tTest1:\t$test1\tTest2:\t$test2\tValid : $valid_my_free/$ITERATIONS"
		i=$(($i + 1))
	done
	echo "\n$valid_my_free/$ITERATIONS passed the condition"
}

free_quality()
{
	echo "\n${_GREEN}Test my Free quality:${_END}\n"
	i=0
	valid_my_free_quality=0
	while [ $i -lt $ITERATIONS ]
	do
		test0=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test0 2>&1 | grep "Maximum" | awk '{print $NF}')`
		test2=`./test_get_pages_used $(./run_linux.sh /usr/bin/time -v ./test2 2>&1 | grep "Maximum" | awk '{print $NF}')`
		ret=`echo "$test2 - $test0" | bc`
		if [ $ret -ge 0 -a $ret -le 2 ]
		then
			valid_my_free_quality=$(($valid_my_free_quality + 1))
		else
			if [ $DISPLAY_ERRORS -eq 1 ]
			then
				echo "\n$ret"
			fi
		fi
		printf "\rValue:\tTest2:\t$test2\tTest0:\t$test0\tValid : $valid_my_free_quality/$ITERATIONS"
		i=$(($i + 1))
	done

	echo "\n${_GREEN}Test real Free quality:${_END}\n"
	i=0
	valid_real_free_quality=0
	while [ $i -lt $ITERATIONS ]
	do
		test0=`./test_get_pages_used $(/usr/bin/time -v ./test0 2>&1 | grep "Maximum" | awk '{print $NF}')`
		test2=`./test_get_pages_used $(/usr/bin/time -v ./test2 2>&1 | grep "Maximum" | awk '{print $NF}')`
		ret=`echo "$test2 - $test0" | bc`
		if [ $ret -ge 0 -a $ret -le 2 ]
		then
			valid_real_free_quality=$(($valid_real_free_quality + 1))
		fi
		printf "\rValue:\tTest2:\t$test2\tTest0:\t$test0\tValid : $valid_real_free_quality/$ITERATIONS"
		i=$(($i + 1))
	done

	echo "\n${_GREEN}Comparing both Free results:${_END}\n"
	echo "\n$valid_my_free_quality/$ITERATIONS vs $valid_real_free_quality/$ITERATIONS"
}

realloc()
{
	echo "\n${_GREEN}Test Realloc:${_END}\n"
	./run_linux.sh ./test3 > realloc_test
	test3=`cat -e realloc_test`
	printf "Bonjour\nBonjour\n" > real_realloc_test
	result=`cat -e real_realloc_test`
	if [ "$test3" = "$result" ]
	then
		echo "\n${_GREEN}[OK]\n${_END}"
	else
		echo "\n${_RED}[KO]\n${_END}"
	fi
	rm realloc_test real_realloc_test
}

realloc_more()
{
	echo "\n${_GREEN}Test Realloc 2:${_END}\n"
	./run_linux.sh ./test4 > realloc_more_test
	test4=`cat -e realloc_more_test`
	printf "Bonjour\nBonjour\n" > real_realloc_more_test
	result=`cat -e real_realloc_more_test`
	if [ "$test4" = "$result" ]
	then
		echo "\n${_GREEN}[OK]\n${_END}"
	else
		echo "\n${_RED}[KO]\n${_END}"
	fi
	rm realloc_more_test real_realloc_more_test
}

error()
{
	echo "\n${_GREEN}Test Error management 2:${_END}\n"
	./run_linux.sh ./test5 > error_management_test
	test5=`cat -e error_management_test`
	printf "Bonjour\n" > real_error_management_test
	result=`cat -e real_error_management_test`
	if [ "$test5" = "$result" ]
	then
		echo "\n${_GREEN}[OK]\n${_END}"
	else
		echo "\n${_RED}[KO]\n${_END}"
	fi
	rm error_management_test real_error_management_test
}

show_alloc_mem()
{
	echo "\n${_GREEN}Test show alloc memory 2:${_END}\n"
	./run_linux.sh ./test6
	echo "\nWith Tiny displayed\n"
	./run_linux.sh ./test6_bis
}

calloc()
{
	echo "\n${_GREEN}Test Calloc:${_END}\n"
	test_calloc=`./run_linux.sh ./test_calloc`
	result=`./test_calloc`
	if [ "$test_calloc" = "$result" ]
	then
		echo "\n${_GREEN}[OK]\n${_END}"
	else
		echo "\n${_RED}[KO]\n${_END}"
	fi
}

test_show_zone_memory_usage()
{
	echo "\n${_GREEN}test_show_zone_memory_usage:${_END}\n"
	./run_linux.sh ./test_show_zone_memory_usage
}

result()
{
	echo "\n${_GREEN}RESULTS SUMMARY:${_END}\n"
	echo "${_GREEN}Comparing both Malloc results:${_END}\n"
	echo "$valid_my_malloc/$ITERATIONS vs $valid_real_malloc/$ITERATIONS"
	echo "${_GREEN}Comparing both Free Quality results:${_END}"
	echo "\n$valid_my_free_quality/$ITERATIONS vs $valid_real_free_quality/$ITERATIONS"
}

if [ $TEST_MALLOC -eq 1 ]
then
	malloc
fi

if [ $TEST_FREE -eq 1 ]
then
	free
fi

if [ $TEST_FREE_QUALITY -eq 1 ]
then
	free_quality
fi



if [ $TEST_REALLOC -eq 1 ]
then
	realloc
	realloc_more
fi

if [ $TEST_ERROR -eq 1 ]
then
	error
fi

if [ $TEST_SHOW_ALLOC_MEMORY -eq 1 ]
then
	show_alloc_mem
fi

if [ $TEST_CALLOC -eq 1 ]
then
	calloc
fi

if [ $TEST_SHOW_ZONE_MEMORY_USAGE -eq 1 ]
then
	test_show_zone_memory_usage
fi

if [ $SUMMARY -eq 1 ]
then
	result
fi

echo
echo "That's all folks!"
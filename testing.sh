#!/bin/bash

DIRGOOD="./test/good/"
DIRBAD="./test/syn-err/"
PROG="./bin/tpcas"
RAPPORT="./rapport/testing.txt"
TOTAL_TEST_GOOD=0
TOTAL_TEST_BAD=0
TEST_COUNTER=1

rm rapport/testing.txt
> testing.txt


TOTAL_FILES_GOOD=$(find "$DIRGOOD" -maxdepth 1 -type f | wc -l)

for file in "$DIRGOOD"*; do
    if [ -f "$file" ]; then
        "$PROG" < "$file"
        ret=$?
        
        if [ $ret -eq 0 ]; then
            echo "$TEST_COUNTER : Passed ($file)" >> $RAPPORT
            TOTAL_TEST_GOOD=$((TOTAL_TEST_GOOD + 1))
        else
            echo "$TEST_COUNTER : Failed. Expected 0, found $ret ($file)" >> $RAPPORT
        fi

        TEST_COUNTER=$((TEST_COUNTER + 1))
    fi
done

echo "Total passed good tests: $TOTAL_TEST_GOOD / $TOTAL_FILES_GOOD" >> $RAPPORT

TEST_COUNTER=1
TOTAL_FILES_BAD=$(find "$DIRBAD" -maxdepth 1 -type f | wc -l)

for file in "$DIRBAD"*; do
    if [ -f "$file" ]; then
        "$PROG" < "$file"
        ret=$?
        
        if [ $ret -eq 1 ]; then
            echo "$TEST_COUNTER : Passed ($file)" >> $RAPPORT
            TOTAL_TEST_BAD=$((TOTAL_TEST_BAD + 1))
        else
            echo "$TEST_COUNTER : Failed. Expected 1, found $ret ($file)" >> $RAPPORT
        fi

        TEST_COUNTER=$((TEST_COUNTER + 1))
    fi
done

echo "Total passed bad tests: $TOTAL_TEST_BAD / $TOTAL_FILES_BAD" >> $RAPPORT

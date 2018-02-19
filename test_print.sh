#!/bin/bash

if which dos2unix ; then
	DOCS2UNIX="docs2unix"
elif which fromdos ; then
	DOCS2UNIX="fromdos"
else
	DOCS2UNIX="sed -e s/\x0D//g -"
fi

echo "===================================="
echo " Cleaning temporaries and outputs"
make clean
echo " Force building bin/print"
make all -B
if [[ "$?" -ne 0 ]]; then
	echo "Build failed.";
fi
echo""
mkdir -p test/out

echo "===================================="

PASSED=0
CHECKED=0

for i in test/in/*.txt; do
	echo "=================================="
	echo "Input file : ${i}"
	BASENAME=$(basename $i .txt)
	cat $i | ${DOCS2UNIX} | ./bin/print > test/out/$BASENAME.stdout.txt 2> test/out/$BASENAME.stderr.txt
	diff <(cat test/ref/$BASENAME.stdout.txt | ${DOCS2UNIX}) <(cat test/out/$BASENAME.stdout.txt) > test/out/$BASENAME.diff.txt
	if [[ "$?" -ne "0" ]]; then
		echo -e "\nERROR"
	else
		PASSED=$(( ${PASSED}+1 ));
	fi
	CHECKED=$(( ${CHECKED}+1 ));
done

echo "###############################################"
echo "Passed ${PASSED} out of ${CHECKED}".
echo ""


RELEASE=$(lsb_release -d)
if [[ $? -ne 0 ]]; then
    echo ""
    echo "Warning: This appears not to be a Linux environment"
    echo "         Make sure you do a final run on a lab machine or an Ubuntu VM"
else
    grep -q "Ubuntu 16.04" <(echo $RELEASE)
    FOUND=$?

    if [[ $FOUND -ne 0 ]]; then
        echo ""
        echo "Warning: This appears not to be the target environment"
        echo "         Make sure you do a final run on a lab machine or an Ubuntu VM"
    fi
fi
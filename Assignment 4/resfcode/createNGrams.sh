#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Invalid arguments"
	echo "Usage: $0 <mail-to-check>"
	exit 1
else
	TOCHECK=$1
fi

# Create log file
exec 1>`date +'%Y%m%d%H%M%S'`.log 2>&1

# 1. Split i/p file so that files for different grams are created starting from 2Grams upto max no of grams i/p by user
# 2. Once split is successful, sort the splitted file
# 3. Once sort is successful, create grams from the sorted file
getProb() {
	for i in `find . -name "*" -type f`
	do
		echo "Processing $i ..."

		../fileSplitter $i 1
		if [ $? -ne 0 ]
		then
			echo "Some error occured during file splitting for 1grams"
			exit 1
		fi
	done

	echo ""

	rm -f ../$mrgFile

	for curFileToMrg in `find . -name "*.1" -type f`
	do
		cat $curFileToMrg >> ../$mrgFile
		echo "" >> ../$mrgFile
	done

	cd ..

	# This check is really important
	err=`cat $mrgFile | awk '{ print NF }' | sort -u | wc -l`
	if [ $err -gt 1 ]
	then
		echo "Number of fields in file $mrgFile are more than 1"
		exit 1
	fi

	# Sort the splitted file
	rm -f $mrgFile.sort
	sort $mrgFile >> $mrgFile.sort

	# Create grams for the sorted file
	uniq -c $mrgFile.sort >> $mrgFile.sort.2

	# Count total no of words
	totCnt=`awk 'BEGIN{i=0} {i+=$1} END{print i}' $mrgFile.sort.2`

	rm -f $mrgFile.sort.prob
	cat $mrgFile.sort.2 | awk -v den=$totCnt '{ printf( "%s %f\n", $2, $1/den ) }' | sort -r -k 2 >> $mrgFile.sort.prob.$1
	if [ $? -eq 0 ]
	then
		echo "Training data generated successfully"
		echo "Total count of words :: "$totCnt
		echo ""
	fi

	# Remove intermediate files created for nGrams
	rm -f $mrgFile
	rm -f $mrgFile.sort
	rm -f $mrgFile.sort.2
}

mrgFile=1.txt

if [ ! -f $mrgFile.sort.prob.N ]
then
	echo "Probability file(for NotSpam) does not exists. Calculating Probability from the training data ..."
	cd ./NotSpam/
	getProb N
else
	echo "Probability file(for NotSpam) already exists."
fi

if [ ! -f $mrgFile.sort.prob.P ]
then
	echo "Probability file(for Spam) does not exists. Calculating Probability from the training data ..."
	cd ./Spam/
	getProb P
else
	echo "Probability file(for Spam) already exists. Checking if the I/P is SPAM ..."
fi

./fileSplitter $TOCHECK 1
if [ $? -ne 0 ]
then
	echo "Some error occured during file splitting for 1grams"
	exit 1
fi

./checkForSpam $mrgFile.sort.prob.N $mrgFile.sort.prob.P $TOCHECK.1
if [ $? -ne 0 ]
then
	echo "Some error occured while checking for SPAM ..."
	exit 1
else
	rm -f $TOCHECK.1
fi

#!/bin/bash

read S1 S2 S3

if [ "${#S1}" -ge "${#S2}" ] && [ "${#S1}" -ge "${#S3}" ] 
then
	echo "$S1"
elif [ "${#S2}" -ge "${#S1}" ] && [ "${#S2}" -ge "${#S3}" ] 
then
	echo "$S2"
elif [ "${#S3}" -ge "${#S2}" ] && [ "${#S3}" -ge "${#S1}" ] 
then
	echo "$S3"
fi
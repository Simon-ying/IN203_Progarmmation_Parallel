#!/bin/bash
for ((i=1;i<=16;i++));
do
	mpiexec -np $i --oversubscribe ../HelloWorld.exe;
done

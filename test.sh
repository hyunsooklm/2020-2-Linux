#! /bin/sh

myFunction (){
if [ $1 -lt 1 -o $2 -lt 1 ];
then
	echo "less than 1"
	exit -1
else
	for row in $(seq 1 $1)
do
	for col in $(seq 1 $2)
	do
 echo "$row * $col =" `expr $row \* $col` 
done
done
fi
}

echo $1 $2
myFunction $1 $2
exit 0

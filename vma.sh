PID=$1
file=$2
mkdir $file
var=1
lastsize=$(grep VmSize /proc/$PID/status |grep -o -E '[0-9]+')

printf -v fname '%s/vma_%d.txt' "$file" "$var"

sudo date +"%T.%N" > $fname
sudo cat /proc/$PID/maps >> $fname
sudo grep VmPeak /proc/$PID/status >> $fname && grep VmSize /proc/$PID/status >> $fname

while 
		  sleep 0.0000001;
do
		  if ps -p $PID > /dev/null
		  then
					 curvsize=$(grep VmSize /proc/$PID/status |grep -o -E '[0-9]+')
					 if [ $curvsize -ne $lastsize ]
					 then
								printf -v fname '%s/vma_%d.txt' "$file" "$(( var++ ))"
								sudo date +"%T.%N" > $fname
								sudo cat /proc/$PID/maps >> $fname
								sudo grep VmPeak /proc/$PID/status >> $fname && grep VmSize /proc/$PID/status >> $fname
								lastsize=$curvsize
					 fi
		  else
					 exit 0
		  fi
done

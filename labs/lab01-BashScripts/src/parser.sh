#!/bin/bash

check_arguments () {

#Check user passed a parameter
#TODO: Implement me

#Check if parameter is a directory
#TODO: Implement me

#Check if directory is empty
#TODO: Implement me

 }	

average_time () {
  
  echo "Average Response Time(us)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/mcperf"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | grep -A1 "avg     std" | grep -v "avg" | grep read | awk '{print $2}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

#TODO: Implement me
99th_time () {
  

}	

#TODO: Implement me
target_qps () {
  
  
}	

#TODO: Implement me
power_pkg0 () {
  
  
}	

parse_data () {
  
  check_arguments "$1"
  average_time "$1"
  99th_time "$1"
  target_qps "$1"
  power_pkg0 "$1"
  
}

"$@"
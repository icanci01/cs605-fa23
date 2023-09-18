#!/bin/bash

check_arguments () {

#Check user passed a parameter
  if [[ -z $1 ]]; then
    echo "Missing argument!"
    exit 
  fi

Check if parameter is a directory
  if [[ ! -d $1 ]]; then
    echo "Argument not a directory!"
    exit 
  fi

#Check if directory is empty
  if [[ -z "$(ls -A $1)" ]]; then
    echo "Directory empty!"
    exit 
  fi
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

99th_time () {
  
  echo "99th Tail Latency(us)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/mcperf"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | grep -A1 "avg     std" | grep -v "avg" | grep read | awk '{print $14}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

target_qps () {
  
  echo "Difference from Target QPS"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/mcperf"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | grep "Total QPS" | awk -F"=" '{print $2}' | awk '{print 10000-$1}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

power_pkg0 () {
  
  echo "Package 0 Power Consumption(W)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/memcached/package-0"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | awk -F',' 'BEGIN{p_time=0; p_power=0; c_time=0; c_power=0;}{p_power=c_power; p_time=c_time; c_power=$2; c_time=$1; if (NR % 3 == 0)print (c_power-p_power)/(c_time-p_time)/1000000}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

power_pkg1 () {
  
  echo "Package 1 Power Consumption(W)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/memcached/package-1"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | awk -F',' 'BEGIN{p_time=0; p_power=0; c_time=0; c_power=0;}{p_power=c_power; p_time=c_time; c_power=$2; c_time=$1; if (NR % 3 == 0)print (c_power-p_power)/(c_time-p_time)/1000000}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

power_dram0 () {
  
  echo "DRAM 0 Power Consumption(W)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/memcached/dram-0"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | awk -F',' 'BEGIN{p_time=0; p_power=0; c_time=0; c_power=0;}{p_power=c_power; p_time=c_time; c_power=$2; c_time=$1; if (NR % 3 == 0)print (c_power-p_power)/(c_time-p_time)/1000000}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

power_dram1 () {
  
  echo "DRAM 1 Power Consumption(W)"
  declare -A results
  for d in `ls $1`;
  do 
    directory=$1"/"$d"/memcached/dram-1"
    key=`echo "$d" | awk -F"qps=" '{print $2}' | awk -F"-" '{print $1}'` 
    value=`cat $directory | awk -F',' 'BEGIN{p_time=0; p_power=0; c_time=0; c_power=0;}{p_power=c_power; p_time=c_time; c_power=$2; c_time=$1; if (NR % 3 == 0)print (c_power-p_power)/(c_time-p_time)/1000000}'`
    results["$key"]=${results["$key"]}$value","
  done

  for i in "${!results[@]}"
  do
    value=`echo ${results[$i]} | tr "," " "`
    echo "$i $value"
  done
}	

parse_data () {
  echo "irtaaaaaaaaaaa"
  check_arguments "$1"
  average_time "$1"
  99th_time "$1"
  target_qps "$1"
  power_pkg0 "$1"
  power_pkg1 "$1"
  power_dram0 "$1"
  power_dram1 "$1"

}

"$@"
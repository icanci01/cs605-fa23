# Lab 6: Power
The goal of this lab tutorial is to familiarize with different aspects of power consumption and computer feautures that aim to save power during active/idle times.  


## Types of Power Consumption
We can divide power consumption for CMOS into the following categories:
*	Dynamic Power Consumption
*	Static Power Consumption

## Dynamic Power Consumption
Dynamic power consumption in CMOS (Complementary Metal-Oxide-Semiconductor) circuits refers to the power that is consumed when transistors switch between their logical states (from 0 to 1 or vice versa). It is one of the two main components of power consumption in CMOS circuits, with the other being static power consumption, which occurs even when the transistors are not actively switching.

* `C(load capacitance):` It depends on wire lengths and on-chip structures.
* `V(supplied voltage):` Main way to reduce power consumption due to the direct quadratic influence
* `A(switching factor):` A number between 0-1 that represents how often wires switch between 0 and 1.
* `F(frequency):` Clock Frequency. Affects directly power dissipation.  It also indirectly affects supplied voltage.

## Static Power Consumption
Static power consumption in CMOS (Complementary Metal-Oxide-Semiconductor) circuits refers to the power that is consumed even when the transistors in the circuit are not actively switching or performing any computation. It is one of the two main components of power consumption in CMOS circuits, with the other being dynamic power consumption, which occurs when transistors switch between their logical states.

`Leakage Current:` Static power consumption is primarily caused by leakage current, which is the small amount of electric current that flows through a transistor even when it is in the off state (not actively switching). 
* `Subthreshold Leakage:` This occurs when transistors are in the off state, and there is a small, but continuous, current flow due to electrons tunneling through the transistor's gate oxide.


## Techniques for Power Effciency

* `Clock Gating:` Technique for reducing dynamic power dissipation. Reduces the Activity factor (from the “equation of dynamic power dissipation”). It “gates” the clock control signal to prevent its toggling within a particular sub-unit. (Switches off a particular cirquit - unit)

* `Dynamic Voltage and Frequency Scaling:` Power Management Technique. Dynamically adjust Frequency and Voltage. Effective when dynamic Power is Dominant. It can address other problems such as temperature, reliability and variability. Trades off performance with energy efficiency. 

## Sleep States

Processor Idle Sleep States. Enable System to Reduce Power Consumption during idle periods. Different Systems have different Sleep states. Enable/Disable through BIOS and OS. Sleep States use techniques like clock gating and power gating to eliminate the power consumption.

### Types of Sleep States

* Core C-States: 
    *	Core + L1/L2 Cache
    * 	Skylake: C0, C1, C1E, C6
*	Package C-States: 
    * Uncore (Memory Controllers + UPI + PCI + LLC etc) + DRAM
	* Skylake: PC0, PC2, PC6
*	Link Sleep States: 
    *	UPI, PCIE
    *	L0, L0s, L0p, L1
*	Dram Power States:
    *	Self Refresh
    *	CKE_OFF


### Core C-States
Enable core to reduce power consumption during idle periods. Higher C-States means higher number of disabled functions and so higher power savings. During transition, the CPU cannot execute any instructions (performance). Tools to Monitor C-states residency, transitions: turbostat, residency counters, SoCwatch. Skylake supports 4 C-States:
*   C0: Active 
*   C1: Shallow Sleep State
*   C1E: Medium Sleep State
*   C6: Deep Sleep State

## CPUFreq Subsystem: 
Linux Kernel Subsystem Responsible for CPU performance scaling. Components include:
*	`Core`
*	`Scaling Governor:` Responsible to decide the frequency of the core dynamically based on a policy specified by the user. Based on the scaling driver the system supports different governors such as performance, powersave, ondemand etc.
*	`Scaling Driver:` It is responsible to communicate the frequency decided by the governor to the hardware. Scaling governor include intel_pstate and acpi_cpufreq.

The scaling governor can be changed at runtime using the cpupower command, but it requires sudo permissions. 
```
sudo cpupower frequency-info # prints details about the scaling governor and scaling driver
sudo cpupower frequency-set -g performance # changes the scaling governor to performance 
sudo cpupower frequency-set -d 1200MHz # sets the minimum frequency limit to 1.2GHz
sudo cpupower frequency-set -u 1200MHz # sets the maximum frequency limit to 1.2GHz
sudo cpupower frequency-set -f 1200MHz # sets the frequency to 1.2GHz
```

Usually the default scaling driver of the system is intel_pstate. In case you want to change it to acpi_cpufreq you have to modify the grub file under the /etc/default/ directory and include the flag intel_pstate=disable in order to disable intel_pstate. Then you have to perform something similar to sudo update-grub and restart the computer in order for the changes to take place. 

### P-States
P-States are Performance States the core resides in while in C0 Core C-State. P-States represent a set of operating points or pairs of frequency-voltage. In order for the OS to change frequency, it requests specific P-States based on the characteristics of the current workload and the scaling governor. Each system supports a different set of P-States. P1 represents the nominal settings of the cpu. Pn is the minimum supported operating frequency-voltage. In P0 turbo is enabled. The system support different P-States for turbo.  

## Exercises

Under the src directory there is a power virus written in assembly. To compile use the following:

```
gcc power_virus.s -o power virus
```
Answer the following questions. Questions 1 and 2 can be tested on the lab machines, question 3 is based on the dynamic power consumption equation. 

1)  What is the impact of time on power consumption? Run the power virus for 5,10,15 and 20 seconds. Measure the power consumption using the following:

```
taskset -c 0 ./power_virus &
perf stat -e power/energy-cores/ sleep 5
perf stat -e power/energy-cores/ sleep 10
perf stat -e power/energy-cores/ sleep 15
perf stat -e power/energy-cores/ sleep 20
```

2) What is the impact of the amount of work on power consumption? Run the power virus for 1,2,3 and 4 instances. Measure the power consumption using the following:

```
taskset -c 0 ./power_virus &
perf stat -e power/energy-cores/ sleep 10
taskset -c 1 ./power_virus &
perf stat -e power/energy-cores/ sleep 10
taskset -c 2 ./power_virus &
perf stat -e power/energy-cores/ sleep 10
taskset -c 3 ./power_virus &
perf stat -e power/energy-cores/ sleep 10
```

3) What is the impact of frequency on the power consumption? For example if we reduce the frequency of the core in steps of 100MHz what would be the graph of frequency vs power consumption?
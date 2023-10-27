/*
 * Copyright 2002-2019 Intel Corporation.
 * 
 * This software is provided to you as Sample Source Code as defined in the accompanying
 * End User License Agreement for the Intel(R) Software Development Products ("Agreement")
 * section 1.L.
 * 
 * This software and the related documents are provided as is, with no express or implied
 * warranties, other than those that are expressly stated in the License.
 */

/*
 *  This file contains an ISA-portable PIN tool for tracing memory accesses.
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "pin.H"
using namespace std;

FILE * trace;
typedef unsigned int UINT32;
typedef unsigned long long UINTT64;
UINTT64 inscount=0;
typedef struct {
	bool valid;
	bool dirty;
	UINTT64 tag;
} SLOT;

UINT32 getBits(UINT32 n) {
	UINT32 bits = 0;
	
	while(n > 1) {
		n = n >> 1;
		bits++;
	}
	
	return bits;
}

class DirectMappedCache {
	private:
		UINT32 sets;
		UINT32 offsetBits;
		UINT32 indexBits;
		UINT32 indexMask;
		SLOT *contents;
		
	public:
		UINTT64 accesses;
		UINTT64 misses;
		UINTT64 hits;
		UINTT64 hits_clean;
		UINTT64 hits_dirty;
		UINTT64 replacements;
		UINTT64 replacements_clean;
		UINTT64 replacements_dirty;
		UINTT64 writebacks;
		
		DirectMappedCache(UINT32 size, UINT32 blockSize) {
			accesses = 0;
			misses = 0;
			hits = 0;
			hits_clean = 0;
			hits_dirty = 0;
			replacements = 0;
			replacements_clean = 0;
			replacements_dirty = 0;
			writebacks = 0;
			sets = size / blockSize;
			offsetBits = getBits(blockSize);
			indexBits = getBits(sets);
			indexMask = sets - 1; // e.g. sets = 64, mask = 63 = 111111
			contents = (SLOT *) calloc(sets, sizeof(SLOT)); // set all slots to 0
		}
		
		void read(UINTT64);
		void write(UINTT64);
		void stats();
		void statsPK(UINTT64);
		void displayContents();
};

void DirectMappedCache::read(UINTT64 address) {
	address >>= offsetBits; // remove offset bits
	UINT32 index = address & indexMask; // get index
	UINTT64 tag = address >> indexBits; 	// get tag
	accesses++; // cache is accessed
	
	if(contents[index].valid) { // if valid 
		if(contents[index].tag == tag) { // if tag is the same, hit
			hits++;
			if (contents[index].dirty)
				hits_dirty++;
			else
				hits_clean++;
		}
		else { // else miss and need to replace
			misses++;
			replacements++;
			
			if(contents[index].dirty) { // if dirty need to writeback
				writebacks++;
				replacements_dirty++;
				contents[index].dirty = 0;
			}
			else {
				replacements_clean++;
			}
			
			contents[index].tag = tag; // replace tag
		}
	}
	else { // else compulsory miss
		misses++;
		contents[index].valid = 1;
		contents[index].tag = tag;
		contents[index].dirty = 1;
	}
}

void DirectMappedCache::write(UINTT64 address) {
	address >>= offsetBits; // remove offset bits
	UINT32 index = address & indexMask; // get index
	UINTT64 tag = address >> indexBits; 	// get tag
	accesses++; // cache is accessed
	
	if(contents[index].valid) { // if valid 
		if(contents[index].tag == tag) { // if tag is the same, hit
			hits++;
			if (contents[index].dirty)
				hits_dirty++;
			else
				hits_clean++;
			contents[index].dirty = 1; // write
		}
		else { // else miss and need to replace
			misses++;
			replacements++;

			if(contents[index].dirty) { // if dirty need to writeback
				writebacks++;
				replacements_dirty++;
			}
			else {
				replacements_clean++;
			}
			
			contents[index].tag = tag; // replace tag
		}
	}
	else { // else compulsory miss
		misses++;
		contents[index].valid = 1;
		contents[index].dirty = 1; // write
		contents[index].tag = tag;
	}
}

void DirectMappedCache::stats() {
	cout << "Total Cache Accesses: " << accesses << endl;
	cout << "Total Number of Cache Misses: " << misses << endl;
	cout << "Total Number of Cache Hits: " << hits << endl;
	cout << "Total Number of Cache Hits_clean: " << hits_clean << endl;
	cout << "Total Number of Cache Hits_dirty: " << hits_dirty << endl;
	cout << "Total Number of replacements: " << replacements << endl;
	cout << "Total Number of replacements_clean: " << replacements_clean << endl;
	cout << "Total Number of replacements_dirty: " << replacements_dirty << endl;
	cout << "Total Number of writebacks: " << writebacks << endl;
	UINT32 dirty = 0;
	for(UINT32 i = 0; i < sets; i++) { // calculate total dirty blocks 
		if(contents[i].dirty) {
			dirty++;
		}
	}
	cout << "Number of dirty blocks: " << dirty << endl;
	
}

void DirectMappedCache::statsPK(UINTT64 instructions) {
	cout << "Accesses per kilo Instructions (APKI): " << (1000.0 * accesses) / instructions << endl;
	cout << "Misses per kilo Instructions (APKI): " << (1000.0 * misses) / instructions << endl;
	cout << "Hits per kilo Instructions (APKI): " << (1000.0 * hits) / instructions << endl;
	cout << "Replacements per kilo Instructions (APKI): " << (1000.0 * replacements) / instructions << endl;
	cout << "Writebacks per kilo Instructions (APKI): " << (1000.0 * writebacks) / instructions << endl;
}

void DirectMappedCache::displayContents() {
	for(UINT32 i = 0; i < sets; i++) {
		cout << i << " " << contents[i].valid << ", " << contents[i].dirty << ", " << hex << contents[i].tag << dec << endl;
	}
}

class SetAssociativeCache {
	private:
		UINT32 sets;
		UINT32 ways;
		UINT32 offsetBits;
		UINT32 indexBits;
		UINT32 indexMask;
		SLOT **contents;

		
		
	public:
		UINTT64 accesses;
		UINTT64 misses;
		UINTT64 hits;
		UINTT64 hits_clean;
		UINTT64 hits_dirty;
		UINTT64 replacements;
		UINTT64 replacements_clean;
		UINTT64 replacements_dirty;
		UINTT64 writebacks;
		//UINTT64* lru_hits;
        int* lru_hits;


		SetAssociativeCache(UINT32 size, UINT32 blockSize, UINT32 ways) {
			accesses = 0;
			misses = 0;
			hits = 0;
			hits_clean = 0;
			hits_dirty = 0;
			replacements = 0;
			replacements_clean = 0;
			replacements_dirty = 0;
			writebacks = 0;
			this->ways = ways;
			sets = size / (ways * blockSize);
			offsetBits = getBits(blockSize);
			indexBits = getBits(sets);
			indexMask = sets - 1; // e.g. sets = 64, mask = 63 = 111111
			lru_hits=new int[ways];
			contents = (SLOT **) malloc(ways * sizeof(SLOT *));
			for(UINT32 i = 0; i < ways; i++) {
				contents[i] = (SLOT *) calloc(sets, sizeof(SLOT)); // set all slots to 0
			}
		}
		
		void read(UINTT64);
		void write(UINTT64);
		void stats();
		void statsPK(UINTT64);
		void displayContents();
};

void SetAssociativeCache::read(UINTT64 address) {
	address >>= offsetBits; // remove offset bits
	UINT32 index = address & indexMask; // get index
	UINTT64 tag = address >> indexBits; 	// get tag
	accesses++; // cache is accessed
	
	UINT32 way = 0; // way block found if hit, starts from 0 (MRU) 
	bool hit = false;
	for(UINT32 w = 0; w < ways; w++) {
		if(contents[w][index].valid && contents[w][index].tag == tag) { // if valid and hit 
			hit = true;
			way = w;
			if (contents[w][index].dirty)
				hits_dirty++;
			else {
				hits_clean++;
				contents[w][index].dirty = true;
			}
			break;
		}
	}
	
	if(hit) { // hit
		hits++;
		// store hit block temporarily to move to MRU
		UINTT64 tempTag = contents[way][index].tag;
		bool tempValid = contents[way][index].valid;
		bool tempDirty = contents[way][index].dirty;

		for(UINT32 w = way; w > 0; w--) { // shift all contents towards LRU way
			contents[w][index].tag = contents[w-1][index].tag;
			contents[w][index].dirty = contents[w-1][index].dirty;
			contents[w][index].valid = contents[w-1][index].valid;
		}
		// move block to MRU
		contents[0][index].tag = tempTag;
		contents[0][index].valid = tempValid;
		contents[0][index].dirty = tempDirty;
		lru_hits[way]++;
	}
	else { // miss
		misses++;
		if(contents[ways-1][index].valid) { // if there is a block in LRU way, it must be replaced
			replacements++;
			if(contents[ways-1][index].dirty) { // if LRU block is dirty, it must be written back
				writebacks++;
				replacements_dirty++;
			}
			else {
				replacements_clean++;
			}
		}
		for(UINT32 w = ways - 1; w > 0; w--) { // shift all contents towards LRU way
			contents[w][index].tag = contents[w-1][index].tag;
			contents[w][index].dirty = contents[w-1][index].dirty;
			contents[w][index].valid = contents[w-1][index].valid;
		}
		// place in cache
		contents[0][index].tag = tag;
		contents[0][index].valid = 1;
		contents[0][index].dirty = 0;
	}
}

void SetAssociativeCache::write(UINTT64 address) {
	address >>= offsetBits; // remove offset bits
	UINT32 index = address & indexMask; // get index
	UINTT64 tag = address >> indexBits; 	// get tag
	accesses++; // cache is accessed
	
	UINT32 way = 0; // way block found if hit, starts from 0 (MRU) 
	bool hit = false;
	for(UINT32 w = 0; w < ways; w++) {
		if(contents[w][index].valid && contents[w][index].tag == tag) { // if valid and hit 
			hit = true;
			way = w;
			if (contents[w][index].dirty)
				hits_dirty++;
			else {
				hits_clean++;
				contents[w][index].dirty = true;
			}
			break;
		}
	}
	
	if(hit) { // hit
		hits++;
		// store hit block temporarily to move to MRU
		UINT64 tempTag = contents[way][index].tag;
		bool tempValid = contents[way][index].valid;

		for(UINT32 w = way; w > 0; w--) { // shift all contents towards LRU way
			contents[w][index].tag = contents[w-1][index].tag;
			contents[w][index].dirty = contents[w-1][index].dirty;
			contents[w][index].valid = contents[w-1][index].valid;
		}
		// move block to MRU
		contents[0][index].tag = tempTag;
		contents[0][index].valid = tempValid;
		contents[0][index].dirty = 1;  // write
		lru_hits[way]++;
	}
	else { // miss
		misses++;
		if(contents[ways-1][index].valid) { // if there is a block in LRU way, it must be replaced
			replacements++;
			if(contents[ways-1][index].dirty) { // if LRU block is dirty, it must be written back
				writebacks++;
				replacements_dirty++;
			}
			else {
				replacements_clean++;
			}
		}
		for(UINT32 w = ways - 1; w > 0; w--) { // shift all contents towards LRU way
			contents[w][index].tag = contents[w-1][index].tag;
			contents[w][index].dirty = contents[w-1][index].dirty;
			contents[w][index].valid = contents[w-1][index].valid;
		}
		// place in cache
		contents[0][index].tag = tag;
		contents[0][index].valid = 1;
		contents[0][index].dirty = 1; // write
	}
}

void SetAssociativeCache::stats() {
	cout << "Total Cache Accesses: " << accesses << endl;
	cout << "Total Number of Cache Misses: " << misses << endl;
	cout << "Total Number of Cache Hits: " << hits << endl;
	cout << "Total Number of Cache Hits_clean: " << hits_clean << endl;
	cout << "Total Number of Cache Hits_dirty: " << hits_dirty << endl;
	cout << "Total Number of replacements: " << replacements << endl;
	cout << "Total Number of replacements_clean: " << replacements_clean << endl;
	cout << "Total Number of replacements_dirty: " << replacements_dirty << endl;
	cout << "Total Number of writebacks: " << writebacks << endl;
	UINT32 dirty = 0;
	for(UINT32 i = 0; i < ways; i++) { // calculate total dirty blocks 
		for(UINT32 j = 0; j < sets; j++) {
			if(contents[i][j].dirty) {
				dirty++;
			}
		}
	}
	cout << "Number of dirty blocks: " << dirty << endl;
	cout << "Histogram:\n";
	for (UINT32 w = 0; w < ways; w++) {
		cout << lru_hits[w] << ", ";
	}
	cout << endl;
}

void SetAssociativeCache::statsPK(UINTT64 instructions) {
	cout << "Accesses per kilo Instructions (APKI): " << (1000.0 * accesses) / instructions << endl;
	cout << "Misses per kilo Instructions (APKI): " << (1000.0 * misses) / instructions << endl;
	cout << "Hits per kilo Instructions (APKI): " << (1000.0 * hits) / instructions << endl;
	cout << "Replacements per kilo Instructions (APKI): " << (1000.0 * replacements) / instructions << endl;
	cout << "Writebacks per kilo Instructions (APKI): " << (1000.0 * writebacks) / instructions << endl;
}

void SetAssociativeCache::displayContents() {
	cout << "Cache contents:\n";
	for(UINT32 i = 0; i < sets; i++) {
		cout << i << " ";
		for(UINT32 w = 0; w < ways; w++) {
			cout << contents[w][i].valid << ", " << contents[w][i].dirty << ", " << hex << contents[w][i].tag << dec << " ";
		}
		cout << endl;
	}
}

SetAssociativeCache *nw16_2 = new SetAssociativeCache(16*1024, 64, 2);
SetAssociativeCache *nw16_4 = new SetAssociativeCache(16*1024, 64, 4);
SetAssociativeCache *nw16_8 = new SetAssociativeCache(16*1024, 64, 8);
SetAssociativeCache *nw32_2 = new SetAssociativeCache(32*1024, 64, 2);
SetAssociativeCache *nw32_4 = new SetAssociativeCache(32*1024, 64, 4);
SetAssociativeCache *nw32_8 = new SetAssociativeCache(32*1024, 64, 8);
SetAssociativeCache *nw64_2 = new SetAssociativeCache(64*1024, 64, 2);
SetAssociativeCache *nw64_4 = new SetAssociativeCache(64*1024, 64, 4);
SetAssociativeCache *nw64_8 = new SetAssociativeCache(64*1024, 64, 8);
DirectMappedCache *dc16 = new DirectMappedCache(16*1024, 64);
DirectMappedCache *dc32 = new DirectMappedCache(32*1024, 64);
DirectMappedCache *dc64 = new DirectMappedCache(64*1024, 64);

SetAssociativeCache *L1 = new SetAssociativeCache(32*1024, 64, 4);
// Print a memory read record
VOID RecordMemRead(VOID * ip, VOID * addr)
{
  //UINTT64 temp= *((UINTT64*)addr);
  //std::cout<<std::hex<<addr<<endl;
  dc16->read(*((UINTT64*)ip));
  dc32->read(*((UINTT64*)ip));
  dc64->read(*((UINTT64*)ip));
  nw16_2->read(*((UINTT64*)ip));
  nw16_4->read(*((UINTT64*)ip));
  nw16_8->read(*((UINTT64*)ip));
  nw32_2->read(*((UINTT64*)ip));
  nw32_4->read(*((UINTT64*)ip));
  nw32_8->read(*((UINTT64*)ip));
  nw64_2->read(*((UINTT64*)ip));
  nw64_4->read(*((UINTT64*)ip));
  nw64_8->read(*((UINTT64*)ip));
  // fprintf(trace,"%p\n", addr);
}

// Print a memory write record
VOID RecordMemWrite(VOID * ip, VOID * addr)
{
  //UINTT64 temp= *((UINTT64*)addr);
  //std::cout<<std::hex<<addr<<endl;
  dc16->write(*((UINTT64*)ip));
  dc32->write(*((UINTT64*)ip));
  dc64->write(*((UINTT64*)ip));
  nw16_2->write(*((UINTT64*)ip));
  nw16_4->write(*((UINTT64*)ip));
  nw16_8->write(*((UINTT64*)ip));
  nw32_2->write(*((UINTT64*)ip));
  nw32_4->write(*((UINTT64*)ip));
  nw32_8->write(*((UINTT64*)ip));
  nw64_2->write(*((UINTT64*)ip));
  nw64_4->write(*((UINTT64*)ip));
  nw64_8->write(*((UINTT64*)ip));
  //fprintf(trace,"%p\n",addr);
}

VOID docount() { 
    inscount++; 
    // call to terminate process after the program executes 10 000 000 000 instructions.
    if (inscount >= 10000000000)
    {
        PIN_ExitApplication(0);
    }

}

// Is called for every instruction and instruments reads and writes
VOID Instruction(INS ins, VOID *v)
{

    // Instruments memory accesses using a predicated call, i.e.
    // the instrumentation is called iff the instruction will actually be executed.
    //
    // On the IA-32 and Intel(R) 64 architectures conditional moves and REP 
    // prefixed instructions appear as predicated instructions in Pin.
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
	
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        if (INS_MemoryOperandIsRead(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
        // Note that in some architectures a single memory operand can be 
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
    }

    //increment instruction count
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);

}

VOID Fini(INT32 code, VOID *v)
{
  dc16->displayContents();
  dc16->stats();
  dc16->statsPK(inscount);
  dc32->displayContents();
  dc32->stats();
  dc32->statsPK(inscount);
  dc64->displayContents();
  dc64->stats();
  dc64->statsPK(inscount);
  nw16_2->displayContents();
  nw16_2->stats();
  nw16_2->statsPK(inscount);
  nw16_4->displayContents();
  nw16_4->stats();
  nw16_4->statsPK(inscount);
  nw16_8->displayContents();
  nw16_8->stats();
  nw16_8->statsPK(inscount);
  nw32_2->displayContents();
  nw32_2->stats();
  nw32_2->statsPK(inscount);
  nw32_4->displayContents();
  nw32_4->stats();
  nw32_4->statsPK(inscount);
  nw32_8->displayContents();
  nw32_8->stats();
  nw32_8->statsPK(inscount);
  nw64_2->displayContents();
  nw64_2->stats();
  nw64_2->statsPK(inscount);
  nw64_4->displayContents();
  nw64_4->stats();
  nw64_4->statsPK(inscount);
  nw64_8->displayContents();
  nw64_8->stats();
  nw64_8->statsPK(inscount);

    fprintf(trace, "#eof\n");
    fclose(trace);
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
   
INT32 Usage()
{
    PIN_ERROR( "This Pintool prints a trace of memory addresses\n" 
              + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    if (PIN_Init(argc, argv)) return Usage();

    trace = fopen("pinatrace.out", "w");

    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();
    
    return 0;
}

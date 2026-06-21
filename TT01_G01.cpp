// ============================================================
// CCP6124 Object-Oriented Programming and Data Structures
// Trimester 2610
// Virtual Machine and Assembly Language Interpreter
// File: TT01_G01.cpp
// Group: TT01_G01
// Due: 4 July 2026
// ============================================================
// GROUP MEMBERS:
//   Student A - [Full Name] - [Student ID]
//   Student B - [Full Name] - [Student ID]
//   Student C - [Full Name] - [Student ID]
//   Student D - [MANESH MATHIALAGEN] - [253UC256FC]
// ============================================================
// HOW TO COMPILE:
//   g++ TT01_G01.cpp -o vm
// HOW TO RUN:
//   ./vm program.asm        (Linux / Mac)
//   vm.exe program.asm      (Windows)
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cctype>

using namespace std;


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 1 : CUSTOM DATA STRUCTURES
// ASSIGNED TO : Student A
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : MyVector
// AUTHOR: Student A - [Full Name]
// DESC  : A simple fixed array that stores up to 256 strings.
//         Used to store lines read from the .asm file.
//         Replaces std::vector (no STL allowed).
// ============================================================
class MyVector {
private:
    // TODO Student A: declare your private variables here
    // Hint: you need a string array and an int for size

public:
    // Author: Student A
    // DESC: Constructor - set size to 0
    MyVector() {
        // TODO Student A: set size = 0
    }

    // Author: Student A
    // DESC: Add a new string item to the end of the array
    void push_back(string item) {
        // TODO Student A: store item, increase size
        // if size >= 256, print error and exit
    }

    // Author: Student A
    // DESC: Get the item at a given index position
    string get(int index) {
        // TODO Student A: return data[index]
        // if index is out of range, print error and exit
        return "";
    }

    // Author: Student A
    // DESC: Returns how many items are stored
    int getSize() {
        // TODO Student A: return size
        return 0;
    }

    // Author: Student A
    // DESC: Remove all items (reset size to 0)
    void clear() {
        // TODO Student A: size = 0
    }
};


// ============================================================
// CLASS : MyStack
// AUTHOR: Student A - [Full Name]
// DESC  : A simple Last-In-First-Out stack with max 8 slots.
//         Used for PUSH and POP instructions in the VM.
//         Replaces std::stack (no STL allowed).
// ============================================================
class MyStack {
private:
    // TODO Student A: declare your private variables here
    // Hint: you need an int array[8] and an int for top index

public:
    // Author: Student A
    // DESC: Constructor - set top to -1 (empty)
    MyStack() {
        // TODO Student A: top = -1
    }

    // Author: Student A
    // DESC: Push a value onto the top of the stack
    //       Print error and exit if stack is already full (8 items)
    void push(int value) {
        // TODO Student A: check if full, then add value
    }

    // Author: Student A
    // DESC: Remove and return the top value from the stack
    //       Print error and exit if stack is empty
    int pop() {
        // TODO Student A: check if empty, then return top value
        return 0;
    }

    // Author: Student A
    // DESC: Returns true if the stack has no items
    bool isEmpty() {
        // TODO Student A: return top < 0
        return true;
    }

    // Author: Student A
    // DESC: Returns how many items are in the stack
    int getSize() {
        // TODO Student A: return top + 1
        return 0;
    }
};


// ============================================================
// CLASS : MyQueue
// AUTHOR: Student A - [Full Name]
// DESC  : A simple First-In-First-Out queue with max 256 slots.
//         Used as an alternative way to store program instructions.
//         Replaces std::queue (no STL allowed).
// ============================================================
class MyQueue {
private:
    // TODO Student A: declare your private variables here
    // Hint: string array[256], int front, int rear, int size

public:
    // Author: Student A
    // DESC: Constructor - set front, rear, size all to 0
    MyQueue() {
        // TODO Student A: front=0, rear=0, size=0
    }

    // Author: Student A
    // DESC: Add an item to the back of the queue
    void enqueue(string item) {
        // TODO Student A: add item at rear, move rear forward
    }

    // Author: Student A
    // DESC: Remove and return item from the front of the queue
    string dequeue() {
        // TODO Student A: return item at front, move front forward
        return "";
    }

    // Author: Student A
    // DESC: Returns true if queue has no items
    bool isEmpty() {
        // TODO Student A: return size == 0
        return true;
    }

    // Author: Student A
    // DESC: Returns how many items are in the queue
    int getSize() {
        // TODO Student A: return size
        return 0;
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 2 : REGISTER CLASSES
// ASSIGNED TO : Student B
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : Register
// AUTHOR: Student B - [Full Name]
// DESC  : Base class for all register types.
//         Holds one integer value.
//         GeneralRegister inherits from this class.
// ============================================================
class Register {
protected:
    // TODO Student B: declare protected int value

public:
    // Author: Student B
    // DESC: Constructor - set value to 0
    Register() {
        // TODO Student B: value = 0
    }

    // Author: Student B
    // DESC: Returns the current value stored in this register
    int getValue() {
        // TODO Student B: return value
        return 0;
    }

    // Author: Student B
    // DESC: Resets register value back to 0
    void reset() {
        // TODO Student B: value = 0
    }

    // Author: Student B
    // DESC: Sets the value (virtual so child class can override)
    virtual void setValue(int v) {
        // TODO Student B: value = v
    }
};


// ============================================================
// CLASS : GeneralRegister
// AUTHOR: Student B - [Full Name]
// DESC  : Represents registers R0 to R7.
//         Inherits from Register.
//         Keeps value clamped between -128 and 127.
// ============================================================
class GeneralRegister : public Register {
public:
    // Author: Student B
    // DESC: Sets the value but clamps it to -128..127 range
    //       Overrides the parent Register's setValue
    void setValue(int v) override {
        // TODO Student B: if v > 127, set v = 127
        //                 if v < -128, set v = -128
        //                 then value = v
    }
};


// ============================================================
// CLASS : FlagRegister
// AUTHOR: Student B - [Full Name]
// DESC  : Stores the 4 flag bits: OF, UF, CF, ZF
//         Each flag is 0 (off) or 1 (on).
//         Flags are updated after every arithmetic operation.
// ============================================================
class FlagRegister {
private:
    // TODO Student B: declare 4 int variables: OF, UF, CF, ZF

public:
    // Author: Student B
    // DESC: Constructor - set all flags to 0
    FlagRegister() {
        // TODO Student B: OF=0, UF=0, CF=0, ZF=0
    }

    // Author: Student B - Getters for each flag
    int getOF() { /* TODO: return OF */ return 0; }
    int getUF() { /* TODO: return UF */ return 0; }
    int getCF() { /* TODO: return CF */ return 0; }
    int getZF() { /* TODO: return ZF */ return 0; }

    // Author: Student B - Setters for each flag
    void setOF(int v) { /* TODO: OF = v */ }
    void setUF(int v) { /* TODO: UF = v */ }
    void setCF(int v) { /* TODO: CF = v */ }
    void setZF(int v) { /* TODO: ZF = v */ }

    // Author: Student B
    // DESC: Set all flags back to 0
    void resetAll() {
        // TODO Student B: OF=0, UF=0, CF=0, ZF=0
    }

    // Author: Student B
    // DESC: Automatically update all flags based on a result value
    //       Called after every arithmetic operation
    //       OF = 1 if result > 127
    //       UF = 1 if result < -128
    //       ZF = 1 if result == 0
    //       CF = 1 if result > 255 or result < -255
    void updateFlags(int result) {
        // TODO Student B: check each condition and set flags
    }

    // Author: Student B
    // DESC: Reset one specific flag to 0 by its name
    //       Valid names: "OF", "UF", "CF", "ZF"
    void resetOne(string name) {
        // TODO Student B: if name == "OF" then OF=0, etc.
        // if name is unknown, print error and exit
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 3 : MEMORY CLASS
// ASSIGNED TO : Student B
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : Memory
// AUTHOR: Student B - [Full Name]
// DESC  : 64 slots of storage, addressed from 0 to 63.
//         Each slot holds one integer value.
//         Only LOAD and STORE instructions can access memory.
// ============================================================
class Memory {
private:
    // TODO Student B: declare int mem[64]

public:
    // Author: Student B
    // DESC: Constructor - fill all 64 slots with 0
    Memory() {
        // TODO Student B: loop and set mem[i] = 0
    }

    // Author: Student B
    // DESC: Read and return value at given memory address
    //       Print error and exit if address is not 0..63
    int read(int address) {
        // TODO Student B: check bounds, then return mem[address]
        return 0;
    }

    // Author: Student B
    // DESC: Write a value to a given memory address
    //       Print error and exit if address is not 0..63
    void write(int address, int value) {
        // TODO Student B: check bounds, then mem[address] = value
    }

    // Author: Student B
    // DESC: Returns the raw memory array (used for output display)
    int* getRaw() {
        // TODO Student B: return mem
        return nullptr;
    }

    // Author: Student B
    // DESC: Clear all memory slots back to 0
    void reset() {
        // TODO Student B: loop and set mem[i] = 0
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 4 : CPU CLASS
// ASSIGNED TO : Student C
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : CPU
// AUTHOR: Student C - [Full Name]
// DESC  : The main brain of the virtual machine.
//         Owns the registers, memory, stack, PC, and SI.
//         All instructions call CPU methods to do their work.
// COMPOSITION : CPU owns Memory, GeneralRegister[8], MyStack
// AGGREGATION : CPU uses FlagRegister
// ============================================================
class CPU {
private:
    GeneralRegister registers[8];   // R0 to R7
    FlagRegister    flags;          // OF, UF, CF, ZF
    Memory          memory;         // 64 bytes of storage
    MyStack         vmStack;        // 8-slot system stack
    int             PC;             // Program Counter
    int             SI;             // Stack Index

public:
    // Author: Student C
    // DESC: Constructor - set PC and SI to 0
    CPU() {
        // TODO Student C: PC = 0, SI = 0
    }

    // ----------------------------------------------------------
    // REGISTER ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Get the value stored in register number n (0 to 7)
    int getRegValue(int n) {
        // TODO Student C: check n is 0..7, return registers[n].getValue()
        return 0;
    }

    // Author: Student C
    // DESC: Set a value into register number n
    //       Also calls flags.updateFlags() before storing
    void setRegValue(int n, int v) {
        // TODO Student C: check n is 0..7
        //                 call flags.updateFlags(v)
        //                 call registers[n].setValue(v)
    }

    // ----------------------------------------------------------
    // FLAG ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Returns reference to the flag register
    FlagRegister& getFlags() {
        // TODO Student C: return flags
        return flags;
    }

    // Author: Student C
    // DESC: Reset one flag by name (calls flags.resetOne)
    void resetFlag(string name) {
        // TODO Student C: flags.resetOne(name)
    }

    // ----------------------------------------------------------
    // MEMORY ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Read value from memory at given address
    int memRead(int address) {
        // TODO Student C: return memory.read(address)
        return 0;
    }

    // Author: Student C
    // DESC: Write value to memory at given address
    void memWrite(int address, int value) {
        // TODO Student C: memory.write(address, value)
    }

    // ----------------------------------------------------------
    // PROGRAM COUNTER ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Returns current value of the Program Counter
    int getPC() {
        // TODO Student C: return PC
        return 0;
    }

    // Author: Student C
    // DESC: Adds 1 to the Program Counter after each instruction
    void incrementPC() {
        // TODO Student C: PC++
    }

    // ----------------------------------------------------------
    // STACK ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Push a value onto the VM stack and increase SI by 1
    void stackPush(int value) {
        // TODO Student C: vmStack.push(value), SI++
    }

    // Author: Student C
    // DESC: Pop a value from the VM stack and decrease SI by 1
    int stackPop() {
        // TODO Student C: SI--, return vmStack.pop()
        return 0;
    }

    // Author: Student C
    // DESC: Returns current Stack Index value
    int getSI() {
        // TODO Student C: return SI
        return 0;
    }

    // ----------------------------------------------------------
    // RESET
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Reset the entire CPU back to starting state
    void reset() {
        // TODO Student C: reset all registers, flags, memory, PC=0, SI=0
    }

    // ----------------------------------------------------------
    // BIT HELPERS (used by ROL, ROR, SHL, SHR)
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Convert a number into 8 individual bits
    //       bits[0] = LSB (least significant bit)
    //       bits[7] = MSB (most significant bit)
    void toBits(int value, int bits[8]) {
        // TODO Student C: use bitwise shift and AND to extract each bit
    }

    // Author: Student C
    // DESC: Convert 8 individual bits back into a number
    int fromBits(int bits[8]) {
        // TODO Student C: combine bits back into a number
        return 0;
    }

    // Author: Student C
    // DESC: Rotate all bits left by count positions
    //       The leftmost bit wraps around to the right
    int rotateLeft(int value, int count) {
        // TODO Student C: use toBits, shift left, wrap, fromBits
        return 0;
    }

    // Author: Student C
    // DESC: Rotate all bits right by count positions
    //       The rightmost bit wraps around to the left
    int rotateRight(int value, int count) {
        // TODO Student C: use toBits, shift right, wrap, fromBits
        return 0;
    }

    // Author: Student C
    // DESC: Shift all bits left by count positions
    //       Empty spots on the right are filled with 0
    int shiftLeft(int value, int count) {
        // TODO Student C: use toBits, shift left, fill 0, fromBits
        return 0;
    }

    // Author: Student C
    // DESC: Shift all bits right by count positions
    //       Empty spots on the left are filled with 0
    int shiftRight(int value, int count) {
        // TODO Student C: use toBits, shift right, fill 0, fromBits
        return 0;
    }

    // ----------------------------------------------------------
    // DISPLAY AND OUTPUT
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Print all register values and flags to the screen
    void displayState() {
        // TODO Student C: print R0..R7, PC, and flags in a readable format
    }

    // Author: Student C
    // DESC: Build the #Registers# line for the output file
    string buildRegistersLine() {
        // TODO Student C: format each register as 4 digits
        // Example: #Registers#0000#0011#0000#0044#0000#0000#0000#0000#
        return "";
    }

    // Author: Student C
    // DESC: Build the #Flags# line for the output file
    string buildFlagsLine() {
        // TODO Student C: format OF UF CF ZF
        // Example: #Flags#0#0#0#0#
        return "";
    }

    // Author: Student C
    // DESC: Build the #Memory# section (8 rows of 8 values)
    string buildMemoryLines() {
        // TODO Student C: loop through all 64 memory slots
        // Example row: #0000#0000#0000#0000#0044#0000#0000#0000#
        return "";
    }

    // Author: Student C
    // DESC: Combine all output lines and write to file and screen
    void writeOutput(string filename) {
        // TODO Student C: combine #Begin# + registers + flags + PC + memory + #End#
        // write to file, also print to screen
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 5 : INSTRUCTION BASE CLASS
// ASSIGNED TO : Student C
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : Instruction (ABSTRACT BASE CLASS)
// AUTHOR: Student C - [Full Name]
// DESC  : Parent class for ALL instruction types.
//         Every instruction must implement execute() and toString().
//         This enables polymorphism - Runner stores Instruction*
//         and calls execute() without knowing the exact type.
// ============================================================
class Instruction {
public:
    // Author: Student C
    // DESC: Run this instruction on the CPU (MUST be overridden)
    virtual void execute(CPU& cpu) = 0;

    // Author: Student C
    // DESC: Return this instruction as readable text (MUST be overridden)
    virtual string toString() = 0;

    // Author: Student C
    // DESC: Virtual destructor for safe cleanup
    virtual ~Instruction() {}
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 6 : CORE INSTRUCTIONS
// ASSIGNED TO : Student C
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : MovInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles the MOV instruction in 3 modes:
//         Mode 1 - MOV R0, 10    (store a number directly)
//         Mode 2 - MOV R0, R1    (copy from another register)
//         Mode 3 - MOV R0, [R1]  (load from memory address in R1)
// ============================================================
class MovInstruction : public Instruction {
private:
    // TODO Student C: declare dest, src, immediate, isImmediate, isIndirect

public:
    // Author: Student C
    // DESC: Constructor for MOV R0, 10 (immediate mode)
    MovInstruction(int dest, int imm) {
        // TODO Student C: store dest and imm, set isImmediate=true
    }

    // Author: Student C
    // DESC: Constructor for MOV R0, R1 or MOV R0, [R1]
    MovInstruction(int dest, int src, bool indirect) {
        // TODO Student C: store dest and src
        //                 set isImmediate=false, isIndirect=indirect
    }

    // Author: Student C
    // DESC: Execute the MOV instruction on the CPU
    void execute(CPU& cpu) override {
        // TODO Student C:
        // if isImmediate: cpu.setRegValue(dest, immediate)
        // if isIndirect:  addr = cpu.getRegValue(src)
        //                 cpu.setRegValue(dest, cpu.memRead(addr))
        // else:           cpu.setRegValue(dest, cpu.getRegValue(src))
    }

    // Author: Student C
    // DESC: Return MOV instruction as text
    string toString() override {
        // TODO Student C: return "MOV R0, 10" or "MOV R0, R1" etc.
        return "";
    }
};


// ============================================================
// CLASS : InputInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles INPUT Rx
//         Reads a number from keyboard and stores in register
// ============================================================
class InputInstruction : public Instruction {
private:
    // TODO Student C: int dest

public:
    // Author: Student C
    InputInstruction(int d) { /* TODO: dest = d */ }

    // Author: Student C
    // DESC: Show "?" prompt, read number, check flags, store in register
    void execute(CPU& cpu) override {
        // TODO Student C:
        // cout << "? "; cin >> value;
        // if value > 127 set OF flag
        // if value < -128 set UF flag
        // if value == 0 set ZF flag
        // cpu.setRegValue(dest, value)
    }

    // Author: Student C
    string toString() override { /* TODO: return "INPUT Rx" */ return ""; }
};


// ============================================================
// CLASS : DisplayInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles DISPLAY Rx
//         Prints the value of a register to the screen
// ============================================================
class DisplayInstruction : public Instruction {
private:
    // TODO Student C: int src

public:
    // Author: Student C
    DisplayInstruction(int s) { /* TODO: src = s */ }

    // Author: Student C
    // DESC: Print the value of the register to screen
    void execute(CPU& cpu) override {
        // TODO Student C: cout << cpu.getRegValue(src)
    }

    // Author: Student C
    string toString() override { /* TODO: return "DISPLAY Rx" */ return ""; }
};


// ============================================================
// CLASS : AddInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles ADD Rd, Rs  and  ADD Rd, number
//         Adds source to destination and stores result in destination
// ============================================================
class AddInstruction : public Instruction {
private:
    // TODO Student C: int dest, src, immediate, bool isImmediate

public:
    // Author: Student C
    AddInstruction(int d, int s, bool imm) {
        // TODO Student C: store d, s, imm
    }

    // Author: Student C
    // DESC: result = dest + src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        // TODO Student C: calculate result, cpu.setRegValue(dest, result)
    }

    // Author: Student C
    string toString() override { /* TODO */ return ""; }
};


// ============================================================
// CLASS : SubInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles SUB Rd, Rs  and  SUB Rd, number
//         Subtracts source from destination
// ============================================================
class SubInstruction : public Instruction {
private:
    // TODO Student C: int dest, src, immediate, bool isImmediate

public:
    // Author: Student C
    SubInstruction(int d, int s, bool imm) {
        // TODO Student C: store d, s, imm
    }

    // Author: Student C
    // DESC: result = dest - src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        // TODO Student C: calculate result, cpu.setRegValue(dest, result)
    }

    // Author: Student C
    string toString() override { /* TODO */ return ""; }
};


// ============================================================
// CLASS : LoadInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles LOAD Rd, [address]  and  LOAD Rd, [Rs]
//         Reads a value from memory into a register
// ============================================================
class LoadInstruction : public Instruction {
private:
    // TODO Student C: int dest, address, addrReg, bool useReg

public:
    // Author: Student C
    // useReg=true means LOAD Rd, [Rs]
    // useReg=false means LOAD Rd, [20]
    LoadInstruction(int d, int addr, bool useReg) {
        // TODO Student C: store values
    }

    // Author: Student C
    // DESC: Get address (from register or fixed), read memory, store in dest
    void execute(CPU& cpu) override {
        // TODO Student C:
        // if useReg: addr = cpu.getRegValue(addrReg)
        // else:      addr = address
        // cpu.setRegValue(dest, cpu.memRead(addr))
    }

    // Author: Student C
    string toString() override { /* TODO */ return ""; }
};


// ============================================================
// CLASS : StoreInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles STORE Rx, address  and  STORE [Rx], Rs
//         Writes a register value into memory
// ============================================================
class StoreInstruction : public Instruction {
private:
    // TODO Student C: int srcReg, address, addrReg, bool useReg

public:
    // Author: Student C
    // useReg=true means STORE [Rd], Rs
    // useReg=false means STORE Rs, 43
    StoreInstruction(int src, int addr, bool useReg) {
        // TODO Student C: store values
    }

    // Author: Student C
    // DESC: Get address (from register or fixed), write register to memory
    void execute(CPU& cpu) override {
        // TODO Student C:
        // if useReg: addr = cpu.getRegValue(addrReg)
        // else:      addr = address
        // cpu.memWrite(addr, cpu.getRegValue(srcReg))
    }

    // Author: Student C
    string toString() override { /* TODO */ return ""; }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 7 : ADVANCED INSTRUCTIONS
// ASSIGNED TO : MANESH MATHIALAGEN
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : MulInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles MUL Rd, Rs  and  MUL Rd, number
//         Multiplies destination by source
// ============================================================
class MulInstruction : public Instruction {
private:
    int dest;
    int src;
    int immediate;
    bool isImmediate;

public:
    // Author: MANESH MATHIALAGEN
    MulInstruction(int d, int s, bool imm) {
        dest = d;
        isImmediate = imm;
        if (isImmediate) {
            immediate = s;
        } else {
            src = s;
        }
    }

    // Author: MANESH MATHIALAGEN
    // DESC: result = dest * src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        int val1 = cpu.getRegValue(dest);
        int val2 = isImmediate ? immediate : cpu.getRegValue(src);
        cpu.setRegValue(dest, val1 * val2);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "MUL R" + to_string(dest) + ", " + (isImmediate ? to_string(immediate) : "R" + to_string(src)); 
    }
};


// ============================================================
// CLASS : DivInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles DIV Rd, Rs  and  DIV Rd, number
//         Divides destination by source
//         IMPORTANT: must print error and exit if dividing by 0
// ============================================================
class DivInstruction : public Instruction {
private:
    int dest;
    int src;
    int immediate;
    bool isImmediate;

public:
    // Author: MANESH MATHIALAGEN
    DivInstruction(int d, int s, bool imm) {
        dest = d;
        isImmediate = imm;
        if (isImmediate) {
            immediate = s;
        } else {
            src = s;
        }
    }

    // Author: MANESH MATHIALAGEN
    // DESC: result = dest / src (or immediate), store back to dest
    //       MUST check for divide by zero before dividing
    void execute(CPU& cpu) override {
        int divisor = isImmediate ? immediate : cpu.getRegValue(src);
        
        if (divisor == 0) {
            cout << "ERROR: Division by zero\n";
            exit(1);
        }
        
        cpu.setRegValue(dest, cpu.getRegValue(dest) / divisor);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "DIV R" + to_string(dest) + ", " + (isImmediate ? to_string(immediate) : "R" + to_string(src)); 
    }
};


// ============================================================
// CLASS : IncInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles INC Rx
//         Adds 1 to the register value
// ============================================================
class IncInstruction : public Instruction {
private:
    int dest;

public:
    // Author: MANESH MATHIALAGEN
    IncInstruction(int d) { 
        dest = d; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: dest = dest + 1
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.getRegValue(dest) + 1);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "INC R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : DecInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles DEC Rx
//         Subtracts 1 from the register value
// ============================================================
class DecInstruction : public Instruction {
private:
    int dest;

public:
    // Author: MANESH MATHIALAGEN
    DecInstruction(int d) { 
        dest = d; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: dest = dest - 1
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.getRegValue(dest) - 1);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "DEC R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : RolInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles ROL Rx, count
//         Rotates bits in register left by count positions
//         Leftmost bit wraps around to the rightmost position
// ============================================================
class RolInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: MANESH MATHIALAGEN
    RolInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Rotate register bits left, store result back
    void execute(CPU& cpu) override {
        int result = cpu.rotateLeft(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "ROL R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : RorInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles ROR Rx, count
//         Rotates bits in register right by count positions
//         Rightmost bit wraps around to the leftmost position
// ============================================================
class RorInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: MANESH MATHIALAGEN
    RorInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Rotate register bits right, store result back
    void execute(CPU& cpu) override {
        int result = cpu.rotateRight(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "ROR R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : ShlInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles SHL Rx, count
//         Shifts bits left by count positions
//         Empty spots on the right are filled with 0
// ============================================================
class ShlInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: MANESH MATHIALAGEN
    ShlInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Shift register bits left, fill with 0, store result back
    void execute(CPU& cpu) override {
        int result = cpu.shiftLeft(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "SHL R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : ShrInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles SHR Rx, count
//         Shifts bits right by count positions
//         Empty spots on the left are filled with 0
// ============================================================
class ShrInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: MANESH MATHIALAGEN
    ShrInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Shift register bits right, fill with 0, store result back
    void execute(CPU& cpu) override {
        int result = cpu.shiftRight(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "SHR R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : PushInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles PUSH Rx
//         Copies register value onto the top of the stack
//         Stack Index (SI) increases by 1
// ============================================================
class PushInstruction : public Instruction {
private:
    int src;

public:
    // Author: MANESH MATHIALAGEN
    PushInstruction(int s) { 
        src = s; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Push register value onto VM stack
    void execute(CPU& cpu) override {
        cpu.stackPush(cpu.getRegValue(src));
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "PUSH R" + to_string(src); 
    }
};


// ============================================================
// CLASS : PopInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles POP Rx
//         Removes top value from stack and stores in register
//         Stack Index (SI) decreases by 1
//         IMPORTANT: crashes if stack is empty
// ============================================================
class PopInstruction : public Instruction {
private:
    int dest;

public:
    // Author: MANESH MATHIALAGEN
    PopInstruction(int d) { 
        dest = d; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Pop value from VM stack into register
    //       MyStack.pop() already handles empty stack error
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.stackPop());
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "POP R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : ResetInstruction
// AUTHOR: MANESH MATHIALAGEN
// DESC  : Handles RESET OF / RESET UF / RESET CF / RESET ZF
//         Resets one specific flag back to 0
// ============================================================
class ResetInstruction : public Instruction {
private:
    string flagName;

public:
    // Author: MANESH MATHIALAGEN
    ResetInstruction(string name) { 
        flagName = name; 
    }

    // Author: MANESH MATHIALAGEN
    // DESC: Reset the named flag to 0
    void execute(CPU& cpu) override {
        cpu.resetFlag(flagName);
    }

    // Author: MANESH MATHIALAGEN
    string toString() override { 
        return "RESET " + flagName; 
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 8 : STRING HELPER FUNCTIONS
// ASSIGNED TO : Student A
// These are standalone functions used by the Runner to
// clean up and split text lines from the .asm file
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// Author: Student A
// DESC: Remove spaces and tabs from the start and end of a string
string trim(string s) {
    // TODO Student A: loop from start removing spaces/tabs
    //                 loop from end removing spaces/tabs
    //                 return the middle part
    return s;
}

// Author: Student A
// DESC: Convert all letters in a string to UPPERCASE
string toUpper(string s) {
    // TODO Student A: loop through each character, call toupper()
    return s;
}

// Author: Student A
// DESC: Remove anything after a semicolon (those are comments)
// Example: "MOV R0, 5 ; this is a comment" → "MOV R0, 5 "
string removeComment(string s) {
    // TODO Student A: find ';', return everything before it
    return s;
}

// Author: Student A
// DESC: Check if a string is a register name like R0, R1 .. R7
bool isReg(string s) {
    // TODO Student A: check s[0]=='R' and s[1] is a digit
    return false;
}

// Author: Student A
// DESC: Check if a string is an indirect register like [R0]
bool isIndReg(string s) {
    // TODO Student A: check s starts with '[', ends with ']', middle is R+digit
    return false;
}

// Author: Student A
// DESC: Check if a string is an indirect number like [20]
bool isIndNum(string s) {
    // TODO Student A: check s starts with '[', ends with ']', middle is digit
    return false;
}

// Author: Student A
// DESC: Remove the [ and ] brackets from a string
// Example: "[R2]" → "R2",   "[20]" → "20"
string removeBrackets(string s) {
    // TODO Student A: return s without first and last character
    return s;
}

// Author: Student A
// DESC: Get the register number from a string like "R3" → returns 3
int getRegNum(string s) {
    // TODO Student A: check isReg(s), then return atoi of s after 'R'
    return 0;
}

// Author: Student A
// DESC: Split a string into two parts at the first comma
// Example: "R0, R1" → left="R0", right="R1"
void splitTwo(string s, char delim, string& left, string& right) {
    // TODO Student A: find delim position
    //                 left  = everything before it (trimmed)
    //                 right = everything after it (trimmed)
}


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 9 : RUNNER CLASS
// ASSIGNED TO : Student A
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : Runner
// AUTHOR: Student A - [Full Name]
// DESC  : Reads the .asm file, parses each line into an
//         Instruction object, and executes them on the CPU.
//         This is the main controller of the whole system.
// ============================================================
class Runner {
private:
    CPU      cpu;
    MyVector instructions;    // stores raw text lines
    MyQueue  programQueue;    // queue version of the program
    string   outputFile;

    // ----------------------------------------------------------
    // PARSE HELPER FUNCTIONS
    // Each function handles one group of instructions
    // Kept separate so no function exceeds 35 lines
    // ----------------------------------------------------------

    // Author: Student A
    // DESC: Parse MOV instruction (all 3 modes)
    Instruction* parseMov(string rest) {
        // TODO Student A:
        // split rest by comma into left and right
        // if right is a register:        return new MovInstruction(dest, src, false)
        // if right is [register]:        return new MovInstruction(dest, src, true)
        // if right is a number:          return new MovInstruction(dest, number)
        return nullptr;
    }

    // Author: Student A
    // DESC: Parse ADD, SUB, MUL, DIV instructions
    Instruction* parseArith(string opcode, string rest) {
        // TODO Student A:
        // split rest by comma
        // get dest register
        // if right is register: create RegisterInstruction version
        // if right is number:   create ImmediateInstruction version
        // use opcode to decide which instruction class to create
        return nullptr;
    }

    // Author: Student A
    // DESC: Parse ROL, ROR, SHL, SHR instructions
    Instruction* parseShift(string opcode, string rest) {
        // TODO Student A:
        // split rest by comma
        // left = register, right = count number
        // use opcode to decide which instruction class to create
        return nullptr;
    }

    // Author: Student A
    // DESC: Parse LOAD and STORE instructions
    Instruction* parseMemory(string opcode, string rest) {
        // TODO Student A:
        // LOAD: split by comma, left=dest register, right=[addr] or [Rx]
        // STORE: split by comma, handle 3 formats:
        //        STORE [R2], R1  (indirect)
        //        STORE R1, 43   (register to fixed address)
        //        STORE 20, R3   (fixed address, register)
        return nullptr;
    }

    // Author: Student A
    // DESC: Parse PUSH, POP, RESET, INPUT, DISPLAY, INC, DEC instructions
    Instruction* parseSimple(string opcode, string rest) {
        // TODO Student A:
        // These all take one operand only
        // use opcode to decide which instruction class to create
        return nullptr;
    }

    // Author: Student A
    // DESC: Parse one full line of assembly text into an Instruction object
    //       Calls the helper parse functions above
    Instruction* parseLine(string line, int lineNum) {
        // TODO Student A:
        // 1. trim and removeComment from line
        // 2. if empty, return nullptr
        // 3. split into opcode and rest
        // 4. toUpper the opcode
        // 5. based on opcode, call correct parse helper:
        //    MOV                    → parseMov(rest)
        //    ADD/SUB/MUL/DIV        → parseArith(opcode, rest)
        //    ROL/ROR/SHL/SHR        → parseShift(opcode, rest)
        //    LOAD/STORE             → parseMemory(opcode, rest)
        //    everything else        → parseSimple(opcode, rest)
        // 6. if opcode unknown, print error and exit
        return nullptr;
    }

public:
    // Author: Student A
    // DESC: Main method - load the file, execute all instructions,
    //       display results, write output file
    void run(string filename) {
        // TODO Student A:
        // 1. set outputFile name
        // 2. open file with ifstream
        // 3. read line by line into instructions MyVector
        //    and also enqueue into programQueue
        // 4. close file
        // 5. print how many instructions were loaded
        // 6. cpu.reset()
        // 7. print START message, cpu.displayState()
        // 8. loop through instructions:
        //    a. parseLine to get Instruction*
        //    b. instr->execute(cpu)
        //    c. cpu.incrementPC()
        //    d. cpu.displayState()
        //    e. delete instr
        // 9. print END message
        // 10. cpu.writeOutput(outputFile)
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 10 : MAIN FUNCTION
// ASSIGNED TO : Student A
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// Author: Student A
// DESC: Entry point of the program.
//       Reads the .asm filename from command line and runs it.
int main(int argc, char* argv[]) {
    // TODO Student A:
    // if argc < 2: print usage instructions and return 1
    // else: create Runner, call runner.run(argv[1])
    return 0;
}

// ============================================================
// END OF TT01_G01.cpp
// ============================================================

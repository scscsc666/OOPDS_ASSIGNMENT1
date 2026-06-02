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
//   Student B - [YIP YU ZHE] - [252UC242YP]
//   Student C - [LOH ZI JIAN] - [252UC2421W]
//   Student D - [Full Name] - [Student ID]
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
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : Base class for all register types.
//         Holds one integer value.
//         GeneralRegister inherits from this class.
// ============================================================
class Register {
protected:
    int value; 

public:
    // Author: Student B
    // DESC: Constructor - set value to 0
    Register() {
        value = 0;
    }

    // Author: Student B
    // DESC: Returns the current value stored in this register
    int getValue() {
        return value;
    }

    // Author: Student B
    // DESC: Resets register value back to 0
    void reset() {
        value = 0;
    }

    // Author: Student B
    // DESC: Sets the value (virtual so child class can override)
    virtual void setValue(int v) {
        value = v;
    }
};


// ============================================================
// CLASS : GeneralRegister
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : Represents registers R0 to R7.
//         Inherits from Register.
//         Keeps value clamped between -128 and 127.
// ============================================================
class GeneralRegister : public Register{
public:    
    // Author: Student B
    // DESC: Sets the value but clamps it to -128..127 range
    //       Overrides the parent Register's setValue
    void setValue(int v) override {
        if (v > 127) v = 127;
        if (v < -128) v = -128;
        value = v;
    }
};


// ============================================================
// CLASS : FlagRegister
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : Stores the 4 flag bits: OF, UF, CF, ZF
//         Each flag is 0 (off) or 1 (on).
//         Flags are updated after every arithmetic operation.
// ============================================================
class FlagRegister {
private:
   int OF;
   int UF;
   int CF;
   int ZF;

public:
    // Author: Student B
    // DESC: Constructor - set all flags to 0
    FlagRegister() {
        OF = 0;
        UF = 0;
        CF = 0;
        ZF = 0;
    }

    // Author: Student B - Getters for each flag
    int getOF(){
        return OF;
    }

    int getUF(){
        return UF;
    }

    int getCF(){
        return CF;
    }

    int getZF(){
        return ZF;
    }

    // Author: Student B - Setters for each flag
    void setOF(int v){
        OF = v;
    }

    void setUF(int v){
        UF = v;
    }

    void setCF(int v){
        CF = v;
    }

    void setZF(int v){
        ZF = v;
    }

    // Author: Student B
    // DESC: Set all flags back to 0
    void resetAll(){
        OF = 0;
        UF = 0;
        CF = 0;
        ZF = 0;
    }

    // Author: Student B
    // DESC: Automatically update all flags based on a result value
    //       Called after every arithmetic operation
    //       OF = 1 if result > 127
    //       UF = 1 if result < -128
    //       ZF = 1 if result == 0
    //       CF = 1 if result > 255 or result < -255
    void updateFlags(int result) {
        // Overflow: result is too big for a signed byte
        if (result > 127)
            OF = 1;
        else
            OF = 0;
 
        // Underflow: result is too small for a signed byte
        if (result < -128)
            UF = 1;
        else
            UF = 0;
 
        // Zero: result is exactly zero
        if (result == 0)
            ZF = 1;
        else
            ZF = 0;
 
        // Carry: result is completely outside 8-bit range
        if (result > 255 || result < -255)
            CF = 1;
        else
            CF = 0;
    }

    // Author: Student B
    // DESC: Reset one specific flag to 0 by its name
    //       Valid names: "OF", "UF", "CF", "ZF"
    void resetOne(string name) {
        if (name == "OF") {
            OF = 0;
        } else if (name == "UF") {
            UF = 0;
        } else if (name == "CF") {
            CF = 0;
        } else if (name == "ZF") {
            ZF = 0;
        } else {
            cout << "ERROR: Unknown flag name: " << name << endl;
            exit(1);
        }
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 3 : MEMORY CLASS
// ASSIGNED TO : Student B
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : Memory
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : 64 slots of storage, addressed from 0 to 63.
//         Each slot holds one integer value.
//         Only LOAD and STORE instructions can access memory.
// ============================================================
class Memory {
private:
    int mem[64];   // 64 memory slots, each holds one number

public:
    // Author: Student B
    // DESC: Constructor - fill all 64 slots with 0
    Memory() {
        for (int i = 0; i < 64; i++) {
            mem[i] = 0;
        }
    }

    // Author: Student B
    // DESC: Read and return value at given memory address
    //       Print error and exit if address is not 0..63
    int read(int address) {
        if (address < 0 || address > 63) {
            cout << "ERROR: Memory address " << address
                 << " is out of range. Must be 0 to 63." << endl;
            exit(1);
        }
        return mem[address];
    }

    // Author: Student B
    // DESC: Write a value to a given memory address
    //       Print error and exit if address is not 0..63
    void write(int address, int value) {
        if (address < 0 || address > 63) {
            cout << "ERROR: Memory address " << address
                 << " is out of range. Must be 0 to 63." << endl;
            exit(1);
        }
        mem[address] = value;
    }

    // Author: Student B
    // DESC: Returns the raw memory array (used for output display)
    int* getRaw() {
        return mem;
    }

    // Author: Student B
    // DESC: Clear all memory slots back to 0
    void reset() {
        for (int i = 0; i < 64; i++) {
            mem[i] = 0;
        }
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 4 : CPU CLASS
// ASSIGNED TO : Student C
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : CPU
// AUTHOR: Student C - [LOH ZI JIAN]
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
        PC = 0;
        SI = 0;
    }

    // ----------------------------------------------------------
    // REGISTER ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Get the value stored in register number n (0 to 7)
    int getRegValue(int n) {
        return registers[n].getValue();
    }

    // Author: Student C
    // DESC: Set a value into register number n
    //       Also calls flags.updateFlags() before storing
    void setRegValue(int n, int v) {
        flags.updateFlags(v);
        registers[n].setValue(v);
    }

    // ----------------------------------------------------------
    // FLAG ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Returns reference to the flag register
    FlagRegister& getFlags() {
        return flags;
    }

    // Author: Student C
    // DESC: Reset one flag by name (calls flags.resetOne)
    void resetFlag(string name) {
        flags.resetOne(name);
    }

    // ----------------------------------------------------------
    // MEMORY ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Read value from memory at given address
    int memRead(int address) {
        return memory.read(address);
    }

    // Author: Student C
    // DESC: Write value to memory at given address
    void memWrite(int address, int value) {
        memory.write(address, value); 
    }

    // ----------------------------------------------------------
    // PROGRAM COUNTER ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Returns current value of the Program Counter
    int getPC() {
        return PC;
    }

    // Author: Student C
    // DESC: Adds 1 to the Program Counter after each instruction
    void incrementPC() {
        PC++;
    }

    // ----------------------------------------------------------
    // STACK ACCESS
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Push a value onto the VM stack and increase SI by 1
    void stackPush(int value) {
        vmStack.push(value);
        SI++;
    }

    // Author: Student C
    // DESC: Pop a value from the VM stack and decrease SI by 1
    int stackPop() {
        SI--;
        return vmStack.pop();
    }
    // Author: Student C
    // DESC: Returns current Stack Index value
    int getSI() {
        return SI;
    }

    // ----------------------------------------------------------
    // RESET
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Reset the entire CPU back to starting state
    void reset() {
        for(int i = 0; i < 8; i++) {
            registers[i].reset();
        }

        flags.resetAll();

        memory.reset();

        PC = 0;
        SI = 0;    
    }

    // ----------------------------------------------------------
    // BIT HELPERS (used by ROL, ROR, SHL, SHR)
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Convert a number into 8 individual bits
    //       bits[0] = LSB (least significant bit)
    //       bits[7] = MSB (most significant bit)
    void toBits(int value, int bits[8]) {

        unsigned char x = static_cast<unsigned char>(value);

        for(int i = 0; i < 8; i++) {
            bits[i] = (x >> i) & 1;
        }
    }

    // Author: Student C
    // DESC: Convert 8 individual bits back into a number
    int fromBits(int bits[8]) {

        unsigned char value = 0;

        for(int i = 0; i < 8; i++) {
            value |= (bits[i] << i);
        }

        return static_cast<signed char>(value);
    }

    // Author: Student C
    // DESC: Rotate all bits left by count positions
    //       The leftmost bit wraps around to the right
    int rotateLeft(int value, int count) {

        int bits[8];

        toBits(value, bits);

        count %= 8;

        for(int c = 0; c < count; c++) {

            int msb = bits[7];

            for(int i = 7; i > 0; i--) {
                bits[i] = bits[i - 1];
            }

            bits[0] = msb;
        }

        return fromBits(bits);
    }

    // Author: Student C
    // DESC: Rotate all bits right by count positions
    //       The rightmost bit wraps around to the left
    int rotateRight(int value, int count) {

        int bits[8];

        toBits(value, bits);

        count %= 8;

        for(int c = 0; c < count; c++) {

            int lsb = bits[0];

            for(int i = 0; i < 7; i++) {
                bits[i] = bits[i + 1];
            }

            bits[7] = lsb;
        }

        return fromBits(bits);
    }

    // Author: Student C
    // DESC: Shift all bits left by count positions
    //       Empty spots on the right are filled with 0
    int shiftLeft(int value, int count) {

        int bits[8];

        toBits(value, bits);

        count %= 8;

        for(int c = 0; c < count; c++) {

            for(int i = 7; i > 0; i--) {
                bits[i] = bits[i - 1];
            }

            bits[0] = 0;
        }

        return fromBits(bits);
    }

    // Author: Student C
    // DESC: Shift all bits right by count positions
    //       Empty spots on the left are filled with 0
    int shiftRight(int value, int count) {

        int bits[8];

        toBits(value, bits);

        count %= 8;

        for(int c = 0; c < count; c++) {

            for(int i = 0; i < 7; i++) {
                bits[i] = bits[i + 1];
            }

            bits[7] = 0;
        }

        return fromBits(bits);
    }

    // ----------------------------------------------------------
    // DISPLAY AND OUTPUT
    // ----------------------------------------------------------

    // Author: Student C
    // DESC: Print all register values and flags to the screen
    void displayState() {

        cout << "Registers:" << endl;

        for(int i = 0; i < 8; i++) {
            cout << "R" << i << " = "
                << registers[i].getValue()
                << endl;
        }

        cout << "PC = " << PC << endl;

        cout << "Flags:" << endl;

        cout << "OF = " << flags.getOF() << endl;
        cout << "UF = " << flags.getUF() << endl;
        cout << "CF = " << flags.getCF() << endl;
        cout << "ZF = " << flags.getZF() << endl;
    }

    // Author: Student C
    // DESC: Build the #Registers# line for the output file
    string buildRegistersLine() {
    string result = "#Registers#";

    for (int i = 0; i < 8; i++) {
        int v = registers[i].getValue();

        // keep within display range (optional safety)
        if (v < 0) v = 0;

        char buffer[10];
        sprintf(buffer, "%04d", v);

        result += string(buffer) + "#";
    }

    return result;
}

    // Author: Student C
    // DESC: Build the #Flags# line for the output file
    string buildFlagsLine() {
    string result = "#Flags#";

    result += to_string(flags.getOF()) + "#";
    result += to_string(flags.getUF()) + "#";
    result += to_string(flags.getCF()) + "#";
    result += to_string(flags.getZF()) + "#";

    return result;
}

    // Author: Student C
    // DESC: Build the #Memory# section (8 rows of 8 values)
    string buildMemoryLines() {
    string result;
    int* mem = memory.getRaw();

    for (int row = 0; row < 8; row++) {
        result += "#";

        for (int col = 0; col < 8; col++) {
            int val = mem[row * 8 + col];

            char buffer[10];
            sprintf(buffer, "%04d", val);

            result += string(buffer) + "#";
        }

        result += "\n";
    }

    return result;
}

    // Author: Student C
    // DESC: Combine all output lines and write to file and screen
    void writeOutput(string filename) {

    ofstream out(filename);

    string regLine = buildRegistersLine();
    string flagLine = buildFlagsLine();
    string memLine = buildMemoryLines();

    string fullOutput;

    fullOutput += "#Begin#\n";
    fullOutput += regLine + "\n";
    fullOutput += flagLine + "\n";
    fullOutput += "PC=" + to_string(PC) + "\n";
    fullOutput += memLine;
    fullOutput += "#End#\n";

    // print to screen
    cout << fullOutput;

    // write to file
    if (out.is_open()) {
        out << fullOutput;
        out.close();
    } else {
        cout << "ERROR: Cannot open output file" << endl;
        exit(1);
    }
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
    int dest;
    int src;
    int immediate;
    bool isImmediate;
    bool isIndirect;
public:
    // Author: Student C
    // DESC: Constructor for MOV R0, 10 (immediate mode)
    MovInstruction(int dest, int imm) {
        dest = d;
        immediate = imm;
        isImmediate = true;
        isIndirect = false;
        src = 0;    
    }

    // Author: Student C
    // DESC: Constructor for MOV R0, R1 or MOV R0, [R1]
    MovInstruction(int dest, int src, bool indirect) {
        dest = d;
        src = s;
        isImmediate = false;
        isIndirect = indirect;
        immediate = 0;
    }

    // Author: Student C
    // DESC: Execute the MOV instruction on the CPU
    void execute(CPU& cpu) override {
        if (isImmediate) {
            cpu.setRegValue(dest, immediate);
        }
        else if (isIndirect) {
            int addr = cpu.getRegValue(src);
            cpu.setRegValue(dest, cpu.memRead(addr));
        }
        else {
            cpu.setRegValue(dest, cpu.getRegValue(src));
        }
    }

    // Author: Student C
    // DESC: Return MOV instruction as text
    string toString() override {
        if (isImmediate)
            return "MOV R" + to_string(dest) + ", " + to_string(immediate);
        if (isIndirect)
            return "MOV R" + to_string(dest) + ", [R" + to_string(src) + "]";
        return "MOV R" + to_string(dest) + ", R" + to_string(src);
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
    int dest;

public:
    // Author: Student C
    InputInstruction(int d) {
        dest = d;
    }
    // Author: Student C
    // DESC: Show "?" prompt, read number, check flags, store in register
    void execute(CPU& cpu) override {
        int value;
        cout << "? ";
        cin >> value;

        cpu.getFlags().updateFlags(value);
        cpu.setRegValue(dest, value);
    }

    // Author: Student C
    string toString() override {
        return "INPUT R" + to_string(dest);
    }
};


// ============================================================
// CLASS : DisplayInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles DISPLAY Rx
//         Prints the value of a register to the screen
// ============================================================
class DisplayInstruction : public Instruction {
private:
    int src;

public:
    // Author: Student C
    DisplayInstruction(int s) {
        src = s;
    }

    // Author: Student C
    // DESC: Print the value of the register to screen
    void execute(CPU& cpu) override {
        cout << cpu.getRegValue(src) << endl;
    }

    // Author: Student C
    string toString() override {
        return "DISPLAY R" + to_string(src);
    }
};


// ============================================================
// CLASS : AddInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles ADD Rd, Rs  and  ADD Rd, number
//         Adds source to destination and stores result in destination
// ============================================================
class AddInstruction : public Instruction {
private:
    int dest;
    int src;
    int immediate;
    bool isImmediate;
public:
    // Author: Student C
    AddInstruction(int d, int s, bool imm) {
        dest = d;
        src = s;
        immediate = s;
        isImmediate = imm;
    }

    // Author: Student C
    // DESC: result = dest + src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        int result;

        if (isImmediate)
            result = cpu.getRegValue(dest) + immediate;
        else
            result = cpu.getRegValue(dest) + cpu.getRegValue(src);

        cpu.setRegValue(dest, result);
    }

    // Author: Student C
    string toString() override {
        if (isImmediate)
            return "ADD R" + to_string(dest) + ", " + to_string(immediate);
        return "ADD R" + to_string(dest) + ", R" + to_string(src);
    }
};


// ============================================================
// CLASS : SubInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles SUB Rd, Rs  and  SUB Rd, number
//         Subtracts source from destination
// ============================================================
class SubInstruction : public Instruction {
private:
    int dest;
    int src;
    int immediate;
    bool isImmediate;

public:
    // Author: Student C
    SubInstruction(int d, int s, bool imm) {
        dest = d;
        src = s;
        immediate = s;
        isImmediate = imm;
    }

    // Author: Student C
    // DESC: result = dest - src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        int result;

        if (isImmediate)
            result = cpu.getRegValue(dest) - immediate;
        else
            result = cpu.getRegValue(dest) - cpu.getRegValue(src);

        cpu.setRegValue(dest, result);
    }

    // Author: Student C
     string toString() override {
        if (isImmediate)
            return "SUB R" + to_string(dest) + ", " + to_string(immediate);
        return "SUB R" + to_string(dest) + ", R" + to_string(src);
    }
};


// ============================================================
// CLASS : LoadInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles LOAD Rd, [address]  and  LOAD Rd, [Rs]
//         Reads a value from memory into a register
// ============================================================
class LoadInstruction : public Instruction {
private:
    int dest;
    int address;
    int addrReg;
    bool useReg;

public:
    // Author: Student C
    // useReg=true means LOAD Rd, [Rs]
    // useReg=false means LOAD Rd, [20]
    LoadInstruction(int d, int addr, bool useR) {
        dest = d;
        address = addr;
        addrReg = addr;
        useReg = useR;
    }

    // Author: Student C
    // DESC: Get address (from register or fixed), read memory, store in dest
    void execute(CPU& cpu) override {
        int addr = useReg ? cpu.getRegValue(addrReg) : address;
        cpu.setRegValue(dest, cpu.memRead(addr));
    }

    // Author: Student C
    string toString() override {
        if (useReg)
            return "LOAD R" + to_string(dest) + ", [R" + to_string(addrReg) + "]";
        return "LOAD R" + to_string(dest) + ", [" + to_string(address) + "]";
    }
};


// ============================================================
// CLASS : StoreInstruction
// AUTHOR: Student C - [Full Name]
// DESC  : Handles STORE Rx, address  and  STORE [Rx], Rs
//         Writes a register value into memory
// ============================================================
class StoreInstruction : public Instruction {
private:
    int srcReg;
    int address;
    int addrReg;
    bool useReg;

public:
    // Author: Student C
    // useReg=true means STORE [Rd], Rs
    // useReg=false means STORE Rs, 43
    StoreInstruction(int src, int addr, bool useR) {
        srcReg = src;
        address = addr;
        addrReg = addr;
        useReg = useR;
    }

    // Author: Student C
    // DESC: Get address (from register or fixed), write register to memory
    void execute(CPU& cpu) override {
        int addr = useReg ? cpu.getRegValue(addrReg) : address;
        cpu.memWrite(addr, cpu.getRegValue(srcReg));
    }

    // Author: Student C
    string toString() override {
        if (useReg)
            return "STORE [R" + to_string(addrReg) + "], R" + to_string(srcReg);
        return "STORE R" + to_string(srcReg) + ", " + to_string(address);
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 7 : ADVANCED INSTRUCTIONS
// ASSIGNED TO : Student D
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : MulInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles MUL Rd, Rs  and  MUL Rd, number
//         Multiplies destination by source
// ============================================================
class MulInstruction : public Instruction {
private:
    // TODO Student D: int dest, src, immediate, bool isImmediate

public:
    // Author: Student D
    MulInstruction(int d, int s, bool imm) {
        // TODO Student D: store d, s, imm
    }

    // Author: Student D
    // DESC: result = dest * src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        // TODO Student D: calculate result, cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO */ return ""; }
};


// ============================================================
// CLASS : DivInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles DIV Rd, Rs  and  DIV Rd, number
//         Divides destination by source
//         IMPORTANT: must print error and exit if dividing by 0
// ============================================================
class DivInstruction : public Instruction {
private:
    // TODO Student D: int dest, src, immediate, bool isImmediate

public:
    // Author: Student D
    DivInstruction(int d, int s, bool imm) {
        // TODO Student D: store d, s, imm
    }

    // Author: Student D
    // DESC: result = dest / src (or immediate), store back to dest
    //       MUST check for divide by zero before dividing
    void execute(CPU& cpu) override {
        // TODO Student D:
        // get divisor (from register or immediate)
        // if divisor == 0: print "ERROR: Division by zero" and exit
        // result = dest / divisor
        // cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO */ return ""; }
};


// ============================================================
// CLASS : IncInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles INC Rx
//         Adds 1 to the register value
// ============================================================
class IncInstruction : public Instruction {
private:
    // TODO Student D: int dest

public:
    // Author: Student D
    IncInstruction(int d) { /* TODO: dest = d */ }

    // Author: Student D
    // DESC: dest = dest + 1
    void execute(CPU& cpu) override {
        // TODO Student D: cpu.setRegValue(dest, cpu.getRegValue(dest) + 1)
    }

    // Author: Student D
    string toString() override { /* TODO: return "INC Rx" */ return ""; }
};


// ============================================================
// CLASS : DecInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles DEC Rx
//         Subtracts 1 from the register value
// ============================================================
class DecInstruction : public Instruction {
private:
    // TODO Student D: int dest

public:
    // Author: Student D
    DecInstruction(int d) { /* TODO: dest = d */ }

    // Author: Student D
    // DESC: dest = dest - 1
    void execute(CPU& cpu) override {
        // TODO Student D: cpu.setRegValue(dest, cpu.getRegValue(dest) - 1)
    }

    // Author: Student D
    string toString() override { /* TODO: return "DEC Rx" */ return ""; }
};


// ============================================================
// CLASS : RolInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles ROL Rx, count
//         Rotates bits in register left by count positions
//         Leftmost bit wraps around to the rightmost position
// ============================================================
class RolInstruction : public Instruction {
private:
    // TODO Student D: int dest, count

public:
    // Author: Student D
    RolInstruction(int d, int c) { /* TODO: dest=d, count=c */ }

    // Author: Student D
    // DESC: Rotate register bits left, store result back
    void execute(CPU& cpu) override {
        // TODO Student D:
        // int result = cpu.rotateLeft(cpu.getRegValue(dest), count)
        // cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO: return "ROL Rx, count" */ return ""; }
};


// ============================================================
// CLASS : RorInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles ROR Rx, count
//         Rotates bits in register right by count positions
//         Rightmost bit wraps around to the leftmost position
// ============================================================
class RorInstruction : public Instruction {
private:
    // TODO Student D: int dest, count

public:
    // Author: Student D
    RorInstruction(int d, int c) { /* TODO: dest=d, count=c */ }

    // Author: Student D
    // DESC: Rotate register bits right, store result back
    void execute(CPU& cpu) override {
        // TODO Student D:
        // int result = cpu.rotateRight(cpu.getRegValue(dest), count)
        // cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO: return "ROR Rx, count" */ return ""; }
};


// ============================================================
// CLASS : ShlInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles SHL Rx, count
//         Shifts bits left by count positions
//         Empty spots on the right are filled with 0
// ============================================================
class ShlInstruction : public Instruction {
private:
    // TODO Student D: int dest, count

public:
    // Author: Student D
    ShlInstruction(int d, int c) { /* TODO: dest=d, count=c */ }

    // Author: Student D
    // DESC: Shift register bits left, fill with 0, store result back
    void execute(CPU& cpu) override {
        // TODO Student D:
        // int result = cpu.shiftLeft(cpu.getRegValue(dest), count)
        // cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO: return "SHL Rx, count" */ return ""; }
};


// ============================================================
// CLASS : ShrInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles SHR Rx, count
//         Shifts bits right by count positions
//         Empty spots on the left are filled with 0
// ============================================================
class ShrInstruction : public Instruction {
private:
    // TODO Student D: int dest, count

public:
    // Author: Student D
    ShrInstruction(int d, int c) { /* TODO: dest=d, count=c */ }

    // Author: Student D
    // DESC: Shift register bits right, fill with 0, store result back
    void execute(CPU& cpu) override {
        // TODO Student D:
        // int result = cpu.shiftRight(cpu.getRegValue(dest), count)
        // cpu.setRegValue(dest, result)
    }

    // Author: Student D
    string toString() override { /* TODO: return "SHR Rx, count" */ return ""; }
};


// ============================================================
// CLASS : PushInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles PUSH Rx
//         Copies register value onto the top of the stack
//         Stack Index (SI) increases by 1
// ============================================================
class PushInstruction : public Instruction {
private:
    // TODO Student D: int src

public:
    // Author: Student D
    PushInstruction(int s) { /* TODO: src = s */ }

    // Author: Student D
    // DESC: Push register value onto VM stack
    void execute(CPU& cpu) override {
        // TODO Student D: cpu.stackPush(cpu.getRegValue(src))
    }

    // Author: Student D
    string toString() override { /* TODO: return "PUSH Rx" */ return ""; }
};


// ============================================================
// CLASS : PopInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles POP Rx
//         Removes top value from stack and stores in register
//         Stack Index (SI) decreases by 1
//         IMPORTANT: crashes if stack is empty
// ============================================================
class PopInstruction : public Instruction {
private:
    // TODO Student D: int dest

public:
    // Author: Student D
    PopInstruction(int d) { /* TODO: dest = d */ }

    // Author: Student D
    // DESC: Pop value from VM stack into register
    //       MyStack.pop() already handles empty stack error
    void execute(CPU& cpu) override {
        // TODO Student D: cpu.setRegValue(dest, cpu.stackPop())
    }

    // Author: Student D
    string toString() override { /* TODO: return "POP Rx" */ return ""; }
};


// ============================================================
// CLASS : ResetInstruction
// AUTHOR: Student D - [Full Name]
// DESC  : Handles RESET OF / RESET UF / RESET CF / RESET ZF
//         Resets one specific flag back to 0
// ============================================================
class ResetInstruction : public Instruction {
private:
    // TODO Student D: string flagName

public:
    // Author: Student D
    ResetInstruction(string name) { /* TODO: flagName = name */ }

    // Author: Student D
    // DESC: Reset the named flag to 0
    void execute(CPU& cpu) override {
        // TODO Student D: cpu.resetFlag(flagName)
    }

    // Author: Student D
    string toString() override { /* TODO: return "RESET flagName" */ return ""; }
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

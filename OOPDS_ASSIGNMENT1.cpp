// ============================================================
// CCP6124 Object-Oriented Programming and Data Structures
// Trimester 2610
// Virtual Machine and Assembly Language Interpreter
// File: OOPDS_ASIGNMENT1.cpp
// Group: 6

// ============================================================
// GROUP MEMBERS:
//   Student A - [YAP SOON CHEE] - [253UC256GH]
//   Student B - [YIP YU ZHE] - [252UC242YP]
//   Student C - [LOH ZI JIAN] - [252UC2421W]
//   Student D - [MANESH MATHIALAGEN] - [253UC256FC]
// ============================================================
// HOW TO COMPILE:
//   g++ OOPDS_ASSIGNMENT1.cpp -o vm
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
#include <cstdint>

using namespace std;


// ============================================================
// SECTION 1 : CUSTOM DATA STRUCTURES
// ASSIGNED TO : Student B
// ============================================================

// ============================================================
// CLASS : MyVector
// AUTHOR: Student B - YIP YU ZHE
// DESC  : A simple fixed array that stores up to 256 strings.
//         Used to store lines read from the .asm file.
//         Replaces std::vector (no STL allowed).
// ============================================================
template <typename T>
class MyVector {
private:
    T   data[256];
    int size;

public:
    // Constructor - set size to 0
    MyVector() {
        size = 0;
    }

    // Add a new item to the end of the array
    void push_back(T item) {
        if (size >= 256) {
            cout << "ERROR: Too many instructions (max 256)." << endl;
            exit(1);
        }
        data[size] = item;
        size++;
    }

    // Get the item at a given index position
    T get(int index) {
        if (index < 0 || index >= size) {
            cout << "ERROR: Index out of range." << endl;
            exit(1);
        }
        return data[index];
    }

    // Returns how many items are stored
    int getSize() {
        return size;
    }

    // Remove all items
    void clear() {
        size = 0;
    }
};


// ============================================================
// CLASS : MyStack
// AUTHOR: Student B - YIP YU ZHE
// DESC  : A simple Last-In-First-Out stack with max 8 slots.
//         Used for PUSH and POP instructions in the VM.
//         Replaces std::stack (no STL allowed).
// ============================================================
template <typename T>
class MyStack {
private:
    T   data[8];
    int top;

public:
    // Constructor - set top to -1 meaning empty
    MyStack() {
        top = -1;
    }

    // Push a value onto the top of the stack
    void push(T value) {
        if (top >= 7) {
            cout << "ERROR: Stack is full. Cannot push." << endl;
            exit(1);
        }
        top++;
        data[top] = value;
    }

    // Remove and return the top value from the stack
    T pop() {
        if (top < 0) {
            cout << "ERROR: Stack is empty. Program crashed." << endl;
            exit(1);
        }
        T value = data[top];
        top--;
        return value;
    }

    // Returns true if the stack has no items
    bool isEmpty() {
        return top < 0;
    }

    // Returns how many items are in the stack
    int getSize() {
        return top + 1;
    }
};


// ============================================================
// CLASS : MyQueue
// AUTHOR: Student B - YIP YU ZHE
// DESC  : A simple First-In-First-Out queue with max 256 slots.
//         Used as an alternative way to store the program.
//         Replaces std::queue (no STL allowed).
// ============================================================
template <typename T>
class MyQueue {
private:
    T   data[256];
    int front;
    int rear;
    int size;

public:
    // Constructor - set front, rear, size all to 0
    MyQueue() {
        front = 0;
        rear  = 0;
        size  = 0;
    }

    // Add an item to the back of the queue
    void enqueue(T item) {
        if (size >= 256) {
            cout << "ERROR: Queue is full." << endl;
            exit(1);
        }
        data[rear] = item;
        rear = (rear + 1) % 256;
        size++;
    }

    // Remove and return item from the front of the queue
    T dequeue() {
        if (size == 0) {
            cout << "ERROR: Queue is empty." << endl;
            exit(1);
        }
        T item = data[front];
        front = (front + 1) % 256;
        size--;
        return item;
    }

    // Returns true if queue has no items
    bool isEmpty() {
        return size == 0;
    }

    // Returns how many items are in the queue
    int getSize() {
        return size;
    }
};

// ============================================================
// SECTION 2 : REGISTER CLASSES
// ASSIGNED TO : Student B
// ============================================================

// ============================================================
// CLASS : Register
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : Base class for all register types.
//         Holds one integer value.
//         GeneralRegister inherits from this class.
// ============================================================
class Register {
protected:
    int8_t value;

public:
    // Constructor - set value to 0
    Register() {
        value = 0;
    }

    // Returns the current value stored in this register
    int getValue() {
        return (int)value;
    }

    // Resets register value back to 0
    void reset() {
        value = 0;
    }

    // Sets the value 
    virtual void setValue(int v) {
        value = (int8_t)v;
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
    // Sets the value but clamps it to -128-127 range and overrides the parent Register's setValue
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
    // Constructor - set all flags to 0
    FlagRegister() {
        OF = 0;
        UF = 0;
        CF = 0;
        ZF = 0;
    }

    // Getters for each flag
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

    // Setters for each flag
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

    // Set all flags back to 0
    void resetAll(){
        OF = 0;
        UF = 0;
        CF = 0;
        ZF = 0;
    }

    // Automatically update all flags based on a result value and called after every arithmetic operation
    // OF = 1 if result > 127
    // UF = 1 if result < -128
    // ZF = 1 if result == 0
    // CF = 1 if result > 127 or result < -128
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
        if (result > 127 || result < -128)
            CF = 1;
        else
            CF = 0;
    }

    // Reset one specific flag to 0 by its name and valid names: "OF", "UF", "CF", "ZF"
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


// ============================================================
// SECTION 3 : MEMORY CLASS
// ASSIGNED TO : Student B
// ============================================================

// ============================================================
// CLASS : Memory
// AUTHOR: Student B - [YIP YU ZHE]
// DESC  : 64 slots of storage, addressed from 0 to 63.
//         Each slot holds one integer value.
//         Only LOAD and STORE instructions can access memory.
// ============================================================
class Memory {
private:
    int8_t mem[64];   // 64 memory slots, each holds one number

public:
    // Constructor - fill all 64 slots with 0
    Memory() {
        for (int i = 0; i < 64; i++) {
            mem[i] = 0;
        }
    }

    // Read and return value at given memory address and print error and exit if address is not 0 to 63
    int read(int address) {
        if (address < 0 || address > 63) {
            cout << "ERROR: Memory address " << address
                 << " is out of range. Must be 0 to 63." << endl;
            exit(1);
        }
        return (int)mem[address];
    }

    // Write a value to a given memory address and print error and exit if address is not 0 to 63
    void write(int address, int value) {
        if (address < 0 || address > 63) {
            cout << "ERROR: Memory address " << address
                 << " is out of range. Must be 0 to 63." << endl;
            exit(1);
        }
        mem[address] = (int8_t)value;
    }

    // Returns the raw memory array 
    int8_t* getRaw() {
        return mem;
    }

    // Clear all memory slots back to 0
    void reset() {
        for (int i = 0; i < 64; i++) {
            mem[i] = 0;
        }
    }
};

//intToString is a simple help function
string intToString(int n) {
    char buf[20];
    sprintf(buf, "%d", n);
    return string(buf);
} 
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
    Memory memory;                  // COMPOSITION - CPU owns Memory
    GeneralRegister* registers;     // AGGREGATION - passed from outside
    FlagRegister* flags;            // AGGREGATION - passed from outside
    MyStack<int>* vmStack;          // AGGREGATION - passed from outside
    int8_t PC;
    int8_t SI;

public:
    // Author: Student C
    // Start PC and SI at 0
    CPU(GeneralRegister* r, FlagRegister* f, MyStack<int>* s) {
        registers = r;
        flags = f;
        vmStack = s;
        PC = 0;
        SI = 0;
    }

    // ----------------------------------------------------------
    // REGISTER ACCESS
    // ----------------------------------------------------------

    // Get value from a register
    int getRegValue(int n) {
        return registers[n].getValue();
    }

    //Save value and update flags
    void setRegValue(int n, int v) {
        flags->updateFlags(v);
        registers[n].setValue(v);
    }

    // Clear one flag
    void resetFlag(string name) {
        flags->resetOne(name);
    }

    // Read from memory
    int memRead(int address) {
        return memory.read(address);
    }

    // Write to memory
    void memWrite(int address, int value) {
        memory.write(address, value); 
    }

    //Get program counter
    int getPC() {
        return (int)PC;
    }

    //Go to next instrution
    void incrementPC() {
        PC++;
    }

    //Push value onto stack
    void stackPush(int value) {
        vmStack->push(value);
        SI++;
    }

    //Pop value from stack
    int stackPop() {
        SI--;
        return vmStack->pop();
    }
    //Get stack index
    int getSI() {
        return (int)SI;
    }

    // Reset everything
    void reset() {
        for(int i = 0; i < 8; i++) {
            registers[i].reset();
        }

        flags->resetAll();

        memory.reset();

        PC = 0;
        SI = 0;    
    }

    //Split numer into 8 bits
    void toBits(int value, int bits[8]) {
        unsigned char x = (unsigned char)value;
        for (int i = 0; i < 8; i++)
            bits[i] = (x >> i) & 1;
    }

    //Join 8 bits into number
    int fromBits(int bits[8]) {
        unsigned char value = 0;
        for (int i = 0; i < 8; i++)
            value |= (bits[i] << i);
        return (signed char)value;
    }

    // Author: Student C
    //Rotate bits left
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
    //rotate bits right
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
    //Shift bits left
    int shiftLeft(int value, int count) {
        if (count >= 8) return 0;

        int bits[8];
        toBits(value, bits);

        for (int c = 0; c < count; c++) {
            for (int i = 7; i > 0; i--)
                bits[i] = bits[i - 1];
            bits[0] = 0;
        }

        return fromBits(bits);
    }

    // Author: Student C
    //Shift bits right
    int shiftRight(int value, int count) {
        if (count >= 8) return 0;

        int bits[8];
        toBits(value, bits);

        for (int c = 0; c < count; c++) {
            for (int i = 0; i < 7; i++)
                bits[i] = bits[i + 1];
            bits[7] = 0;
        }

        return fromBits(bits);
    }

    // ----------------------------------------------------------
    // DISPLAY AND OUTPUT
    // ----------------------------------------------------------

    // Author: Student C
    //Show registers and flags
    void displayState() {

        cout << "Registers:" << endl;

        for(int i = 0; i < 8; i++) {
            cout << "R" << i << " = "
                << registers[i].getValue()
                << endl;
        }

        cout << "PC = " << PC << endl;

        cout << "Flags:" << endl;

        cout << "OF = " << flags->getOF() << endl;
        cout << "UF = " << flags->getUF() << endl;
        cout << "CF = " << flags->getCF() << endl;
        cout << "ZF = " << flags->getZF() << endl;
    }

    // Author: Student C
    //Make register and flags
    string buildRegistersLine() {
    string result = "#Registers#";

    for (int i = 0; i < 8; i++) {
        int v = registers[i].getValue();

        char buffer[12];
        if (v < 0)
            sprintf(buffer, "%d", v);      // e.g. "-5" as-is
        else
            sprintf(buffer, "%04d", v);    // e.g. "0005"
        result += string(buffer) + "#";
    }

    return result;
}

    // Author: Student C
    //Make flag output line
    string buildFlagsLine() {
        string result = "#Flags#";
        char buf[5];

        sprintf(buf, "%d", flags->getOF());
        result += "OF#" + string(buf) + "#";

        sprintf(buf, "%d", flags->getUF());
        result += "UF#" + string(buf) + "#";

        sprintf(buf, "%d", flags->getCF());
        result += "CF#" + string(buf) + "#";

        sprintf(buf, "%d", flags->getZF());
        result += "ZF#" + string(buf) + "#";

        return result;
    }

    // Author: Student C
    // memory output lines
    string buildMemoryLines() {
    string result;
    int8_t* mem = memory.getRaw();

    for (int row = 0; row < 8; row++) {
        result += "#";

        for (int col = 0; col < 8; col++) {
            int val = mem[row * 8 + col];

            char buffer[12];
            sprintf(buffer, "%04d", val);

            result += string(buffer) + "#";
        }

        result += "\n";
    }

    return result;
}

    // Author: Student C
    //Save output to file
    void writeOutput(string filename) {

    ofstream out(filename);

    string regLine = buildRegistersLine();
    string flagLine = buildFlagsLine();
    string memLine = buildMemoryLines();

    string fullOutput;

    fullOutput += "#Begin#\n";
    fullOutput += regLine + "\n";
    fullOutput += flagLine + "\n";
    char pcBuf[8];
    sprintf(pcBuf, "%04d", PC);
    fullOutput += "#PC#" + string(pcBuf) + "#\n";
    fullOutput += "#Memory#\n";
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
// AUTHOR: Student C - [LOH ZI JIAN]
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
// AUTHOR: Student C - [LOH ZI JIAN]
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
    MovInstruction(int d, int imm) {
        dest = d;
        immediate = imm;
        isImmediate = true;
        isIndirect = false;
        src = 0;    
    }
  

    // Author: Student C
    // DESC: Constructor for MOV R0, R1 or MOV R0, [R1]
    MovInstruction(int d, int s, bool indirect) {
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
            return "MOV R" + intToString(dest) + ", " + intToString(immediate);
        if (isIndirect)
            return "MOV R" + intToString(dest) + ", [R" + intToString(src) + "]";
        return "MOV R" + intToString(dest) + ", R" + intToString(src);
    }
};


// ============================================================
// CLASS : InputInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
    // DESC: Read number and store in register
    void execute(CPU& cpu) override {
        int value;
        cout << "? ";
        cin >> value;

        cpu.setRegValue(dest, value);
    }

    // Author: Student C
    string toString() override {
        return "INPUT R" + intToString(dest);
    }
};


// ============================================================
// CLASS : DisplayInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
        return "DISPLAY R" + intToString(src);
    }
};


// ============================================================
// CLASS : AddInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
        isImmediate = imm;
        if (imm) { 
            immediate = s; src = 0; 
        }
        else { 
            src = s; immediate = 0; 
        }
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
            return "ADD R" + intToString(dest) + ", " + intToString(immediate);
        return "ADD R" + intToString(dest) + ", R" + intToString(src);
    }
};


// ============================================================
// CLASS : SubInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
    isImmediate = imm;
    if (imm) { 
        immediate = s; src = 0; 
    }
    else { 
        src = s; immediate = 0; 
    }
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
            return "SUB R" + intToString(dest) + ", " + intToString(immediate);
        return "SUB R" + intToString(dest) + ", R" + intToString(src);
    }
};


// ============================================================
// CLASS : LoadInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
            return "LOAD R" + intToString(dest) + ", [R" + intToString(addrReg) + "]";
        return "LOAD R" + intToString(dest) + ", [" + intToString(address) + "]";
    }
};


// ============================================================
// CLASS : StoreInstruction
// AUTHOR: Student C - [LOH ZI JIAN]
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
        useReg = useR;
        if (useR) {
            addrReg = addr; address = 0; 
        }
        else {
            address = addr; addrReg = 0; 
        }
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
            return "STORE [R" + intToString(addrReg) + "], R" + intToString(srcReg);
        return "STORE R" + intToString(srcReg) + ", " + intToString(address);
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 7 : ADVANCED INSTRUCTIONS
// ASSIGNED TO : Student D
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ============================================================
// CLASS : MulInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
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
    // Author: Student D
    MulInstruction(int d, int s, bool imm) {
        dest = d;
        isImmediate = imm;
        if (isImmediate) {
            immediate = s;
        } else {
            src = s;
        }
    }

    // Author: Student D
    // DESC: result = dest * src (or immediate), store back to dest
    void execute(CPU& cpu) override {
        int val1 = cpu.getRegValue(dest);
        int val2 = isImmediate ? immediate : cpu.getRegValue(src);
        cpu.setRegValue(dest, val1 * val2);
    }

    // Author: Student D
    string toString() override { 
        return "MUL R" + to_string(dest) + ", " + (isImmediate ? to_string(immediate) : "R" + to_string(src)); 
    }
};


// ============================================================
// CLASS : DivInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
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
    // Author: Student D
    DivInstruction(int d, int s, bool imm) {
        dest = d;
        isImmediate = imm;
        if (isImmediate) {
            immediate = s;
        } else {
            src = s;
        }
    }

    // Author: Student D
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

    // Author: Student D
    string toString() override { 
        return "DIV R" + to_string(dest) + ", " + (isImmediate ? to_string(immediate) : "R" + to_string(src)); 
    }
};


// ============================================================
// CLASS : IncInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles INC Rx
//         Adds 1 to the register value
// ============================================================
class IncInstruction : public Instruction {
private:
    int dest;

public:
    // Author: Student D
    IncInstruction(int d) { 
        dest = d; 
    }

    // Author: Student D
    // DESC: dest = dest + 1
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.getRegValue(dest) + 1);
    }

    // Author: Student D
    string toString() override { 
        return "INC R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : DecInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles DEC Rx
//         Subtracts 1 from the register value
// ============================================================
class DecInstruction : public Instruction {
private:
    int dest;

public:
    // Author: Student D
    DecInstruction(int d) { 
        dest = d; 
    }

    // Author: Student D
    // DESC: dest = dest - 1
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.getRegValue(dest) - 1);
    }

    // Author: Student D
    string toString() override { 
        return "DEC R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : RolInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles ROL Rx, count
//         Rotates bits in register left by count positions
//         Leftmost bit wraps around to the rightmost position
// ============================================================
class RolInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: Student D
    RolInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: Student D
    // DESC: Rotate register bits left, store result back
    void execute(CPU& cpu) override {
        int result = cpu.rotateLeft(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: Student D
    string toString() override { 
        return "ROL R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : RorInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles ROR Rx, count
//         Rotates bits in register right by count positions
//         Rightmost bit wraps around to the leftmost position
// ============================================================
class RorInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: Studnet D
    RorInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: Student D
    // DESC: Rotate register bits right, store result back
    void execute(CPU& cpu) override {
        int result = cpu.rotateRight(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: Student D
    string toString() override { 
        return "ROR R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : ShlInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles SHL Rx, count
//         Shifts bits left by count positions
//         Empty spots on the right are filled with 0
// ============================================================
class ShlInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: Student D
    ShlInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: Student D
    // DESC: Shift register bits left, fill with 0, store result back
    void execute(CPU& cpu) override {
        int result = cpu.shiftLeft(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: Student D
    string toString() override { 
        return "SHL R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : ShrInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles SHR Rx, count
//         Shifts bits right by count positions
//         Empty spots on the left are filled with 0
// ============================================================
class ShrInstruction : public Instruction {
private:
    int dest;
    int count;

public:
    // Author: Student D
    ShrInstruction(int d, int c) { 
        dest = d;
        count = c; 
    }

    // Author: Student D
    // DESC: Shift register bits right, fill with 0, store result back
    void execute(CPU& cpu) override {
        int result = cpu.shiftRight(cpu.getRegValue(dest), count);
        cpu.setRegValue(dest, result);
    }

    // Author: Student D
    string toString() override { 
        return "SHR R" + to_string(dest) + ", " + to_string(count); 
    }
};


// ============================================================
// CLASS : PushInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles PUSH Rx
//         Copies register value onto the top of the stack
//         Stack Index (SI) increases by 1
// ============================================================
class PushInstruction : public Instruction {
private:
    int src;

public:
    // Author: Student D
    PushInstruction(int s) { 
        src = s; 
    }

    // Author: Student D
    // DESC: Push register value onto VM stack
    void execute(CPU& cpu) override {
        cpu.stackPush(cpu.getRegValue(src));
    }

    // Author: Student D
    string toString() override { 
        return "PUSH R" + to_string(src); 
    }
};


// ============================================================
// CLASS : PopInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles POP Rx
//         Removes top value from stack and stores in register
//         Stack Index (SI) decreases by 1
//         IMPORTANT: crashes if stack is empty
// ============================================================
class PopInstruction : public Instruction {
private:
    int dest;

public:
    // Author: Student D
    PopInstruction(int d) { 
        dest = d; 
    }

    // Author: Student D
    // DESC: Pop value from VM stack into register
    //       MyStack.pop() already handles empty stack error
    void execute(CPU& cpu) override {
        cpu.setRegValue(dest, cpu.stackPop());
    }

    // Author: Student D
    string toString() override { 
        return "POP R" + to_string(dest); 
    }
};


// ============================================================
// CLASS : ResetInstruction
// AUTHOR: Student D - [MANESH MATHIALAGEN]
// DESC  : Handles RESET OF / RESET UF / RESET CF / RESET ZF
//         Resets one specific flag back to 0
// ============================================================
class ResetInstruction : public Instruction {
private:
    string flagName;

public:
    // Author: Student D
    ResetInstruction(string name) { 
        flagName = name; 
    }

    // Author: Student D
    // DESC: Reset the named flag to 0
    void execute(CPU& cpu) override {
        cpu.resetFlag(flagName);
    }

    // Author: Student D
    string toString() override { 
        return "RESET " + flagName; 
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 8 : STRING HELPER FUNCTIONS
// ASSIGNED TO : Student A [soon chee]
// These are standalone functions used by the Runner to
// clean up and split text lines from the .asm file
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// Author: Student A
// DESC: Remove spaces and tabs from the start and end of a string
string trim(string s) {
    if (s.empty()) return s; 
    int start = 0;
    int end = (int)s.size() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t')) end--;
    return s.substr(start, end - start + 1);
}

// Author: Student A
// DESC: Convert all letters in a string to UPPERCASE
string toUpper(string s) {
    for (int i = 0; i < (int)s.size(); i++)
        s[i] = toupper(s[i]); 
        // TODO Student A: loop through each character, call toupper()
    return s;
}

// Author: Student A
// DESC: Remove anything after a semicolon (those are comments)
// Example: "MOV R0, 5 ; this is a comment" → "MOV R0, 5 "
string removeComment(string s) {
    int pos = s.find(';');
    if (pos != (int)string::npos)
        return s.substr(0, pos); 
        // TODO Student A: find ';', return everything before it
    return s;
}

// Author: Student A
// DESC: Check if a string is a register name like R0, R1 .. R7
bool isReg(string s) {
    return s.size() == 2 && s[0] == 'R' && isdigit(s[1]);
    // TODO Student A: check s[0]=='R' and s[1] is a digit

}

// Author: Student A
// DESC: Check if a string is an indirect register like [R0]
bool isIndReg(string s) {
    return s.size() == 4 && s[0] == '[' && s[1] == 'R' && isdigit(s[2]) && s[3] == ']';
    // TODO Student A: check s starts with '[', ends with ']', middle is R+digit

}

// Author: Student A
// DESC: Check if a string is an indirect number like [20]
bool isIndNum(string s) {
    if (s.size() < 3) return false;
    if (s[0] != '[' || s[s.size()-1] != ']') return false;
    string inner = s.substr(1, s.size()-2);
    for (int i = 0; i < (int)inner.size(); i++)
        if (!isdigit(inner[i])) return false;
    return inner.size() > 0; 
    // TODO Student A: check s starts with '[', ends with ']', middle is digit

}

// Author: Student A
// DESC: Remove the [ and ] brackets from a string
// Example: "[R2]" → "R2",   "[20]" → "20"
string removeBrackets(string s) {
    return s.substr(1, s.size() - 2); 
    // TODO Student A: return s without first and last character

}

// Author: Student A
// DESC: Get the register number from a string like "R3" → returns 3
int getRegNum(string s) {
    if (!isReg(s)) {
        cout << "ERROR: Not a valid register: " << s << endl;
        exit(1);
    }
    return atoi(s.substr(1).c_str());
}

// Author: Student A
// DESC: Split a string into two parts at the first comma
// Example: "R0, R1" → left="R0", right="R1"
void splitTwo(string s, char delim, string& left, string& right) {
    int pos = s.find(delim);
    left  = trim(s.substr(0, pos));
    right = trim(s.substr(pos + 1)); 
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
// AUTHOR: Student A - [soon chee]
// DESC  : Reads the .asm file, parses each line into an
//         Instruction object, and executes them on the CPU.
//         This is the main controller of the whole system.
// ============================================================
class Runner {
private:
    GeneralRegister  registers[8];  // created here, outside CPU
    FlagRegister     flags;         // created here, outside CPU
    MyStack<int>     vmStack;       // created here, outside CPU
    CPU              cpu;
    MyVector<string> instructions;
    MyQueue<string>  programQueue;
    string           outputFile;

public:
    Runner() : cpu(registers, &flags, &vmStack) {}


    // Author: Student A
    // DESC: Parse MOV instruction (all 3 modes)
    Instruction* parseMov(string rest) {
        string left, right;
        splitTwo(rest, ',', left, right);
        int dest = getRegNum(left);
        if (isReg(right))
            return new MovInstruction(dest, getRegNum(right), false);
        if (isIndReg(right))
            return new MovInstruction(dest, getRegNum(removeBrackets(right)), true);
        return new MovInstruction(dest, atoi(right.c_str())); 

    }

    // Author: Student A
    // DESC: Parse ADD, SUB, MUL, DIV instructions
    Instruction* parseArith(string opcode, string rest) {
        string left, right;
        splitTwo(rest, ',', left, right);
        int dest = getRegNum(left);
        bool isImm = !isReg(right);
        int val = isImm ? atoi(right.c_str()) : getRegNum(right);
        if (opcode == "ADD") return new AddInstruction(dest, val, isImm);
        if (opcode == "SUB") return new SubInstruction(dest, val, isImm);
        if (opcode == "MUL") return new MulInstruction(dest, val, isImm);
        if (opcode == "DIV") return new DivInstruction(dest, val, isImm);

        return nullptr;
    }

    // Author: Student A
    // DESC: Parse ROL, ROR, SHL, SHR instructions
    Instruction* parseShift(string opcode, string rest) {
        string left, right;
        splitTwo(rest, ',', left, right);
        int dest  = getRegNum(left);
        int count = atoi(right.c_str());
        if (opcode == "ROL") return new RolInstruction(dest, count);
        if (opcode == "ROR") return new RorInstruction(dest, count);
        if (opcode == "SHL") return new ShlInstruction(dest, count);
        if (opcode == "SHR") return new ShrInstruction(dest, count);

        return nullptr;
    }

    // Author: Student A
    // DESC: Parse LOAD and STORE instructions
    Instruction* parseMemory(string opcode, string rest) {
        string left, right;
        splitTwo(rest, ',', left, right);

        if (opcode == "LOAD") {
            int dest = getRegNum(left);
            if (isIndReg(right))
                return new LoadInstruction(dest, getRegNum(removeBrackets(right)), true);
            // isIndNum
            return new LoadInstruction(dest, atoi(removeBrackets(right).c_str()), false);
        }
        // STORE
        if (isIndReg(left)) {
            // STORE [Rx], Rs
            int addrReg = getRegNum(removeBrackets(left));
            int srcReg  = getRegNum(right);
            return new StoreInstruction(srcReg, addrReg, true);
        }
        if (isReg(left)) {
            int srcReg = getRegNum(left);
            // NEW: STORE Rx, [Ry]
            if (isIndReg(right)) {
                int addrReg = getRegNum(removeBrackets(right));
                return new StoreInstruction(srcReg, addrReg, true);
            }
            // STORE Rx, 43
            int addr = atoi(right.c_str());
            return new StoreInstruction(srcReg, addr, false);
        }
        // STORE 20, Rs
        int addr   = atoi(left.c_str());
        int srcReg = getRegNum(right);
        return new StoreInstruction(srcReg, addr, false);
    }
        


    // Author: Student A
    // DESC: Parse PUSH, POP, RESET, INPUT, DISPLAY, INC, DEC instructions
    Instruction* parseSimple(string opcode, string rest) {
        rest = trim(rest);
        if (opcode == "PUSH")    return new PushInstruction(getRegNum(rest));
        if (opcode == "POP")     return new PopInstruction(getRegNum(rest));
        if (opcode == "INC")     return new IncInstruction(getRegNum(rest));
        if (opcode == "DEC")     return new DecInstruction(getRegNum(rest));
        if (opcode == "INPUT")   return new InputInstruction(getRegNum(rest));
        if (opcode == "DISPLAY") return new DisplayInstruction(getRegNum(rest));
        if (opcode == "RESET")   return new ResetInstruction(rest);

        return nullptr;
    }

    // Author: Student A
    // DESC: Parse one full line of assembly text into an Instruction object
    //       Calls the helper parse functions above
    Instruction* parseLine(string line, int lineNum) {
        line = trim(removeComment(line));
        if (line.empty()) return nullptr;

        // split opcode from rest
        string opcode, rest;
        int spacePos = line.find(' ');
        if (spacePos == (int)string::npos) {
            opcode = line;
            rest   = "";
        } else {
            opcode = trim(line.substr(0, spacePos));
            rest   = trim(line.substr(spacePos + 1));
        }
        opcode = toUpper(opcode);

        if (opcode == "MOV")
            return parseMov(rest);
        if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL" || opcode == "DIV")
            return parseArith(opcode, rest);
        if (opcode == "ROL" || opcode == "ROR" || opcode == "SHL" || opcode == "SHR")
            return parseShift(opcode, rest);
        if (opcode == "LOAD" || opcode == "STORE")
            return parseMemory(opcode, rest);

        // everything else (PUSH, POP, INC, DEC, INPUT, DISPLAY, RESET)
        Instruction* instr = parseSimple(opcode, rest);
        if (instr == nullptr) {
            cout << "ERROR: Unknown opcode '" << opcode
                 << "' on line " << lineNum << endl;
            exit(1);
        }
        return instr;

    }

private:
    // Author: Student A
    // DESC: Load .asm file lines into instructions vector and programQueue
    void loadFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "ERROR: Cannot open file: " << filename << endl;
            exit(1);
        }
        string line;
        while (getline(file, line)) {
            instructions.push_back(line);
            programQueue.enqueue(line);
        }
        file.close();
        cout << "Loaded " << instructions.getSize()
             << " lines from " << filename << endl;
    }

    // Author: Student A
    // DESC: Execute all loaded instructions on the CPU
    void executeAll() {
        cpu.reset();
        cout << "=== START ===" << endl;
        cpu.displayState();
        for (int i = 0; i < instructions.getSize(); i++) {
            Instruction* instr = parseLine(instructions.get(i), i + 1);
            if (instr == nullptr) continue;
            instr->execute(cpu);
            cpu.incrementPC();
            cpu.displayState();
            delete instr;
        }
        cout << "=== END ===" << endl;
    }

public:
    // Author: Student A
    // DESC: Main entry - load file, execute, write output
    void run(string filename) {
        outputFile = filename + ".out";
        loadFile(filename);
        executeAll();
        cpu.writeOutput(outputFile);
    }
};


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// SECTION 10 : MAIN FUNCTION
// ASSIGNED TO : Student A (soon chee)
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// Author: Student A
// DESC: Entry point of the program.
//       Reads the .asm filename from command line and runs it.
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: vm <program.asm>" << endl;
        cout << "  Windows: vm.exe program.asm" << endl;
        cout << "  Linux:   ./vm program.asm" << endl;
        return 1;
    }
    Runner runner;
    runner.run(argv[1]);

    // if argc < 2: print usage instructions and return 1
    // else: create Runner, call runner.run(argv[1])
    return 0;
}

// ============================================================
// END OF TT01_G01.cpp
// ============================================================

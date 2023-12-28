#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "Operations.h"
using namespace std;

map<string, int> labels;     // <label name, label index>
map<int, string> memory;

// INITIALIZE REGISTISERS
map<string, int> registers = { {"x0", 0},{"x1",0},{"x2",0},{"x3",0},
    {"x4",0},{"x5",0},{"x6",0}, {"x7", 0} };

// REGISTERS OPERATION
map<string, string> registersop = { {"x0", ""},{"x1", ""},{"x2", ""},{"x3", ""},
    {"x4", ""},{"x5", ""},{"x6", ""},{"x7", ""} };

// FUNCTIONS
string binaryform(int num);
int decimalform_unsigned(string binary);
int decimalform(string binaryString);
string hexform(int decimal);

void printreg();
void printmem();

void IntializeMemory(vector<string> memorycode) {

    string address;
    string value;
    string byte_0;
    string byte_1;
    string byte_2;
    string byte_3;
    string word;
    // cout << memorycode.size();

    for (int i = 0; i < memorycode.size() * 4 + 1; i++)
    {
        memory[i] = "00000000";
    }

    for (int i = 0; i < memorycode.size(); i++)
    {

        long comma_index = memorycode[i].find(", ");
        long space_index = memorycode[i].find(" ");
        if (comma_index != string::npos && space_index != string::npos) // if it is a address
        {
            address = memorycode[i].substr(0, comma_index);
            value = memorycode[i].substr(comma_index + 1, -1);
            word = binaryform(stoi(value));
            // cout << word << endl;


            byte_0 = word.substr(24, 8);
            byte_1 = word.substr(16, 8);
            byte_2 = word.substr(8, 8);
            byte_3 = word.substr(0, 8);

            memory[stoi(address) + 3] = byte_3;
            memory[stoi(address) + 2] = byte_2;
            memory[stoi(address) + 1] = byte_1;
            memory[stoi(address)] = byte_0;
        }
    }
    // printmem();
}
void IntializeLabels(vector<string> inputcode) {

    string rest;
    string labelname;
    for (int i = 0; i < inputcode.size(); i++)
    {
        long colon_index = inputcode[i].find(":");
        if (colon_index != string::npos) // if it is a label
        {
            labelname = inputcode[i].substr(0, colon_index);
            labels[labelname] = i;

            rest = inputcode[i].substr(colon_index + 1, -1);
            inputcode[i] = rest;
        }

    }
}
vector<string> ParseInstruction(vector<string> inputcode, int PC, string& name) {

    vector<string> adjustedcode;

    for (auto character : inputcode[PC])
    {
        if (character == ',' || character == ' ' || character == '(')
        {
            if (name != "")
                adjustedcode.push_back(name);

            name = "";
        }
        else
        {
            if (character == ')') continue; // skippity
            name = name + character;
        }


    }
    
    return adjustedcode;
}

void AddOperation(ReservationStationAdd& add1, ReservationStationAdd& add2, ReservationStationAdd& add3, vector<string> adjustedcode, int PC)
{
    RFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[2];
    instruction.rs2 = adjustedcode[3];
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (add1.busy == false)
    {
        // ISSUE
        add1.R = instruction;
        add1.busy = true;
        add1.op = "add";
        if (registersop[instruction.rs1] == "")
            add1.Vj = instruction.rs1;
        else
            add1.Vj = "";
        if (registersop[instruction.rs2] == "")
            add1.Vk = instruction.rs2;
        else
            add1.Vk = "";
        if (registersop[instruction.rs1] != "")
            add1.Qj = registersop[instruction.rs1];
        else
            add1.Qj = "";
        if (registersop[instruction.rs2] == "")
            add1.Qk = registersop[instruction.rs2];
        else
            add1.Qk = "";
        add1.A = 0;
        add1.countdown = 2;
        registersop[instruction.rd] = "add1";
        add1.R.issued = PC;

    }

    else if (add2.busy == false)
    {
        add2.R = instruction;
        add2.busy = true;
        add2.op = "add";
        if (registersop[instruction.rs1] == "")
            add2.Vj = instruction.rs1;
        else
            add2.Vj = "";
        if (registersop[instruction.rs2] == "")
            add2.Vk = instruction.rs2;
        else
            add2.Vk = "";
        if (registersop[instruction.rs1] != "")
            add2.Qj = registersop[instruction.rs1];
        else
            add2.Qj = "";
        if (registersop[instruction.rs2] == "")
            add2.Qk = registersop[instruction.rs2];
        else
            add2.Qk = "";
        add2.A = 0;
        add2.countdown = 2;
        registersop[instruction.rd] = "add2";
        add2.I.issued = PC;
    }

    else if (add3.busy == false)
    {
        add3.R = instruction;
        add3.busy = true;
        add3.op = "add";
        if (registersop[instruction.rs1] == "")

            add3.Vj = instruction.rs1;
        else
            add3.Vj = "";
        if (registersop[instruction.rs2] == "")
            add3.Vk = instruction.rs2;
        else
            add3.Vk = "";
        if (registersop[instruction.rs1] != "")
            add3.Qj = registersop[instruction.rs1];
        else
            add3.Qj = "";
        if (registersop[instruction.rs2] == "")
            add3.Qk = registersop[instruction.rs2];
        else
            add3.Qk = "";
        add3.A = 0;
        add3.countdown = 2;
        registersop[instruction.rd] = "add3";
        add3.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void NandOperation(ReservationStationNand& nand, vector<string> adjustedcode, int PC)
{
    RFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[2];
    instruction.rs2 = adjustedcode[3];
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;


    if (nand.busy == false)
    {
        nand.I = instruction;
        nand.busy = true;
        nand.op = "nand";
        if (registersop[instruction.rs1] == "")
            nand.Vj = instruction.rs1;
        else
            nand.Vj = "";
        if (registersop[instruction.rs2] == "")
            nand.Vk = instruction.rs2;
        else
            nand.Vk = "";
        if (registersop[instruction.rs1] != "")
            nand.Qj = registersop[instruction.rs1];
        else
            nand.Qj = "";
        if (registersop[instruction.rs2] == "")
            nand.Qk = registersop[instruction.rs2];
        else
            nand.Qk = "";
        nand.A = 0;
        nand.countdown = 1;
        registersop[instruction.rd] = "nand";
        nand.I.issued = PC;

    }
    else
    {
        cout << "Error" << endl;
    }
}
void DivOperation(ReservationStationDiv& div, vector<string> adjustedcode, int PC) {
    RFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[2];
    instruction.rs2 = adjustedcode[3];
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;


    if (div.busy == false)
    {
        div.I = instruction;
        div.busy = true;
        div.op = "div";
        if (registersop[instruction.rs1] == "")
            div.Vj = instruction.rs1;
        else
            div.Vj = "";
        if (registersop[instruction.rs2] == "")
            div.Vk = instruction.rs2;
        else
            div.Vk = "";
        if (registersop[instruction.rs1] != "")
            div.Qj = registersop[instruction.rs1];
        else
            div.Qj = "";
        if (registersop[instruction.rs2] == "")
            div.Qk = registersop[instruction.rs2];
        else
            div.Qk = "";
        div.A = 0;
        div.countdown = 10;
        registersop[instruction.rd] = "div";
        div.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void AddiOperation(ReservationStationAdd& add1, ReservationStationAdd& add2, ReservationStationAdd& add3, vector<string> adjustedcode, int PC) {
    IFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[2];
    instruction.imm = stoi(adjustedcode[3]);
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (add1.busy == false)
    {
        add1.I = instruction;
        add1.busy = true;
        add1.op = "addi";
        if (registersop[instruction.rs1] == "")
            add1.Vj = instruction.rs1;
        else
            add1.Vj = "";
        add1.Vk = "";
        if (registersop[instruction.rs1] != "")
            add1.Qj = registersop[instruction.rs1];
        else
            add1.Qj = "";
        add1.Qk = "";
        add1.A = 0;
        add1.countdown = 2;
        registersop[instruction.rd] = "add1";
        add1.I.issued = PC;
    }
    else if (add2.busy == false)
    {
        add2.busy = true;
        add2.I = instruction;
        add2.op = "addi";
        if (registersop[instruction.rs1] == "")
            add2.Vj = instruction.rs1;
        else
            add2.Vj = "";
        add2.Vk = "";
        if (registersop[instruction.rs1] != "")
            add2.Qj = registersop[instruction.rs1];
        else
            add2.Qj = "";
        add2.Qk = "";
        add2.A = 0;
        add2.countdown = 2;
        registersop[instruction.rd] = "add2";
        add2.I.issued = PC;
    }
    else if (add3.busy == false)
    {
        add3.busy = true;
        add3.I = instruction;
        add3.op = "addi";
        if (registersop[instruction.rs1] == "")
            add3.Vj = instruction.rs1;
        else
            add3.Vj = "";
        add3.Vk = "";
        if (registersop[instruction.rs1] != "")
            add3.Qj = registersop[instruction.rs1];
        else
            add3.Qj = "";
        add3.Qk = "";
        add3.A = 0;
        add3.countdown = 2;
        registersop[instruction.rd] = "add3";
        add3.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void LoadOperation(ReservationStationLoad& load1, ReservationStationLoad& load2, vector<string> adjustedcode, int PC) {
    IFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[3];
    instruction.imm = stoi(adjustedcode[2]);
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (load1.busy == false)
    {
        load1.I = instruction;
        load1.busy = true;
        load1.op = "load";
        if (registersop[instruction.rs1] == "")
            load1.Vj = instruction.rs1;
        else
            load1.Vj = "";
        load1.Vk = "";
        if (registersop[instruction.rs1] != "")
            load1.Qj = registersop[instruction.rs1];
        else
            load1.Qj = "";
        load1.Qk = "";
        load1.A = registers[instruction.rs1] + instruction.imm;
        load1.countdown = 3;
        registersop[instruction.rd] = "load1";
        load1.I.issued = PC;
    }
    else if (load2.busy == false)
    {
        load2.I = instruction;
        load2.busy = true;
        load2.op = "load";
        if (registersop[instruction.rs1] == "")
            load2.Vj = instruction.rs1;
        else
            load2.Vj = "";
        load2.Vk = "";
        if (registersop[instruction.rs1] != "")
            load2.Qj = registersop[instruction.rs1];
        else
            load2.Qj = "";
        load2.Qk = "";
        load2.A = registers[instruction.rs1] + instruction.imm;;
        load2.countdown = 3;
        registersop[instruction.rd] = "load2";
        load2.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void StoreOperation(ReservationStationStore& store1, ReservationStationStore& store2, vector<string> adjustedcode, int PC) {
    IFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.rs1 = adjustedcode[3];
    instruction.imm = stoi(adjustedcode[2]);
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (store1.busy == false)
    {
        store1.I = instruction;
        store1.busy = true;
        store1.op = "store";
        if (registersop[instruction.rs1] == "")
            store1.Vj = instruction.rs1;
        else
            store1.Vj = "";
        store1.Vk = "";
        if (registersop[instruction.rs1] != "")
            store1.Qj = registersop[instruction.rs1];
        else
            store1.Qj = "";
        store1.Qk = "";
        store1.A = registers[instruction.rs1] + instruction.imm;
        store1.countdown = 3;
        registersop[instruction.rd] = "store1";
        store1.I.issued = PC;
    }
    else if (store2.busy == false)
    {
        store2.I = instruction;
        store2.busy = true;
        store2.op = "store";
        if (registersop[instruction.rs1] == "")
            store2.Vj = instruction.rs1;
        else
            store2.Vj = "";
        store2.Vk = "";
        if (registersop[instruction.rs1] != "")
            store2.Qj = registersop[instruction.rs1];
        else
            store2.Qj = "";
        store2.Qk = "";
        store2.A = registers[instruction.rs1] + instruction.imm;
        store2.countdown = 3;
        registersop[instruction.rd] = "store2";
        store2.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void BneOperation(ReservationStationBne& bne, vector<string> adjustedcode, int& PC) {
    SBFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rs1 = adjustedcode[1];
    instruction.rs2 = adjustedcode[2];
    instruction.imm = stoi(adjustedcode[3]);
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (bne.busy == false)
    {
        bne.I = instruction;
        bne.busy = true;
        bne.op = "bne";
        if (registersop[instruction.rs1] == "")
            bne.Vj = instruction.rs1;
        else
            bne.Vj = "";
        bne.Vk = "";
        if (registersop[instruction.rs1] != "")
            bne.Qj = registersop[instruction.rs1];
        else
            bne.Qj = "";
        bne.Qk = "";
        bne.A = 0;
        bne.countdown = 1;
        bne.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void CallOperation(ReservationStationCallRet& callret, vector<string> adjustedcode, int& PC) {
    UJFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.label = adjustedcode[2];
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (callret.busy == false)
    {
        callret.I = instruction;
        callret.busy = true;
        callret.op = "call";
        callret.Vj = "";
        callret.Vk = "";
        callret.Qj = "";
        callret.Qk = "";
        callret.A = 0;
        callret.countdown = 1;
        callret.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}
void RetOperation(ReservationStationCallRet& callret, vector<string> adjustedcode, int& PC) {
    UJFormat instruction;
    instruction.instr = adjustedcode[0];
    instruction.rd = adjustedcode[1];
    instruction.imm = stoi(adjustedcode[2]);
    instruction.issued = 0;
    instruction.startexec = 0;
    instruction.finishexec = 0;
    instruction.writeback = 0;

    if (callret.busy == false)
    {
        callret.I = instruction;
        callret.busy = true;
        callret.op = "load";
        callret.Vj = "";
        callret.Vk = "";
        callret.Qj = "";
        callret.Qk = "";
        callret.A = 0;
        callret.countdown = 1;
        callret.I.issued = PC;
    }
    else
    {
        cout << "Error" << endl;
    }
}


void printreg();
void printmem();


// RESERVATION STATIONS

int main()
{

    ofstream myfile;
    myfile.open("Table.txt");
    myfile << "Instruction" << "    " << " Issued" << "    " << "Start Executing" << "    " << "Finish Executing" << "    " << "Write Back" << endl;

    // -------------------- INSTANTIATE RESERVATION STATIONS --------------------

    ReservationStationLoad load1;
    ReservationStationLoad load2;
    ReservationStationStore store1;
    ReservationStationStore store2;
    ReservationStationBne bne;
    ReservationStationCallRet callret;
    ReservationStationAdd add1;
    ReservationStationAdd add2;
    ReservationStationAdd add3;
    ReservationStationNand nand;
    ReservationStationDiv div;

    // -------------------- STORE INSTRUCTIONS --------------------

    string filename, memoryfilename;
    // cout << "Enter program file name: " << endl;
    // cin >> filename;

    ifstream inputfile;
    inputfile.open("Instruction.txt");

    if (!inputfile.is_open())
    {
        cout << "Error: Unable to open program file " << endl;
        return 0;
    }

    string line;
    vector<string> inputcode;

    while (getline(inputfile, line))
    {
        inputcode.push_back(line);
    }

    // -------------------- STORE MEMORY --------------------

    // cout << "Enter memory file name: " << endl;
    // cin >> memoryfilename;
    ifstream memoryfile;
    memoryfile.open("Mem.txt");

    if (!memoryfile.is_open())
    {
        cout << "Error: Unable to open memory file " << endl;
        return 0;
    }

    string memoryline;
    vector<string> memorycode;

    while (getline(memoryfile, memoryline))
    {
        memorycode.push_back(memoryline);
    }

    // -------------------- PARSING --------------------

    IntializeMemory(memorycode);
    IntializeLabels(inputcode);

    // -------------------- PC --------------------

    int PC = 0;
    string function;
    // cout << "Enter PC initial state :" << endl;
    // cin >> PC;
    // PC = PC / 4;            //as user will input it in multiple of 4 in assembly

    // -------------------- EXECUTE --------------------

    bool newi = true;
    string name;
    vector<string> adjustedcode;
    int i = 0;
    while ((PC >= 0) && (PC < inputcode.size() || (add1.busy == true || add2.busy == true || add3.busy == true || load1.busy == true || load2.busy == true || store1.busy == true || store2.busy == true || div.busy == true || nand.busy == true || callret.busy == true || add1.busy == true || bne.busy == true))
        )
    {

        registers["x0"] = 0;
        // NEW INSTRUCTION
        if (newi == true && PC < inputcode.size()) {
            adjustedcode.clear(); // clear
            name = "";
            adjustedcode = ParseInstruction(inputcode, PC, name);
            adjustedcode.push_back(name);
        }

        if (!adjustedcode.empty()) {
            if (!adjustedcode[0].empty()) {
                function = adjustedcode[0];
            }
        }

        if (PC >= inputcode.size()) {
            function = "";
            newi = false;
        }
         // EXECUTE CURRENT RESERVATION STATIONS
        if (add1.busy == true)
        {
            add1.Update();
            add1.Executing(PC);
        }
        if (add2.busy == true)
        {
            add2.Update();
            add2.Executing(PC);
        }
        if (add3.busy == true)
        {
            add3.Update();
            add3.Executing(PC);
        }
        if (load1.busy == true)
        {
            load1.Update();
            load1.Executing(PC);
        }
        if (load2.busy == true)
        {
            load2.Update();
            load2.Executing(PC);
        }
        if (store1.busy == true)
        {
            store1.Update();
            store1.Executing(PC);
        }
        if (store2.busy == true)
        {
            store2.Update();
            store2.Executing(PC);
        }
        if (div.busy == true)
        {
            div.Update();
            div.Executing(PC);
        }
        if (nand.busy == true)
        {
            nand.Update();
            nand.Executing(PC);
        }
        if (callret.busy == true)
        {
            callret.Executing(PC);
        }
        if (bne.busy == true)
        {
            bne.Update();
            bne.Executing(PC);
        }

        if (function == "add") {
            if (add1.busy == true && add2.busy == true && add3.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                AddOperation(add1, add2, add3, adjustedcode, PC);

            }
        }
        // NAND OPERATION
        else if (function == "nand")
        {
            if (nand.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                NandOperation(nand, adjustedcode, PC);
            }
        }

        // DIV OPERATION
        if (function == "div")
        {
            if (div.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                DivOperation(div, adjustedcode, PC);
            }
        }

        // ADDI OPERATION
        else if (function == "addi")
        {
            if (add1.busy == true && add2.busy == true && add3.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                AddiOperation(add1, add2, add3, adjustedcode, PC);
            }
        }

        // LOAD OPERATION
        else if (function == "load")
        {
            if (load1.busy == true && load1.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                LoadOperation(load1, load2, adjustedcode, PC);
            }
        }

        // STORE OPERATION
        else if (function == "store")
        {
            if (store1.busy == true && store2.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                StoreOperation(store1, store2, adjustedcode, PC);
            }
        }

        // BNE OPERATION
        else if (function == "bne")
        {
            if (bne.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                BneOperation(bne, adjustedcode, PC);
            }
        }

        // CALL OPERATION
        else if (function == "call")
        {
            if (callret.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                CallOperation(callret, adjustedcode, PC);
            }
        }

        // UJ-FORMAT OPERATION
        else if (function == "ret")
        {
            if (callret.busy == true)
            {
                newi = false;
            }
            else
            {
                newi = true;
                RetOperation(callret, adjustedcode, PC);
            }
        }
        else
            cout << "Done" << endl;

        cout << "AT PC " << PC << endl;
        cout << "newi " << newi << endl;
        cout << "add1 " << add1.busy << endl;
        cout << "add2 " << add2.busy << endl;
        cout << "add3 " << add3.busy << endl;
        cout << "load1 " << load1.busy << endl;
        cout << "load2 " << load2.busy << endl;
        cout << "store1 " << store1.busy << endl;
        cout << "store2 " << store2.busy << endl;
        cout << "nand " <<nand.busy << endl;
        cout << "div " << div.busy << endl;
        cout << "bne " << bne.busy << endl;
        cout << "callret " << callret.busy << endl;
        
        adjustedcode.clear(); // clear
        PC += 1;
        i++;
        
    }

    cout << endl;
    cout << "________________________________________________________________________" << endl;
    cout << endl;
    // cout << "PC :" << PC * 4 << endl;
    cout << endl;
    registers["x0"] = 0;
    // printmem();
    // printreg();

}


// -------------------- BINARY, DECIMAL, UNSIGNED, HEX --------------------

string binaryform(int num)
{
    string binary = "";
    for (int i = 31; i >= 0; i--)
    {
        int bit = num >> i;
        if (bit & 1)
            binary.push_back('1');
        else
            binary.push_back('0');
    }
    return binary;
}

int decimalform_unsigned(string binary) {
    int decimal = 0;
    int power = 1;

    for (int i = binary.length() - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            decimal += power;
        }
        power *= 2;
    }
    return decimal;
}

int decimalform(string binary) {
    int decimal = 0;
    int power = 0;
    bool isNegative = false;

    if (binary[0] == '1') {
        isNegative = true;
        // Invert all bits
        for (int i = 0; i < binary.length(); i++) {
            if (binary[i] == '0') {
                binary[i] = '1';
            }
            else {
                binary[i] = '0';
            }
        }
        // Add 1 to get the 2's complement
        for (int i = binary.length() - 1; i >= 0; i--) {
            if (binary[i] == '0') {
                binary[i] = '1';
                break;
            }
            else {
                binary[i] = '0';
            }
        }
    }

    for (int i = binary.length() - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            decimal += pow(2, power);
        }
        power++;
    }

    if (isNegative) {
        decimal = -decimal;
    }

    return decimal;
}

string hexform(int decimal)
{
    stringstream ss;
    ss << hex << decimal;
    string res(ss.str());
    return res;
}

void printreg() {
    for (auto it = registers.cbegin(); it != registers.cend(); ++it) {
        int second = it->second;
        cout << it->first << left << ": " << "| decimal form: " << second << "|  binary form: " << binaryform(second) << " | hexadecimal form: " << hexform(it->second) << "|" << "\n";
    }
}

void printmem() {
    for (auto it = memory.cbegin(); it != memory.cend(); ++it) {
        string second = it->second;
        if (it->second == "") { second = "00000000"; }
        cout << it->first << ": " << second << "\n";
    }
}
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

extern map<string, int> registers;
extern map<string, int> labels;     // <label name, label index>
extern map<int, string> memory;

// REGISTERS OPERATION
extern map<string, string> registersop;

extern string hexform(int decimal);
extern string binaryform(int num);
extern int decimalform(string binary); 

class Instruction {
public:
    string type;
    void execute(int &pc)
    {};
};

// STRUCT OF TYPE OF INSTRUCTIONS
class RFormat : public Instruction {
public:

    string instr;
    string rd;
    string rs1;
    string rs2;
    int issued;
    int startexec;
    int finishexec;
    int writeback;

    void execute(int& pc) { // Execute RISC-V instruction
        {
            if (instr == "add") {
                registers[rd] = registers[rs1] + registers[rs2];
                pc++;
            }
            else if (instr == "nand") {
                registers[rd] = ~(registers[rs1] & registers[rs2]);
                pc++;
            }
            else if (instr == "div") {
                registers[rd] = registers[rs1] / registers[rs2];
                pc++;
            }
            else {
                cout << "Error: Unsupported instruction" << endl;
                exit(0);  // Exits the program
            }
        }
    };

};

class IFormat : public Instruction {
    public:

        string instr = "test";
        string rd;
        string rs1;
        int imm;
        int issued;
        int startexec;
        int finishexec;
        int writeback;

        void execute(int& pc)
        {
            if (instr == "load") {
                string finalword;
                finalword = binaryform(decimalform(memory[imm + registers[rs1] + 1]) & 0b11111111).substr(24, 8) + binaryform(decimalform(memory[imm + registers[rs1]]) & 0b11111111).substr(24, 8);
                registers[rd] = decimalform(finalword);
                pc++;
            }
            else if (instr == "store") {
                memory[imm + registers[rs1]] = binaryform(registers[rd] & 0b00000000000000000000000011111111).substr(24, 8); // we store the first byte into memory
                memory[imm + registers[rs1] + 1] = binaryform(registers[rd] & 0b00000000000000001111111100000000).substr(16, 8); // we store the second byte into memory
                pc++;

            }
            else if (instr == "addi") {
                registers[rd] = registers[rs1] + imm;
                pc++;
            }
            else {
                cout << "Error: Unsupported instruction" << endl;
                exit(0);  // Exits the program
            }
        }

    };

class SBFormat : public Instruction {
    public:

        string instr;
        string rs1;
        string rs2;
        int imm;
        int issued;
        int startexec;
        int finishexec;
        int writeback;

        void execute(int& pc)
        {

            if (instr == "bne") {
                if (registers[rs1] != registers[rs2])
                    pc = imm;
                else
                    pc = pc + 1;
            }
            else {
                cout << "Error: Unsupported instruction" << endl;
                exit(0);  // Exits the program
            }
        }
    };

class UJFormat : public Instruction {
    public:

        string instr;
        string rd;
        int imm;
        string label;
        int issued;
        int startexec;
        int finishexec;
        int writeback;

        void execute(int& pc)
        {

            if (instr == "call")
            {
                if (labels[label]) //not sure
                {
                    pc = labels[label];
                }
                else
                    cout << "Error, label not found" << endl;
            }
            else if (instr == "ret") {
                pc = registers["x1"];
            }
            else {
                cout << "Error: Unsupported instruction" << endl;
                exit(0);  // Exits the program
            }
        }
    
};


    // RESERVATION STATION
class ReservationStationAdd
    {
    public:

        RFormat R;
        IFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationAdd()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 2;
        }

        void Update()
        {
            if (op == "add") {
                if (registersop[R.rs1] == "")
                    Vj = R.rs1;
                else
                    Vj = "";
                if (registersop[R.rs2] == "")
                    Vk = R.rs2;
                else
                    Vk = "";
                if (registersop[R.rs1] != "")
                    Qj = registersop[R.rs1];
                else
                    Qj = "";
                if (registersop[R.rs2] == "")
                    Qk = registersop[R.rs2];
                else
                    Qk = "";
            }
            else if (op == "addi") {
                if (registersop[I.rs1] == "")
                    Vj = I.rs1;
                else
                    Vj = "";
                if (registersop[I.rd] == "")
                    Vk = I.rd;
                else
                    Vk = "";
                if (registersop[I.rs1] != "")
                    Qj = registersop[I.rs1];
                else
                    Qj = "";
                if (registersop[I.rd] == "")
                    Qk = registersop[I.rd];
                else
                    Qk = "";
            }
        }

        void Executing(int& pc)
        {
            if (op == "addi") {
                if (registersop[I.rs1] == "")
                {
                    if (I.startexec == 0)
                        I.startexec = pc;
                    if (countdown <= 0 && busy == true) {
                        I.execute(pc);
                        busy = false;
                        op = "";
                        Vj = "";
                        Vk = "";
                        Qj = "";
                        Qk = "";
                        A = 0;
                        countdown = 0;
                        registersop[I.rd] = "";
                        I.finishexec = pc;
                        I.writeback = pc + 1;
                        cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;
                    }
                    else if (countdown > 0 && busy == true)
                    {
                        countdown--;
                    }

                }

            }
            else if (op == "add")
            {
                if (registersop[R.rs1] == "" && registersop[R.rs2] == "")
                {
                    R.startexec = pc;
                    if (countdown <= 0 && busy == true) {
                        R.execute(pc);
                        busy = false;
                        op = "";
                        Vj = "";
                        Vk = "";
                        Qj = "";
                        Qk = "";
                        A = 0;
                        countdown = 0;
                        registersop[R.rd] = "";
                        R.finishexec = pc;
                        R.writeback = pc + 1;
                        cout << R.instr << " " << R.rd << "         " << R.issued << "         " << R.startexec << "         " << R.finishexec << "         " << R.writeback << endl;
                    }
                    else if (countdown > 0 && busy == true)
                    {
                        countdown--;
                    }

                }
            }

            return;
        }

    };

    class ReservationStationNand
    {
    public:

        RFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationNand()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Update()
        {
            if (registersop[I.rs1] == "")
                Vj = I.rs1;
            else
                Vj = "";
            if (registersop[I.rs2] == "")
                Vk = I.rs2;
            else
                Vk = "";
            if (registersop[I.rs1] != "")
                Qj = registersop[I.rs1];
            else
                Qj = "";
            if (registersop[I.rs2] == "")
                Qk = registersop[I.rs2];
            else
                Qk = "";
        }

        void Executing(int& pc)
        {
            if (registersop[I.rs1] == "" && registersop[I.rs2] == "")
            {
                I.startexec = pc;
                if (countdown <= 0 && busy == true) {
                    I.execute(pc);
                    busy = false;
                    op = "";
                    Vj = "";
                    Vk = "";
                    Qj = "";
                    Qk = "";
                    A = 0;
                    countdown = 0;
                    registersop[I.rd] = "";
                    I.finishexec = pc;
                    I.writeback = pc + 1;
                    cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;
                }
                else if (countdown > 0 && busy == true)
                {
                    countdown--;
                }

            }

        }
    };

    class ReservationStationDiv
    {
    public:

        RFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationDiv()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Update()
        {
            if (registersop[I.rs1] == "")
                Vj = I.rs1;
            else
                Vj = "";
            if (registersop[I.rs2] == "")
                Vk = I.rs2;
            else
                Vk = "";
            if (registersop[I.rs1] != "")
                Qj = registersop[I.rs1];
            else
                Qj = "";
            if (registersop[I.rs2] == "")
                Qk = registersop[I.rs2];
            else
                Qk = "";
        }

        void Executing(int& pc)
        {
            if (registersop[I.rs1] == "" && registersop[I.rs2] == "")
            {
                I.startexec = pc;
                if (countdown <= 0 && busy == true) {
                    I.execute(pc);
                    busy = false;
                    op = "";
                    Vj = "";
                    Vk = "";
                    Qj = "";
                    Qk = "";
                    A = 0;
                    countdown = 0;
                    registersop[I.rd] = "";
                    I.finishexec = pc;
                    I.writeback = pc + 1;
                    cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;
                }
                else if (countdown > 0 && busy == true)
                {
                    countdown--;
                }

            }

        }
    };


    class ReservationStationLoad
    {
    public:

        IFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationLoad()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Update()
        {
            if (registersop[I.rs1] == "")
                Vj = I.rs1;
            else
                Vj = "";
            if (registersop[I.rd] == "")
                Vk = I.rd;
            else
                Vk = "";
            if (registersop[I.rs1] != "")
                Qj = registersop[I.rs1];
            else
                Qj = "";
            if (registersop[I.rd] == "")
                Qk = registersop[I.rd];
            else
                Qk = "";
        }

        void Executing(int& pc)
        {
            if (registersop[I.rs1] == "" && registersop[I.rd] == "")
            {
                I.startexec = pc;
                if (countdown <= 0 && busy == true) {
                    I.execute(pc);
                    busy = false;
                    op = "";
                    Vj = "";
                    Vk = "";
                    Qj = "";
                    Qk = "";
                    A = 0;
                    countdown = 0;
                    registersop[I.rd] = "";
                    I.finishexec = pc;
                    I.writeback = pc + 1;
                    cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;

                }
                else if (countdown > 0 && busy == true)
                {
                    countdown--;
                }

            }

        }
    };

    class ReservationStationStore
    {
    public:

        IFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationStore()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Update()
        {
            if (registersop[I.rs1] == "")
                Vj = I.rs1;
            else
                Vj = "";
            if (registersop[I.rd] == "")
                Vk = I.rd;
            else
                Vk = "";
            if (registersop[I.rs1] != "")
                Qj = registersop[I.rs1];
            else
                Qj = "";
            if (registersop[I.rd] == "")
                Qk = registersop[I.rd];
            else
                Qk = "";
        }

        void Executing(int& pc)
        {
            if (registersop[I.rs1] == "" && registersop[I.rd] == "")
            {
                I.startexec = pc;
                if (countdown <= 0 && busy == true) {
                    I.execute(pc);
                    busy = false;
                    op = "";
                    Vj = "";
                    Vk = "";
                    Qj = "";
                    Qk = "";
                    A = 0;
                    countdown = 0;
                    registersop[I.rd] = "";
                    I.finishexec = pc;
                    I.writeback = pc + 1;
                    cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;
                }
                else if (countdown > 0 && busy == true)
                {
                    countdown--;
                }

            }
        }
    };

    class ReservationStationBne
    {
    public:

        SBFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationBne()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Update()
        {
            if (registersop[I.rs1] == "")
                Vj = I.rs1;
            else
                Vj = "";
            if (registersop[I.rs2] == "")
                Vk = I.rs2;
            else
                Vk = "";
            if (registersop[I.rs1] != "")
                Qj = registersop[I.rs1];
            else
                Qj = "";
            if (registersop[I.rs2] == "")
                Qk = registersop[I.rs2];
            else
                Qk = "";
        }

        void Executing(int& pc)
        {
            if (registersop[I.rs1] == "" && registersop[I.rs2] == "")
            {
                I.startexec = pc;
                if (countdown <= 0 && busy == true) {
                    I.execute(pc);
                    busy = false;
                    op = "";
                    Vj = "";
                    Vk = "";
                    Qj = "";
                    Qk = "";
                    A = 0;
                    countdown = 0;
                    I.finishexec = pc;
                    I.writeback = pc + 1;
                    cout << I.instr << " " << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;

                }
                else if (countdown > 0 && busy == true)
                {
                    countdown--;
                }

            }

        }
    };

    class ReservationStationCallRet
    {
    public:

        UJFormat I;
        bool busy;
        string op;
        string Vj;
        string  Vk;
        string Qj;
        string  Qk;
        int A;
        int countdown;

        ReservationStationCallRet()
        {
            busy = false;
            op = "";
            Vj = "";
            Vk = "";
            Qj = "";
            Qk = "";
            A = 0;
            countdown = 0;
        }

        void Executing(int& pc)
        {
            I.startexec = pc;
            if (countdown <= 0 && busy == true) {
                I.execute(pc);
                busy = false;
                op = "";
                Vj = "";
                Vk = "";
                Qj = "";
                Qk = "";
                A = 0;
                countdown = 0;
                I.finishexec = pc;
                I.writeback = pc + 1;
                cout << I.instr << " " << I.rd << "         " << I.issued << "         " << I.startexec << "         " << I.finishexec << "         " << I.writeback << endl;
            }
            else if (countdown > 0 && busy == true)
            {
                countdown--;
            }

            

        }
    };

    
#ifndef PORT_H
#define PORT_H

/*
    一、CXL Type3 HDM-DB 硬件中有哪些接口/字段 ？

        参考：CPI spec v1.0 -> Page 11 -> Table 4.1 :
                            |                A2F                 |                F2A                 |
                            |    REQ    |    RSP    |    DATA    |    REQ    |    RSP    |    DATA    |
        CXL.MEM(Downstream) | S2M BISnp |  S2M NDR  |   S2M DRS  |  M2S Req  | M2S BIRsp |   M2S RwD  |
        总共有六个通道，每个通道有若干接口/字段。
    二、C++ Reference Model 软件中有哪些接口/字段 ？

        各接口/字段在 port.h 中以 class 成员的方式被罗列。"C++ Reference Model 软件接口/字段" 是 "CXL Type3 HDM-DB 硬件接口/字段"的子集。

        Why 只是子集?
        1. 部分接口/字段的功能暂不使用 --> 这些接口/字段在 UVM 环境中不需要考虑。
        2. 部分接口/字段和传输语义无关 --> 这些接口/字段在 UVM_reference_model 中不需要考虑，在 UVM_moniter, UVM_driver 中需要考虑。
        3. 部分接口/字段和传输语义有关 --> 这些接口/字段需要在该工程中补充。
*/

#include "define.h"

/*
    Now Support:
    BISnpInv
    BISnpData
*/
class S2M_Snp_type
{
public:
    bool valid;

    u32 opcode; //  4bit
    enum Opcode_Enum
    {
        BISnpCur = 0b0000,
        BISnpData = 0b0001,
        BISnpInv = 0b0010,
        BISnpCurBlk = 0b0100,
        BISnpDataBlk = 0b0101,
        BISnpInvBlk = 0b0110
    };
    inline std::string get_opcode_name() const
    {
        switch (opcode)
        {
        case BISnpCur:
            return "BISnpCur";
        case BISnpData:
            return "BISnpData";
        case BISnpInv:
            return "BISnpInv";
        case BISnpCurBlk:
            return "BISnpCurBlk";
        case BISnpDataBlk:
            return "BISnpDataBlk";
        case BISnpInvBlk:
            return "BISnpInvBlk";
        default:
            return "Unknown";
        }
    }
    inline bool set_opcode_name(std::string name)
    {
        if (name == "BISnpCur")
            opcode = BISnpCur;
        else if (name == "BISnpData")
            opcode = BISnpData;
        else if (name == "BISnpInv")
            opcode = BISnpInv;
        else if (name == "BISnpCurBlk")
            opcode = BISnpCurBlk;
        else if (name == "BISnpDataBlk")
            opcode = BISnpDataBlk;
        else if (name == "BISnpInvBlk")
            opcode = BISnpInvBlk;
        else
            return false;
        return true;
    }

    /*                */ u32 bi_id;           // 12bit
    u32 bitag;                                // 12bit
    /*                */ u32 address_partity; //  1bit
    u64 address;                              // 46bit
    /*                */ u32 flit_mode;       //  2bit
    /*                */ u32 epoch_valid;     //  1bit
    /*                */ u32 epoch_id;        //  1bit
    /*                */ u32 cmd_parity;      //  1bit
    u32 spid;                                 // 12bit
    u32 dpid;                                 // 12bit

    S2M_Snp_type() : valid(false), opcode(0), bi_id(0), bitag(0), address_partity(0), address(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), spid(0), dpid(0) {}

    void print() const;
};

/*
    Now Support:
    Cmp
    BIConflictAck
*/
class S2M_NDR_Type
{
public:
    bool valid;

    u32 opcode; //  4bit
    enum Opcode_Enum
    {
        Cmp = 0b000,
        CmpS = 0b001,
        CmpE = 0b010,
        BIConflictAck = 0b100
    };
    inline std::string get_opcode_name() const
    {
        switch (opcode)
        {
        case Cmp:
            return "Cmp";
        case CmpS:
            return "CmpS";
        case CmpE:
            return "CmpE";
        case BIConflictAck:
            return "BIConflictAck";
        default:
            return "Unknown";
        }
    }
    inline bool set_opcode_name(std::string name)
    {
        if (name == "Cmp")
            opcode = Cmp;
        else if (name == "CmpS")
            opcode = CmpS;
        else if (name == "CmpE")
            opcode = CmpE;
        else if (name == "BIConflictAck")
            opcode = BIConflictAck;
        else
            return false;
        return true;
    }

    /*                */ u32 metafield;   //  2bit
    /*                */ u32 metavalue;   //  2bit
    u32 tag;                              // 16bit
    /*                */ u32 ld_id;       //  4bit
    /*                */ u32 devload;     //  2bit
    /*                */ u32 flit_mode;   //  2bit
    /*                */ u32 epoch_valid; //  1bit
    /*                */ u32 epoch_id;    //  1bit
    /*                */ u32 cmd_parity;  //  1bit
    /*                */ u32 spid;        // 12bit
    u32 dpid;                             // 12bit

    S2M_NDR_Type() : valid(false), opcode(0), metafield(0), metavalue(0), tag(0), ld_id(0), devload(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), spid(0), dpid(0) {}

    void print() const;
};

/*
    Now Support:
    MemData
*/
class S2M_DRS_Type
{
public:
    bool valid;

    u32 opcode; //  4bit
    enum Opcode_Enum
    {
        MemData = 0b000,
        MemDataNXM = 0b001
    };
    inline std::string get_opcode_name() const
    {
        switch (opcode)
        {
        case MemData:
            return "MemData";
        case MemDataNXM:
            return "MemDataNXM";
        default:
            return "Unknown";
        }
    }
    inline bool set_opcode_name(std::string name)
    {
        if (name == "MemData")
            opcode = MemData;
        else if (name == "MemDataNXM")
            opcode = MemDataNXM;
        else
            return false;
        return true;
    }

    /*                */ u32 metafield;        //  2bit
    /*                */ u32 metavalue;        //  2bit
    u32 tag;                                   // 16bit
    /*                */ u32 ld_id;            //  4bit
    /*                */ u32 devload;          //  2bit
    /*                */ u32 flit_mode;        //  2bit
    /*                */ u32 epoch_valid;      //  1bit
    /*                */ u32 epoch_id;         //  1bit
    /*                */ u32 cmd_parity;       //  1bit
    /*                */ u32 spid;             // 12bit
    u32 dpid;                                  // 12bit
    /*                */ u32 data_poison;      //  1bit
    /*                */ u64 data_byte_enable; // 64bit
    u512 data_body;                            // 512bit

    S2M_DRS_Type() : valid(false), opcode(0), metafield(0), metavalue(0), tag(0), ld_id(0), devload(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), spid(0), dpid(0), data_poison(0), data_byte_enable(0), data_body(0) {}

    void print() const;
};

/*
    Now Support:
    SnpInv  + MemRd  + Meta0State + A
    SnpData + MemRd  + Meta0State + S
    SnpInv  + MemInv + Meta0State + I
*/
class M2S_Req_Type
{
public:
    bool valid;

    u32 memopcode; //  4bit
    enum MemOpcode_Enum
    {
        MemInv = 0b0000,
        MemRd = 0b0001,
        MemRdData = 0b0010,
        MemRdFwd = 0b0011,
        MemWrFwd = 0b0100,
        MemSpecRd = 0b1000,
        MemInvNT = 0b1001,
        MemClnEvct = 0b1010
    };
    inline std::string get_memopcode_name() const
    {
        switch (memopcode)
        {
        case MemInv:
            return "MemInv";
        case MemRd:
            return "MemRd";
        case MemRdData:
            return "MemRdData";
        case MemRdFwd:
            return "MemRdFwd";
        case MemWrFwd:
            return "MemWrFwd";
        case MemSpecRd:
            return "MemSpecRd";
        case MemInvNT:
            return "MemInvNT";
        case MemClnEvct:
            return "MemClnEvct";
        default:
            return "Unknown";
        }
    }
    inline bool set_memopcode_name(std::string name)
    {
        if (name == "MemInv")
            memopcode = MemInv;
        else if (name == "MemRd")
            memopcode = MemRd;
        else if (name == "MemRdData")
            memopcode = MemRdData;
        else if (name == "MemRdFwd")
            memopcode = MemRdFwd;
        else if (name == "MemWrFwd")
            memopcode = MemWrFwd;
        else if (name == "MemSpecRd")
            memopcode = MemSpecRd;
        else if (name == "MemInvNT")
            memopcode = MemInvNT;
        else if (name == "MemClnEvct")
            memopcode = MemClnEvct;
        else
            return false;
        return true;
    }

    u32 snptype; //  3bit
    enum SnpType_Enum
    {
        NoOp = 0b000,
        SnpData = 0b001,
        SnpCur = 0b010,
        SnpInv = 0b011
    };
    inline std::string get_snptype_name() const
    {
        switch (snptype)
        {
        case NoOp:
            return "NoOp";
        case SnpData:
            return "SnpData";
        case SnpCur:
            return "SnpCur";
        case SnpInv:
            return "SnpInv";
        default:
            return "Unknown";
        }
    }
    inline bool set_snptype_name(std::string name)
    {
        if (name == "NoOp")
            snptype = NoOp;
        else if (name == "SnpData")
            snptype = SnpData;
        else if (name == "SnpCur")
            snptype = SnpCur;
        else if (name == "SnpInv")
            snptype = SnpInv;
        else
            return false;
        return true;
    }

    u32 metafield; //  2bit
    enum MetaField_Enum
    {
        Meta0State = 0b00,
        NoOp_ = 0b11
    };
    inline std::string get_metafield_name() const
    {
        switch (metafield)
        {
        case Meta0State:
            return "Meta0State";
        case NoOp_:
            return "NoOp";
        default:
            return "Unknown";
        }
    }
    inline bool set_metafield_name(std::string name)
    {
        if (name == "Meta0State")
            metafield = Meta0State;
        else if (name == "NoOp")
            metafield = NoOp_;
        else
            return false;
        return true;
    }

    u32 metavalue; //  2bit
    enum MetaValue_Enum
    {
        I = 0b00,
        A = 0b10,
        S = 0b11
    };
    inline std::string get_metavalue_name() const
    {
        switch (metavalue)
        {
        case I:
            return "I";
        case A:
            return "A";
        case S:
            return "S";
        default:
            return "Unknown";
        }
    }
    inline bool set_metavalue_name(std::string name)
    {
        if (name == "I")
            metavalue = I;
        else if (name == "A")
            metavalue = A;
        else if (name == "S")
            metavalue = S;
        else
            return false;
        return true;
    }

    u32 tag;                                  // 16bit
    /*                */ u32 tc;              //  2bit
    /*                */ u32 address5;        //  1bit
    /*                */ u32 address_partity; //  1bit
    u64 address;                              // 46bit
    /*                */ u32 ld_id;           //  4bit
    /*                */ u32 flit_mode;       //  2bit
    /*                */ u32 cmd_parity;      //  1bit
    u32 spid;                                 // 12bit
    u32 dpid;                                 // 12bit

    M2S_Req_Type() : valid(false), memopcode(0), snptype(0), metafield(0), metavalue(0), tag(0), tc(0), address5(0), address_partity(0), address(0), ld_id(0), flit_mode(0), cmd_parity(0), spid(0), dpid(0) {}

    void print() const;
};

/*
    Now Support:
    BIRspI
    BIRspS
*/
class M2S_Rsp_type
{
public:
    bool valid;

    u32 opcode; //  4bit
    enum Opcode_Enum
    {
        BIRspI = 0b0000,
        BIRspS = 0b0001,
        BIRspE = 0b0010,
        BIRspIBlk = 0b0100,
        BIRspSBlk = 0b0101,
        BIRspEBlk = 0b0110
    };
    inline std::string get_opcode_name() const
    {
        switch (opcode)
        {
        case BIRspI:
            return "BIRspI";
        case BIRspS:
            return "BIRspS";
        case BIRspE:
            return "BIRspE";
        case BIRspIBlk:
            return "BIRspIBlk";
        case BIRspSBlk:
            return "BIRspSBlk";
        case BIRspEBlk:
            return "BIRspEBlk";
        default:
            return "Unknown";
        }
    }
    inline bool set_opcode_name(std::string name)
    {
        if (name == "BIRspI")
            opcode = BIRspI;
        else if (name == "BIRspS")
            opcode = BIRspS;
        else if (name == "BIRspE")
            opcode = BIRspE;
        else if (name == "BIRspIBlk")
            opcode = BIRspIBlk;
        else if (name == "BIRspSBlk")
            opcode = BIRspSBlk;
        else if (name == "BIRspEBlk")
            opcode = BIRspEBlk;
        else
            return false;
        return true;
    }

    /*                */ u32 bi_id;      // 12bit
    u32 bitag;                           // 12bit
    /*                */ u32 lowaddr;    //  2bit
    /*                */ u32 flit_mode;  //  2bit
    /*                */ u32 cmd_parity; //  1bit
    u32 dpid;                            // 12bit

    S2M_Snp_type snp; // 非接口，用来简化代码的
    M2S_Rsp_type() : valid(false), opcode(0), bi_id(0), bitag(0), lowaddr(0), flit_mode(0), cmd_parity(0), dpid(0), snp() {}

    void print() const;
};

/*
    Now Support:
    MemWr + DontCare + DontCare + DontCare
    BIConflict + DontCare + DontCare + DontCare
*/
class M2S_RwD_Type
{
public:
    bool valid; //  1bit

    u32 memopcode; //  4bit
    enum MemOpcode_Enum
    {
        MemWr = 0b0001,
        MemWrPtl = 0b0010,
        BIConflict = 0b0100
    };
    inline std::string get_memopcode_name() const
    {
        switch (memopcode)
        {
        case MemWr:
            return "MemWr";
        case MemWrPtl:
            return "MemWrPtl";
        case BIConflict:
            return "BIConflict";
        default:
            return "Unknown";
        }
    }
    inline bool set_memopcode_name(std::string name)
    {
        if (name == "MemWr")
            memopcode = MemWr;
        else if (name == "MemWrPtl")
            memopcode = MemWrPtl;
        else if (name == "BIConflict")
            memopcode = BIConflict;
        else
            return false;
        return true;
    }

    u32 snptype; //  3bit
    enum SnpType_Enum
    {
        NoOp = 0b000,
        SnpData = 0b001,
        SnpCur = 0b010,
        SnpInv = 0b011
    };
    inline std::string get_snptype_name() const
    {
        switch (snptype)
        {
        case NoOp:
            return "NoOp";
        case SnpData:
            return "SnpData";
        case SnpCur:
            return "SnpCur";
        case SnpInv:
            return "SnpInv";
        default:
            return "Unknown";
        }
    }
    inline bool set_snptype_name(std::string name)
    {
        if (name == "NoOp")
            snptype = NoOp;
        else if (name == "SnpData")
            snptype = SnpData;
        else if (name == "SnpCur")
            snptype = SnpCur;
        else if (name == "SnpInv")
            snptype = SnpInv;
        else
            return false;
        return true;
    }

    u32 metafield; //  2bit
    enum MetaField_Enum
    {
        Meta0State = 0b00,
        NoOp_ = 0b11
    };
    inline std::string get_metafield_name() const
    {
        switch (metafield)
        {
        case Meta0State:
            return "Meta0State";
        case NoOp_:
            return "NoOp";
        default:
            return "Unknown";
        }
    }
    inline bool set_metafield_name(std::string name)
    {
        if (name == "Meta0State")
            metafield = Meta0State;
        else if (name == "NoOp")
            metafield = NoOp_;
        else
            return false;
        return true;
    }

    u32 metavalue; //  2bit
    enum MetaValue_Enum
    {
        I = 0b00,
        A = 0b10,
        S = 0b11
    };
    inline std::string get_metavalue_name() const
    {
        switch (metavalue)
        {
        case I:
            return "I";
        case A:
            return "A";
        case S:
            return "S";
        default:
            return "Unknown";
        }
    }
    inline bool set_metavalue_name(std::string name)
    {
        if (name == "I")
            metavalue = I;
        else if (name == "A")
            metavalue = A;
        else if (name == "S")
            metavalue = S;
        else
            return false;
        return true;
    }

    u32 tag;                                   // 16bit
    /*                */ u32 tc;               //  2bit
    /*                */ u32 address_partity;  //  1bit
    u64 address;                               // 46bit
    /*                */ u32 ld_id;            //  4bit
    /*                */ u32 flit_mode;        //  2bit
    /*                */ u32 cmd_parity;       //  1bit
    u32 spid;                                  // 12bit
    u32 dpid;                                  // 12bit
    /*                */ u32 data_poison;      //  1bit
    /*                */ u64 data_byte_enable; // 64bit
    u512 data_body;                            // 512bit

    M2S_RwD_Type() : valid(false), memopcode(0), snptype(0), metafield(0), metavalue(0), tag(0), tc(0), address_partity(0), address(0), ld_id(0), flit_mode(0), cmd_parity(0), spid(0), dpid(0), data_poison(0), data_byte_enable(0), data_body(0) {}

    void print() const;
};

// AXI 读地址通道 Address Read Channel
class Rd_Addr_Type
{
public:
    bool valid;
    u64 address;

    Rd_Addr_Type() : valid(false), address(0) {}

    Rd_Addr_Type(bool Valid, u64 Address) : valid(Valid), address(Address) {}

    void print() const;
};

// AXI 读数据通道 Read Channel
class Rd_Data_Type
{
public:
    bool valid;
    u64 address;
    u512 data_body;

    Rd_Data_Type() : valid(false), address(0), data_body(0) {}

    Rd_Data_Type(bool Valid, u64 Address, u512 Data) : valid(Valid), address(Address), data_body(Data) {}

    void print() const;
};

// AXI 写地址/数据通道 Write Address Channel + Write Data Channel
class Wr_Type
{
public:
    bool valid;
    u64 address;
    u512 data_body;

    Wr_Type() : valid(false), address(0), data_body(0) {}

    Wr_Type(bool Valid, u64 Address, u512 Data) : valid(Valid), address(Address), data_body(Data) {}

    void print() const;
};

class Victim_Type
{
public:
    M2S_Req_Type cmd;
};

class CXL_io_Req_Type
{
public:
    bool valid;
};

class CXL_io_Rsp_Type
{
public:
    bool valid;
    inline void print() {};
};

#endif
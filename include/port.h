/*
    接口/字段命名规则：
    参考：CPI spec v1.0 -> Page 11 -> Table 4.1
                        |                A2F                 |                F2A                 |
                        |    REQ    |    RSP    |    DATA    |    REQ    |    RSP    |    DATA    |
    CXL.MEM(Downstream) | S2M BISnp |  S2M NDR  |   S2M DRS  |  M2S Req  | M2S BIRsp |   M2S RwD  |
*/
/*
    除了以下列举的接口，CPI spec 还定义了其他接口/字段：
    1. 部分接口/字段的功能暂不支持 --> 这些接口/字段在 UVM 环境中不需要考虑
    2. 部分接口/字段和传输语义无关 --> 这些接口/字段在 UVM_reference_model 中不需要考虑，在 UVM_moniter, UVM_driver 中需要考虑
    3. 部分接口/字段和传输语义有关 --> 这些接口/字段需要在该工程中补充
*/

#include "define.h"

#define DONT_PRINT_LD_PID
#define S2M_SPACE "                                                                                             "
#define MEM_SPACE "                                                                                                                                     "


/*
    Now Support:
    BISnpInv
    BISnpData
*/
class S2M_BISnp_Type {
    public:
    bool valid;
    
    u32 opcode;             //  4bit
    enum Opcode_Enum {
        BISnpCur     = 0b0000,
        BISnpData    = 0b0001,
        BISnpInv     = 0b0010,
        BISnpCurBlk  = 0b0100,
        BISnpDataBlk = 0b0101,
        BISnpInvBlk  = 0b0110
    };
    inline std::string getOpcodeName() const {
        switch (opcode) {
            case BISnpCur:     return "BISnpCur";
            case BISnpData:    return "BISnpData";
            case BISnpInv:     return "BISnpInv";
            case BISnpCurBlk:  return "BISnpCurBlk";
            case BISnpDataBlk: return "BISnpDataBlk";
            case BISnpInvBlk:  return "BISnpInvBlk";
            default:           return "Unknown";
        }
    }

                                    u32 bi_id;              // 12bit
    u32 bitag;              // 12bit
                                    u32 address_partity;    //  1bit
    u64 address;            // 46bit
                                    u32 flit_mode;          //  2bit
                                    u32 epoch_valid;        //  1bit
                                    u32 epoch_id;           //  1bit
                                    u32 cmd_parity;         //  1bit
    u32 spid;               // 12bit
    u32 dpid;               // 12bit

    S2M_BISnp_Type() : valid(false), opcode(0), bi_id(0), bitag(0), address_partity(0), address(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), spid(0), dpid(0) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color << S2M_SPACE
                  << "S2M_BISnp :  "    << std::setw(10) << getOpcodeName()
                  << "BITag:"  << reset << "<" << color << std::setw(9) << std::to_string(bitag   % 1000) + reset + "> " << color
                  << "Addr :"  << reset << "(" << color << std::setw(9) << std::to_string(address % 1000) + reset + ") " << color
                  << "DPID :"  << reset << "$" << color << std::setw(9) << std::to_string(dpid    % 1000) + reset + "$ " << color
#ifndef DONT_PRINT_LD_PID
                  << "SPID :"  << std::setw(4) << (spid % 1000)
#endif
                  << reset << std::endl;
    }
};

/*
    Now Support:
    Cmp          
    BIConflictAck
*/
class S2M_NDR_Type {
    public:

    bool valid;
    
    u32 opcode;             //  4bit
    enum Opcode_Enum {
        Cmp           = 0b000,
        CmpS          = 0b001,
        CmpE          = 0b010,
        BIConflictAck = 0b100
    };
    inline std::string getOpcodeName() const {
        switch (opcode) {
            case Cmp:           return "Cmp";
            case CmpS:          return "CmpS";
            case CmpE:          return "CmpE";
            case BIConflictAck: return "BIConflictAck";
            default:            return "Unknown";
        }
    }

                                    u32 metafield;          //  2bit
                                    u32 metavalue;          //  2bit
    u32 tag;                // 16bit
                                    u32 ld_id;              //  4bit
                                    u32 devload;            //  2bit
                                    u32 flit_mode;          //  2bit
                                    u32 epoch_valid;        //  1bit
                                    u32 epoch_id;           //  1bit
                                    u32 cmd_parity;         //  1bit
    u32 dpid;               // 12bit

    S2M_NDR_Type() : valid(false), opcode(0), metafield(0), metavalue(0), tag(0), ld_id(0), devload(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), dpid(0) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color << S2M_SPACE
                  << "S2M_NDR   :  "    << std::setw(10) << getOpcodeName()
                  << "Tag  :"  << reset << "<" << color << std::setw(6) << (tag     % 1000) << reset << "> " << color
                  << "DPID :"  << reset << "$" << color << std::setw(6) << (dpid    % 1000) << reset << "$ " << color
                  << reset << std::endl;
    }
};

/*
    Now Support:
    MemData
*/
class S2M_DRS_Type {
    public:

    bool valid;
    
    u32 opcode;             //  4bit
    enum Opcode_Enum {
        MemData    = 0b000,
        MemDataNXM = 0b001
    };
    inline std::string getOpcodeName() const {
        switch (opcode) {
            case MemData:    return "MemData";
            case MemDataNXM: return "MemDataNXM";
            default:         return "Unknown";
        }
    }

                            u32 metafield;          //  2bit
                            u32 metavalue;          //  2bit
    u32 tag;                // 16bit
                            u32 ld_id;              //  4bit
                            u32 devload;            //  2bit
                            u32 flit_mode;          //  2bit
                            u32 epoch_valid;        //  1bit
                            u32 epoch_id;           //  1bit
                            u32 cmd_parity;         //  1bit
    u32 dpid;               // 12bit
                            u32 data_poison;        //  1bit
                            u64 data_byte_enable;   // 64bit
    u512 data_body;         //512bit

    S2M_DRS_Type() : valid(false), opcode(0), metafield(0), metavalue(0), tag(0), ld_id(0), devload(0), flit_mode(0), epoch_valid(0), epoch_id(0), cmd_parity(0), dpid(0), data_poison(0), data_byte_enable(0), data_body(0) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color << S2M_SPACE
                  << "S2M_DRS   :  "    << std::setw(10) << getOpcodeName()
                  << "Tag  :"  << reset << "<" << color << std::setw(6) << (tag       % 1000) << reset << "> " << color
                  << "Data :"  << reset << "[" << color << std::setw(6) << (data_body % 1000) << reset << "] " << color
                  << "DPID :"  << reset << "$" << color << std::setw(6) << (dpid      % 1000) << reset << "$ " << color
                  << reset << std::endl;
    }
};

/*
    Now Support:
    SnpInv  + MemRd  + Meta0State + A
    SnpData + MemRd  + Meta0State + S
    SnpInv  + MemInv + Meta0State + I
*/
class M2S_Req_Type {
    public:
    bool valid;         //1bit
    
    u32 memopcode;      //4bit
    enum MemOpcode_Enum {
        MemInv     = 0b0000,
        MemRd      = 0b0001,
        MemRdData  = 0b0010,
        MemRdFwd   = 0b0011,
        MemWrFwd   = 0b0100,
        MemSpecRd  = 0b1000,
        MemInvNT   = 0b1001,
        MemClnEvct = 0b1010
    };
    inline std::string getMemOpcodeName() const {
        switch (memopcode) {
            case MemInv:     return "MemInv";
            case MemRd:      return "MemRd";
            case MemRdData:  return "MemRdData";
            case MemRdFwd:   return "MemRdFwd";
            case MemWrFwd:   return "MemWrFwd";
            case MemSpecRd:  return "MemSpecRd";
            case MemInvNT:   return "MemInvNT";
            case MemClnEvct: return "MemClnEvct";
            default:         return "Unknown";
        }
    }
    
    u32 snptype;        //3bit
    enum SnpType_Enum {
        NoOp    = 0b000,
        SnpData = 0b001,
        SnpCur  = 0b010,
        SnpInv  = 0b011
    };
    inline std::string getSnpTypeName() const {
        switch (snptype) {
            case NoOp:    return "NoOp";
            case SnpData: return "SnpData";
            case SnpCur:  return "SnpCur";
            case SnpInv:  return "SnpInv";
            default:      return "Unknown";
        }
    }
    
    u32 metafield;      //2bit
    enum MetaField_Enum {
        Meta0State  = 0b00,
        NoOp_       = 0b11
    };
    inline std::string getMetaFieldName() const {
        switch (metafield) {
            case Meta0State:  return "Meta0State";
            case NoOp_:       return "NoOp";
            default:          return "Unknown";
        }
    }
    
    u32 metavalue;      //2bit
    enum MetaValue_Enum {
        I           = 0b00,
        A           = 0b10,
        S           = 0b11
    };
    inline std::string getMetaValueName() const {
        switch (metavalue) {
            case I:           return "I";
            case A:           return "A";
            case S:           return "S";
            default:          return "Unknown";
        }
    }

    u32 tag;                // 16bit
                                    u32 tc;                 //  2bit    
                                    u32 address5;           //  1bit
                                    u32 address_partity;    //  1bit
    u64 address;            // 46bit
                                    u32 ld_id;              //  4bit
                                    u32 flit_mode;          //  2bit
                                    u32 cmd_parity;         //  1bit
    u32 spid;               // 12bit
    u32 dpid;               // 12bit

    M2S_Req_Type() : valid(false), memopcode(0), snptype(0), metafield(0), metavalue(0), tag(0), tc(0), address5(0), address_partity(0), address(0), ld_id(0), flit_mode(0), cmd_parity(0), spid(0), dpid(0) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "M2S_Req   :  "    << std::setw(22) << getMemOpcodeName() + " + " + getSnpTypeName() + " + " + getMetaValueName()
                  << "Tag  :"  << reset << "<" << color << std::setw(6) << (tag     % 1000) << reset << "> " << color
                  << "Addr :"  << reset << "(" << color << std::setw(6) << (address % 1000) << reset << ") " << color
                  << "SPID :"  << reset << "$" << color << std::setw(6) << (spid    % 1000) << reset << "$ " << color
#ifndef DONT_PRINT_LD_PID
                  << "DPID :"  << std::setw(4) << (dpid % 1000)
#endif
                  << reset << std::endl;
    }
};

/*
    Now Support:
    BIRspI
    BIRspS
*/
class M2S_BIRsp_Type {
    public:
    bool valid;
    
    u32 opcode;             //  4bit
    enum Opcode_Enum {
        BIRspI    = 0b0000,
        BIRspS    = 0b0001,
        BIRspE    = 0b0010,
        BIRspIBlk = 0b0100,
        BIRspSBlk = 0b0101,
        BIRspEBlk = 0b0110
    };
    inline std::string getOpcodeName() const {
        switch (opcode) {
            case BIRspI:    return "BIRspI";
            case BIRspS:    return "BIRspS";
            case BIRspE:    return "BIRspE";
            case BIRspIBlk: return "BIRspIBlk";
            case BIRspSBlk: return "BIRspSBlk";
            case BIRspEBlk: return "BIRspEBlk";
            default:        return "Unknown";
        }
    }

                                    u32 bi_id;              // 12bit
    u32 bitag;              // 12bit
                                    u32 lowaddr;            //  2bit
                                    u32 flit_mode;          //  2bit
                                    u32 cmd_parity;         //  1bit
    u32 dpid;               // 12bit

    M2S_BIRsp_Type() : valid(false), opcode(0), bi_id(0), bitag(0), lowaddr(0), flit_mode(0), cmd_parity(0), dpid(0) {}
    
    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "M2S_BIRsp :  "    << std::setw(22) << getOpcodeName()
                  << "BITag:"  << reset << "<" << color << std::setw(6) << (bitag   % 1000) << reset << "> " << color
#ifndef DONT_PRINT_LD_PID
                  << "DPID :"  << std::setw(4) << (dpid % 1000)
#endif
                  << reset << std::endl;
    }
};

/*
    Now Support:
    MemWr + DontCare + DontCare + DontCare
    BIConflict + DontCare + DontCare + DontCare
*/
class M2S_RwD_Type {
    public:
    bool valid;         //1bit
    
    u32 memopcode;      //4bit
    enum MemOpcode_Enum {
        MemWr       = 0b0001,
        MemWrPtl    = 0b0010,
        BIConflict  = 0b0100
    };
    inline std::string getMemOpcodeName() const {
        switch (memopcode) {
            case MemWr:         return "MemWr";
            case MemWrPtl:      return "MemWrPtl";
            case BIConflict:    return "BIConflict";
            default:            return "Unknown";
        }
    }
    
    u32 snptype;        //3bit
    enum SnpType_Enum {
        NoOp    = 0b000,
        SnpData = 0b001,
        SnpCur  = 0b010,
        SnpInv  = 0b011
    };
    inline std::string getSnpTypeName() const {
        switch (snptype) {
            case NoOp:    return "NoOp";
            case SnpData: return "SnpData";
            case SnpCur:  return "SnpCur";
            case SnpInv:  return "SnpInv";
            default:      return "Unknown";
        }
    }
    
    u32 metafield;      //2bit
    enum MetaField_Enum {
        Meta0State  = 0b00,
        NoOp_       = 0b11
    };
    inline std::string getMetaFieldName() const {
        switch (metafield) {
            case Meta0State:  return "Meta0State";
            case NoOp_:       return "NoOp";
            default:          return "Unknown";
        }
    }
    
    u32 metavalue;      //2bit
    enum MetaValue_Enum {
        I           = 0b00,
        A           = 0b10,
        S           = 0b11
    };
    inline std::string getMetaValueName() const {
        switch (metavalue) {
            case I:           return "I";
            case A:           return "A";
            case S:           return "S";
            default:          return "Unknown";
        }
    }

    u32 tag;                // 16bit
                                    u32 tc;                 //  2bit
                                    u32 address_partity;    //  1bit
    u64 address;            // 46bit
                                    u32 ld_id;              //  4bit
                                    u32 flit_mode;          //  2bit
                                    u32 cmd_parity;         //  1bit
    u32 spid;               // 12bit
    u32 dpid;               // 12bit
                                u32 data_poison;        //  1bit
                                u64 data_byte_enable;   // 64bit
    u512 data_body;         //512bit

    M2S_RwD_Type() : valid(false), memopcode(0), snptype(0), metafield(0), metavalue(0), tag(0), tc(0), address_partity(0), address(0), ld_id(0), flit_mode(0), cmd_parity(0), spid(0), dpid(0), data_poison(0), data_byte_enable(0), data_body(0){}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "M2S_RwD   :  "    << std::setw(22) << getMemOpcodeName() + " + " + getSnpTypeName() + " + " + getMetaValueName()
                  << "Tag  :"  << reset << "<" << color << std::setw(6) << (tag       % 1000) << reset << "> " << color
                  << "Addr :"  << reset << "(" << color << std::setw(6) << (address   % 1000) << reset << ") " << color
                  << "Data :"  << reset << "[" << color << std::setw(6) << (data_body % 1000) << reset << "] " << color
                  << "SPID :"  << reset << "$" << color << std::setw(6) << (spid      % 1000) << reset << "$ " << color
#ifndef DONT_PRINT_LD_PID
                  << "DPID :"  << std::setw(4) << (dpid % 1000)
#endif
                  << reset << std::endl;
    }
};

// AXI 读地址通道 Address Read Channel
class Rd_Addr_Type {
    public:
    bool valid;
    u64 address;

    Rd_Addr_Type() : valid(false), address(0) {}

    Rd_Addr_Type(bool Valid, u64 Address) : valid(Valid), address(Address) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "Rd_Addr   :  " << MEM_SPACE
                  << "Addr :"  << reset << "(" << color << std::setw(6) << (address   % 1000) << reset << ") " << color
                  << reset << std::endl;
    }
};

// AXI 读数据通道 Read Channel
class Rd_Data_Type {
    public:
    bool valid;
    u64 address;
    u512 data_body;

    Rd_Data_Type() : valid(false), address(0), data_body(0) {}

    Rd_Data_Type(bool Valid, u64 Address, u512 Data) : valid(Valid), address(Address), data_body(Data) {}

    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "Rd_Data   :  " << MEM_SPACE
                  << "Addr :"  << reset << "(" << color << std::setw(6) << (address   % 1000) << reset << ") " << color
                  << "Data :"  << reset << "[" << color << std::setw(6) << (data_body % 1000) << reset << "] " << color
                  << reset << std::endl;
    }
};

// AXI 写地址/数据通道 Write Address Channel + Write Data Channel
class Wr_Type {
    public:
    bool valid;
    u64 address;
    u512 data_body;

    Wr_Type() : valid(false), address(0), data_body(0) {}

    Wr_Type(bool Valid, u64 Address, u512 Data) : valid(Valid), address(Address), data_body(Data) {}

    //Print
    inline void print(std::string color, std::string reset) const {
        std::cout << std::left << color
                  << "Wr_Addr   :  " << MEM_SPACE
                  << "Addr :"  << reset << "(" << color << std::setw(6) << (address   % 1000) << reset << ") " << color
                  << "Data :"  << reset << "[" << color << std::setw(6) << (data_body % 1000) << reset << "] " << color
                  << reset << std::endl;
    }
};


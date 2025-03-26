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

/*
    Now Support:
    BISnpInv
    BISnpData
*/
class S2M_BISnp_Type {
    public:

    //1bit
    bool Valid;

    //4bit
    unsigned int Opcode;
    enum Opcode_Enum {
        BISnpCur     = 0b0000,
        BISnpData    = 0b0001,
        BISnpInv     = 0b0010,
        BISnpCurBlk  = 0b0100,
        BISnpDataBlk = 0b0101,
        BISnpInvBlk  = 0b0110
    };

    //12bit
    int BITag;

    //46bit
    u64 Address;

    //12bit
    unsigned int SPID;

    //12bit
    unsigned int DPID;

    S2M_BISnp_Type() : Valid(false), Opcode(0), BITag(0), Address(0), SPID(0), DPID(0) {}
    S2M_BISnp_Type(bool Valid, int Opcode, int BITag, u64 Address, int SPID, int DPID) :
        Valid(Valid), Opcode(Opcode), BITag(BITag), Address(Address), SPID(SPID), DPID(DPID) {}

    //Print
    std::string getOpcodeName() const {
        switch (Opcode) {
            case BISnpCur:      return "BISnpCur";
            case BISnpData:     return "BISnpData";
            case BISnpInv:      return "BISnpInv";
            case BISnpCurBlk:   return "BISnpCurBlk";
            case BISnpDataBlk:  return "BISnpDataBlk";
            case BISnpInvBlk:   return "BISnpInvBlk";
            default:            return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33)
                  << "S2M_BISnp :  " + getOpcodeName()
                  << std::setw(6) << "BITag:" << std::setw(4) << (BITag % 1000)   << " "
                  << std::setw(6) << "Addr:"  << std::setw(4) << (Address % 1000) << " "
                  << std::setw(6) << "SPID:"  << std::setw(4) << (SPID % 1000)    << " "
                  << std::setw(6) << "DPID:"  << std::setw(4) << (DPID % 1000)
                  << std::endl;
    }
};

/*
    Now Support:
    Cmp          
    CmpS         
    CmpE         
    BIConflictAck
*/
class S2M_NDR_Type {
    public:

    //1bit
    bool Valid;

    //3bit
    unsigned int Opcode;
    enum Opcode_Enum {
        Cmp             = 0b000,
        CmpS            = 0b001,
        CmpE            = 0b010,
        BIConflictAck   = 0b100
    };

    //16bit
    unsigned int Tag;

    //12bit
    unsigned int DPID;

    S2M_NDR_Type() : Valid(false), Opcode(0), Tag(0), DPID(0) {}

    S2M_NDR_Type(bool Valid, int Opcode, int Tag, int DPID) :
        Valid(Valid), Opcode(Opcode), Tag(Tag), DPID(DPID) {}

    //Print
    std::string getOpcodeName() const {
        switch (Opcode) {
            case Cmp:           return "Cmp";
            case CmpS:          return "CmpS";
            case CmpE:          return "CmpE";
            case BIConflictAck: return "BIConflictAck";
            default:            return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33) 
                  << "S2M_NDR :  " + getOpcodeName()
                  << std::setw(6) << "Tag:"  << std::setw(4) << (Tag % 1000) << " "
                  << std::setw(6) << "DPID:" << std::setw(4) << (DPID % 1000)
                  << std::endl;
    }
};

/*
    Now Support:
    MemData
*/
class S2M_DRS_Type {
    public:

    //1bit
    bool Valid;

    //3bit
    unsigned int Opcode;
    enum Opcode_Enum {
        MemData      = 0b000,
        MemDataNXM   = 0b001
    };

    //16bit
    unsigned int Tag;

    //12bit
    unsigned int DPID;

    u512 Data;

    S2M_DRS_Type() : Valid(false), Opcode(0), Tag(0), DPID(0), Data(0){}

    S2M_DRS_Type(bool Valid, int Opcode, int Tag, int DPID, u512 Data) :
        Valid(Valid), Opcode(Opcode), Tag(Tag), DPID(DPID), Data(Data) {}

    //Print
    std::string getOpcodeName() const {
        switch (Opcode) {
            case MemData:       return "MemData";
            case MemDataNXM:    return "MemDataNXM";
            default:            return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33) 
                  << "S2M_DRS :  " + getOpcodeName()
                  << std::setw(6) << "Tag:"  << std::setw(4) << (Tag % 1000)  << " "
                  << std::setw(6) << "DPID:" << std::setw(4) << (DPID % 1000) << " "
                  << std::setw(6) << "Data:" << std::setw(4) << (static_cast<unsigned long long>(Data) % 1000)
                  << std::endl;
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

    //1bit
    bool Valid;

    //4bit
    unsigned int MemOpcode;
    enum MemOpcode_Enum {
        MemInv      = 0b0000,
        MemRd       = 0b0001,
        MemRdData   = 0b0010,
        MemRdFwd    = 0b0011,
        MemWrFwd    = 0b0100,
        MemSpecRd   = 0b1000,
        MemInvNT    = 0b1001,
        MemClnEvct  = 0b1010
    };

    //3bit
    unsigned int SnpType;
    enum SnpType_Enum {
        NoOp        = 0b000,
        SnpData     = 0b001,
        SnpCur      = 0b010,
        SnpInv      = 0b011
    };

    //2bit
    unsigned int MetaField;
    enum MetaField_Enum {
        Meta0State  = 0b00,
        NoOp_       = 0b11
    };

    //2bit
    unsigned int MetaValue;
    enum MetaValue_Enum {
        I           = 0b00,
        A           = 0b10,
        S           = 0b11
    };

    //16bit
    unsigned int Tag;

    //46bit
    u64 Address;

    //12bit
    unsigned int SPID;

    //12bit
    unsigned int DPID;

    M2S_Req_Type() : Valid(false), MemOpcode(0), SnpType(0), MetaField(0), MetaValue(0), Tag(0), Address(0), SPID(0), DPID(0) {}

    M2S_Req_Type(bool Valid, int MemOpcode, int SnpType, int MetaField, int MetaValue, int Tag, u64 Address, int SPID, int DPID) :
        Valid(Valid), MemOpcode(MemOpcode), SnpType(SnpType), MetaField(MetaField), MetaValue(MetaValue), Tag(Tag), Address(Address), SPID(SPID), DPID(DPID) {}

    //Print
    std::string getMemOpcodeName() const {
        switch (MemOpcode) {
            case MemInv:      return "MemInv";
            case MemRd:       return "MemRd";
            case MemRdData:   return "MemRdData";
            case MemRdFwd:    return "MemRdFwd";
            case MemWrFwd:    return "MemWrFwd";
            case MemSpecRd:   return "MemSpecRd";
            case MemInvNT:    return "MemInvNT";
            case MemClnEvct:  return "MemClnEvct";
            default:          return "Unknown";
        }
    }
    std::string getSnpTypeName() const {
        switch (SnpType) {
            case NoOp:        return "NoOp";
            case SnpData:     return "SnpData";
            case SnpCur:      return "SnpCur";
            case SnpInv:      return "SnpInv";
            default:          return "Unknown";
        }
    }
    std::string getMetaFieldName() const {
        switch (MetaField) {
            case Meta0State:  return "Meta0State";
            case NoOp_:       return "NoOp";
            default:          return "Unknown";
        }
    }
    std::string getMetaValueName() const {
        switch (MetaValue) {
            case I:           return "I";
            case A:           return "A";
            case S:           return "S";
            default:          return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33) 
                  << "M2S_Req :  " + getMemOpcodeName() + " + " + getSnpTypeName() + " + " /*+ getMetaFieldName() << " + "*/ + getMetaValueName()
                  << std::setw(6) << "Tag:"  << std::setw(4) << (Tag % 1000)     << " "
                  << std::setw(6) << "Addr:" << std::setw(4) << (Address % 1000) << " "
                  << std::setw(6) << "SPID:" << std::setw(4) << (SPID % 1000)    << " "
                  << std::setw(6) << "DPID:" << std::setw(4) << (DPID % 1000)    << " "
                  << std::endl;
    }
};

/*
    Now Support:
    BIRspI
    BIRspS
*/
class M2S_BIRsp_Type {
    public:

    //1bit
    bool Valid;

    //4bit
    unsigned int Opcode;
    enum Opcode_Enum {
        BIRspI     = 0b0000,
        BIRspS     = 0b0001,
        BIRspE     = 0b0010,
        BIRspIBlk  = 0b0100,
        BIRspSBlk  = 0b0101,
        BIRspEBlk  = 0b0110
    };

    //12bit
    unsigned int BITag;

    //12bit
    unsigned int DPID;

    M2S_BIRsp_Type() : Valid(false), Opcode(0), BITag(0), DPID(0) {}

    M2S_BIRsp_Type(bool Valid, int Opcode, int BITag, int DPID) :
        Valid(Valid), Opcode(Opcode), BITag(BITag), DPID(DPID) {}

    //Print
    std::string getOpcodeName() const {
        switch (Opcode) {
            case BIRspI:     return "BIRspI";
            case BIRspS:     return "BIRspS";
            case BIRspE:     return "BIRspE";
            case BIRspIBlk:  return "BIRspIBlk";
            case BIRspSBlk:  return "BIRspSBlk";
            case BIRspEBlk:  return "BIRspEBlk";
            default:         return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33) 
                  << "M2S_BIRsp :  " + getOpcodeName()
                  << std::setw(6) << "BITag:" << std::setw(4) << (BITag % 1000) << " "
                  << std::setw(6) << "DPID:"  << std::setw(4) << (DPID % 1000)
                  << std::endl;
    }
};

/*
    Now Support:
    MemWr + NoOp + NoOp + DontCare
    BIConflict + NoOp + NoOp + DontCare
*/
class M2S_RwD_Type {
    public:

    //1bit
    bool Valid;

    //4bit
    unsigned int MemOpcode;
    enum MemOpcode_Enum {
        MemWr       = 0b0001,
        MemWrPtl    = 0b0010,
        BIConflict  = 0b0100
    };

    //3bit
    unsigned int SnpType;
    enum SnpType_Enum {
        NoOp        = 0b000,
        SnpData     = 0b001,
        SnpCur      = 0b010,
        SnpInv      = 0b011
    };

    //2bit
    unsigned int MetaField;
    enum MetaField_Enum {
        Meta0State  = 0b00,
        NoOp_       = 0b11
    };

    //2bit
    unsigned int MetaValue;
    enum MetaValue_Enum {
        I           = 0b00,
        A           = 0b10,
        S           = 0b11
    };

    //16bit
    unsigned int Tag;

    //46bit
    u64 Address;

    //12bit
    unsigned int SPID;

    //12bit
    unsigned int DPID;

    u512 Data;

    M2S_RwD_Type() : Valid(false), MemOpcode(0), SnpType(0), MetaField(0), MetaValue(0), Tag(0), Address(0), SPID(0), DPID(0), Data(0){}

    M2S_RwD_Type(bool Valid, int MemOpcode, int SnpType, int MetaField, int MetaValue, int Tag, u64 Address, int SPID, int DPID, u512 Data) :
        Valid(Valid), MemOpcode(MemOpcode), SnpType(SnpType), MetaField(MetaField), MetaValue(MetaValue), Tag(Tag), Address(Address), SPID(SPID), DPID(DPID), Data(Data) {}

    //Print
    std::string getMemOpcodeName() const {
        switch (MemOpcode) {
            case MemWr:         return "MemWr";
            case MemWrPtl:      return "MemWrPtl";
            case BIConflict:    return "BIConflict";
            default:            return "Unknown";
        }
    }
    std::string getSnpTypeName() const {
        switch (SnpType) {
            case NoOp:          return "NoOp";
            case SnpData:       return "SnpData";
            case SnpCur:        return "SnpCur";
            case SnpInv:        return "SnpInv";
            default:            return "Unknown";
        }
    }
    std::string getMetaFieldName() const {
        switch (MetaField) {
            case Meta0State:    return "Meta0State";
            case NoOp_:         return "NoOp_Meta";
            default:            return "Unknown";
        }
    }
    std::string getMetaValueName() const {
        switch (MetaValue) {
            case I:             return "I";
            case A:             return "A";
            case S:             return "S";
            default:            return "Unknown";
        }
    }
    void print() const {
        std::cout << std::left
                  << std::setw(33) 
                  << "M2S_RwD :  " + getMemOpcodeName() + " + " + getSnpTypeName() + " + " /*+ getMetaFieldName() << " + "*/ + getMetaValueName()
                  << std::setw(6) << "Tag:"  << std::setw(4) << (Tag % 1000)     << " "
                  << std::setw(6) << "Addr:" << std::setw(4) << (Address % 1000) << " "
                  << std::setw(6) << "SPID:" << std::setw(4) << (SPID % 1000)    << " "
                  << std::setw(6) << "DPID:" << std::setw(4) << (DPID % 1000)    << " "
                  << std::setw(6) << "Data:" << std::setw(4) << (static_cast<unsigned long long>(Data) % 1000)
                  << std::endl;
    }
};

// 读地址通道 Address Read Channel
class Rd_Addr_Type {
    public:
    bool Valid;
    u64 Address;

    Rd_Addr_Type() : Valid(false), Address(0) {}

    Rd_Addr_Type(bool Valid, u64 Address) : Valid(Valid), Address(Address) {}

    //Print
    void print() const {
        std::cout << std::left
                  << std::setw(33) << "Rd_Addr :  "
                  << std::setw(6)  << "Addr:" << std::setw(4) << (Address % 1000)
                  << std::endl;
    }
};

// 读数据通道 Read Channel
class Rd_Data_Type {
    public:
    bool Valid;
    u64 Address;
    u512 Data;

    Rd_Data_Type() : Valid(false), Address(0), Data(0) {}

    Rd_Data_Type(bool Valid, u64 Address, u512 Data) : Valid(Valid), Address(Address), Data(Data) {}

    //Print
    void print() const {
        std::cout << std::left
                  << std::setw(33) << "Rd_Data :  "
                  << std::setw(6)  << "Addr:" << std::setw(4) << (Address % 1000) << " "
                  << std::setw(6)  << "Data:" << std::setw(4) << (static_cast<unsigned long long>(Data) % 1000)
                  << std::endl;
    }
};

// 写地址/数据通道 Write Address Channel + Write Data Channel
class Wr_Type {
    public:
    bool Valid;
    u64 Address;
    u512 Data;

    Wr_Type() : Valid(false), Address(0), Data(0) {}

    Wr_Type(bool Valid, u64 Address, u512 Data) : Valid(Valid), Address(Address), Data(Data) {}

    //Print
    void print() const {
        std::cout << std::left
                  << std::setw(33) << "Wr :  "
                  << std::setw(6)  << "Addr:" << std::setw(4) << (Address % 1000) << " "
                  << std::setw(6)  << "Data:" << std::setw(4) << (static_cast<unsigned long long>(Data) % 1000)
                  << std::endl;
    }
};

#include <list>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <string>

#define u64         unsigned long long
#define u512        unsigned long long  //unsigned long long位宽不足512位，暂定如此

#define HOST_NUM    7                   //此段共享内存有 HOST_NUM 个主机共享
#define LD_PID      9                   //LD(Logic Device) PID
  
#define CPP_ERROR(message)        printf(message)









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





/*  
    vsf 即 Virtual Snoop Filter
*/



// 功能：用于创建 BITag 和 HostID 之间的映射关系
// 使用：Virtual Snoop Filter 内只有一个 BITag_HostID_Mapper 类的对象
// 目前策略：BITag 采用递增的方式分配
class BITag_S2MBISnp_Mapper{
    private:
    std::unordered_map<unsigned int, S2M_BISnp_Type> mapper;
    // 当前BITag的值
    unsigned int BItag_now;

    public:
        // 将当前BITag和S2M_BISnp_Type的映射关系存入mapper中
    // 根据 HostID 申请 BITag
        // BITag的值加1
    unsigned int push(S2M_BISnp_Type s2m_bisnp){
        mapper[BItag_now] = s2m_bisnp;
        return BItag_now++;
    // 根据 BITag 获取 S2M_BISnp_Type
    }
        // 如果mapper中存在该BITag，则返回对应的S2M_BISnp_Type

    S2M_BISnp_Type find(unsigned int BItag){
        if(mapper.find(BItag) != mapper.end()){
        // 否则，输出错误信息，并返回空的S2M_BISnp_Type
            return mapper[BItag];
        }
        else{
            CPP_ERROR("BITag_HostID_Mapper: find error");
            return S2M_BISnp_Type();
        }
    }

    // 根据 BITag 获取 HostID
    S2M_BISnp_Type erase(unsigned int BItag){
        if(mapper.find(BItag) != mapper.end()){
            S2M_BISnp_Type ret = mapper[BItag];
            mapper.erase(BItag);
            return ret;
        }
        else{
            CPP_ERROR("BITag_HostID_Mapper: pop error");
            return S2M_BISnp_Type();
        }
    }

    BITag_S2MBISnp_Mapper() : BItag_now(0) { }
    ~BITag_S2MBISnp_Mapper() { 
        mapper.clear();
    }
};

// Virtual Snoop Filter 中增加状态 Uncertain, NeedToBeRead
enum CACHELINE_STATE{
    EM,
    S,
    I,
    NeedForRspI,
    NeedForRspIS,
    NeedForMemData
};

// Virtual Snoop Filter 记录一个 Host 的一个 Cacheline 的状态
class VSF_Elecment_Type{
    public:
    CACHELINE_STATE cacheline_state;
    int HostID;

    VSF_Elecment_Type() : cacheline_state(CACHELINE_STATE::I), HostID(0) { }
    VSF_Elecment_Type(int HostID) : cacheline_state(CACHELINE_STATE::I) {
        this->HostID = HostID;
    }
};

// Virtual Snoop Filter 记录全部 Host 的一个 Cacheline 的状态
class VSF_Line_Type{
    private:
    VSF_Elecment_Type vsf_elecment[HOST_NUM]; //记录全部 Host 的一个 Cacheline 的状态

    enum {
        Idle,
        Wait_For_BIRsp,
        Wait_For_RdAddr,
        Wait_For_RdData,
        Wait_For_DRS_NDR
    } state;                    // 一次 M2S_Req 事务进行到哪一步
    bool skip_RdData;           // 用于判断是否需要发送 DRS 或 NDR
    bool skip_DRS_NDR;          //
    unsigned int tag;
    unsigned int host_id;
    u64 cacheline_addr;
    u512 cacheline_data;

    public:
    VSF_Line_Type() : state(Idle), skip_RdData(0), skip_DRS_NDR(0), host_id(0){
        for (unsigned int i = 0; i < HOST_NUM; i++) {
            vsf_elecment[i] = VSF_Elecment_Type(i);
        }
    }

    bool isIdle() { return state == Idle; }

    // 返回值为1，则让 Virtual Snoop Filter "victim 该 Cacheline" 
    bool produce(
        BITag_S2MBISnp_Mapper& bitag_s2mbisnp_mapper,
        const M2S_Req_Type& m2s_req,
        const M2S_BIRsp_Type& m2s_birsp,
        const Rd_Data_Type& rd_data,
        std::list<S2M_BISnp_Type>& s2m_bisnp,
        Rd_Addr_Type& rd_addr,
        S2M_NDR_Type& s2m_ndr,
        S2M_DRS_Type& s2m_drs
    ) {
        if(state == Idle){
            if(m2s_req.Valid){
                skip_RdData = 0;
                skip_DRS_NDR = 0;
                tag = m2s_req.Tag;
                host_id = m2s_req.SPID;
                cacheline_addr = m2s_req.Address;
                cacheline_data = 0;

                state = Wait_For_BIRsp;
                if( m2s_req.SnpType   == M2S_Req_Type::SnpType_Enum  ::SnpInv &&
                    m2s_req.MemOpcode == M2S_Req_Type::MemOpcode_Enum::MemRd &&
                    m2s_req.MetaField == M2S_Req_Type::MetaField_Enum::Meta0State &&
                    m2s_req.MetaValue == M2S_Req_Type::MetaValue_Enum::A
                ){
                    for(unsigned int i = 0; i < HOST_NUM; i++){
                        if(i == host_id){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::NeedForMemData;
                        }
                        else if(vsf_elecment[i].cacheline_state != I){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::NeedForRspI;
                            S2M_BISnp_Type tmp(
                                true,  
                                S2M_BISnp_Type::Opcode_Enum::BISnpInv,
                                0,
                                cacheline_addr,
                                LD_PID,
                                i
                            );
                            tmp.BITag = bitag_s2mbisnp_mapper.push(tmp);
                            s2m_bisnp.push_back(tmp);
                        }
                    }
                }
                else if(
                    m2s_req.SnpType   == M2S_Req_Type::SnpType_Enum  ::SnpData &&
                    m2s_req.MemOpcode == M2S_Req_Type::MemOpcode_Enum::MemRd &&
                    m2s_req.MetaField == M2S_Req_Type::MetaField_Enum::Meta0State &&
                    m2s_req.MetaValue == M2S_Req_Type::MetaValue_Enum::S
                ){
                    for(unsigned int i = 0; i < HOST_NUM; i++){
                        if(i == host_id){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::NeedForMemData;
                        }
                        else if(vsf_elecment[i].cacheline_state != I){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::NeedForRspIS;
                            S2M_BISnp_Type tmp(
                                true,  
                                S2M_BISnp_Type::Opcode_Enum::BISnpData,
                                0,
                                cacheline_addr,
                                LD_PID,
                                i
                            );
                            tmp.BITag = bitag_s2mbisnp_mapper.push(tmp);
                            s2m_bisnp.push_back(tmp);
                        }
                    }
                }
                else if(
                    m2s_req.SnpType   == M2S_Req_Type::SnpType_Enum  ::SnpInv &&
                    m2s_req.MemOpcode == M2S_Req_Type::MemOpcode_Enum::MemInv &&
                    m2s_req.MetaField == M2S_Req_Type::MetaField_Enum::Meta0State &&
                    m2s_req.MetaValue == M2S_Req_Type::MetaValue_Enum::I
                ){
                    for(unsigned int i = 0; i < HOST_NUM; i++){
                        if(i == host_id){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::I;
                        }
                        else if(vsf_elecment[i].cacheline_state != I){
                            vsf_elecment[i].cacheline_state = CACHELINE_STATE::NeedForRspI;
                            S2M_BISnp_Type tmp(
                                true,  
                                S2M_BISnp_Type::Opcode_Enum::BISnpInv,
                                0,
                                cacheline_addr,
                                LD_PID,
                                i
                            );
                            tmp.BITag = bitag_s2mbisnp_mapper.push(tmp);
                            s2m_bisnp.push_back(tmp);
                            skip_RdData = 0;
                            skip_DRS_NDR = 0;
                        }
                    }
                }
                else{
                    CPP_ERROR("vsf: Unknown SnpType/MemOpcode/MetaField/MetaValue in M2S_Req");
                }
            }
            else{
                return 0;
            }
        }

        if(state == Wait_For_BIRsp){
            if(m2s_birsp.Valid){
                S2M_BISnp_Type tmp = bitag_s2mbisnp_mapper.find(m2s_birsp.BITag);
                if(tmp.Address == cacheline_addr){
                    bitag_s2mbisnp_mapper.erase(m2s_birsp.BITag); //可优化

                    if(m2s_birsp.Opcode == M2S_BIRsp_Type::Opcode_Enum::BIRspI){
                        if(vsf_elecment[tmp.DPID].cacheline_state == NeedForRspI || vsf_elecment[tmp.DPID].cacheline_state == NeedForRspIS){
                            vsf_elecment[tmp.DPID] = I;
                        }
                        else{
                            CPP_ERROR("vsf: Wrong Opcode in M2S_BIRsp");
                        }
                    }
                    else if(m2s_birsp.Opcode == M2S_BIRsp_Type::Opcode_Enum::BIRspS){
                        if(vsf_elecment[tmp.DPID].cacheline_state == NeedForRspIS){
                            vsf_elecment[tmp.DPID] = S;
                        }
                        else{
                            CPP_ERROR("vsf: Wrong Opcode in M2S_BIRsp");
                        }
                    }
                    else{
                        CPP_ERROR("vsf: Unknown Opcode in M2S_BIRsp");
                    }
                }
            }

            bool uncertain_flag = 0;
            for(unsigned int i = 0; i < HOST_NUM; i++){
                if(vsf_elecment[i].cacheline_state == NeedForRspI || vsf_elecment[i].cacheline_state == NeedForRspIS){
                    uncertain_flag = 1;
                    break;
                }
            }
            if(uncertain_flag == 0){
                state = Wait_For_RdAddr;
            }
            else{
                return 0;
            }
        }

        if(state == Wait_For_RdAddr){
            state = Wait_For_RdData;
            if(vsf_elecment[host_id].cacheline_state == NeedForMemData){
                rd_addr = Rd_Addr_Type(true, cacheline_addr);

                unsigned int Icnt = 0;
                for(unsigned int i = 0; i < HOST_NUM; i++){
                    if(vsf_elecment[i].cacheline_state == I){
                        Icnt++;
                    }
                }
                if(Icnt == HOST_NUM - 1){
                    vsf_elecment[host_id].cacheline_state = CACHELINE_STATE::EM;
                }
                else{
                    vsf_elecment[host_id].cacheline_state = CACHELINE_STATE::S;
                }
            }
        }

        if(state == Wait_For_RdData){
            if(skip_RdData){
                state = Wait_For_DRS_NDR;
            }
            else if(rd_data.Valid && rd_data.Address == cacheline_addr){ //可优化
                state = Wait_For_DRS_NDR;
                cacheline_data = rd_data.Data;
            }
            else{
                return 0;
            }
        }

        if(state == Wait_For_DRS_NDR){
            state = Idle;
            if(skip_DRS_NDR == 0){
                if(vsf_elecment[host_id].cacheline_state == EM){
                    s2m_ndr = S2M_NDR_Type(true, S2M_NDR_Type::Opcode_Enum::CmpE, tag, host_id);
                }
                else if(vsf_elecment[host_id].cacheline_state == S){
                    s2m_ndr = S2M_NDR_Type(true, S2M_NDR_Type::Opcode_Enum::CmpS, tag, host_id);
                }
                s2m_drs = S2M_DRS_Type(true, S2M_DRS_Type::Opcode_Enum::MemData, tag, host_id, cacheline_data);
            }

            //自动victim
                unsigned int Icnt = 0;
                for(unsigned int i = 0; i < HOST_NUM; i++){
                    if(vsf_elecment[i].cacheline_state == I){
                        Icnt++;
                    }
                }
                if(Icnt == HOST_NUM){
                    return 1;
                }
            
        }
        return 0;
    }
};

// Victim/Alloc Strategy 未知
// Victim Addr X 暂为 " Host 主动发送 SnpInv + MemInv + I，且不用回复 Host "

class vsf_Type{
    public:
    BITag_S2MBISnp_Mapper bitag_s2mbisnp_mapper;
    std::list<M2S_Req_Type> m2s_req_list;
    std::unordered_map<u64, VSF_Line_Type> vsf_line;

    vsf_Type(){ }
    ~vsf_Type(){ 
        m2s_req_list.clear();
        vsf_line.clear();
    }

    void produce(
        const M2S_Req_Type& m2s_req,
        const M2S_BIRsp_Type& m2s_birsp,
        const M2S_RwD_Type& m2s_rwd,
        const Rd_Data_Type& rd_data,
        std::list<S2M_BISnp_Type>& s2m_bisnp,
        std::list<Rd_Addr_Type>& rd_addr,
        std::list<S2M_NDR_Type>& s2m_ndr,
        std::list<S2M_DRS_Type>& s2m_drs,
        std::list<Wr_Type>& wr
        //vector
    ) {
        //pre_function();
        //main();
        //post();
        std::list<S2M_BISnp_Type>  s2m_bisnp_empty;

        Rd_Addr_Type rd_addr_empty;
        Rd_Addr_Type rd_addr_priority_0;
        Rd_Addr_Type rd_addr_priority_1;

        S2M_NDR_Type s2m_ndr_empty;
        S2M_NDR_Type s2m_ndr_priority_0;
        S2M_NDR_Type s2m_ndr_priority_1;
        S2M_NDR_Type s2m_ndr_priority_2;
        S2M_NDR_Type s2m_ndr_priority_3;

        S2M_DRS_Type s2m_drs_empty;
        S2M_DRS_Type s2m_drs_priority_0;
        S2M_DRS_Type s2m_drs_priority_1;

        if(m2s_rwd.Valid){
            if(m2s_rwd.MemOpcode == M2S_RwD_Type::MemOpcode_Enum::BIConflict){
                s2m_ndr_priority_0 = S2M_NDR_Type(true, S2M_NDR_Type::Opcode_Enum::BIConflictAck, m2s_rwd.Tag, m2s_rwd.SPID);
            }
            else if(m2s_rwd.MemOpcode == M2S_RwD_Type::MemOpcode_Enum::MemWr){
                s2m_ndr_priority_0 = S2M_NDR_Type(true, S2M_NDR_Type::Opcode_Enum::Cmp, m2s_rwd.Tag, m2s_rwd.SPID);
                wr.push_back(Wr_Type(true, m2s_rwd.Address, m2s_rwd.Data));
            }
        }

        if(rd_data.Valid){
            int tag = rd_data.Address;
            auto it = vsf_line.find(tag);
            if (it != vsf_line.end()) {
                if(it->second.produce(
                    bitag_s2mbisnp_mapper, 
                    M2S_Req_Type(), 
                    M2S_BIRsp_Type(),
                    rd_data,
                    s2m_bisnp_empty,
                    rd_addr_empty,
                    s2m_ndr_priority_1,
                    s2m_drs_priority_0
                )){
                    vsf_line.erase(it);
                }
            }
            else {
                CPP_ERROR("rd_data Valid but not found in vsf_line");
            }
        }

        if(m2s_birsp.Valid){
            int tag = bitag_s2mbisnp_mapper.find(m2s_birsp.BITag).Address;
            auto it = vsf_line.find(tag);
            if (it != vsf_line.end()) {
                if(it->second.produce(
                    bitag_s2mbisnp_mapper, 
                    M2S_Req_Type(), 
                    m2s_birsp,
                    Rd_Data_Type(),
                    s2m_bisnp_empty,
                    rd_addr_priority_0,
                    s2m_ndr_priority_2,
                    s2m_drs_priority_1
                )){
                    vsf_line.erase(it);
                }
            }
            else {
                CPP_ERROR("rd_data Valid but not found in vsf_line");
            }
        }

        if(m2s_req.Valid){
            m2s_req_list.push_back(m2s_req);
        }

        for (auto it = m2s_req_list.begin(); it != m2s_req_list.end(); ) {
            M2S_Req_Type m2s_req_tmp = *it;

            int tag = m2s_req_tmp.Address;
            auto it_ = vsf_line.find(tag);
            if (it_ != vsf_line.end()) {
                if(!it_->second.isIdle()) continue;

                if(it_->second.produce(
                    bitag_s2mbisnp_mapper, 
                    m2s_req_tmp, 
                    M2S_BIRsp_Type(),
                    Rd_Data_Type(),
                    s2m_bisnp,
                    rd_addr_priority_0,
                    s2m_ndr_priority_3,
                    s2m_drs_empty
                )){
                    vsf_line.erase(it_);
                }
                m2s_req_list.erase(it);
                break;
            } else {
                // 如果不存在，则新建一个键值对，并调用 produce_m2s_req
                VSF_Line_Type tmp;
                
                if(tmp.produce(
                    bitag_s2mbisnp_mapper, 
                    m2s_req_tmp, 
                    M2S_BIRsp_Type(),
                    Rd_Data_Type(),
                    s2m_bisnp,
                    rd_addr_priority_0,
                    s2m_ndr_priority_3,
                    s2m_drs_empty
                )){
                    vsf_line.erase(it_);
                }
                vsf_line[tag] = tmp;
                m2s_req_list.erase(it);
                break;
            }
            
        }

        if(rd_addr_priority_0.Valid) rd_addr.push_back(rd_addr_priority_0);
        if(rd_addr_priority_1.Valid) rd_addr.push_back(rd_addr_priority_1);

        if(s2m_ndr_priority_0.Valid) s2m_ndr.push_back(s2m_ndr_priority_0);
        if(s2m_ndr_priority_1.Valid) s2m_ndr.push_back(s2m_ndr_priority_1);
        if(s2m_ndr_priority_2.Valid) s2m_ndr.push_back(s2m_ndr_priority_2);
        if(s2m_ndr_priority_3.Valid) s2m_ndr.push_back(s2m_ndr_priority_3);

        if(s2m_drs_priority_0.Valid) s2m_drs.push_back(s2m_drs_priority_0);
        if(s2m_drs_priority_1.Valid) s2m_drs.push_back(s2m_drs_priority_1);
    }
};





class vsf_test_Type
{
private:
    vsf_Type vsf;
    std::list<S2M_BISnp_Type> s2m_bisnp;
    std::list<Rd_Addr_Type> rd_addr;
    std::list<S2M_NDR_Type> s2m_ndr;
    std::list<S2M_DRS_Type> s2m_drs;
    std::list<Wr_Type> wr;

    std::list<Rd_Data_Type> rd_data_list;
public:

    void produce_pop_and_print_all(
        const M2S_Req_Type& m2s_req,
        const M2S_BIRsp_Type& m2s_birsp,
        const M2S_RwD_Type& m2s_rwd,
        const Rd_Data_Type& rd_data
    ) {
        produce(m2s_req, m2s_birsp, m2s_rwd, rd_data);
        pop_and_print_all();
    }

    void produce(
        const M2S_Req_Type& m2s_req,
        const M2S_BIRsp_Type& m2s_birsp,
        const M2S_RwD_Type& m2s_rwd,
        const Rd_Data_Type& rd_data
    ) {
        //
        Rd_Data_Type tmp_rd_data;
        if(!rd_data_list.empty()){
            tmp_rd_data = rd_data_list.front();
            rd_data_list.pop_front();
        }


        vsf.produce(m2s_req, m2s_birsp, m2s_rwd, tmp_rd_data, s2m_bisnp, rd_addr, s2m_ndr, s2m_drs, wr);
        if(m2s_req.Valid) m2s_req.print();
        if(m2s_birsp.Valid) m2s_birsp.print();
        if(m2s_rwd.Valid) m2s_rwd.print();
        if(tmp_rd_data.Valid) tmp_rd_data.print();
    }
    void pop_and_print_selected(bool pop_bisnp, bool pop_rdaddr, bool pop_ndr, bool pop_drs, bool pop_wr) {
        if (pop_bisnp && !s2m_bisnp.empty()) {
            std::cout << "                                                                ";
            s2m_bisnp.front().print();
            s2m_bisnp.pop_front();
        }
        if (pop_rdaddr && !rd_addr.empty()) {
            std::cout << "                                                                ";
            rd_addr.front().print();
            rd_data_list.push_back(Rd_Data_Type(true, rd_addr.front().Address, rd_addr.front().Address + 500)); //
            rd_addr.pop_front();
        }
        if (pop_ndr && !s2m_ndr.empty()) {
            std::cout << "                                                                ";
            s2m_ndr.front().print();
            s2m_ndr.pop_front();
        }
        if (pop_drs && !s2m_drs.empty()) {
            std::cout << "                                                                ";
            s2m_drs.front().print();
            s2m_drs.pop_front();
        }
        if (pop_wr && !wr.empty()) {
            std::cout << "                                                                ";
            wr.front().print();
            wr.pop_front();
        }
    }
    void pop_and_print_all() {
        while (!s2m_bisnp.empty() || !rd_addr.empty() || !s2m_ndr.empty() || !s2m_drs.empty() || !wr.empty()) {
            pop_and_print_selected(true, true, true, true, true);
        }
    }
};














/*
    Assume:
    M2S REQ/RSP/DATA channel


    应该还有一个HDM decoder管理HPA <-> DPA, 本文件简化，直接用DPA代替HPA
*/




int main() {
    
    vsf_test_Type test;
    test.produce_pop_and_print_all(
        M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpInv, M2S_Req_Type::Meta0State, M2S_Req_Type::A, 900, 11, 1, LD_PID),
        M2S_BIRsp_Type(0, 0, 0 ,0), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    test.produce_pop_and_print_all(
        M2S_Req_Type(),
        M2S_BIRsp_Type(), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    std::cout << std::endl;

    test.produce_pop_and_print_all(
        M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpInv, M2S_Req_Type::Meta0State, M2S_Req_Type::A, 901, 11, 2, LD_PID),
        M2S_BIRsp_Type(),  
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    test.produce_pop_and_print_all(
        M2S_Req_Type(),
        M2S_BIRsp_Type(1, M2S_BIRsp_Type::BIRspI, 0, LD_PID), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    test.produce_pop_and_print_all(
        M2S_Req_Type(),
        M2S_BIRsp_Type(), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    std::cout << std::endl;


    test.produce_pop_and_print_all(
        M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpData, M2S_Req_Type::Meta0State, M2S_Req_Type::S, 902, 11, 3, LD_PID),
        M2S_BIRsp_Type(), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    test.produce_pop_and_print_all(
        M2S_Req_Type(),
        M2S_BIRsp_Type(1, M2S_BIRsp_Type::BIRspS, 1, LD_PID), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    test.produce_pop_and_print_all(
        M2S_Req_Type(),
        M2S_BIRsp_Type(), 
        M2S_RwD_Type(), 
        Rd_Data_Type()
    );
    std::cout << std::endl;

    return 0;
}

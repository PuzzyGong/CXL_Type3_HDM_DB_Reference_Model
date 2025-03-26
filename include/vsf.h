/*  
    vsf 即 Virtual Snoop Filter
*/

#include "define.h"
#include "port.h"

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
        std::list<S2M_BISnp_Type> s2m_bisnp,
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
                if(vsf_elecment[i].cacheline_state != NeedForRspI && vsf_elecment[i].cacheline_state != NeedForRspIS){
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
        std::list<S2M_BISnp_Type> s2m_bisnp,
        std::list<Rd_Addr_Type> rd_addr,
        std::list<S2M_NDR_Type> s2m_ndr,
        std::list<S2M_DRS_Type> s2m_drs,
        std::list<Wr_Type> wr
    ) {
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
                    std::list<S2M_BISnp_Type>(),
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
                    std::list<S2M_BISnp_Type>(),
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
                vsf_line[tag] = tmp;
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
                m2s_req_list.erase(it);
                break;
            }
            
        }

        rd_addr.push_back(rd_addr_priority_0);
        rd_addr.push_back(rd_addr_priority_1);

        s2m_ndr.push_back(s2m_ndr_priority_0);
        s2m_ndr.push_back(s2m_ndr_priority_1);
        s2m_ndr.push_back(s2m_ndr_priority_2);
        s2m_ndr.push_back(s2m_ndr_priority_3);

        s2m_drs.push_back(s2m_drs_priority_0);
        s2m_drs.push_back(s2m_drs_priority_1);
    }
};


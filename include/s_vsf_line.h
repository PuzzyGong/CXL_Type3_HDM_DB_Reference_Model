#ifndef S_VSF_LINE_H
#define S_VSF_LINE_H

#include "define.h"
#include "s_decoder.h"

enum CACHELINE_STATE{
    EM,
    S,
    I,
    NeedForRspI,
    NeedForRspIS,
    NeedForMemData
};
class S_VSF_Line_Type{
    private:
    std::unordered_map<u32, CACHELINE_STATE> vsf_line; //记录全部 Host 的一个 Cacheline 的状态

    enum {
        Idle,
        Wait_For_Rsp,
        Wait_For_RdAddr,
        Wait_For_RdData,
        Wait_For_DRS_NDR
    } state;
    bool skip_RdData;
    bool skip_DRS_NDR;
    bool victim;
    unsigned int tag;
    unsigned int host_pid;
    u64 cacheline_addr;
    u512 cacheline_data;

    public:
    S_VSF_Line_Type() : state(Idle), skip_RdData(false), skip_DRS_NDR(false), victim(false) {}

    inline bool isIdle() { return state == Idle; }

    void check_host(const std::list<u32>& host_pid_list);
    bool product(
        const u32 device_pid,

        const M2S_Req_Type& m2s_req,
        const M2S_Rsp_type& m2s_rsp,
        
        std::list<S2M_Snp_type>& s2m_snp,
        S2M_NDR_Type& s2m_ndr,
        S2M_DRS_Type& s2m_drs,

        const Rd_Data_Type& rd_data,
        Rd_Addr_Type& rd_addr
    );
};

#endif
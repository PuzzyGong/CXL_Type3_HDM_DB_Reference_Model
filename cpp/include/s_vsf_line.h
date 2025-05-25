#ifndef S_VSF_LINE_H
#define S_VSF_LINE_H

#include "define.h"

class S_VSF_Line_Type
{
private:
    // FSM(Finite State Machine) 该类的唯一状态机
    enum
    {
        Idle,
        Wait_For_Rsp,
        Wait_For_RdAddr,
        Wait_For_RdData,
        Wait_For_DRS_NDR
    } state;

    // vsf_line[MAX_NUM_HOST] : Snoop Filter 中一个 Cacheline 记录的各个 Host 的状态
    // --- NeedForRspI, NeedForRspIS, NeedForMemData : 临时 Cacheline State （辅助 FSM 进行状态转换）
    enum
    {
        I,
        S,
        EM,
        NeedForRspI,
        NeedForRspIS,
        NeedForMemData
    } vsf_line[MAX_NUM_HOST];

    // FSM flag （辅助 FSM 进行状态转换）
    bool skip_RdData;
    bool skip_DRS_NDR;
    bool victim;

    // FSM buffer （辅助 FSM 进行数据输出）
    u32 tag;
    u32 host_pid;
    u64 cacheline_addr;
    u512 cacheline_data;

public:
    S_VSF_Line_Type() : vsf_line(), state(Idle), skip_RdData(false), skip_DRS_NDR(false), victim(false), tag(0), host_pid(0), cacheline_addr(0), cacheline_data()
    {
        for (int i = 0; i < MAX_NUM_HOST; i++)
            vsf_line[i] = I;
    }

    inline bool is_idle() { return state == Idle; }

    void check_host(const limited_list<u32> &host_pid_list);
    bool product(
        const u32 device_pid,

        const M2S_Req_Type &m2s_req,
        const M2S_Rsp_type &m2s_rsp,

        limited_list<S2M_Snp_type> &s2m_snp,
        S2M_NDR_Type &s2m_ndr,
        S2M_DRS_Type &s2m_drs,

        const Rd_Data_Type &rd_data,
        Rd_Addr_Type &rd_addr);
};

#endif
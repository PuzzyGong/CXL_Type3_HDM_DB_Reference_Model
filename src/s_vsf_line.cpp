#include "s_vsf_line.h"

void S_VSF_Line_Type::check_host(const std::list<u32> &host_pid_list)
{
    auto it = vsf_line.begin();
    while (it != vsf_line.end())
    {
        u32 key = it->first;
        if (std::find(host_pid_list.begin(), host_pid_list.end(), key) == host_pid_list.end())
        {
            if (it->second != I)
                CPP_ERROR("ERROR: A Dirty Cache Line May Be Left in a Deleted Host!");
            it = vsf_line.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (u32 new_key : host_pid_list)
    {
        if (vsf_line.find(new_key) == vsf_line.end())
        {
            vsf_line.emplace(new_key, I);
        }
    }
}

bool S_VSF_Line_Type::product(
    const u32 device_pid,

    const M2S_Req_Type &m2s_req,
    const M2S_Rsp_type &m2s_rsp,

    std::list<S2M_Snp_type> &s2m_snp,
    S2M_NDR_Type &s2m_ndr,
    S2M_DRS_Type &s2m_drs,

    const Rd_Data_Type &rd_data,
    Rd_Addr_Type &rd_addr)
{
    if (state == Idle)
    {
        if (m2s_req.valid)
        {
            state = Wait_For_Rsp;
            skip_RdData = 0;
            skip_DRS_NDR = 0;
            victim = 0;
            tag = m2s_req.tag;
            host_pid = m2s_req.spid;
            cacheline_addr = m2s_req.address;
            cacheline_data = 0;

            if (m2s_req.snptype == M2S_Req_Type::SnpType_Enum ::SnpInv &&
                m2s_req.memopcode == M2S_Req_Type::MemOpcode_Enum::MemRd &&
                m2s_req.metafield == M2S_Req_Type::MetaField_Enum::Meta0State &&
                m2s_req.metavalue == M2S_Req_Type::MetaValue_Enum::A)
            {
                
                for (auto it = vsf_line.begin(); it != vsf_line.end(); ++it)
                {
                    auto& vsf_elecment = *it;
                    if (vsf_elecment.first == host_pid)
                    {
                        vsf_elecment.second = NeedForMemData;
                    }
                    else if (vsf_elecment.second != I)
                    {
                        vsf_elecment.second = NeedForRspI;
                        S2M_Snp_type s2m_snp_single;
                        s2m_snp_single.valid = true;
                        s2m_snp_single.opcode = S2M_Snp_type::Opcode_Enum::BISnpInv;
                        s2m_snp_single.address = cacheline_addr;
                        s2m_snp_single.spid = device_pid;
                        s2m_snp_single.dpid = vsf_elecment.first;

                        s2m_snp.push_back(s2m_snp_single);
                    }
                }
            }
            else if (
                m2s_req.snptype == M2S_Req_Type::SnpType_Enum ::SnpData &&
                m2s_req.memopcode == M2S_Req_Type::MemOpcode_Enum::MemRd &&
                m2s_req.metafield == M2S_Req_Type::MetaField_Enum::Meta0State &&
                m2s_req.metavalue == M2S_Req_Type::MetaValue_Enum::S)
            {
                for (auto it = vsf_line.begin(); it != vsf_line.end(); ++it)
                {
                    auto& vsf_elecment = *it;
                    if (vsf_elecment.first == host_pid)
                    {
                        vsf_elecment.second = NeedForMemData;
                    }
                    else if (vsf_elecment.second != I)
                    {
                        vsf_elecment.second = NeedForRspIS;
                        S2M_Snp_type s2m_snp_single;
                        s2m_snp_single.valid = true;
                        s2m_snp_single.opcode = S2M_Snp_type::Opcode_Enum::BISnpData;
                        s2m_snp_single.address = cacheline_addr;
                        s2m_snp_single.spid = device_pid;
                        s2m_snp_single.dpid = vsf_elecment.first;

                        s2m_snp.push_back(s2m_snp_single);
                    }
                }
            }
            else if (
                m2s_req.snptype == M2S_Req_Type::SnpType_Enum ::SnpInv &&
                m2s_req.memopcode == M2S_Req_Type::MemOpcode_Enum::MemInv &&
                m2s_req.metafield == M2S_Req_Type::MetaField_Enum::Meta0State &&
                m2s_req.metavalue == M2S_Req_Type::MetaValue_Enum::I)
            {
                for (auto it = vsf_line.begin(); it != vsf_line.end(); ++it)
                {
                    auto& vsf_elecment = *it;
                    if (vsf_elecment.first == host_pid)
                    {
                        vsf_elecment.second = I;
                    }
                    else if (vsf_elecment.second != I)
                    {
                        vsf_elecment.second = NeedForRspI;
                        S2M_Snp_type s2m_snp_single;
                        s2m_snp_single.valid = true;
                        s2m_snp_single.opcode = S2M_Snp_type::Opcode_Enum::BISnpInv;
                        s2m_snp_single.address = cacheline_addr;
                        s2m_snp_single.spid = device_pid;
                        s2m_snp_single.dpid = vsf_elecment.first;

                        s2m_snp.push_back(s2m_snp_single);
                    }
                }
                skip_RdData = 1;
                skip_DRS_NDR = 1;
                victim = 1;
            }
            else
            {
                CPP_ERROR("vsf: Unknown SnpType/MemOpcode/MetaField/MetaValue in M2S_Req");
            }
        }
        else
        {
            return 0;
        }
    }

    if (state == Wait_For_Rsp)
    {
        if (m2s_rsp.valid)
        {
            if (m2s_rsp.opcode == M2S_Rsp_type::Opcode_Enum::BIRspI)
            {
                if (vsf_line[m2s_rsp.snp.dpid] == NeedForRspI || vsf_line[m2s_rsp.snp.dpid] == NeedForRspIS)
                {
                    vsf_line[m2s_rsp.snp.dpid] = I;
                }
                else
                {
                    CPP_ERROR("vsf: Wrong Opcode in M2S_Rsp");
                }
            }
            else if (m2s_rsp.opcode == M2S_Rsp_type::Opcode_Enum::BIRspS)
            {
                if (vsf_line[m2s_rsp.snp.dpid] == NeedForRspIS)
                {
                    vsf_line[m2s_rsp.snp.dpid] = S;
                }
                else
                {
                    CPP_ERROR("vsf: Wrong Opcode in M2S_Rsp");
                }
            }
            else
            {
                CPP_ERROR("vsf: Unknown Opcode in M2S_Rsp");
            }
        }

        bool has_needfor_flag = 0;
        for (auto vsf_elecment : vsf_line)
        {
            if (vsf_elecment.second == NeedForRspI || vsf_elecment.second == NeedForRspIS)
            {
                has_needfor_flag = 1;
                break;
            }
        }
        if (has_needfor_flag == 0)
        {
            state = Wait_For_RdAddr;
        }
        else
        {
            return 0;
        }
    }

    if (state == Wait_For_RdAddr)
    {
        state = Wait_For_RdData;
        if (vsf_line[host_pid] == NeedForMemData)
        {
            rd_addr = Rd_Addr_Type(true, cacheline_addr);

            vsf_line[host_pid] = EM;
            for (auto vsf_elecment : vsf_line)
            {
                if (vsf_elecment.second == S)
                {
                    vsf_line[host_pid] = S;
                    break;
                }
            }
        }
    }

    if (state == Wait_For_RdData)
    {
        if (skip_RdData)
        {
            state = Wait_For_DRS_NDR;
        }
        else if (rd_data.valid)
        {
            state = Wait_For_DRS_NDR;
            cacheline_data = rd_data.data_body;
        }
        else
        {
            return 0;
        }
    }

    if (state == Wait_For_DRS_NDR)
    {
        state = Idle;
        if (skip_DRS_NDR == 0)
        {
            if (vsf_line[host_pid] == EM)
            {
                s2m_ndr = S2M_NDR_Type();
                s2m_ndr.valid = true;
                s2m_ndr.opcode = S2M_NDR_Type::Opcode_Enum::CmpE;
                s2m_ndr.tag = tag;
                s2m_ndr.dpid = host_pid;
            }
            else if (vsf_line[host_pid] == S)
            {
                s2m_ndr = S2M_NDR_Type();
                s2m_ndr.valid = true;
                s2m_ndr.opcode = S2M_NDR_Type::Opcode_Enum::CmpS;
                s2m_ndr.tag = tag;
                s2m_ndr.dpid = host_pid;
            }
            S2M_DRS_Type s2m_drs;
            s2m_drs.valid = true;
            s2m_drs.opcode = S2M_DRS_Type::Opcode_Enum::MemData;
            s2m_drs.tag = tag;
            s2m_drs.dpid = host_pid;
            s2m_drs.data_body = cacheline_data;
        }

        if (victim)
            return 1;
    }
    return 0;
}
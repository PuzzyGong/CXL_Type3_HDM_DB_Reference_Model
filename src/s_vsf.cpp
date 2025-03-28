#include "s_vsf.h"

void S_VSF_Type::main_product(
    const M2S_Rsp_type &m2s_rsp,
    const M2S_RwD_Type &m2s_rwd,
    std::list<S2M_Snp_type> &s2m_snp,
    std::list<S2M_NDR_Type> &s2m_ndr,
    std::list<S2M_DRS_Type> &s2m_drs,
    const Rd_Data_Type &rd_data,
    std::list<Rd_Addr_Type> &rd_addr,
    std::list<Wr_Type> &wr)
{
    std::list<S2M_Snp_type> s2m_snp_empty;
    S2M_Snp_type s2m_snp_priority_0;

    S2M_NDR_Type s2m_ndr_empty;
    S2M_NDR_Type s2m_ndr_priority_0;
    S2M_NDR_Type s2m_ndr_priority_1;
    S2M_NDR_Type s2m_ndr_priority_2;
    S2M_NDR_Type s2m_ndr_priority_3;

    S2M_DRS_Type s2m_drs_empty;
    S2M_DRS_Type s2m_drs_priority_0;
    S2M_DRS_Type s2m_drs_priority_1;

    Rd_Addr_Type rd_addr_empty;
    Rd_Addr_Type rd_addr_priority_0;
    Rd_Addr_Type rd_addr_priority_1;

    Wr_Type wr_empty;
    Wr_Type wr_priority_0;

    if (m2s_rwd.valid)
    {
        if (m2s_rwd.memopcode == M2S_RwD_Type::MemOpcode_Enum::BIConflict)
        {
            s2m_ndr_priority_0.valid = true;
            s2m_ndr_priority_0.opcode = S2M_NDR_Type::Opcode_Enum::BIConflictAck;
            s2m_ndr_priority_0.tag = m2s_rwd.tag;
            s2m_ndr_priority_0.spid = decoder.get_device_pid();
            s2m_ndr_priority_0.dpid = m2s_rwd.spid;
        }
        else if (m2s_rwd.memopcode == M2S_RwD_Type::MemOpcode_Enum::MemWr)
        {
            s2m_ndr_priority_0.valid = true;
            s2m_ndr_priority_0.opcode = S2M_NDR_Type::Opcode_Enum::Cmp;
            s2m_ndr_priority_0.tag = m2s_rwd.tag;
            s2m_ndr_priority_0.spid = decoder.get_device_pid();
            s2m_ndr_priority_0.dpid = m2s_rwd.spid;

            wr_priority_0 = Wr_Type(true, m2s_rwd.address, m2s_rwd.data_body);
        }
    }

    if (rd_data.valid)
    {
        u32 tag = rd_data.address;
        auto it = vsf_line.find(tag);
        if (it != vsf_line.end())
        {
            if (it->second.product(
                    decoder.get_device_pid(),

                    M2S_Req_Type(),
                    M2S_Rsp_type(),

                    s2m_snp_empty,
                    s2m_ndr_priority_1,
                    s2m_drs_priority_0,

                    rd_data,
                    rd_addr_empty))
            {
                vsf_line.erase(it);
            }
        }
        else
        {
            CPP_ERROR("rd_data Valid but not found in vsf_line");
        }
    }

    if (m2s_rsp.valid)
    {
        u32 tag = m2s_rsp.snp.address;
        auto it = vsf_line.find(tag);
        if (it != vsf_line.end())
        {
            if (it->second.product(
                    decoder.get_device_pid(),

                    M2S_Req_Type(),
                    m2s_rsp,

                    s2m_snp_empty,
                    s2m_ndr_priority_2,
                    s2m_drs_priority_1,

                    Rd_Data_Type(),
                    rd_addr_priority_0))
            {
                vsf_line.erase(it);
            }
        }
        else
        {
            CPP_ERROR("rd_data Valid but not found in vsf_line");
        }
    }

    for (auto it_list = m2s_req_list.begin(); it_list != m2s_req_list.end();)
    {
        M2S_Req_Type m2s_req_single = *it_list;

        u32 tag = m2s_req_single.address;
        auto it = vsf_line.find(tag);
        if (it != vsf_line.end())
        {
            if (!it->second.isIdle())
                continue;

            if (it->second.product(
                    decoder.get_device_pid(),
                    m2s_req_single,
                    M2S_Rsp_type(),

                    s2m_snp,
                    s2m_ndr_priority_3,
                    s2m_drs_empty,

                    Rd_Data_Type(),
                    rd_addr_priority_0))
            {
                vsf_line.erase(it);
            }
            m2s_req_list.erase(it_list);
            break;
        }
        else
        {
            S_VSF_Line_Type tmp;
            std::list<u32> host_id_list;
            decoder.get_host_pid(host_id_list);
            tmp.check_host(host_id_list);

            if (tmp.product(
                    decoder.get_device_pid(),
                    m2s_req_single,
                    M2S_Rsp_type(),

                    s2m_snp,
                    s2m_ndr_priority_3,
                    s2m_drs_empty,

                    Rd_Data_Type(),
                    rd_addr_priority_0))
            {
                vsf_line.erase(it);
            }
            vsf_line[tag] = tmp;
            m2s_req_list.erase(it_list);
            break;
        }
    }
    if (vsf_line.size() > vsf_line_size)
        CPP_ERROR("ERROR: vsf_line size exceed limit");

    if (s2m_ndr_priority_0.valid)
        s2m_ndr.push_back(s2m_ndr_priority_0);
    if (s2m_ndr_priority_1.valid)
        s2m_ndr.push_back(s2m_ndr_priority_1);
    if (s2m_ndr_priority_2.valid)
        s2m_ndr.push_back(s2m_ndr_priority_2);
    if (s2m_ndr_priority_3.valid)
        s2m_ndr.push_back(s2m_ndr_priority_3);

    if (s2m_drs_priority_0.valid)
        s2m_drs.push_back(s2m_drs_priority_0);
    if (s2m_drs_priority_1.valid)
        s2m_drs.push_back(s2m_drs_priority_1);

    if (rd_addr_priority_0.valid)
        rd_addr.push_back(rd_addr_priority_0);
    if (rd_addr_priority_1.valid)
        rd_addr.push_back(rd_addr_priority_1);

    if (wr_priority_0.valid)
        wr.push_back(wr_priority_0);
}

void S_VSF_Type::main_product(
    const M2S_Req_Type &m2s_req,
    const M2S_Rsp_type &m2s_rsp,
    const M2S_RwD_Type &m2s_rwd,
    std::list<S2M_Snp_type> &s2m_snp,
    std::list<S2M_NDR_Type> &s2m_ndr,
    std::list<S2M_DRS_Type> &s2m_drs,

    const Rd_Data_Type &rd_data,
    std::list<Rd_Addr_Type> &rd_addr,
    std::list<Wr_Type> &wr,

    const CXL_io_Req_Type &cxl_io_req,
    std::list<CXL_io_Rsp_Type> &cxl_io_rsp,
    const Victim_Type &victim,
    const S2M_Snp_type &s2m_snp_provide_bitag)
{
    decoder.product(cxl_io_req, cxl_io_rsp);

    M2S_Rsp_type m2s_rsp_tmp = m2s_rsp;
    if (m2s_rsp.valid)
    {
        if (mapper.find(m2s_rsp.bitag) != mapper.end())
        {
            m2s_rsp_tmp.snp = mapper[m2s_rsp.bitag];
            mapper.erase(m2s_rsp.bitag);
        }
        else
        {
            CPP_ERROR("ERROR: BITag not found in mapper");
        }
    }

    M2S_Req_Type m2s_req_dpa;
    M2S_Rsp_type m2s_rsp_dpa;
    M2S_RwD_Type m2s_rwd_dpa;
    decoder.decode(m2s_req, m2s_req_dpa);     // m2s_req_dpa <- m2s_req
    decoder.decode(m2s_rsp_tmp, m2s_rsp_dpa); // m2s_rsp_dpa <- m2s_rsp
    decoder.decode(m2s_rwd, m2s_rwd_dpa);     // m2s_rwd_dpa <- m2s_rwd

    if (victim.cmd.valid)
        m2s_req_list.push_back(victim.cmd);
    if (m2s_req_dpa.valid)
    {
        if (decoder.is_host_pid(m2s_req_dpa.spid))
            m2s_req_list.push_back(m2s_req_dpa);
        else
            CPP_ERROR("ERROR: m2s_req_dpa.spid is not host pid");
    }
    if( m2s_rwd_dpa.valid && !decoder.is_host_pid(m2s_rwd_dpa.spid))
        CPP_ERROR("ERROR: m2s_rwd_dpa.spid is not host pid");

    if (m2s_req_list.size() > m2s_req_list_size)
        CPP_ERROR("ERROR: m2s_req_list size > m2s_req_list_size");

    std::list<S2M_Snp_type> s2m_snp_dpa;
    std::list<S2M_NDR_Type> s2m_ndr_dpa;
    std::list<S2M_DRS_Type> s2m_drs_dpa;
    main_product(m2s_rsp_dpa, m2s_rwd_dpa, s2m_snp_dpa, s2m_ndr_dpa, s2m_drs_dpa, rd_data, rd_addr, wr);

    for (auto s2m_snp_dpa_single : s2m_snp_dpa)
    {
        S2M_Snp_type s2m_snp_single;
        decoder.decode(s2m_snp_dpa_single, s2m_snp_single);
        s2m_snp_not_alloc_bitag.push_back(s2m_snp_single);
    }
    for (auto s2m_ndr_dpa_single : s2m_ndr_dpa)
    {
        S2M_NDR_Type s2m_ndr_single;
        decoder.decode(s2m_ndr_dpa_single, s2m_ndr_single);
        s2m_ndr.push_back(s2m_ndr_single);
    }
    for (auto s2m_drs_dpa_single : s2m_drs_dpa)
    {
        S2M_DRS_Type s2m_drs_single;
        decoder.decode(s2m_drs_dpa_single, s2m_drs_single);
        s2m_drs.push_back(s2m_drs_single);
    }

#ifdef BITAG_AUTO_ALLOC
    for (auto s2m_snp_single : s2m_snp_not_alloc_bitag)
    {
        s2m_snp_single.bitag = mapper_bitag_now;
        s2m_snp.push_back(s2m_snp_single);
        mapper[mapper_bitag_now] = s2m_snp_single;
        mapper_bitag_now = (mapper_bitag_now + 1) % (1 << 12);
    }
#else
    if (s2m_snp_provide_bitag.valid)
    {
        for (auto s2m_snp_single : s2m_snp_not_alloc_bitag)
        {
            if (s2m_snp_single.opcode == s2m_snp_provide_bitag.opcode && s2m_snp_single.address == s2m_snp_provide_bitag.address && s2m_snp_single.spid == s2m_snp_provide_bitag.spid && s2m_snp_single.dpid == s2m_snp_provide_bitag.dpid)
            {
                s2m_snp_single.bitag = s2m_snp_provide_bitag.bitag;
                mapper[s2m_snp_single.bitag] = s2m_snp_single;
                s2m_snp.push_back(s2m_snp_single);
                break;
            }
        }
    }
    if (s2m_snp_not_alloc_bitag.size() > s2m_snp_not_alloc_bitag_size)
        CPP_ERROR("ERROR: s2m_snp_not_alloc_bitag size > s2m_snp_not_alloc_bitag_size");
#endif
    if (mapper.size() > mapper_size)
        CPP_ERROR("ERROR: mapper size > mapper_size");
}
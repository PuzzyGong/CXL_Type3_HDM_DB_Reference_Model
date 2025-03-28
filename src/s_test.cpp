#include "s_test.h"

void S_Test_Type::pop_and_print_selected(bool pop_bisnp, bool pop_rdaddr, bool pop_ndr, bool pop_drs, bool pop_wr, bool pop_cxl_io_rsp)
{
    if (pop_rdaddr && !rd_addr.empty())
    {
        rd_addr.front().print();
        if (rd_data_list.size() > 20)
            rd_data_list.pop_front();
        rd_data_list.push_back(Rd_Data_Type(true, rd_addr.front().address, rd_addr.front().address + 500)); //
        rd_addr.pop_front();
    }
    if (pop_ndr && !s2m_ndr.empty())
    {
        s2m_ndr.front().print();
        s2m_ndr.pop_front();
    }
    if (pop_drs && !s2m_drs.empty())
    {
        s2m_drs.front().print();
        s2m_drs.pop_front();
    }
    if (pop_bisnp && !s2m_snp.empty())
    {
        s2m_snp.front().print();
        s2m_snp.pop_front();
    }
    if (pop_wr && !wr.empty())
    {
        wr.front().print();
        wr.pop_front();
    }
    if (pop_cxl_io_rsp && !cxl_io_rsp.empty())
    {
        cxl_io_rsp.front().print();
        cxl_io_rsp.pop_front();
    }
}

void S_Test_Type::pop_and_print_all()
{
    while (!s2m_snp.empty() || !rd_addr.empty() || !s2m_ndr.empty() || !s2m_drs.empty() || !wr.empty() || !cxl_io_rsp.empty())
    {
        pop_and_print_selected(true, true, true, true, true, true);
    }
}

void S_Test_Type::device_self_test_once(
    const M2S_Req_Type &m2s_req,
    const M2S_Rsp_type &m2s_rsp,
    const M2S_RwD_Type &m2s_rwd)
{
    if (m2s_req.valid)
        m2s_req.print();
    if (m2s_rsp.valid)
        m2s_rsp.print();
    if (m2s_rwd.valid)
        m2s_rwd.print();

    Rd_Data_Type rd_data;
    if (!rd_data_list.empty())
    {
        rd_data = rd_data_list.front();
        rd_data.print();
        rd_data_list.pop_front();
    }
    std::list<S2M_Snp_type> s2m_snp_tmp;
    std::list<S2M_NDR_Type> s2m_ndr_tmp;
    std::list<S2M_DRS_Type> s2m_drs_tmp;
    std::list<Rd_Addr_Type> rd_addr_tmp;
    std::list<Wr_Type> wr_tmp;
    std::list<CXL_io_Rsp_Type> cxl_io_rsp_tmp;

    vsf.product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp_tmp, s2m_ndr_tmp, s2m_drs_tmp, rd_data, rd_addr_tmp, wr_tmp, CXL_io_Req_Type(), cxl_io_rsp_tmp, Victim_Type(), S2M_Snp_type());
    s2m_snp.insert(s2m_snp.end(), s2m_snp_tmp.begin(), s2m_snp_tmp.end());
    s2m_ndr.insert(s2m_ndr.end(), s2m_ndr_tmp.begin(), s2m_ndr_tmp.end());
    s2m_drs.insert(s2m_drs.end(), s2m_drs_tmp.begin(), s2m_drs_tmp.end());
    rd_addr.insert(rd_addr.end(), rd_addr_tmp.begin(), rd_addr_tmp.end());
    wr.insert(wr.end(), wr_tmp.begin(), wr_tmp.end());
    cxl_io_rsp.insert(cxl_io_rsp.end(), cxl_io_rsp_tmp.begin(), cxl_io_rsp_tmp.end());
}

void S_Test_Type::process_chunk(const std::string &chunk, char end_char)
{
    // 正则匹配非英文字符作为分隔符
    std::regex delimiters("[^a-zA-Z0-9-]+"); // 允许数字、英文和负号，其它字符都当作分隔符
    std::sregex_token_iterator it(chunk.begin(), chunk.end(), delimiters, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> tokens;
    while (it != end)
    {
        if (!it->str().empty())
        { // 避免空字符串
            tokens.push_back(it->str());
        }
        ++it;
    }

    if (tokens.size() != 15)
    {
        std::cerr << "Error: Invalid input block \"" << chunk << "\" (must have 15 values)" << std::endl;
        return;
    }

    try
    {
        M2S_Req_Type m2s_req;
        M2S_Rsp_type m2s_rsp;
        M2S_RwD_Type m2s_rwd;

        if (m2s_req.setMemOpcodeName(tokens[0]))
        {
            m2s_req.valid = true;
            m2s_req.setSnpTypeName(tokens[1]);
            m2s_req.setMetaValueName(tokens[2]);
            m2s_req.tag = std::stoll(tokens[3]);
            m2s_req.spid = std::stoll(tokens[4]);
            m2s_req.address = std::stoll(tokens[5]);
        }

        if (m2s_rsp.setOpcodeName(tokens[6]))
        {
            m2s_rsp.valid = true;
            m2s_rsp.bitag = std::stoll(tokens[7]);
        }

        if (m2s_rwd.setMemOpcodeName(tokens[8]))
        {
            m2s_rwd.valid = true;
            m2s_rwd.setSnpTypeName(tokens[9]);
            m2s_rwd.setMetaValueName(tokens[10]);
            m2s_rwd.tag = std::stoll(tokens[11]);
            m2s_rwd.spid = std::stoll(tokens[12]);
            m2s_rwd.address = std::stoll(tokens[13]);
            m2s_rwd.data_body = std::stoll(tokens[14]);
        }

        if (end_char == ';')
        {
            device_self_test_once(m2s_req, m2s_rsp, m2s_rwd);
        }
        else if (end_char == '.')
        {
            device_self_test_once(m2s_req, m2s_rsp, m2s_rwd);
            pop_and_print_all();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: Invalid number format in \"" << chunk << "\"" << std::endl;
    }
}

void S_Test_Type::device_self_test(const std::string &cmd)
{
    std::string chunk;
    char end_char = '\0';

    for (char ch : cmd)
    {
        if (ch == ';' || ch == '.')
        {
            if (!chunk.empty())
            {
                process_chunk(chunk, ch);
                chunk.clear();
            }
            end_char = ch; // 记录结束符号
        }
        else
        {
            chunk += ch;
        }
    }

    // 处理最后一部分（如果输入没有以 `;` 或 `.` 结尾）
    if (!chunk.empty())
    {
        process_chunk(chunk, end_char);
    }
}

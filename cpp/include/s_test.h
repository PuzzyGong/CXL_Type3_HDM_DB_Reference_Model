#ifndef S_TEST_H
#define S_TEST_H

#include "define.h"
#include "s_vsf.h"

class S_Test_Type
{
private:
    S_VSF_Type vsf;
    limited_list<S2M_Snp_type> s2m_snp;
    limited_list<S2M_NDR_Type> s2m_ndr;
    limited_list<S2M_DRS_Type> s2m_drs;

    limited_list<Rd_Addr_Type> rd_addr;
    limited_list<Wr_Type> wr;

    limited_list<CXL_io_Rsp_Type> cxl_io_rsp;

    limited_list<Rd_Data_Type> rd_data_list;

    public:
    void pop_and_print_selected(bool pop_bisnp, bool pop_rdaddr, bool pop_ndr, bool pop_drs, bool pop_wr, bool pop_cxl_io_rsp);
    void pop_and_print_all();

    void process_chunk(const std::string& chunk, char end_char);
    void device_self_test_once(
        const M2S_Req_Type& m2s_req,
        const M2S_Rsp_type& m2s_rsp,
        const M2S_RwD_Type& m2s_rwd
    );

public:
    S_Test_Type() {};

    void device_self_test(const std::string &cmd);
};

#endif
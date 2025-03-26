#include "define.h"
#include "port.h"
#include "vsf.h"

class vsf_test_Type
{
private:
    vsf_Type vsf;
    std::list<S2M_BISnp_Type> s2m_bisnp;
    std::list<Rd_Addr_Type> rd_addr;
    std::list<S2M_NDR_Type> s2m_ndr;
    std::list<S2M_DRS_Type> s2m_drs;
    std::list<Wr_Type> wr;
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
        if(!rd_addr.empty()) {
            Rd_Addr_Type tmp = rd_addr.front();
            tmp_rd_data = Rd_Data_Type(true, tmp.Address, tmp.Address + 500);
            rd_addr.pop_front();
        }

        vsf.produce(m2s_req, m2s_birsp, m2s_rwd, tmp_rd_data, s2m_bisnp, rd_addr, s2m_ndr, s2m_drs, wr);
        m2s_req.print();
        m2s_birsp.print();
        m2s_rwd.print();
        rd_data.print();
    }
    void pop_and_print_selected(bool pop_bisnp, bool pop_rdaddr, bool pop_ndr, bool pop_drs, bool pop_wr) {
        if (pop_bisnp && !s2m_bisnp.empty()) {
            std::cout << "        ";
            s2m_bisnp.front().print();
            s2m_bisnp.pop_front();
        }
        if (pop_rdaddr && !rd_addr.empty()) {
            std::cout << "        ";
            rd_addr.front().print();
            rd_addr.pop_front();
        }
        if (pop_ndr && !s2m_ndr.empty()) {
            std::cout << "        ";
            s2m_ndr.front().print();
            s2m_ndr.pop_front();
        }
        if (pop_drs && !s2m_drs.empty()) {
            std::cout << "        ";
            s2m_drs.front().print();
            s2m_drs.pop_front();
        }
        if (pop_wr && !wr.empty()) {
            std::cout << "        ";
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

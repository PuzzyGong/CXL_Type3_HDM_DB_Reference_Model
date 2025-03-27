
// class vsf_test_Type
// {
// private:
//     vsf_Type vsf;
//     std::list<S2M_Snp_type> s2m_bisnp;
//     std::list<Rd_Addr_Type> rd_addr;
//     std::list<S2M_NDR_Type> s2m_ndr;
//     std::list<S2M_DRS_Type> s2m_drs;
//     std::list<Wr_Type> wr;

//     std::list<Rd_Data_Type> rd_data_list;
// public:

//     void produce_pop_and_print_all(
//         const M2S_Req_Type& m2s_req,
//         const M2S_Rsp_type& m2s_birsp,
//         const M2S_RwD_Type& m2s_rwd,
//         const Rd_Data_Type& rd_data
//     ) {
//         produce(m2s_req, m2s_birsp, m2s_rwd, rd_data);
//         pop_and_print_all();
//     }

//     void produce(
//         const M2S_Req_Type& m2s_req,
//         const M2S_Rsp_type& m2s_birsp,
//         const M2S_RwD_Type& m2s_rwd,
//         const Rd_Data_Type& rd_data
//     ) {
//         //
//         Rd_Data_Type tmp_rd_data;
//         if(!rd_data_list.empty()){
//             tmp_rd_data = rd_data_list.front();
//             rd_data_list.pop_front();
//         }


//         vsf.produce(m2s_req, m2s_birsp, m2s_rwd, tmp_rd_data, s2m_bisnp, rd_addr, s2m_ndr, s2m_drs, wr);
//         if(m2s_req.Valid) m2s_req.print();
//         if(m2s_birsp.Valid) m2s_birsp.print();
//         if(m2s_rwd.Valid) m2s_rwd.print();
//         if(tmp_rd_data.Valid) tmp_rd_data.print();
//     }
//     void pop_and_print_selected(bool pop_bisnp, bool pop_rdaddr, bool pop_ndr, bool pop_drs, bool pop_wr) {
//         if (pop_bisnp && !s2m_bisnp.empty()) {
//             std::cout << "                                                                ";
//             s2m_bisnp.front().print();
//             s2m_bisnp.pop_front();
//         }
//         if (pop_rdaddr && !rd_addr.empty()) {
//             std::cout << "                                                                ";
//             rd_addr.front().print();
//             rd_data_list.push_back(Rd_Data_Type(true, rd_addr.front().Address, rd_addr.front().Address + 500)); //
//             rd_addr.pop_front();
//         }
//         if (pop_ndr && !s2m_ndr.empty()) {
//             std::cout << "                                                                ";
//             s2m_ndr.front().print();
//             s2m_ndr.pop_front();
//         }
//         if (pop_drs && !s2m_drs.empty()) {
//             std::cout << "                                                                ";
//             s2m_drs.front().print();
//             s2m_drs.pop_front();
//         }
//         if (pop_wr && !wr.empty()) {
//             std::cout << "                                                                ";
//             wr.front().print();
//             wr.pop_front();
//         }
//     }
//     void pop_and_print_all() {
//         while (!s2m_bisnp.empty() || !rd_addr.empty() || !s2m_ndr.empty() || !s2m_drs.empty() || !wr.empty()) {
//             pop_and_print_selected(true, true, true, true, true);
//         }
//     }
// };





// int main() {
    
//     std::cout << RED     << "This is red text" << RESET << std::endl;
//     std::cout << GREEN   << "This is green text" << RESET << std::endl;
//     std::cout << YELLOW  << "This is yellow text" << RESET << std::endl;
//     std::cout << BLUE    << "This is blue text" << RESET << std::endl;
//     std::cout << MAGENTA << "This is magenta text" << RESET << std::endl;
//     std::cout << CYAN    << "This is cyan text" << RESET << std::endl;

//     vsf_test_Type test;
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpInv, M2S_Req_Type::Meta0State, M2S_Req_Type::A, 900, 11, 1, LD_PID),
//         M2S_Rsp_type(0, 0, 0 ,0), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(),
//         M2S_Rsp_type(), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     std::cout << std::endl;

//     test.produce_pop_and_print_all(
//         M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpInv, M2S_Req_Type::Meta0State, M2S_Req_Type::A, 901, 11, 2, LD_PID),
//         M2S_Rsp_type(),  
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(),
//         M2S_Rsp_type(1, M2S_Rsp_type::BIRspI, 0, LD_PID), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(),
//         M2S_Rsp_type(), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     std::cout << std::endl;


//     test.produce_pop_and_print_all(
//         M2S_Req_Type(1, M2S_Req_Type::MemRd, M2S_Req_Type::SnpData, M2S_Req_Type::Meta0State, M2S_Req_Type::S, 902, 11, 3, LD_PID),
//         M2S_Rsp_type(), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(),
//         M2S_Rsp_type(1, M2S_Rsp_type::BIRspS, 1, LD_PID), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     test.produce_pop_and_print_all(
//         M2S_Req_Type(),
//         M2S_Rsp_type(), 
//         M2S_RwD_Type(), 
//         Rd_Data_Type()
//     );
//     std::cout << std::endl;

//     return 0;
// }

#include "port.h"

int main() {






    //M2S_RwD_Type m2s_rwd;
    //m2s_rwd.address = 1;
    //m2s_rwd.print();

    //
    M2S_Req_Type m2s_req;
    m2s_req.tag = 901;
    m2s_req.address = 11;
    m2s_req.spid = 3;
    m2s_req.print();
    m2s_req.tag = 901;
    m2s_req.address = 11;
    m2s_req.spid = 4;
    m2s_req.print();
    m2s_req.tag = 901;
    m2s_req.address = 11;
    m2s_req.spid = 5;
    m2s_req.print();
    m2s_req.tag = 901;
    m2s_req.address = 11;
    m2s_req.spid = 6;
    m2s_req.print();


    //M2S_Req_Type m2s_req;
    m2s_req.tag = 901;
    m2s_req.address = 11;
    m2s_req.spid = 2;
    m2s_req.print();

    S2M_Snp_type s2m_bisnp;
    s2m_bisnp.bitag = 0;
    s2m_bisnp.address = 11;
    s2m_bisnp.dpid = 1;
    s2m_bisnp.print();

    M2S_Rsp_type m2s_birsp;
    m2s_birsp.bitag = 0;
    m2s_birsp.print();



    Rd_Addr_Type rd_addr;
    rd_addr.address = 11;
    rd_addr.print();
    
    Rd_Data_Type rd_data;
    rd_data.address = 11;
    rd_data.data_body = 511;
    rd_data.print();

    S2M_NDR_Type s2m_ndr;
    s2m_ndr.tag = 901;
    s2m_ndr.dpid = 2;
    s2m_ndr.print();

    S2M_DRS_Type s2m_drs;
    s2m_drs.tag = 901;
    s2m_drs.dpid = 2;
    s2m_drs.data_body = 511;
    s2m_drs.print();




    return 0;
}
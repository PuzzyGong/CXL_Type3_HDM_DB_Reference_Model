#ifndef TEST_DEVICE_H
#define TEST_DEVICE_H

#include "../conf/global_conf.h"
#include "../device_model/shared_var_pool/shared_var_pool.h"

class test_device{
public:
    std::string& stim_file_name;
    shared_var_pool s_pool;
    test_device(const std::string& stim_file_name, const std::string& conf_file_name) : s_pool(conf_file_name), stim_file_name(stim_file_name){
        
    };
    void process() {
        // TODO

        Device device(s_pool);

        std::vector<std::string> stim_vectors = conversion_tool::read_file(stim_file_name);

        for( auto stim_string : stim_vectors){
            // TODO
            
             

            conversion_tool::get_channel_msg (stim_string,
                m2s_req_msg,
                m2s_BIrsp_msg,
                
                m2s_rwd_msg,
                m2s_uio,
                m2s_axi_read_data,
                m2s_axi_write_rsp
            );

           device.process(
            m2s_req_msg,
            m2s_BIrsp_msg,
            
            m2s_rwd_msg,
            m2s_uio,
            m2s_axi_read_data,
            m2s_axi_write_rsp,


            s2m_BIsnp_msg_list,
            s2m_req_msg_list,
            s2m_req_msg_li
               

           );

           m2s_req_msg          .print()
           m2s_BIrsp_msg        .print()
           m2s_rwd_msg          .print()
           m2s_uio              .print()
           m2s_axi_read_data.print()
           m2s_axi_write_rsp.print()

           for(auto elecment:   s2m_BIsnp_msg_list)  elecment.print();
           for(auto elecment:   s2m_BIsnp_msg_list)  elecment.print();
           for(auto elecment:   s2m_BIsnp_msg_list)  elecment.print();
        }
    }

};

#endif
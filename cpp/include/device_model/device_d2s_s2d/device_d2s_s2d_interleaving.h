#ifndef DEVICE_D2S_S2D_INTERLEAVING_H
#define DEVICE_D2S_S2D_INTERLEAVING_H

#include "../../../conf/global_conf_with_channel.h"
#include "../../shared_var_pool/shared_var_pool.h"

class device_d2s_s2d_interleaving
{
  private:
    shared_var_pool var_pool;

  public:
    explicit device_d2s_s2d_interleaving(const shared_var_pool &var_pool) : var_pool(var_pool)
    {
    }

    void s2d_process(limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg_list,  // io
                     limited_list<s2d_axi_wraddr_msg> &axi_wraddr_msg_list   // io
    )
    {
        axi_rdaddr_msg_list.clear();
        axi_wraddr_msg_list.clear();

        for (auto &axi_rdaddr_msg : axi_rdaddr_msg_list) {
            axi_rdaddr_msg.set_address(axi_rdaddr_msg.get_address() - 1);
        }
        for (auto &axi_wraddr_msg : axi_wraddr_msg_list) {
            axi_wraddr_msg.set_address(axi_wraddr_msg.get_address() - 1);
        }
    }

    void d2s_process(d2s_axi_rddata_msg &axi_rddata_msg,  // io
                     d2s_axi_wrrsp_msg &axi_wrrsp_msg     // io
    )
    {
    }
};

#endif

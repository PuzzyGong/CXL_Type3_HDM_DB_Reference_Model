#ifndef DEVICE_GFAM_DECODERS_H
#define DEVICE_GFAM_DECODERS_H

#include "../base/device_gfam_base.h"

class device_gfam_decoders : public device_gfam_base
{
  public:
    explicit device_gfam_decoders(const shared_var_pool &shared_pool) : device_gfam_base(shared_pool)
    {
        init();
    }
    ~device_gfam_decoders() = default;
    bool m2s_process(unsigned long long hpa, unsigned long long &dpa) override; //重载该函数
    bool s2m_process(unsigned long long dpa, unsigned int dpid_6bit, unsigned long long &hpa) override; //重载该函数

  private:
    void init(); //重载该函数
    //****************************** 自定义
    int X;
    //****************************** 自定义
};

#endif
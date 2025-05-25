#ifndef DEVICE_LDFAM_DECODERS_H
#define DEVICE_LDFAM_DECODERS_H

#include "../base/device_ldfam_base.h"

class device_ldfam_decoders : public device_ldfam_base
{
  public:
    explicit device_ldfam_decoders(const shared_var_pool &shared_pool) : device_ldfam_base(shared_pool)
    {
        init();
    }
    ~device_ldfam_decoders() = default;
    bool m2s_process(unsigned long long hpa, unsigned int ld_id, unsigned long long &dpa) override; //重载该函数
    bool s2m_process(unsigned long long dpa, unsigned int ld_id, unsigned long long &hpa) override; //重载该函数

  private:
    void init(); //重载该函数
    //****************************** 自定义
    int X;
    //****************************** 自定义
};

#endif
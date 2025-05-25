#ifndef DEVICE_LDFAM_DCD_H
#define DEVICE_LDFAM_DCD_H

#include "../base/device_ldfam_base.h"

class device_ldfam_dcd : public device_ldfam_base
{
  public:
    explicit device_ldfam_dcd(const shared_var_pool &shared_pool) : device_ldfam_base(shared_pool)
    {
        init();
    }
    ~device_ldfam_dcd() = default;
    bool m2s_process(unsigned long long dpa, unsigned int ld_id, unsigned long long &mpa) override; //重载该函数
    bool s2m_process(unsigned long long mpa, unsigned int ld_id, unsigned long long &dpa) override; //重载该函数

  private:
    void init(); //重载该函数
    //****************************** 自定义
    int X;
    //****************************** 自定义
};

#endif
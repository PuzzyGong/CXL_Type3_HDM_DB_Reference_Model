#ifndef PRINT_TOOl_H
#define PRINT_TOOl_H

#include "../conf/global_conf.h"
#include <cmath>

#define SIMPLE_PRINT
#define COLORFUL_PRINT

#define SIMPLE_PRINT_S2M_SPACE 0
#define SIMPLE_PRINT_M2S_SPACE 85
#define SIMPLE_PRINT_S2D_SPACE 145
#define SIMPLE_PRINT_D2S_SPACE 150
#define SIMPLE_PRINT_S2M_NAME_LENGTH 30
#define SIMPLE_PRINT_M2S_NAME_LENGTH 20
#define SIMPLE_PRINT_S2D_NAME_LENGTH 12
#define SIMPLE_PRINT_D2S_NAME_LENGTH 12

#define NO_SIMPLE_PRINT_S2M_SPACE 0
#define NO_SIMPLE_PRINT_M2S_SPACE 85
#define NO_SIMPLE_PRINT_S2D_SPACE 145
#define NO_SIMPLE_PRINT_D2S_SPACE 150
#define NO_SIMPLE_PRINT_S2M_NAME_LENGTH 30
#define NO_SIMPLE_PRINT_M2S_NAME_LENGTH 20
#define NO_SIMPLE_PRINT_S2D_NAME_LENGTH 12
#define NO_SIMPLE_PRINT_D2S_NAME_LENGTH 12

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define LIGHT_RED "\033[91m"
#define LIGHT_GREEN "\033[92m"
#define LIGHT_YELLOW "\033[93m"
#define LIGHT_BLUE "\033[94m"
#define LIGHT_MAGENTA "\033[95m"
#define LIGHT_CYAN "\033[96m"
const std::string color_array[12] = {RED,       GREEN,       YELLOW,       BLUE,       MAGENTA,       CYAN,
                                     LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW, LIGHT_BLUE, LIGHT_MAGENTA, LIGHT_CYAN};

/*
    介绍：此工具类包含无状态逻辑。所有成员函数都是 static
    使用方式：直接通过类名调用，无需实例化
*/
class print_tool
{
  public:
    print_tool();
    ~print_tool();

    typedef enum
    {
        S2M = 0,
        M2S = 1,
        S2D = 2,
        D2S = 3
    } Direction;

    /*
        参数：
            direction：决定打印多少个空格
            is_light：决定是否亮色
            tag：决定颜色
            name：决定内容
    */
    static void start(const Direction &direction,
                      bool is_light,
                      const unsigned int &tag,
                      const std::string &name)
    {
#ifdef COLORFUL_PRINT
        color = color_array[tag % 6 + (is_light ? 6 : 0)];
#endif

        std::string space = "";
        unsigned int name_length = 0;
#ifdef SIMPLE_PRINT
        if (direction == S2M) {
            space = std::string(SIMPLE_PRINT_S2M_SPACE, ' ');
            name_length = SIMPLE_PRINT_S2M_NAME_LENGTH;
        } else if (direction == M2S) {
            space = std::string(SIMPLE_PRINT_M2S_SPACE, ' ');
            name_length = SIMPLE_PRINT_M2S_NAME_LENGTH;
        } else if (direction == S2D) {
            space = std::string(SIMPLE_PRINT_S2D_SPACE, ' ');
            name_length = SIMPLE_PRINT_S2D_NAME_LENGTH;
        } else if (direction == D2S) {
            space = std::string(SIMPLE_PRINT_D2S_SPACE, ' ');
            name_length = SIMPLE_PRINT_D2S_NAME_LENGTH;
        }
#else
        if (direction == S2M) {
            space = std::string(NO_SIMPLE_PRINT_S2M_SPACE, ' ');
            name_length = NO_SIMPLE_PRINT_S2M_NAME_LENGTH;
        } else if (direction == M2S) {
            space = std::string(NO_SIMPLE_PRINT_M2S_SPACE, ' ');
            name_length = NO_SIMPLE_PRINT_M2S_NAME_LENGTH;
        } else if (direction == S2D) {
            space = std::string(NO_SIMPLE_PRINT_S2D_SPACE, ' ');
            name_length = NO_SIMPLE_PRINT_S2D_NAME_LENGTH;
        } else if (direction == D2S) {
            space = std::string(NO_SIMPLE_PRINT_D2S_SPACE, ' ');
            name_length = NO_SIMPLE_PRINT_D2S_NAME_LENGTH;
        }
#endif
        std::cout << space;

        std::cout << color;
        std::cout << std::left << std::setfill(' ') << std::setw(name_length);
        std::cout << name;
    }
    static void elecment(const std::string &name,
                         const std::string &left,
                         const std::string &right,
                         const long long &value,
                         const int &value_length)
    {
        long long mod = static_cast<long long>(std::pow(10, value_length));

        std::cout << color;
        std::cout << name;

        std::cout << RESET;
        std::cout << left;

        std::cout << color;
        std::cout << std::right << std::setfill('0') << std::setw(value_length);
        std::cout << std::to_string(value % mod);

        std::cout << RESET;
        std::cout << right;
    }

  private:
    static std::string color;
};
std::string print_tool::color = RESET;

#endif
#ifndef ERROR_AND_WARNING_H
#define ERROR_AND_WARNING_H

// ----------------------------- ERROR AND WARNING -----------------------------

#define CPP_WARNING(message) std::cout << std::endl << "WARNING:   " << message << std::endl << std::endl;

#define CPP_ERROR(message)                                                                                                         \
    do {                                                                                                                           \
        std::cerr << std::endl << "ERROR:   " << message << std::endl << std::endl;                                                  \
        std::abort();                                                                                                              \
    } while (0)

/*
    目前：CPP_ASSERT 只在 /include/channel_model 中被使用
    目前：CPP_ASSERT 不符合则 error
*/
#define CPP_ASSERT(expr)                                                                                                           \
    do {                                                                                                                           \
        if (!(expr)) {                                                                                                             \
            std::cerr << std::endl << "ASSERTION ERROR:   " << #expr << std::endl << std::endl;                                      \
            std::abort();                                                                                                          \
        }                                                                                                                          \
    } while (0)

#endif
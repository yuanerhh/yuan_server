#include <exception>
#include <cstddef>
#include <string>
#include <cstring>
#include <error.h>

using ERROR_CODE = std::int32_t;

namespace yuan {

    class CException
    {
    public:
        CException(const std::string& file, const std::string& line, 
                    const std::string& func, const std::string& errInfo)
                    : m_file(file)
                    , m_line(line)
                    , m_func(func)
                    , m_errInfo(errInfo)
        {}

        std::string what() const
        {
            return m_file + "[" + m_line +  "]" + " " + m_func + 
             + ": " + m_errInfo;
        }

    public:
        std::string m_file;
        std::string m_line;
        std::string m_func;
        std::string m_errInfo;

    };

#define MAKE_EXCEPTION(ExceptName) \
    class ExceptName : public CException \
    { \
    public: \
        ExceptName(const std::string& file, const std::string& line, \ 
                    const std::string& func, const std::string& errInfo) : \ 
                     CException(file, line, func, errInfo) {} \
    };

    MAKE_EXCEPTION(CUnexpected)
    MAKE_EXCEPTION(CInvalidParam)
    MAKE_EXCEPTION(CInvalidCall)
    MAKE_EXCEPTION(CInvalidAccess)
    MAKE_EXCEPTION(CNotSupport)

    #define CUnexpectedThrow(info) throw CUnexpected(__FILE__, std::to_string(__LINE__), __FUNCTION__, info)
    #define CInvalidParamThrow(info) throw CInvalidParam(__FILE__, std::to_string(__LINE__), __FUNCTION__, info)
    #define CInvalidCallThrow(info) throw CInvalidCall(__FILE__, std::to_string(__LINE__), __FUNCTION__, info)
    #define CInvalidAccessThrow(info) throw CInvalidAccess(__FILE__, std::to_string(__LINE__), __FUNCTION__, info)
    #define CNotSupportThrow(info) throw CNotSupport(__FILE__, std::to_string(__LINE__), __FUNCTION__, info)

#undef MAKE_EXCEPTION

}
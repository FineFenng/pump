#ifndef PUMP_UTILITY_SOURCEFILE_H_
#define PUMP_UTILITY_SOURCEFILE_H_

#include <pump/Common.h>
#include <cstring>

/*May be can use c++17 file system instead*/
namespace pump { namespace utility
{

class SourceFile
{
public:
    template <unsigned int N>
    inline SourceFile(const char (&array)[N])
    {
        data_ = array;
        size_ = N;

        
#ifdef PUMP_PLATFORM_WIN
        const char* slash = strrchr(data_, '\\');
#else
        const char* slash = strrchr(data_, '/');
#endif
        if (slash) {
            data_ = slash + 1;
            size_ -= (data_ - array);
            
        }
    }

    explicit SourceFile(const char* file_name)
        : data_(file_name)
    {


#ifdef PUMP_PLATFORM_WIN
        const char* slash = strrchr(file_name, '\\');
#else
        const char* slash = strrchr(file_name, '/');
#endif
            
        if (slash) {
            data_ = slash + 1;
        }
        size_ = strlen(data_);
    }

    const char* data() const { return data_; }

private:
    const char* data_;
    size_t size_;
};
}}
#endif

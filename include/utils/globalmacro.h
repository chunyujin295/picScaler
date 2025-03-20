/*************************************************
 * 描述：公共宏定义
 *
 * globalmacro.h
 * Author：qijiangyong@mozihealthcare.cn
 * Date：2025-01-03
 * Update：
 * ************************************************/

#ifndef GLOBALMACRO_H
#define GLOBALMACRO_H

#include <boost/any.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <boost/optional.hpp>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <utils/stringutils.h>
#include <utils/loghelper.h>

namespace common_ced {
    // 通用类型转换
    /**
     * @newtype : 转换后的类型
     * @val     : 转换的表达式
     */
#define S_CAST(newtype, val) static_cast<newtype>(val)
#define D_CAST(newtype, val) dynamic_cast<newtype>(val)
#define C_CAST(newtype, val) const_cast<newtype>(val)
#define R_CAST(newtype, val) reinterpret_cast<newtype>(val)
#define B_CAST(newtype, val) boost::any_cast<newtype>(val)
    // 枚举值与字符串互转辅助宏
    /**
     * 枚举值转字符串
     * @EnumType  : 枚举类型
     * @EnumValue : 枚举值
     */
#define ENUM_TO_STRING(EnumType, EnumValue) case EnumType::EnumValue: return #EnumValue;
    /**
     * 字符串转枚举值
     * @EnumType  : 枚举类型
     * @EnumValue : 枚举值
     * @strVal	  : 字符串值
     */
#define STRING_TO_ENUM(EnumType, EnumValue, strVal) \
    if (strVal == #EnumValue) { \
        return EnumType::EnumValue; \
    }
    /**
    * 生成枚举和枚举字符串转换方法（类外,也可用于强类型类内的情况）
    * @param T
    * @param ... 可变长参数，枚举值
    */
#define CED_ENUMCLS_DECLARE(T, ...) \
    enum class T { __VA_ARGS__}; \
    static inline std::string T##ToString(T i){ \
    static const std::unordered_map<T, std::string> res = [] {\
    std::string s = #__VA_ARGS__;\
    std::unordered_map<T, std::string> result;\
    std::istringstream iss(s);\
    std::string item;\
    int index = 0;\
    while (std::getline(iss, item, ',')) {\
        size_t pos = item.find('=');\
        std::string k;\
        std::string v;\
        if (pos != std::string::npos) {\
            v = item.substr(0, pos);\
            k = item.substr(pos + 1, item.length());\
            k.erase(std::remove_if(k.begin(), k.end(), ::isspace), k.end());\
            v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());\
            index = common_ced::StringUtils::strToInt(k).get();\
            result.insert(std::pair<T, std::string>{(T)index, v});\
        } else {\
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());\
            result.insert(std::pair<T, std::string>{(T)index, item});\
        }\
    index ++;\
    }\
    return result;}();\
return res.at(i);\
    } \
    static inline T T##FromString(const std::string& i) { \
    static const std::unordered_map<std::string,T> res = [] {\
    std::string s = #__VA_ARGS__;\
    std::unordered_map<std::string,T> result;\
    std::istringstream iss(s);\
    std::string item;\
    int index = 0;\
    while (std::getline(iss, item, ',')) {\
        size_t pos = item.find('=');\
        std::string k;\
        std::string v;\
        if (pos != std::string::npos) {\
            v = item.substr(0, pos);\
            k = item.substr(pos + 1, item.length());\
            k.erase(std::remove_if(k.begin(), k.end(), ::isspace), k.end());\
            v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());\
            index = common_ced::StringUtils::strToInt(k).get();\
            result.insert(std::pair<std::string,T>{v,(T)index});\
        } else {\
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());\
            result.insert(std::pair<std::string,T>{item,(T)index});\
        }\
    index ++;\
    }\
    return result;}();\
return res.at(i);\
    }
    /**
     * 生成枚举和枚举字符串转换方法（类内）
     * @param T
     * @param ... 可变长参数，枚举值
     */
#define CED_ENUM_DECLARE(T, ...) \
    enum T { __VA_ARGS__}; \
    static inline std::string T##ToString(T i){ \
    static const std::unordered_map<T, std::string> res = [] {\
    std::string s = #__VA_ARGS__;\
    std::unordered_map<T, std::string> result;\
    std::istringstream iss(s);\
    std::string item;\
    int index = 0;\
    while (std::getline(iss, item, ',')) {\
        size_t pos = item.find('=');\
        std::string k;\
        std::string v;\
        if (pos != std::string::npos) {\
            v = item.substr(0, pos);\
            k = item.substr(pos + 1, item.length());\
            k.erase(std::remove_if(k.begin(), k.end(), ::isspace), k.end());\
            v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());\
            index = common_ced::StringUtils::strToInt(k).get();\
            result.insert(std::pair<T, std::string>{(T)index, v});\
        } else {\
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());\
            result.insert(std::pair<T, std::string>{(T)index, item});\
        }\
    index ++;\
    }\
    return result;}();\
return res.at(i);\
    } \
    static inline T T##FromString(const std::string& i) { \
    static const std::unordered_map<std::string,T> res = [] {\
    std::string s = #__VA_ARGS__;\
    std::unordered_map<std::string,T> result;\
    std::istringstream iss(s);\
    std::string item;\
    int index = 0;\
    while (std::getline(iss, item, ',')) {\
        size_t pos = item.find('=');\
        std::string k;\
        std::string v;\
        if (pos != std::string::npos) {\
            v = item.substr(0, pos);\
            k = item.substr(pos + 1, item.length());\
            k.erase(std::remove_if(k.begin(), k.end(), ::isspace), k.end());\
            v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());\
            index = common_ced::StringUtils::strToInt(k).get();\
            result.insert(std::pair<std::string,T>{v,(T)index});\
        } else {\
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());\
            result.insert(std::pair<std::string,T>{item,(T)index});\
        }\
    index ++;\
    }\
    return result;}();\
return res.at(i);\
}

    /**
    * 可空类型
    * @param ClassName
    */
#define CED_NULLABLE(ClassName) boost::optional<ClassName>

    /**
    * 空值
    */
#define CED_NULL boost::none

    /**
    * 时间测试宏
    * @param code 测试代码
    * @param level
    */
#define CODE_TIME(code) ( [&] { \
    auto start = std::chrono::steady_clock::now(); \
    code; \
    auto end = std::chrono::steady_clock::now(); \
    return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()); \
}() )
} // namespace common_ced

#endif//GLOBALMACRO_H

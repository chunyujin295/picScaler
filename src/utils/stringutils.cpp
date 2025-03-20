#include <codecvt>
#include <utils/loghelper.h>
#include <utils/stringutils.h>
#include <memory>
#include <numeric>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <regex>

namespace common_ced
{
	std::string StringUtils::anyToString(const boost::any& data)
	{
		if (!data.empty())
		{
			if (data.type() == typeid(std::string))
			{
				return boost::any_cast<std::string>(data);
			}
			if (data.type() == typeid(const char*))// 添加对 const char* 的支持
			{
				return boost::any_cast<const char*>(data);// 直接返回字符串
			}
			if (data.type() == typeid(long long))// 添加对 const char* 的支持
			{
				return std::to_string(boost::any_cast<long long>(data));
			}
			if (data.type() == typeid(char*))
			{
				return boost::any_cast<char*>(data);
			}
			if (data.type() == typeid(int))
			{
				return std::to_string(boost::any_cast<int>(data));
			}
			if (data.type() == typeid(double))
			{
				return std::to_string(boost::any_cast<double>(data));
			}
			if (data.type() == typeid(bool))
			{
				return boost::any_cast<bool>(data) ? "true" : "false";
			}
			if (data.type() == typeid(boost::posix_time::ptime))
			{
				return timeToString(boost::any_cast<boost::posix_time::ptime>(data));
			}
			if (data.type() == typeid(std::wstring))
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				std::string utf8String = converter.to_bytes(boost::any_cast<std::wstring>(data));
				return utf8String;
			}
			if (data.type() == typeid(wchar_t*))
			{
				std::wstring wStr = boost::any_cast<wchar_t*>(data);
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string utf8String = converter.to_bytes(wStr);
				return utf8String;
			}
			if (data.type() == typeid(const wchar_t*))
			{
				std::wstring wStr = boost::any_cast<const wchar_t*>(data);
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string utf8String = converter.to_bytes(wStr);
				return utf8String;
			}
		}
		LOG_WARN("类型转换失败");
		return {};
	}

	std::string StringUtils::timeToString(const boost::posix_time::ptime& time)
	{
		try
		{
			// 创建时间格式
			const auto facet = new boost::posix_time::time_facet("%Y-%m-%dT%H:%M:%S.%f");
			// 创建字符串流对象
			std::stringstream ss;
			// 设置字符串流的本地化信息
			ss.imbue(std::locale(ss.getloc(), facet));
			ss << time;

			// delete facet;

			return ss.str().substr(0, 23);// 截取到毫秒（3位）
		}
		catch (...)
		{
			return "";// 返回空字符串表示转换失败
		}
	}

	std::string StringUtils::timeToString(const boost::posix_time::ptime& time, const std::string& format)
	{
		try
		{
			// 创建时间格式
			const auto facet = new boost::posix_time::time_facet(format.c_str());
			// 创建字符串流对象
			std::stringstream ss;
			// 设置字符串流的本地化信息
			ss.imbue(std::locale(ss.getloc(), facet));
			ss << time;

			// delete facet;

			return ss.str().substr(0, 23);// 截取到毫秒（3位）
		}
		catch (...)
		{
			return {};
		}
	}

	void StringUtils::toLowerStr(std::string& str)
	{
		boost::to_lower(str);
	}
}// namespace common_ced
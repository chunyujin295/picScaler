/*************************************************
 * 描述：字符串工具类
 *
 * File：stringutils.h
 * Author：zhangzhiming@mozihealthcare.cn
 * Date：2025/2/19
 * Update：
 * ************************************************/
#ifndef EDITORCONTROLV5_STRINGUTILS_H
#define EDITORCONTROLV5_STRINGUTILS_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>
namespace common_ced
{

	class StringUtils
	{
	public:
		/**
		 * boost::any数据转string
		 * @param data
		 * @return
		 */
		static std::string anyToString(const boost::any& data);

		/**
		 * 时间转化成字符串
		 */
		static std::string timeToString(const boost::posix_time::ptime& time);

		/**
		 * 日期转字符串
		 * @param time 时间
		 * @param format 格式化串
		 * @return 格式化后的字符串
		 */
		static std::string timeToString(const boost::posix_time::ptime& time,const std::string& format);

		/**
		 * 全转小写
		 * @param str
		 */
		static void toLowerStr(std::string& str);
	};
}// namespace common_ced

#endif//EDITORCONTROLV5_STRINGUTILS_H
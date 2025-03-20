/*************************************************
 * 描述：日志对象封装类
 *
 * File：loghelper.h
 * Author：chenyujin@mozihealthcare.cn
 * Date：25-1-14
 * Update：
 * ************************************************/
#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <boost/any.hpp>

namespace spdlog
{
	namespace details
	{
		struct log_msg;
	}

	class logger;
}// namespace spdlog

namespace common_ced
{

	using cusTomSlot = std::function<void()>;// 槽函数类型

	class LogCusTomSignalSlot// 用于实现自定义信号槽机制的类
	{
	public:
		// 连接槽函数
		static void cusTomConnect(const cusTomSlot& slot);

		// 触发信号并传递参数
		static void cusTomEmit();

	private:
		LogCusTomSignalSlot() = default;

		LogCusTomSignalSlot(const LogCusTomSignalSlot&) = delete;

		LogCusTomSignalSlot& operator=(const LogCusTomSignalSlot&) = delete;

		static LogCusTomSignalSlot* getInstance();

		// 静态互斥锁，用于确保线程安全
		static std::mutex s_mutex;

		static LogCusTomSignalSlot* s_logCusTomSignalSlot;

		std::vector<cusTomSlot> m_slots;// 存储槽函数
	};


	enum class LogLevel: int
	{
		Trace = 0,
		Debug = 1,
		Info = 2,
		Warn = 3,
		Error = 4,
		Critical = 5
	};

	class LogHelper
	{
	public:
		/**
         * 删除拷贝构造，确保单例
         */
		LogHelper(const LogHelper&) = delete;

		/**
         * 删除赋值符号重载，确保单例
         * @return
         */
		LogHelper& operator=(const LogHelper&) = delete;

		/**
		 * 手动修改读取配置文件的路径
		 * 若不调用此方法修改，默认读取位置为可执行程序所在路径下
		 * @param configFilePath 配置文件路径
		 */
		void setConfigPath(const std::string& configFilePath);

		/**
         * 输出trace级别日志文本，严重级别0
         * @param msgList 日志文本信息列表
         */
		// static void trace(const std::initializer_list<boost::any>& msgList);

		/**
         * 输出带行号的trace级别日志文本，严重级别0
         * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
         * @param fileName 日志输出位置文件名，应传入宏__FILE__
         * @param fileLine 日志输出位置行号，应传入宏__LINE__
         * @param function 日志输出所在函数，应传入宏__FUNCTION__
         * @param msgList 日志文本信息列表
         */
		static void trace(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		/**
         * 输出debug级别日志文本，严重级别1
         * @param msgList 日志文本信息列表
         */
		// static void debug(const std::initializer_list<boost::any>& msgList);

		/**
         * 输出带行号的debug级别日志文本，严重级别1
         * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
         * @param fileName 日志输出位置文件名，应传入宏__FILE__
         * @param fileLine 日志输出位置行号，应传入宏__LINE__
         * @param function 日志输出所在函数，应传入宏__FUNCTION__
         * @param msgList 日志文本信息列表
         */
		static void debug(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		/**
         * 输出info级别日志文本，严重级别2
         * @param msgList 日志文本信息列表
         */
		// static void info(const std::initializer_list<boost::any>& msgList);

		/**
         * 输出带行号的info级别日志文本，严重级别2
         * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
         * @param fileName 日志输出位置文件名，应传入宏__FILE__
         * @param fileLine 日志输出位置行号，应传入宏__LINE__
         * @param function 日志输出所在函数，应传入宏__FUNCTION__
         * @param msgList 日志文本信息列表
         */
		static void info(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		//用来处理右值的
		// static void info(const char* fileName, int fileLine, const char* function, std::initializer_list<boost::any>&& msgList);

		/**
         * 输出warn级别日志文本，严重级别3
         * @param msgList 日志文本信息列表
         */
		// static void warn(const std::initializer_list<boost::any>& msgList);

		/**
         * 输出带行号的warn级别日志文本，严重级别3
         * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
         * @param fileName 日志输出位置文件名，应传入宏__FILE__
         * @param fileLine 日志输出位置行号，应传入宏__LINE__
         * @param function 日志输出所在函数，应传入宏__FUNCTION__
         * @param msgList 日志文本信息列表
         */
		static void warn(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		/**
         * 输出error级别日志文本，严重级别4
         * @param msgList 日志文本信息列表
         */
		// static void error(const std::initializer_list<boost::any>& msgList);

		/**
         * 输出带行号的error级别日志文本，严重级别4
         * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
         * @param fileName 日志输出位置文件名，应传入宏__FILE__
         * @param fileLine 日志输出位置行号，应传入宏__LINE__
         * @param function 日志输出所在函数，应传入宏__FUNCTION__
         * @param msgList 日志文本信息列表
         */
		static void error(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		/**
		 * 输出critical级别日志文本，严重级别5
		 * @param msgList 日志文本信息列表
		 */
		// static void critical(const std::initializer_list<boost::any>& msgList);

		/**
		 * 输出带行号的critical级别日志文本，严重级别5
		 * fileName,fileLine,funtion参数可使用宏定义GET_LINE取代
		 * @param fileName 日志输出位置文件名，应传入宏__FILE__
		 * @param fileLine 日志输出位置行号，应传入宏__LINE__
		 * @param function 日志输出所在函数，应传入宏__FUNCTION__
		 * @param msgList 日志文本信息列表
		 */
		static void critical(const char* fileName, int fileLine, const char* function, const std::initializer_list<boost::any>& msgList);

		/**
         * 创建一个输出到流的日志输出器，并添加到日志对象中
         * 值得注意的是，这种方式不支持输出线程号
         * @param stream 要输出到的流对象
         * @param flush 是否立即刷新流缓冲区
         * @param level 日志级别
         */
		static void setStreamOutPut(std::ostringstream& stream, bool flush = false, LogLevel level = LogLevel::Trace);

		/**
         * 生成一个回调类型日志输出对象
         * 里面调用了回调函数logCallBack
         * 里面调用了setStreamOutPut方式，用来输出到流
         * @param stream 要输出到的流对象
         * @param flush 是否立即刷新流缓冲区
         * @param level 日志级别
         */
		static void setCallBackSink(std::ostringstream& stream, bool flush = false, LogLevel level = LogLevel::Trace);

	private:
		/**
         * 构造
         */
		LogHelper();

		/**
         * 析构，默认
         */
		~LogHelper() = default;

		/**
         * 进行初始化
         * @return
         */
		static LogHelper* getInstance();

		/**
         * 获取内部日志对象spdlog指针
         * @return
         */
		std::shared_ptr<spdlog::logger> getLogger();

		/**
         * 子程序，用来读取ini配置文件内容
         */
		void configFromIni(const std::string& configFilePath);

		/**
         * 子程序，一套默认配置，当配置文件读取失败时调用
         */
		void defaultConfig();

		/**
         * 子程序，用于检测ini中属性对应值是否为空，为空则打印错误并返回默认属性值
         * @param itemValue 属性值
         * @param itemName 属性名
         * @param defaultValue 默认属性值
         * @return
         */
		std::string getItemValue(const std::string& itemValue, const std::string& itemName,
		                         const std::string& defaultValue);

		/**
         * 子程序，用于检测sink输出路径是否存在，不存在则打印错误
         * @param sinkType
         * @param filePath
         */
		bool checkSinkFilePath(const std::string& sinkType, const std::string& filePath);

		/**
         * 回调函数，用来被回调型日志输出器调用
         * 用来发送自定义信号槽类LogCusTomSignalSlot的信号
         * @param logMsg 回调型日志输出器固定参数，无需关心
         */
		void logCallBack(const spdlog::details::log_msg& logMsg);

		/**
		 * 用于拼接字符串
		 * @param msgList
		 * @return
		 */
		static std::string linkString(const std::initializer_list<boost::any> &msgList);

	private:
		// 静态互斥锁，用于确保线程安全
		static std::mutex s_mutex;
		// 静态对象指针
		static LogHelper* s_instance;
		// spdlog库logger类对象指针
		std::shared_ptr<spdlog::logger> m_logger;

		static bool m_traceShowLine;
		static bool m_debugShowLine;
		static bool m_infoShowLine;
		static bool m_warnShowLine;
		static bool m_errorShowLine;
		static bool m_criticalShowLine;
	};


	// 通过宏定义方式调用日志输出：
#define GET_LINE __FILE__, __LINE__, __FUNCTION__// 宏，用来代替位置、行号、函数信息这三个宏

#define LOG_TRACE(...) LogHelper::trace(GET_LINE, {__VA_ARGS__})// 日志宏，[trace级别]
// #define LOG_TRACE(...) LogHelper::trace(GET_LINE, {boost::any(__VA_ARGS__)})// 日志宏，[trace级别]
#define LOG_DEBUG(...) LogHelper::debug(GET_LINE, {__VA_ARGS__})// 同上 [debug级别]
// #define LOG_DEBUG(...) LogHelper::debug(GET_LINE, {boost::any(__VA_ARGS__)})// 日志宏，[trace级别]
#define LOG_INFO(...) LogHelper::info(GET_LINE, {__VA_ARGS__})  // 同上 [info级别]
// #define LOG_INFO(...) LogHelper::info(GET_LINE, {boost::any(__VA_ARGS__)})  // 同上 [info级别]
#define LOG_WARN(...) LogHelper::warn(GET_LINE, {__VA_ARGS__})  // 同上 [warn级别]
#define LOG_ERROR(...) LogHelper::error(GET_LINE, {__VA_ARGS__})// 同上 [error级别]
#define LOG_CRITI(...) LogHelper::critical(GET_LINE, {__VA_ARGS__})// 同上 [critical级别]
}// namespace common_ced

#endif//LOGHELPER_H
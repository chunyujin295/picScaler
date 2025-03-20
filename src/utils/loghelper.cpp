#include <boost/optional.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <utils/loghelper.h>
#include <cstdarg>
#include <iostream>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog-inl.h>
#include <utils/stringutils.h>

using namespace common_ced;
std::mutex LogCusTomSignalSlot::s_mutex;
LogCusTomSignalSlot* LogCusTomSignalSlot::s_logCusTomSignalSlot = nullptr;

LogHelper* LogHelper::s_instance = nullptr;
std::mutex LogHelper::s_mutex;

bool LogHelper::m_traceShowLine = false;
bool LogHelper::m_debugShowLine = false;
bool LogHelper::m_infoShowLine = false;
bool LogHelper::m_warnShowLine = true;
bool LogHelper::m_errorShowLine = true;
bool LogHelper::m_criticalShowLine = true;

LogCusTomSignalSlot* LogCusTomSignalSlot::getInstance()
{
    // DCL双检锁创建单例
    if (s_logCusTomSignalSlot == nullptr)
    {
        std::lock_guard<std::mutex> lock(s_mutex); // 确保多线程创建单例时线程安全
        if (s_logCusTomSignalSlot == nullptr)
        {
            s_logCusTomSignalSlot = new LogCusTomSignalSlot();
        }
    }
    return s_logCusTomSignalSlot;
}

void LogCusTomSignalSlot::cusTomConnect(const cusTomSlot& slot)
{
    getInstance()->m_slots.push_back(slot);
}

void LogCusTomSignalSlot::cusTomEmit()
{
    for (auto& slot : getInstance()->m_slots)
    {
        if (slot)
        {
            // 确保槽函数有效
            slot();
        }
    }
}

void LogHelper::setConfigPath(const std::string& configFilePath)
{
    try
    {
        this->configFromIni(configFilePath);
        if (this->m_logger->level() == spdlog::level::off) // 日志级别设置失败，为off
        {
            throw spdlog::spdlog_ex("log level is off");
        }
    }
    catch (const spdlog::spdlog_ex& ex) // 捕获读取配置文件过程中遇到的异常
    {
        std::cerr << "Log initialization error: " << ex.what() << std::endl;

        this->defaultConfig(); // 采用默认配置
    }
}

// void LogHelper::trace(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->trace("{}", msg);
// }

void LogHelper::trace(const char* fileName, int fileLine, const char* function,
                      const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->trace("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_traceShowLine)
    {
        getInstance()->getLogger()->trace("{}", msg);
        return;
    }
    getInstance()->getLogger()->trace("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

// void LogHelper::debug(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->debug("{}", msg);
// }

void LogHelper::debug(const char* fileName, int fileLine, const char* function,
                      const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->debug("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_debugShowLine)
    {
        getInstance()->getLogger()->debug("{}", msg);
        return;
    }
    getInstance()->getLogger()->debug("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

// void LogHelper::info(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->info("{}", msg);
// }

void LogHelper::info(const char* fileName, int fileLine, const char* function,
                     const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->info("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_infoShowLine)
    {
        getInstance()->getLogger()->info("{}", msg);
        return;
    }
    getInstance()->getLogger()->info("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

// void LogHelper::info(const char* fileName, int fileLine, const char* function, std::initializer_list<boost::any>&& msgList)
// {
// 	const std::initializer_list<boost::any> list = msgList;
// 	info(fileName, fileLine, function, list);
// }

// void LogHelper::warn(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->warn("{}", msg);
// }

void LogHelper::warn(const char* fileName, int fileLine, const char* function,
                     const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->warn("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_warnShowLine)
    {
        getInstance()->getLogger()->warn("{}", msg);
        return;
    }
    getInstance()->getLogger()->warn("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

// void LogHelper::error(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->error("{}", msg);
// }

void LogHelper::error(const char* fileName, int fileLine, const char* function,
                      const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->error("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_errorShowLine)
    {
        getInstance()->getLogger()->error("{}", msg);
        return;
    }
    getInstance()->getLogger()->error("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

// void LogHelper::critical(const std::initializer_list<boost::any>& msgList)
// {
// 	std::string msg;
// 	msg.clear();
// 	msg = linkString(msgList);
// 	if (msg.empty())
// 	{
// 		return;
// 	}
// 	getInstance()->getLogger()->critical("{}", msg);
// }

void LogHelper::critical(const char* fileName, int fileLine, const char* function,
                         const std::initializer_list<boost::any>& msgList)
{
    std::string msg;
    msg.clear();
    msg = linkString(msgList);
    if (msg.empty())
    {
        getInstance()->getLogger()->critical("[{}:{}][{}] 日志打印失败，数据类型转换错误", fileName, fileLine, function);
        return;
    }
    if (!m_criticalShowLine)
    {
        getInstance()->getLogger()->critical("{}", msg);
        return;
    }
    getInstance()->getLogger()->critical("[{}:{}][{}]{}", fileName, fileLine, function, msg);
}

void LogHelper::setStreamOutPut(std::ostringstream& stream, bool flush, LogLevel level)
{
    auto streamSink = std::make_shared<spdlog::sinks::ostream_sink_mt>(stream, flush);
    spdlog::level::level_enum spdlogLevel = spdlog::level::trace;
    switch (level)
    {
    case LogLevel::Trace:
        spdlogLevel = spdlog::level::trace;
        break;
    case LogLevel::Debug:
        spdlogLevel = spdlog::level::debug;
    case LogLevel::Info:
        spdlogLevel = spdlog::level::info;
    case LogLevel::Warn:
        spdlogLevel = spdlog::level::warn;
    case LogLevel::Error:
        spdlogLevel = spdlog::level::err;
    case LogLevel::Critical:
        spdlogLevel = spdlog::level::critical;
    defalut:
        break;
    }

    streamSink->set_level(spdlogLevel);
    getInstance()->getLogger()->sinks().push_back(streamSink);
}

void LogHelper::setCallBackSink(std::ostringstream& stream, bool flush, LogLevel level)
{
    LogHelper::setStreamOutPut(stream, flush, level);

    auto callback = std::bind(&common_ced::LogHelper::logCallBack, getInstance(), std::placeholders::_1);
    auto callBackSink = std::make_shared<spdlog::sinks::callback_sink_mt>(callback);

    callBackSink->set_level(static_cast<spdlog::level::level_enum>(level));
    getInstance()->getLogger()->sinks().push_back(callBackSink);
}

LogHelper::LogHelper()
{
    // 设置全局错误处理程序
    spdlog::set_error_handler(
        [](const std::string& msg)
        {
            std::cerr << "Log initialization failed: " << msg << std::endl;
        });

    try
    {
        this->configFromIni("./log_config.ini"); //默认配置文件存在于可执行程序所在路径
        if (this->m_logger->level() == spdlog::level::off) // 日志级别设置失败，为off
        {
            throw spdlog::spdlog_ex("log level is off");
        }
    }
    catch (const spdlog::spdlog_ex& ex) // 捕获读取配置文件过程中遇到的异常
    {
        std::cerr << "Log initialization error: " << ex.what() << std::endl;

        this->defaultConfig(); // 采用默认配置
    }

#ifndef DEBUG //release模式下，提升日志级别，或关闭日志输出
    this->m_logger->set_level(spdlog::level::warn);
    // this->m_logger->set_level(spdlog::level::off);
#endif
    // this->m_logger->set_level(spdlog::level::off);
}

LogHelper* LogHelper::getInstance()
{
    // DCL双检锁创建单例
    if (s_instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(s_mutex); // 确保多线程创建单例时线程安全
        if (s_instance == nullptr)
        {
            s_instance = new LogHelper();
        }
    }
    return s_instance;
}

std::shared_ptr<spdlog::logger> LogHelper::getLogger()
{
    return m_logger;
}

void LogHelper::configFromIni(const std::string& configFilePath)
{
    this->m_logger.reset(); //重新设置日志

    // 读取INI配置文件
    boost::property_tree::ptree pt;
    try
    {
        boost::property_tree::ini_parser::read_ini(configFilePath, pt);
    }
    catch (const boost::property_tree::ini_parser_error& e) // 打开配置文件失败，捕获异常
    {
        throw spdlog::spdlog_ex("open log_ini_file error: " + std::string(e.what()));
    }

    // 获取logger名称，如果不存在或为空，则使用默认值
    boost::optional<std::string> loggerNameOpt = pt.get_optional<std::string>("LogHelper.name");
    std::string loggerName = this->getItemValue(*loggerNameOpt, "name", "defalut_loghelper");

    // 获取logger级别，如果不存在或为空，则使用默认值
    boost::optional<std::string> levelStrOpt = pt.get_optional<std::string>("LogHelper.level");
    std::string levelStr = this->getItemValue(*levelStrOpt, "level", "info");
    spdlog::level::level_enum level = spdlog::level::from_str(levelStr);

    // 获取flush_on级别，如果不存在或为空，则使用默认值
    boost::optional<std::string> flushOnStrOpt = pt.get_optional<std::string>("LogHelper.flush_on");
    std::string flushOnStr = this->getItemValue(*flushOnStrOpt, "flush_on", "warn");
    spdlog::level::level_enum flush_on = spdlog::level::from_str(flushOnStr);

    // 获取输出格式，如果不存在或为空，则使用默认值
    boost::optional<std::string> logPatternOpt = pt.get_optional<std::string>("LogHelper.pattern");
    std::string logPatternStr = this->getItemValue(*logPatternOpt, "pattern",
                                                   "[%Y-%m-%d %H:%M:%S.%e][%n][%^%l%$][thread %t]%v");
    {
        boost::optional<std::string> traceShowLineOpt = pt.get_optional<std::string>("showLine.trace");
        std::string traceShowLineStr = this->getItemValue(*traceShowLineOpt, "trace", "false");
        this->m_traceShowLine = traceShowLineStr.compare("true") == 0 ? true : false;

        boost::optional<std::string> debugShowLineOpt = pt.get_optional<std::string>("showLine.debug");
        std::string debugShowLineStr = this->getItemValue(*debugShowLineOpt, "debug", "false");
        this->m_debugShowLine = debugShowLineStr.compare("true") == 0 ? true : false;

        boost::optional<std::string> infoShowLineOpt = pt.get_optional<std::string>("showLine.info");
        std::string infoShowLineStr = this->getItemValue(*infoShowLineOpt, "info", "false");
        this->m_infoShowLine = infoShowLineStr.compare("true") == 0 ? true : false;

        boost::optional<std::string> warnShowLineOpt = pt.get_optional<std::string>("showLine.warn");
        std::string warnShowLineStr = this->getItemValue(*warnShowLineOpt, "warn", "false");
        this->m_warnShowLine = warnShowLineStr.compare("true") == 0 ? true : false;

        boost::optional<std::string> errorShowLineOpt = pt.get_optional<std::string>("showLine.error");
        std::string errorShowLineStr = this->getItemValue(*errorShowLineOpt, "error", "false");
        this->m_errorShowLine = errorShowLineStr.compare("true") == 0 ? true : false;

        boost::optional<std::string> criticalShowLineOpt = pt.get_optional<std::string>("showLine.critical");
        std::string criticalShowLineStr = this->getItemValue(*criticalShowLineOpt, "critical", "false");
        this->m_criticalShowLine = criticalShowLineStr.compare("true") == 0 ? true : false;
    }
    // 遍历所有节
    std::unordered_map<std::string, boost::property_tree::ptree> sinksOpts;
    for (const auto& section : pt)
    {
        std::string sectionName = section.first;
        if (sectionName.substr(0, 6) == "sinks.")
        {
            std::string sinkName = sectionName.substr(6);
            sinksOpts[sinkName] = section.second;
        }
    }
    if (sinksOpts.empty())
    {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); // 创建控制台sink
        consoleSink->set_pattern(logPatternStr);
        this->m_logger = std::make_shared<spdlog::logger>("console", consoleSink);
        std::cerr << "LogHelper not set Sink, used default: console!" << std::endl;
        return;
    }
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
    for (const auto& sinkSection : sinksOpts)
    {
        std::string sinkName = sinkSection.first;
        const auto& sinkParams = sinkSection.second;

        // 获取sink类型，如果不存在或为空，则跳过
        boost::optional<std::string> typeOpt = sinkParams.get_optional<std::string>("type");
        if ((*typeOpt).empty())
        {
            std::cerr << "LogHelper sink not exists, jumped this sink config!" << std::endl;
            continue;
        }
        const std::string& type = *typeOpt;

        // 获取sink级别，如果不存在或为空，则使用默认值
        boost::optional<std::string> sinkLevelStrOpt = sinkParams.get_optional<std::string>("level");
        std::string sinkLevelStr = this->getItemValue(*sinkLevelStrOpt, "sinkLevel", "info");
        spdlog::level::level_enum sinkLevel = spdlog::level::from_str(sinkLevelStr);

        if (type == "console")
        {
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            consoleSink->set_level(sinkLevel);
            sinks.push_back(consoleSink);
        }
        else if (type == "rotating_file")
        {
            // 获取文件路径，如果不存在或为空，则跳过
            boost::optional<std::string> filePathOpt = sinkParams.get_optional<std::string>("file_path");
            if (!this->checkSinkFilePath(type, *filePathOpt))
            {
                //TODO 后续增加路径判断处理
                continue;
            }
            std::string filePath = *filePathOpt;

            size_t maxSize = sinkParams.get<size_t>("max_size", 10485760);
            size_t maxFiles = sinkParams.get<size_t>("max_files", 5);
            auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filePath, maxSize, maxFiles);
            fileSink->set_level(sinkLevel);
            sinks.push_back(fileSink);
        }
        else if (type == "daily_file")
        {
            // 获取文件路径，如果不存在或为空，则跳过
            boost::optional<std::string> filePathOpt = sinkParams.get_optional<std::string>("file_path");
            if (!this->checkSinkFilePath(type, *filePathOpt))
            {
                //TODO 后续增加路径判断处理
                continue;
            }
            std::string filePath = *filePathOpt;

            int rotationHour = sinkParams.get<int>("rotation_hour", 0);
            int rotationMin = sinkParams.get<int>("rotation_min", 0);
            auto fileSink =
                std::make_shared<spdlog::sinks::daily_file_sink_mt>(filePath, rotationHour, rotationMin);
            fileSink->set_level(sinkLevel);
            sinks.push_back(fileSink);
        }
        else if (type == "async_rotating_file")
        {
            // 获取文件路径，如果不存在或为空，则跳过
            boost::optional<std::string> filePathOpt = sinkParams.get_optional<std::string>("file_path");
            if (!this->checkSinkFilePath(type, *filePathOpt))
            {
                //TODO 后续增加路径判断处理
                continue;
            }
            std::string filePath = *filePathOpt;

            size_t maxSize = sinkParams.get<size_t>("max_size", 10485760);
            size_t maxFiles = sinkParams.get<size_t>("max_files", 3);
            auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filePath, maxSize, maxFiles);
            fileSink->set_level(sinkLevel);
            sinks.push_back(fileSink);
        }
        else if (type == "basic_file_sink_mt")
        {
            // 获取文件路径，如果不存在或为空，则跳过
            boost::optional<std::string> filePathOpt = sinkParams.get_optional<std::string>("file_path");
            if (!this->checkSinkFilePath(type, *filePathOpt))
            {
                //TODO 后续增加路径判断处理
                continue;
            }
            std::string filePath = *filePathOpt;
            bool truncate = sinkParams.get<bool>("truncate", false);
            auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath, truncate);
            fileSink->set_level(sinkLevel);
            sinks.push_back(fileSink);
        }
        else
        {
            std::cerr << "Unknown sink type: " << type << std::endl;
            std::cerr << "unknown sink type" << std::endl;
        }
    }
    this->m_logger = std::make_shared<spdlog::logger>(loggerName);
    for (const auto& sink : sinks)
    {
        this->m_logger->sinks().push_back(sink);
    }
    this->m_logger->set_level(level);
    this->m_logger->flush_on(flush_on);
    this->m_logger->set_pattern(logPatternStr);
}

void LogHelper::defaultConfig()
{
    // 进行默认配置
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); //
    this->m_logger = std::make_shared<spdlog::logger>("cedLog_default", consoleSink);

    // 设置日志级别
    this->m_logger->set_level(spdlog::level::trace);
    // 设置日志格式
    this->m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%^%l%$][thread %t]%v");

    m_traceShowLine = false;
    m_debugShowLine = false;
    m_infoShowLine = false;
    m_warnShowLine = true;
    m_errorShowLine = true;
    m_criticalShowLine = true;
}

std::string LogHelper::getItemValue(const std::string& itemValue, const std::string& itemName,
                                    const std::string& defaultValue)
{
    if (itemValue.empty())
    {
        std::cerr << "LogHelper " + itemName + " not set! Used default" + defaultValue + "!" << std::endl;
        return defaultValue;
    }

    return itemValue;
}

bool LogHelper::checkSinkFilePath(const std::string& sinkType, const std::string& filePath)
{
    if (!filePath.empty())
    {
        return true;
    }
    std::cerr << "LogHelper sink " + sinkType + "filePath is not set, jumped this sink config!" << std::endl;
    return false;
}

void LogHelper::logCallBack(const spdlog::details::log_msg& logMsg)
{
    LogCusTomSignalSlot::cusTomEmit();
}

std::string LogHelper::linkString(const std::initializer_list<boost::any>& msgList)
{
    if (empty(msgList))
    {
        return {};
    }
    std::string linkStr;
    linkStr.clear();
    for (const auto& msg : msgList)
    {
        std::string msgStr = StringUtils::anyToString(msg);
        if (msgStr.empty())
        {
            return {};
        }
        linkStr += msgStr;
    }
    return linkStr;
}

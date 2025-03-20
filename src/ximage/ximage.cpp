#include <ximage/ximage.h>
#include <ximage/base64.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <utils/stringutils.h>

#include <stb_image_resize2.h>
#include <fstream>
#include <utils/swapUtf8Gbk.h>
#include <string>
#include <sstream>
#include <utils/loghelper.h>
// #include <containstringutils.her/container_fwd.hpp>
#include <spdlog/fmt/bundled/chrono.h>


utils::ximage::ximage()
{
	this->m_meta = new MetaData();
}

utils::ximage::ximage(int width, int height, xcolor color, ColorFormat format)
{
	this->m_meta = new MetaData();
	this->m_meta->width = width;
	this->m_meta->height = height;
	this->m_meta->format = format;
	unsigned char red = color.red();
	unsigned char green = color.green();
	unsigned char blue = color.blue();
	try
	{
		switch (format)
		{
		case ColorFormat::Format_RGB888:
			this->m_meta->channels = RGB_CHANNELS;
			this->m_meta->pixelData = new unsigned char[width * height * this->m_meta->channels];
			for (int i = 0; i < width * height + RGB_CHANNELS; i += RGB_CHANNELS)
			{
				this->m_meta->pixelData[i] = red;
				this->m_meta->pixelData[i + 1] = green;
				this->m_meta->pixelData[i + 2] = blue;
			}
			break;
		case ColorFormat::Format_RGBA8888:
			this->m_meta->channels = RGBA_CHANNELS;
			this->m_meta->pixelData = new unsigned char[width * height * this->m_meta->channels];
			for (int i = 0; i < width * height + RGBA_CHANNELS; i += RGBA_CHANNELS)
			{
				this->m_meta->pixelData[i] = red;
				this->m_meta->pixelData[i + 1] = green;
				this->m_meta->pixelData[i + 2] = blue;
				this->m_meta->pixelData[i + 3] = ALPHA_VAL_DEFAULT;
			}
			break;
		case ColorFormat::Format_Grayscale8:
			this->m_meta->channels = GRAYSCALE_CHANNELS;
			this->m_meta->pixelData = new unsigned char[width * height * this->m_meta->channels];
			for (int i = 0; i < width * height + RGBA_CHANNELS; i += GRAYSCALE_CHANNELS)
			{
				this->m_meta->pixelData[i] = red;// 8位单通道，这里取红色
			}
			break;
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
	}
}

utils::ximage::ximage(const unsigned char* data, int width, int height, ColorFormat format, bool beenAlignMemory)
{
	this->m_meta = new MetaData();
	if (!this->loadFromMemory(data, width, height, format, beenAlignMemory))
	{
		this->clear();
	}
}

utils::ximage::ximage(const unsigned char* rgb, const unsigned char* alpha, int width, int height, bool beenAlignMemory)
{
	this->m_meta = new MetaData();
	if (!this->loadFromMemory(rgb, alpha, width, height, beenAlignMemory))
	{
		this->clear();
	}
}

utils::ximage::ximage(const char* fileName)
{
	this->m_meta = new MetaData();
	if (!this->loadFromFile(fileName))
	{
		this->clear();
	}
}

utils::ximage::ximage(const std::string& base64Data)
{
	this->m_meta = new MetaData();
	if (!this->loadFormBase64(base64Data))
	{
		this->clear();
	}
}

utils::ximage::ximage(const ximage& other)
{
	deepCopy(other);
}

utils::ximage::ximage(ximage&& other) noexcept : m_meta(other.m_meta)
{
	other.m_meta = nullptr;
}

utils::ximage::ximage(const ximage* other)
{
	deepCopy(*other);
}

utils::ximage& utils::ximage::operator=(const ximage& other)
{
	// XImage temp(other);
	// this->swap(temp);// 交换当前对象和临时对象的资源
	// return *this;

	if (this != &other)
	{
		this->clear();// 释放当前对象的资源

		deepCopy(other);
	}
	return *this;
}

utils::ximage& utils::ximage::operator=(ximage&& other) noexcept
{
	if (this != &other)
	{
		this->clear();// 释放当前对象的资源

		this->m_meta = other.m_meta;// 窃取资源
		other.m_meta = nullptr;// 清空源对象的资源
	}
	return *this;
}

// void utils::XImage::swap(XImage& other)
// {
// 	std::swap(this->m_meta, other.m_meta);
// }

bool utils::ximage::operator==(const ximage& other) const
{
	if (this == &other || this->m_meta == other.m_meta)
	{
		return true;
	}
	if (this->m_meta->channels == other.m_meta->channels &&
	    this->m_meta->height == other.m_meta->height &&
	    this->m_meta->width == other.m_meta->width &&
	    this->m_meta->format == other.m_meta->format)
	{
		if (memcmp(this->m_meta->pixelData, other.m_meta->pixelData, this->m_meta->width * this->m_meta->height * this->m_meta->channels) == 0)
		{
			return true;
		}
	}
	return false;
}

utils::ximage::~ximage()
{
	if (this->m_meta)
	{
		delete[] this->m_meta->pixelData;
		this->m_meta->pixelData = nullptr;
	}
	delete this->m_meta;
	this->m_meta = nullptr;
}

void utils::ximage::clear()
{
	if (this->m_meta)
	{
		delete[] this->m_meta->pixelData;
		this->m_meta->pixelData = nullptr;
		delete this->m_meta;
	}
	this->m_meta = nullptr;
}

bool utils::ximage::loadFromFile(const char* fileName) const
{
	if (fileName == nullptr)
	{
		//TODO 抛出异常
		return false;
	}
	std::string path = fileName;

#ifdef PLAT_WIN
	path = utf8ToGbk(fileName);
#endif

	std::ifstream file(path);
	if (!file)// 查看图片是否存在
	{
		//TODO 抛出异常
		return false;
	}
	file.close();
	//文件是否为图片等，应由上层调用时进行判断


	delete[] this->m_meta->pixelData;

	// 加载图片获取宽、高、颜色通道信息
	this->m_meta->pixelData = stbi_load(path.c_str(), &this->m_meta->width, &this->m_meta->height, &this->m_meta->channels, 0);


	switch (this->m_meta->channels)
	{
	case RGB_CHANNELS:
		this->m_meta->format = ColorFormat::Format_RGB888;
		break;

	case RGBA_CHANNELS:
		this->m_meta->format = ColorFormat::Format_RGBA8888;
		break;

	case GRAYSCALE_CHANNELS:
		this->m_meta->format = ColorFormat::Format_Grayscale8;
		break;

	default:
		// TODO: 抛出异常
		utils::LOG_ERROR("通道数目不存在");
		delete[] this->m_meta->pixelData;
		this->m_meta->pixelData = nullptr;
		break;
	}
	return this->isOK();
}

bool utils::ximage::loadFromMemory(const unsigned char* data, int width, int height, ColorFormat format, bool beenAlignMemory) const
{
	if (data == nullptr)
	{
		return false;
	}
	//TODO 其他合法性检查
	switch (format)
	{
	case ColorFormat::Format_RGB888:
		this->m_meta->channels = RGB_CHANNELS;
		break;
	case ColorFormat::Format_RGBA8888:
		this->m_meta->channels = RGBA_CHANNELS;
		break;
	case ColorFormat::Format_Grayscale8:
		this->m_meta->channels = GRAYSCALE_CHANNELS;
		break;
	// case ColorFormat::Format_Grayscale1:
	// 	return upTo8Bit(pixelData, width, height);
	// 	break;
	default:
		return false;
	}
	this->m_meta->width = width;
	this->m_meta->height = height;
	this->m_meta->format = format;

	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];
	if (!beenAlignMemory)//外部data不是内存对齐的
	{
		memcpy(this->m_meta->pixelData, data, width * this->m_meta->height * this->m_meta->channels);
	}
	else
	{
		//如果外部data经过了内存对齐，不能直接拷贝
		//需要按边界逐像素拷贝
		pixelCopy(data);
	}

	return this->isOK();
}

bool utils::ximage::loadFromMemory(const unsigned char* rgb, const unsigned char* alpha, int width, int height, bool beenAlignMemory) const
{
	if (rgb == nullptr)
	{
		return false;
	}
	this->m_meta->width = width;
	this->m_meta->height = height;

	if (alpha == nullptr)
	{
		this->m_meta->channels = RGB_CHANNELS;
		this->m_meta->format = ColorFormat::Format_RGB888;
		if (this->m_meta->pixelData)
		{
			delete[] this->m_meta->pixelData;
			this->m_meta->pixelData = nullptr;
		}
		this->m_meta->pixelData = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];
		if (!beenAlignMemory)//外部data不是内存对齐的
		{
			memcpy(this->m_meta->pixelData, rgb, width * this->m_meta->height * this->m_meta->channels);
		}
		else
		{
			//如果外部data经过了内存对齐，不能直接拷贝
			//需要按边界逐像素拷贝
			pixelCopy(rgb);
		}
	}
	else
	{
		this->m_meta->channels = RGBA_CHANNELS;
		this->m_meta->format = ColorFormat::Format_RGBA8888;
		if (this->m_meta->pixelData)
		{
			delete[] this->m_meta->pixelData;
			this->m_meta->pixelData = nullptr;
		}
		this->m_meta->pixelData = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];

		// rgb和alpha合成data
		try
		{
			for (int y = 0; y < this->m_meta->height; ++y)
			{
				for (int x = 0; x < this->m_meta->width; ++x)
				{
					int offsetOrigin = y * this->m_meta->width * this->m_meta->channels + x * this->m_meta->channels;// 控制原始数据读取时偏移
					int offsetRGB = y * this->m_meta->width * RGB_CHANNELS + x * RGB_CHANNELS;// 控制RGB data块读取时偏移
					int offsetAlpha = y * this->m_meta->width + x;
					{
						this->m_meta->pixelData[offsetOrigin] = rgb[offsetRGB];
						this->m_meta->pixelData[offsetOrigin + 1] = rgb[offsetRGB + 1];
						this->m_meta->pixelData[offsetOrigin + 2] = rgb[offsetRGB + 2];
						this->m_meta->pixelData[offsetOrigin + RGB_CHANNELS] = alpha[offsetAlpha];
					}
				}
			}
		}
		catch (const std::out_of_range& e)
		{
			utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
			return false;
		}
	}
	return this->isOK();
}

bool utils::ximage::loadFormBase64(const std::string& base64String) const
{
	std::string decodeStr = base64_decode(base64String);
	unsigned char* pixelData = new unsigned char[decodeStr.size()];
	memcpy(pixelData, decodeStr.c_str(), decodeStr.size());

	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = stbi_load_from_memory(pixelData, decodeStr.size(), &this->m_meta->width, &this->m_meta->height, &this->m_meta->channels, 0);

	delete[] pixelData;
	switch (this->m_meta->channels)
	{
	case RGB_CHANNELS:
		this->m_meta->format = ColorFormat::Format_RGB888;
		break;
	case RGBA_CHANNELS:
		this->m_meta->format = ColorFormat::Format_RGBA8888;
		break;
	case GRAYSCALE_CHANNELS:
		this->m_meta->format = ColorFormat::Format_Grayscale8;
		break;
	default:
		utils::LOG_ERROR("channels is not available");
		return false;
	}

	return true;
}

std::string utils::ximage::switchToBase64(FileFormat fileFormat) const
{
	if (!isOK())
	{
		utils::LOG_WARN("cedImage is empty, switch to base64 failed");
		return {};
	}

	std::vector<unsigned char> fileData = saveAsMemoryFile(fileFormat);

	if (fileData.empty())
	{
		utils::LOG_ERROR("base64转换失败");
		return {};
	}
	std::string str64 = base64_encode(fileData.data(), fileData.size());
	return str64;
}

bool utils::ximage::isOK() const
{
	if (!this->m_meta || !this->m_meta->pixelData)
	{
		utils::LOG_ERROR("cedImage is not OK");
		return false;
	}
	//TODO 其他的数据合法性判断以及异常的抛出
	return true;
}

int utils::ximage::getWidth() const
{
	return this->m_meta->width;
}

int utils::ximage::getHeight() const
{
	return this->m_meta->height;
}

int utils::ximage::getDataSize() const
{
	return this->m_meta->width * this->m_meta->height * this->m_meta->channels;
}

int utils::ximage::getBytesPerLine() const
{
	int bytesOffset = (4 - ((this->m_meta->width * this->m_meta->channels) % 4)) % 4;// 需要进行内存对齐
	return this->m_meta->width * this->m_meta->channels + bytesOffset;
}

int utils::ximage::getChannels() const
{
	return this->m_meta->channels;
}

utils::ximage::ColorFormat utils::ximage::getFormat() const
{
	return this->m_meta->format;
}

unsigned char* utils::ximage::getData() const
{
	unsigned char* data = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];
	memcpy(data, this->m_meta->pixelData, this->m_meta->width * this->m_meta->height * this->m_meta->channels);
	return data;
}

// utils::XImage::MetaData* utils::XImage::getMeta() const
// {
// 	return this->m_meta;
// }


unsigned char* utils::ximage::getRGB() const
{
	if (this->m_meta->channels < RGB_CHANNELS)
	{
		//TODO 通道数小于3不能获取RGB
		return nullptr;
	}
	//utils::LOG_INFO("获取RGB数据");
	if (!isOK())
	{
		utils::LOG_ERROR("cedImage初始化失败，不可用");
		return nullptr;// 外界需要判空
	}
	unsigned char* dataRgb = new unsigned char[this->m_meta->width * this->m_meta->height * RGB_CHANNELS];

	if (this->m_meta->channels == RGB_CHANNELS)// 如果通道数位3，直接返回深拷贝之后的rgb数据
	{
		memcpy(dataRgb, this->m_meta->pixelData, this->m_meta->width * this->m_meta->height * this->m_meta->channels);
		return dataRgb;
	}
	try
	{
		for (int y = 0; y < this->m_meta->height; ++y)
		{
			for (int x = 0; x < this->m_meta->width; ++x)
			{
				int offsetOrigin = y * this->m_meta->width * this->m_meta->channels + x * this->m_meta->channels;// 控制原始数据读取时偏移
				int offsetRGB = y * this->m_meta->width * RGB_CHANNELS + x * RGB_CHANNELS;// 控制RGB data块读取时偏移
				{
					dataRgb[offsetRGB] = this->m_meta->pixelData[offsetOrigin];
					dataRgb[offsetRGB + 1] = this->m_meta->pixelData[offsetOrigin + 1];
					dataRgb[offsetRGB + 2] = this->m_meta->pixelData[offsetOrigin + 2];
				}
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
		return nullptr;
	}

	return dataRgb;
}

unsigned char* utils::ximage::getAlpha() const
{
	if (this->m_meta->channels < RGB_CHANNELS)
	{
		//TODO 通道数小于3不能获取透明度
		return nullptr;
	}
	utils::LOG_INFO("获取Alpha数据");
	if (!isOK())
	{
		utils::LOG_ERROR("cedImage初始化失败，不可用");
		return nullptr;// 外界需要判空
	}
	unsigned char* dataAlpha = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];

	if (this->m_meta->channels == RGB_CHANNELS)
	{
		memset(dataAlpha, ALPHA_VAL_DEFAULT, this->m_meta->width * this->m_meta->height * this->m_meta->channels);
		return dataAlpha;
	}
	try
	{
		for (int y = 0; y < this->m_meta->height; ++y)
		{
			for (int x = 0; x < this->m_meta->width; ++x)
			{
				int offsetOrigin = y * this->m_meta->width * this->m_meta->channels + x * this->m_meta->channels;
				int offsetAlpha = y * this->m_meta->width + x;
				dataAlpha[offsetAlpha] = this->m_meta->pixelData[offsetOrigin + RGB_CHANNELS];

			}
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
		return nullptr;
	}
	return dataAlpha;
}

bool utils::ximage::saveAsFile(const char* fileName) const
{
	//目前没有将内部图像类保存在本地的需求，一般都是上下游的图片自己保存到本地
	std::string path(fileName);
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}

	utils::StringUtils::toLowerStr(path);// 转小写
	if (path.find(".png") != std::string::npos)
	{
		return stbi_write_png(fileName, this->m_meta->width, this->m_meta->height, this->m_meta->channels, this->m_meta->pixelData, 0);
	}
	else if (path.find(".jpg") != std::string::npos)
	{
		return stbi_write_jpg(fileName, this->m_meta->width, this->m_meta->height, this->m_meta->channels, this->m_meta->pixelData, 100);
	}
	else if (path.find(".bmp") != std::string::npos)
	{
		return stbi_write_bmp(fileName, this->m_meta->width, this->m_meta->height, 1, this->m_meta->pixelData);
	}
	return false;
}

std::vector<unsigned char> utils::ximage::saveAsMemoryFile(FileFormat fileFormat) const
{
	if (!isOK())
	{
		utils::LOG_WARN("cedImage is empty, switch to base64 failed");
		return {};
	}

	switch (fileFormat)
	{
	case FileFormat::Format_PNG: {
		int len;
		unsigned char* fullData = stbi_write_png_to_mem(this->m_meta->pixelData, 0, this->m_meta->width, this->m_meta->height, this->m_meta->channels, &len);
		if (fullData == nullptr)
		{
			LOG_ERROR("base64转换png失败");
			return {};
		}
		std::vector<unsigned char> fileData(fullData, fullData + len);
		delete[] fullData;
		return fileData;
		break;
	}
	case FileFormat::Format_JPG: {
		std::vector<unsigned char> fileData;
		fileData.clear();
		if (!stbi_write_jpg_to_func(memoryWrite,
		                            &fileData,
		                            this->m_meta->width,
		                            this->m_meta->height,
		                            this->m_meta->channels,
		                            this->m_meta->pixelData,
		                            100))
		{
			utils::LOG_ERROR("base64转换jpg失败");
			return {};
		}
		return fileData;
		break;
	}
	case FileFormat::Format_BMP: {
		std::vector<unsigned char> fileData;
		fileData.clear();
		if (!stbi_write_bmp_to_func(memoryWrite,
		                            &fileData,
		                            this->m_meta->width,
		                            this->m_meta->height,
		                            this->m_meta->channels,
		                            this->m_meta->pixelData))
		{
			utils::LOG_ERROR("base64转换bmp失败");
			return {};
		}
		return fileData;
		break;
	}
	}
	utils::LOG_ERROR("图片保存没有匹配到合适的图片类型");
	return {};
}

bool utils::ximage::grayscaleUpToRGB() const
{
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (this->m_meta->format != ColorFormat::Format_Grayscale8)
	{
		utils::LOG_ERROR("该方法只支持8位灰度图提升为RGB888");
		return false;
	}
	unsigned char* newPixelData = new unsigned char[this->m_meta->width * this->m_meta->height * RGB_CHANNELS];
	int offset = 0;
	try
	{
		for (int i = 0; i < this->m_meta->width * this->m_meta->height; ++i)
		{
			newPixelData[offset] = this->m_meta->pixelData[i];
			newPixelData[offset + 1] = this->m_meta->pixelData[i];
			newPixelData[offset + 2] = this->m_meta->pixelData[i];
			offset += 3;
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
		return false;
	}

	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = newPixelData;
	this->m_meta->channels = RGB_CHANNELS;
	this->m_meta->format = ColorFormat::Format_RGB888;

	return true;
}

bool utils::ximage::scaleProportional(int scaleFactor)
{
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (scaleFactor <= 1)
	{
		utils::LOG_ERROR("放大倍数小于1");
		return false;
	}
	int newWidth = this->m_meta->width * scaleFactor;
	int newHeight = this->m_meta->height * scaleFactor;
	// unsigned char* newPixelData = new unsigned char[newWidth * newHeight * this->m_meta->channels];
	// for (int y = 0; y < newWidth; ++y)
	// {
	// 	for (int x = 0; x < newHeight; ++x)
	// 	{
	// 		// 计算源图像中的对应像素位置
	// 		int srcX = std::floor(x / scaleFactor);
	// 		int srcY = std::floor(y / scaleFactor);
	//
	// 		int offsetOrigin = (y * newWidth + x) * 3;
	// 		int offsetScaled = (srcY * this->m_meta->width + srcX) * 3;
	// 		newPixelData[offsetOrigin] = this->m_meta->pixelData[offsetScaled];
	// 		newPixelData[offsetOrigin + 1] = this->m_meta->pixelData[offsetScaled + 1];
	// 		newPixelData[offsetOrigin + 2] = this->m_meta->pixelData[offsetScaled + 2];
	// 		// 下面这样就不用判断了
	// 		newPixelData[offsetOrigin + this->m_meta->channels - 1] = this->m_meta->pixelData[offsetScaled + this->m_meta->channels - 1];
	// 	}
	// }
	// delete[] this->m_meta->pixelData;
	// this->m_meta->pixelData = newPixelData;
	// this->m_meta->width = newWidth;
	// this->m_meta->height = newHeight;
	//
	// return true;

	return this->scale(newWidth, newHeight);
}

bool utils::ximage::scale(double scaleFactor)
{
	//3通道默认RGB，4通道默认RGBA
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (scaleFactor <= 0)
	{
		utils::LOG_ERROR("缩放倍数小于等于0");
		return false;
	}
	int newWidth = static_cast<int>(this->m_meta->width * scaleFactor);
	int newHeight = static_cast<int>(this->m_meta->height * scaleFactor);

	return this->scale(newWidth, newHeight);
}

bool utils::ximage::scale(double scaleFactorX, double scaleFactorY)
{

	//3通道默认RGB，4通道默认RGBA
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (scaleFactorX <= 0 || scaleFactorY <= 0)
	{
		utils::LOG_ERROR("缩放倍数小于等于0");
		return false;
	}
	int newWidth = static_cast<int>(this->m_meta->width * scaleFactorX);
	int newHeight = static_cast<int>(this->m_meta->height * scaleFactorY);

	return this->scale(newWidth, newHeight);
}

bool utils::ximage::scale(int newWidth, int newHeight)
{
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (newWidth <= 0 || newHeight <= 0)
	{
		utils::LOG_ERROR("缩放后长或宽小于等于0");
		return false;
	}

	stbir_filter filter = STBIR_FILTER_DEFAULT;
	float scaleFactorX = static_cast<float>(newWidth) / static_cast<float>(this->m_meta->width);
	float scaleFactorY = static_cast<float>(newHeight) / static_cast<float>(this->m_meta->height);
	if (scaleFactorX == scaleFactorY && std::fmod(scaleFactorX, 1) == 0)
	//如果是整体整数倍缩放，则使用插值而不是默认的平滑
	{
		filter = STBIR_FILTER_BOX;
	}

	unsigned char* newPixelData = new unsigned char[newWidth * newHeight * this->m_meta->channels];
	if (
		stbir_resize(this->m_meta->pixelData,
		             this->m_meta->width,
		             this->m_meta->height,
		             0,
		             newPixelData,
		             newWidth,
		             newHeight,
		             0,
		             this->m_meta->channels == 3 ? stbir_pixel_layout::STBIR_RGB : stbir_pixel_layout::STBIR_RGBA,
		             STBIR_TYPE_UINT8,
		             STBIR_EDGE_CLAMP,
		             filter)//三次滤波器，使用三次多项式进行插值
		== nullptr
	)
	{
		delete[] newPixelData;
		newPixelData = nullptr;
		utils::LOG_ERROR("缩放失败");
		return false;
	}

	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = newPixelData;
	this->m_meta->width = newWidth;
	this->m_meta->height = newHeight;
	return true;
}

bool utils::ximage::upQuality()
{
	unsigned char* newPixelData = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];
	if (
		stbir_resize(this->m_meta->pixelData,
		             this->m_meta->width,
		             this->m_meta->height,
		             0,
		             newPixelData,
		             this->m_meta->width,
		             this->m_meta->height,
		             0,
		             this->m_meta->channels == 3 ? stbir_pixel_layout::STBIR_RGB : stbir_pixel_layout::STBIR_RGBA,
		             STBIR_TYPE_UINT8,
		             STBIR_EDGE_CLAMP,
		             STBIR_FILTER_DEFAULT)//默认是STBIR_FILTER_CATMULLROM三次滤波器，使用三次多项式进行插值
		== nullptr
	)
	{
		delete[] newPixelData;
		newPixelData = nullptr;
		utils::LOG_ERROR("缩放失败");
		return false;
	}

	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = newPixelData;
	return true;
}

bool utils::ximage::changeOpacity(float opacityFactor)
{
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (this->m_meta->channels < RGB_CHANNELS)// 只有RGB或RGBA才能增加/调整透明度
	{
		utils::LOG_ERROR("该图片类型不支持调整透明度");
		return false;
	}
	if (opacityFactor < 0.0f || opacityFactor > 1.0f)
	{
		utils::LOG_ERROR("透明度因子小于0或大于1");
		return false;
	}
	unsigned char alpha = static_cast<unsigned char>(static_cast<int>(opacityFactor * 255));
	if (alpha < 0 || alpha > 255)
	{
		utils::LOG_ERROR("透明度因子转换后小于0或大于255");
		return false;
	}

	if (this->m_meta->channels == RGB_CHANNELS)
	{
		return this->RGBToRGBA(alpha);
	}
	else
	{
		try
		{
			for (int y = 0; y < this->m_meta->height; ++y)
			{
				for (int x = 0; x < this->m_meta->width; ++x)
				{
					int offsetRGBA = y * this->m_meta->width * RGBA_CHANNELS + x * RGBA_CHANNELS;// 控制原始数据读取时偏移

					this->m_meta->pixelData[offsetRGBA + 3] = alpha;
				}
			}
		}
		catch (const std::out_of_range& e)
		{
			utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
			return false;
		}
		return true;
	}
}

bool utils::ximage::RGBToRGBA(const unsigned alpha)
{
	if (!this->isOK())
	{
		utils::LOG_ERROR("cedImage不可用");
		return false;
	}
	if (this->m_meta->channels != RGB_CHANNELS)// 只有RGB或RGBA才能增加/调整透明度
	{
		utils::LOG_ERROR("该图片类型不支持调整透明度");
		return false;
	}
	if (alpha < 0 || alpha > 255)
	{
		utils::LOG_ERROR("透明度因子转换后小于0或大于255");
		return false;
	}
	unsigned char* newPixelData = new unsigned char[this->m_meta->width * this->m_meta->height * RGBA_CHANNELS];
	try
	{
		for (int y = 0; y < this->m_meta->height; ++y)
		{
			for (int x = 0; x < this->m_meta->width; ++x)
			{
				int offsetRGB = y * this->m_meta->width * RGB_CHANNELS + x * RGB_CHANNELS;
				int offsetRGBA = y * this->m_meta->width * RGBA_CHANNELS + x * RGBA_CHANNELS;// 控制原始数据读取时偏移

				newPixelData[offsetRGBA] = this->m_meta->pixelData[offsetRGB];
				newPixelData[offsetRGBA + 1] = this->m_meta->pixelData[offsetRGB + 1];
				newPixelData[offsetRGBA + 2] = this->m_meta->pixelData[offsetRGB + 2];
				newPixelData[offsetRGBA + 3] = alpha;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
		delete[] newPixelData;
		return false;
	}
	delete[] this->m_meta->pixelData;
	this->m_meta->pixelData = newPixelData;
	this->m_meta->channels = RGBA_CHANNELS;
	this->m_meta->format = ColorFormat::Format_RGBA8888;
	return true;
}

void utils::ximage::setReWitdh(int width)
{
	this->m_meta->reWitdh = width;
}

void utils::ximage::setReHeight(int height)
{
	this->m_meta->reHeight = height;
}

int utils::ximage::getReWidth() const
{
	return this->m_meta->reWitdh;
}

int utils::ximage::getReHeight() const
{
	return this->m_meta->reHeight;
}


void utils::ximage::pixelCopy(const unsigned char* data) const
{
	try
	{
		for (int y = 0; y < this->m_meta->height; ++y)
		{
			for (int x = 0; x < this->m_meta->width; ++x)
			{
				int offsetOrigin = y * this->m_meta->width * this->m_meta->channels + x * this->m_meta->channels;// 控制原始数据读取时偏移
				this->m_meta->pixelData[offsetOrigin] = data[offsetOrigin];
				if (this->m_meta->channels >= RGB_CHANNELS)
				{
					this->m_meta->pixelData[offsetOrigin + 1] = data[offsetOrigin + 1];
					this->m_meta->pixelData[offsetOrigin + 2] = data[offsetOrigin + 2];
					if (this->m_meta->channels == RGBA_CHANNELS)
					{
						this->m_meta->pixelData[offsetOrigin + 3] = data[offsetOrigin + 3];
					}
				}
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		utils::LOG_ERROR("访问数组越界：", e.what());// 输出越界信息
	}
}

void utils::ximage::deepCopy(const ximage& other)
{
	if (other.m_meta != nullptr)
	{
		this->m_meta = new MetaData();
		this->m_meta->width = other.m_meta->width;
		this->m_meta->height = other.m_meta->height;
		this->m_meta->channels = other.m_meta->channels;
		this->m_meta->format = other.m_meta->format;
		this->m_meta->pixelData = new unsigned char[this->m_meta->width * this->m_meta->height * this->m_meta->channels];
		if (other.m_meta->pixelData != nullptr)
		{
			memcpy(this->m_meta->pixelData, other.m_meta->pixelData, this->m_meta->width * this->m_meta->height * this->m_meta->channels);
		}
	}
}

void utils::ximage::memoryWrite(void* context, void* data, int size)
{
	std::vector<unsigned char>* ctx = static_cast<std::vector<unsigned char>*>(context);
	ctx->insert(ctx->end(), static_cast<unsigned char*>(data), static_cast<unsigned char*>(data) + size);
}

// bool utils::XImage::grayscale8ToRGB(const unsigned char* pixelData, int width, int height) const
// {
// 	if (!pixelData)
// 	{
// 		return false;
// 	}
// 	if (this->m_meta->pixelData)
// 	{
// 		delete[] this->m_meta->pixelData;
// 	}
//
// 	this->m_meta->width = width * 3;
// 	this->m_meta->height = height * 3;
// 	this->m_meta->channels = RGB_CHANNELS;
// 	this->m_meta->pixelData = new unsigned char[this->m_meta->width * this->m_meta->height * RGB_CHANNELS];
//
// 	// int test = 0;
// 	// for (int i = 0; i < width * height; ++i)
// 	// {
// 	// 	this->m_meta->pixelData[test] = pixelData[i] == 0 ? 255 : 0;
// 	// 	this->m_meta->pixelData[test + 1] = pixelData[i] == 255 ? 255 : 0;
// 	// 	this->m_meta->pixelData[test + 2] = pixelData[i] == 255 ? 255 : 0;
// 	// 	test += 3;
// 	// }
//
//
// 	// 将二维码矩阵转换为图像数据
// 	for (int y = 0; y < width; ++y)
// 	{
// 		for (int x = 0; x < width; ++x)
// 		{
// 			if (pixelData[width * y + x])
// 			{
// 				// 黑色像素
// 				this->m_meta->pixelData[(y * width + x) * 3] = 255;// R
// 				this->m_meta->pixelData[(y * width + x) * 3 + 1] = 255;// G
// 				this->m_meta->pixelData[(y * width + x) * 3 + 2] = 255;// B
// 				// this->m_meta->pixelData[(y * width + x) * 4 + 3] = 255;// A
// 			}
// 			else
// 			{
// 				// 白色像素
// 				this->m_meta->pixelData[(y * width + x) * 3] = 0;
// 				this->m_meta->pixelData[(y * width + x) * 3 + 1] = 0;
// 				this->m_meta->pixelData[(y * width + x) * 3 + 2] = 0;
// 				// this->m_meta->pixelData[(y * width + x) * 3 + 3] = 255;
// 			}
// 		}
// 	}
// 	return this->isOK();
// }


// bool utils::XImage::upTo8Bit(const unsigned char* data, int width, int height) const
// {
// 	if (width <= 0 || height <= 0)
// 	{
// 		utils::LOG_ERROR("width or height is <= 0");
// 		return false;
// 	}
//
// 	if (this->m_meta->pixelData)
// 	{
// 		delete[] this->m_meta->pixelData;
// 		this->m_meta->pixelData = nullptr;
// 	}
// 	this->m_meta->pixelData = new unsigned char[width * height];
//
// 	for (int y = 0; y < width; ++y)
// 	{
// 		for (int x = 0; x < width; ++x)
// 		{
// 			// 从1位位图数据中提取第x个像素的值
// 			// unsigned char pixel = pixelData[y * width + x / 8] & (1 << (7 - (x % 8))) ? 1 : 0;
//
// 			// 将1位像素值转换为8位像素值
// 			this->m_meta->pixelData[y * width + x] = data[y * width + x ] & 0x00000001 ? 0 : 255;
// 		}
// 	}
//
// 	// // 计算每行字节数（libqrencode数据紧凑存储）
// 	// int srcStride = (width + 7) / 8;  // 无需BMP的4字节对齐
// 	// int dstStride = width;
// 	//
// 	// for (int y = 0; y < height; ++y) {
// 	// 	const uint8_t* srcRow = &pixelData[y * srcStride];
// 	// 	uint8_t* dstRow = &this->m_meta->pixelData[y * dstStride];
// 	//
// 	// 	for (int x = 0; x < width; ++x) {
// 	// 		// 提取位值（高位优先）
// 	// 		int byteIdx = x / 8;
// 	// 		int bitIdx = 7 - (x % 8);
// 	// 		uint8_t bitVal = (srcRow[byteIdx] >> bitIdx) & 0x01;
// 	//
// 	// 		// 映射到8位灰度：0→255（白），1→0（黑）
// 	// 		dstRow[x] = (bitVal == 0) ? 255 : 0;
// 	// 	}
// 	// }
//
//
// 	if (!this->isOK())
// 	{
// 		//TODO
// 		return false;
// 	}
//
// 	this->m_meta->width = width;
// 	this->m_meta->height = height;
// 	this->m_meta->channels = GRAYSCALE_CHANNELS;
// 	this->m_meta->format = ColorFormat::Format_Grayscale8;
//
// 	return true;
// }
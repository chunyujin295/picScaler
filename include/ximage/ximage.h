#ifndef CEDIMAGE_H
#define CEDIMAGE_H

#define GRAYSCALE_CHANNELS 1
#define RGB_CHANNELS 3
#define RGBA_CHANNELS 4
#define ALPHA_VAL_DEFAULT 255
#define BITS_PER_CHANNELS 8
#include <vector>
#include <ximage/xcolor.h>

namespace utils
{
	class ximage
	{
	public:
		enum class ColorFormat// 表示图片的色彩模式
		{
			Format_RGB888,
			Format_RGBA8888,
			Format_Grayscale8,// 单通道灰度，8位/像素（1字节）
			// Format_Grayscale1// 单通道灰度，1bit/像素
		};

		enum class FileFormat// 表示图片保存例如转base64时的文件格式
		{
			Format_PNG,
			Format_JPG,
			Format_BMP
		};

		struct MetaData
		{
			int width = 0;
			int height = 0;
			int channels = 0;
			unsigned char* pixelData = nullptr;
			ColorFormat format = ColorFormat::Format_RGB888;
			// 存储预期缩放的宽度
			int reWitdh = 0;
			// 存储预期缩放的高度
			int reHeight = 0;
		};

	public:
		/**
		 * 无参构造
		 */
		ximage();


		/**
		 * 有参构造，用来生成默认纯色图片
		 * @param width
		 * @param height
		 * @param color
		 * @param format
		 */
		ximage(int width, int height, xcolor color = xcolor(CED_COLOR_WHITE), ColorFormat format = ColorFormat::Format_RGB888);


		/**
		 * 有参构造
		 * @param data
		 * @param width
		 * @param height
		 * @param format
		 * @param beenAlignMemory
		 */
		ximage(const unsigned char* data, int width, int height, ColorFormat format, bool beenAlignMemory = false);


		/**
		 * 有参构造
		 * @param rgb
		 * @param alpha
		 * @param width
		 * @param height
		 * @param beenAlignMemory
		 */
		ximage(const unsigned char* rgb, const unsigned char* alpha, int width, int height, bool beenAlignMemory = false);


		/**
		 * 有参构造，从图片文件中创建CedImage
		 * @param fileName
		 */
		explicit ximage(const char* fileName);

		/**
		 * 有参构造，从base64创建CedImage
		 * @param base64Data
		 */
		ximage(const std::string& base64Data);

		/**
		 * 拷贝构造
		 * @param other
		 */
		ximage(const ximage& other);

		/**
		 * 移动构造
		 * @param other
		 */
		ximage(ximage&& other) noexcept;

		/**
		 * 赋值构造
		 * @param other
		 */
		explicit ximage(const ximage* other);


		/**
		 * 赋值运算符重载
		 * @param other
		 * @return
		 */
		ximage& operator=(const ximage& other);

		/**
		 * 赋值运算符重载(移动构造)
		 * @param other 赋值
		 * @return
		 */
		ximage& operator=(ximage&& other) noexcept;

		/**
		 * ==运算符重载
		 * @param other 赋值
		 * @return
		 */
		bool operator==(const ximage& other) const;

		~ximage();

		/**
		 * 清空内容
		 * @return
		 */
		void clear();
		/**
		 * 加载本地图片文件
		 * @param fileName 图片文件路径
		 * @return 函数执行是否成功
		 */
		bool loadFromFile(const char* fileName) const;

		/**
		 * 加载内存图片，场景包含:QImage->cedImage
		 * @param data 图片像素原始数据
		 * @param width 图片宽度，单位：像素
		 * @param height 图片高度，单位：像素
		 * @param format 图片格式
		 * @param beenAlignMemory 像素数据是否进行过内存对齐
		 * @return
		 */
		bool loadFromMemory(const unsigned char* data, int width, int height, ColorFormat format, bool beenAlignMemory = false) const;

		/**
		 * 加载内存图片, 场景:wxImage->cedImage
		 * @param rgb 图片像素rgb数据
		 * @param alpha 图片像素透明度数据
		 * @param width 图片宽度，单位：像素
		 * @param height 图片高度，单位：像素
		 * @param beenAlignMemory 像素数据是否经过了内存对齐
		 * @return
		 */
		bool loadFromMemory(const unsigned char* rgb, const unsigned char* alpha, int width, int height, bool beenAlignMemory = false) const;

		/**
		 * 从base64加载，注意，该base64头格式为内部自定义，CedImage能解析由自己生成的base64，因此只允许在内部进行流通使用
		 * @param base64Data base64字符串
		 * @return 函数执行是否成功
		 */
		bool loadFormBase64(const std::string& base64Data) const;

		/**
		 * CedImage转base64，注意，该base64头格式为内部自定义，仅CedImage能解析，因此只允许在内部进行流通使用
		 * @return
		 */
		std::string switchToBase64(FileFormat fileFormat  = FileFormat::Format_PNG) const;

		/**
		 * 判断CedImage是否有效
		 * @return
		 */
		bool isOK() const;

		/**
		 * 获取图片宽度
		 * @return
		 */
		int getWidth() const;

		/**
		 * 获取图片高度
		 * @return
		 */
		int getHeight() const;

		/**
		 * 获取图片像素data所占字节总大小
		 * @return
		 */
		int getDataSize() const;

		/**
		 * 获取图片每行所占像素数，这是经过内存对齐之后的结果，适用于构建QImage
		 * @return
		 */
		int getBytesPerLine() const;

		/**
		 * 获取深度/通道数
		 * @return
		 */
		int getChannels() const;

		/**
		 * 获取色彩模式
		 * @return
		 */
		ColorFormat getFormat() const;

		/**
		 * 获取像素数据(返回的结果是深拷贝的)
		 * @return
		 */
		unsigned char* getData() const;

		/**
		 * 返回CedImage的元数据(返回原始指针，适用于外部想要修改CedImage内部数据时调用)
		 * 已禁用，防止类外delete
		 * @return
		 */
		// MetaData* getMeta() const;

		//不需要set方法，因为想要修改内容的时候，再走一遍loadFormMemeory就行

		/**
		 * 像素数据拆解成为RGB + Alpha 用来构建wxImage
		 * @return
		 */
		unsigned char* getRGB() const;
		//若原像素数据中不含alpha，则返回一个值全为255的数组
		unsigned char* getAlpha() const;

		// std::vector<unsigned char*> unpackToRGBandAlpha();

		// void switchFormat(ColorFormat format);

		/**
		 * 保存到本地，支持导出为png, jpg, bmp
		 * 目前没有将内部图像类保存在本地的需求，一般都是上下游的图片自己保存到本地
		 * @param fileName 图片路径
		 * @return
		 */
		bool saveAsFile(const char* fileName) const;

		/**
		 * 将CedImage封装成完整的内存图片文件
		 * @param fileFormat
		 * @return
		 */
		std::vector<unsigned char> saveAsMemoryFile(FileFormat fileFormat = FileFormat::Format_PNG) const;


		/**
		 * 8位灰度图提升为RGB888
		 * @return
		 */
		bool grayscaleUpToRGB() const;

		/**
		 * 按比例放大(整数倍)
		 * 该方法主要用于二维码和条形码确保不失真的放大
		 * @return
		 */
		bool scaleProportional(int scaleFactor);

		/**
		 * 等比例缩放
		 * @return
		 */
		bool scale(double scaleFactor);

		/**
		 * 按照指定长宽缩放比缩放
		 * @param scaleFactorX
		 * @param scaleFactorY
		 * @return
		 */
		bool scale(double scaleFactorX, double scaleFactorY);

		/**
		 * 按照指定宽高缩放
		 * @param newWidth
		 * @param newHeight
		 * @return
		 */
		bool scale(int newWidth, int newHeight);

		/**
		 * 提升图片质量，三次滤波器，使用三次多项式进行插值
		 */
		bool upQuality();


		/**
		 * 修改图片透明度
		 * @param opacityFactor 图片透明度系数，0.0~1.0
		 * @return
		 */
		bool changeOpacity(float opacityFactor);

		/**
		 * RGB提升为RGBA
		 * @param alpha 提升后的透明度
		 * @return
		 */
		bool RGBToRGBA(unsigned alpha = 255);

		/**
		 * RGB提升为RGBA
		 * @param alphaList 提升后的透明度列表
		 * @return
		 */
		// bool RGBTORGBA(unsigned char* alphaList);

		void setReWitdh(int width);

		void setReHeight(int height);

		int getReWidth() const;

		int getReHeight() const;

	private:
		/**
		 * 双重for循环进行拷贝，避免原数据经过了内存对齐
		 * @param data 源数据
		 */
		void pixelCopy(const unsigned char* data) const;
		/**
		 * 深拷贝
		 * @param other 源CedImage
		 */
		void deepCopy(const ximage& other);

		/**
		 * 用于适配stb_image将图片保存在内存上下文的方法
		 * @param context
		 * @param data
		 * @param size
		 */
		static void memoryWrite(void* context, void* data, int size);

		/**
		 * 确保内存对齐，主要用于CedImage返回给外部自己的data的时候，进行内存对齐
		 * 目前该方法已被分解到其他方法内部了
		 * @param data 外部来的原始数据
		 */
		// void ensureMemoryAlignment(unsigned char* pixelData);

		/**
		 * 灰度图转RGB
		 * @param data
		 * @param width
		 * @param height
		 * @return
		 */
		// bool grayscale8ToRGB(const unsigned char* pixelData, int width, int height) const;


		/**
		 * 将1bit深度转8bit
		 * @param data
		 * @return
		 */
		// bool upTo8Bit(const unsigned char* pixelData, int width, int height) const;

		/**
		 * swap用于内存交换
		 * @param other
		 */
		// void swap(XImage& other);

	private:
		// 图像元数据信息
		MetaData* m_meta = nullptr;
	};
}

#endif //CEDIMAGE_H
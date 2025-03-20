#ifndef EDITORCONTROLV5_CEDCOLOR_H
#define EDITORCONTROLV5_CEDCOLOR_H

#include <string>
#include <unordered_map>
#include <ximage/cedcolorconstants.h>

namespace utils
{
	// 颜色的alpha值的默认值
	constexpr unsigned char CED_COLOR_ALPHA_OPAQUE = 0xff;
	// 颜色分量最大值
	constexpr unsigned char CED_COLOR_MAX_COMPONENT = 0xff;
	// 颜色分量最小值
	constexpr unsigned char CED_COLOR_MIN_COMPONENT = 0x00;
	// 用于表示8位的位移
	constexpr unsigned char CED_COLOR_SHIFT_8_BITS = 8;
	// 用于表示16位的位移
	constexpr unsigned char CED_COLOR_SHIFT_16_BITS = 16;
	// 用于表示24位的位移
	constexpr unsigned int CED_COLOR_SHIFT_24_BITS = 24;
	// 用于掩码操作，提取最低的8位。
	constexpr unsigned int CED_COLOR_MASK_8_BITS = 0xFF;

	class xcolor
	{
	public:
		// 默认构造函数,创建一个无效的颜色对象
		xcolor();

		/**
         * 颜色构造函数
         * @param red 红色值(0~255)
         * @param green 绿色值(0~255)
         * @param blue 蓝色值(0~255)
         * @param alpha alpha值(0~255)
         */
		xcolor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = CED_COLOR_ALPHA_OPAQUE);

		/**
         * 颜色构造函数
         * @param red 红色值(0~255)
         * @param green 绿色值(0~255)
         * @param blue 蓝色值(0~255)
         * @param alpha alpha值(0~255)
         */
		xcolor(int red, int green, int blue, int alpha = 255);

		/**
         * 拷贝构造函数
         * @param color
         */
		xcolor(const xcolor& color);

		/**
		 * 字符串构造颜色对象
		 * 支持16进制(A)RGB, 如FFFFFF，#ABABAB，#FF0000FF
		 * 支持颜色名称, 如Red，Black，Blue等
		 * 支持(A)RGB字符串, 如"123,210,213" "255,230,123,213"
		 * @param colorString
		 */
		xcolor(const std::string& colorString);

		/**
		 * 判断颜色是否有效
		 * @return
		 */
		bool isValid() const;

		/**
         * 获取红色值
         * @return 红色值(0~255)
         */
		unsigned char red() const;

		/**
         * 设置红色值
         * @param red 红色值
         */
		void setRed(unsigned char red);

		/**
         * 获取蓝色值
         * @return 蓝色值(0~255)
         */
		unsigned char blue() const;

		/**
         * 设置蓝色值
         * @param blue 蓝色值
         */
		void setBlue(unsigned char blue);

		/**
         * 获取绿色值
         * @return 绿色值
         */
		unsigned char green() const;

		/**
         * 设置绿色值
         * @param green 绿色值
         */
		void setGreen(unsigned char green);

		/**
         * 获取alpha值
         * @return alpha值
         */
		unsigned char alpha() const;

		/**
         * 设置alpha值
         * @param alpha alpha值
         */
		void setAlpha(unsigned char alpha);

		/**
         * 设置所有颜色值
         * @param red 红色值
         * @param green 绿色值
         * @param blue 蓝色值
         * @param alpha alpha值
         */
		void setAll(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

		/**
		 * 获取颜色的RGB
		 * @return 颜色的RGB(0x00BBGGRR)
		 */
		unsigned int getRGB() const;

		/**
		 * 通过设置RGB来设置颜色
		 * @param colRGB 要设置的RGB(0x00BBGGRR)
		 */
		void setRGB(unsigned int colRGB);

		/**
		 * 获取颜色的RGBA
		 * @return 颜色的RGBA(0xAABBGGRR)
		 */
		unsigned int getRGBA() const;

		/**
		 * 通过设置RGBA来设置颜色
		 * @param colRGBA 要设置的RGBA(0xAABBGGRR)
		 */
		void setRGBA(unsigned int colRGBA);

		/**
		 * 将颜色转换为16进制ARGB的字符串(例如 XColor(255,0,0,255) == "#FFFF0000")
		 * @return
		 */
		std::string getAsString() const;

//		/**
//		 * 获取颜色的感知亮度
//		 * @return
//		 */
//		double luminance() const;

		bool operator!=(const xcolor& color) const;

		xcolor& operator=(const xcolor& color);

		bool operator==(const xcolor& color) const;

	private:
		/**
		 * 16进制字符串转颜色
		 * @param hex 16进制颜色，如FFFFFF,#FFABABAB
		 * @return 是否成功转化
		 */
		bool hexToColor(const std::string& hex);

		/**
		 * 10进制字符串转颜色
		 * @param dec
		 * @return
		 */
		bool decToColor(const std::string& dec);
	private:
		// 颜色名称与ARGB的键值对集合
		static const std::unordered_map<std::string, std::string> m_colorNameToArgb;
		unsigned char m_red;
		unsigned char m_blue;
		unsigned char m_green;
		unsigned char m_alpha;
		// 标识颜色对象是否有效
		bool m_isValid;
	};
#define CED_WINDOW_COLOR
}// namespace utils


#endif//EDITORCONTROLV5_CEDCOLOR_H

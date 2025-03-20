/*************************************************
 * 描述: 编辑器颜色类实现
 *
 * File: cedcolor.cpp
 * Author: wangzihan@mozihealthcare.cn
 * Date: 2025-01-21
 * Update:
 * ************************************************/

#include <algorithm>
#include <ximage/xcolor.h>
#include <utils/globalmacro.h>
#include <iomanip>
#include <sstream>
#include <vector>

const std::unordered_map<std::string, std::string> utils::xcolor::m_colorNameToArgb = {
		{"Transparent", "#00FFFFFF"},
		{"AliceBlue", "#FFF0F8FF"},
		{"AntiqueWhite", "#FFFAEBD7"},
		{"Aqua", "#FF00FFFF"},
		{"Aquamarine", "#FF7FFFD4"},
		{"Azure", "#FFF0FFFF"},
		{"Beige", "#FFF5F5DC"},
		{"Bisque", "#FFFFE4C4"},
		{"Black", "#FF000000"},
		{"BlanchedAlmond", "#FFFFEBCD"},
		{"Blue", "#FF0000FF"},
		{"BlueViolet", "#FF8A2BE2"},
		{"Brown", "#FFA52A2A"},
		{"BurlyWood", "#FFDEB887"},
		{"CadetBlue", "#FF5F9EA0"},
		{"Chartreuse", "#FF7FFF00"},
		{"Chocolate", "#FFD2691E"},
		{"Coral", "#FFFF7F50"},
		{"CornflowerBlue", "#FF6495ED"},
		{"Cornsilk", "#FFFFF8DC"},
		{"Crimson", "#FFDC143C"},
		{"Cyan", "#FF00FFFF"},
		{"DarkBlue", "#FF00008B"},
		{"DarkCyan", "#FF008B8B"},
		{"DarkGoldenrod", "#FFB8860B"},
		{"DarkGray", "#FFA9A9A9"},
		{"DarkGreen", "#FF006400"},
		{"DarkKhaki", "#FFBDB76B"},
		{"DarkMagenta", "#FF8B008B"},
		{"DarkOliveGreen", "#FF556B2F"},
		{"DarkOrange", "#FFFF8C00"},
		{"DarkOrchid", "#FF9932CC"},
		{"DarkRed", "#FF8B0000"},
		{"DarkSalmon", "#FFE9967A"},
		{"DarkSeaGreen", "#FF8FBC8F"},
		{"DarkSlateBlue", "#FF483D8B"},
		{"DarkSlateGray", "#FF2F4F4F"},
		{"DarkTurquoise", "#FF00CED1"},
		{"DarkViolet", "#FF9400D3"},
		{"DeepPink", "#FFFF1493"},
		{"DeepSkyBlue", "#FF00BFFF"},
		{"DimGray", "#FF696969"},
		{"DodgerBlue", "#FF1E90FF"},
		{"Firebrick", "#FFB22222"},
		{"FloralWhite", "#FFFFFAF0"},
		{"ForestGreen", "#FF228B22"},
		{"Fuchsia", "#FFFF00FF"},
		{"Gainsboro", "#FFDCDCDC"},
		{"GhostWhite", "#FFF8F8FF"},
		{"Gold", "#FFFFD700"},
		{"Goldenrod", "#FFDAA520"},
		{"Gray", "#FF808080"},
		{"Green", "#FF008000"},
		{"GreenYellow", "#FFADFF2F"},
		{"Honeydew", "#FFF0FFF0"},
		{"HotPink", "#FFFF69B4"},
		{"IndianRed", "#FFCD5C5C"},
		{"Indigo", "#FF4B0082"},
		{"Ivory", "#FFFFFFF0"},
		{"Khaki", "#FFF0E68C"},
		{"Lavender", "#FFE6E6FA"},
		{"LavenderBlush", "#FFFFF0F5"},
		{"LawnGreen", "#FF7CFC00"},
		{"LemonChiffon", "#FFFFFACD"},
		{"LightBlue", "#FFADD8E6"},
		{"LightCoral", "#FFF08080"},
		{"LightCyan", "#FFE0FFFF"},
		{"LightGoldenrodYellow", "#FFFAFAD2"},
		{"LightGreen", "#FF90EE90"},
		{"LightGray", "#FFD3D3D3"},
		{"LightPink", "#FFFFB6C1"},
		{"LightSalmon", "#FFFFA07A"},
		{"LightSeaGreen", "#FF20B2AA"},
		{"LightSkyBlue", "#FF87CEFA"},
		{"LightSlateGray", "#FF778899"},
		{"LightSteelBlue", "#FFB0C4DE"},
		{"LightYellow", "#FFFFFFE0"},
		{"Lime", "#FF00FF00"},
		{"LimeGreen", "#FF32CD32"},
		{"Linen", "#FFFAF0E6"},
		{"Magenta", "#FFFF00FF"},
		{"Maroon", "#FF800000"},
		{"MediumAquamarine", "#FF66CDAA"},
		{"MediumBlue", "#FF0000CD"},
		{"MediumOrchid", "#FFBA55D3"},
		{"MediumPurple", "#FF9370DB"},
		{"MediumSeaGreen", "#FF3CB371"},
		{"MediumSlateBlue", "#FF7B68EE"},
		{"MediumSpringGreen", "#FF00FA9A"},
		{"MediumTurquoise", "#FF48D1CC"},
		{"MediumVioletRed", "#FFC71585"},
		{"MidnightBlue", "#FF191970"},
		{"MintCream", "#FFF5FFFA"},
		{"MistyRose", "#FFFFE4E1"},
		{"Moccasin", "#FFFFE4B5"},
		{"NavajoWhite", "#FFFFDEAD"},
		{"Navy", "#FF000080"},
		{"OldLace", "#FFFDF5E6"},
		{"Olive", "#FF808000"},
		{"OliveDrab", "#FF6B8E23"},
		{"Orange", "#FFFFA500"},
		{"OrangeRed", "#FFFF4500"},
		{"Orchid", "#FFDA70D6"},
		{"PaleGoldenrod", "#FFEEE8AA"},
		{"PaleGreen", "#FF98FB98"},
		{"PaleTurquoise", "#FFAFEEEE"},
		{"PaleVioletRed", "#FFDB7093"},
		{"PapayaWhip", "#FFFFEFD5"},
		{"PeachPuff", "#FFFFDAB9"},
		{"Peru", "#FFCD853F"},
		{"Pink", "#FFFFC0CB"},
		{"Plum", "#FFDDA0DD"},
		{"PowderBlue", "#FFB0E0E6"},
		{"Purple", "#FF800080"},
		{"Red", "#FFFF0000"},
		{"RosyBrown", "#FFBC8F8F"},
		{"RoyalBlue", "#FF4169E1"},
		{"SaddleBrown", "#FF8B4513"},
		{"Salmon", "#FFFA8072"},
		{"SandyBrown", "#FFF4A460"},
		{"SeaGreen", "#FF2E8B57"},
		{"SeaShell", "#FFFFF5EE"},
		{"Sienna", "#FFA0522D"},
		{"Silver", "#FFC0C0C0"},
		{"SkyBlue", "#FF87CEEB"},
		{"SlateBlue", "#FF6A5ACD"},
		{"SlateGray", "#FF708090"},
		{"Snow", "#FFFFFAFA"},
		{"SpringGreen", "#FF00FF7F"},
		{"SteelBlue", "#FF4682B4"},
		{"Tan", "#FFD2B48C"},
		{"Teal", "#FF008080"},
		{"Thistle", "#FFD8BFD8"},
		{"Tomato", "#FFFF6347"},
		{"Turquoise", "#FF40E0D0"},
		{"Violet", "#FFEE82EE"},
		{"Wheat", "#FFF5DEB3"},
		{"White", "#FFFFFFFF"},
		{"WhiteSmoke", "#FFF5F5F5"},
		{"Yellow", "#FFFFFF00"},
		{"YellowGreen", "#FF9ACD32"}};

utils::xcolor::xcolor()
	: m_red(0), m_green(0), m_blue(0), m_alpha(0), m_isValid(false)
{
}

utils::xcolor::xcolor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	: m_red(red), m_green(green), m_blue(blue), m_alpha(alpha), m_isValid(true)
{
}

utils::xcolor::xcolor(int red, int green, int blue, int alpha)
	: m_red(S_CAST(unsigned char, (std::min(std::max(red, (int)CED_COLOR_MIN_COMPONENT), (int)CED_COLOR_MAX_COMPONENT)))),
	  m_green(S_CAST(unsigned char, (std::min(std::max(green, (int)CED_COLOR_MIN_COMPONENT), (int)CED_COLOR_MAX_COMPONENT)))),
	  m_blue(S_CAST(unsigned char, (std::min(std::max(blue, (int)CED_COLOR_MIN_COMPONENT), (int)CED_COLOR_MAX_COMPONENT)))),
	  m_alpha(S_CAST(unsigned char, (std::min(std::max(alpha, (int)CED_COLOR_MIN_COMPONENT), (int)CED_COLOR_MAX_COMPONENT)))),
	  m_isValid(true)
{
}


utils::xcolor::xcolor(const utils::xcolor& color)
	: m_red(color.m_red), m_green(color.m_green), m_blue(color.m_blue), m_alpha(color.m_alpha), m_isValid(color.m_isValid)
{
}

utils::xcolor::xcolor(const std::string& colorString)
{
	bool res = false;
	if (colorString.find(',') != std::string::npos)
	{
		res = decToColor(colorString);
	}
	else
	{
		std::string colorStrCopy = colorString;
		auto it = m_colorNameToArgb.find(colorString);
		if (it != m_colorNameToArgb.end())
		{
			colorStrCopy = it->second;
		}
		res = hexToColor(colorStrCopy);
	}

	if (!res)
	{
		m_alpha = 0;
		m_red = 0;
		m_green = 0;
		m_blue = 0;
		m_isValid = false;
	}
}

bool utils::xcolor::isValid() const
{
	return m_isValid;
}

unsigned char utils::xcolor::red() const
{
	return m_red;
}

void utils::xcolor::setRed(unsigned char red)
{
	m_red = red;
	m_isValid = true;
}

unsigned char utils::xcolor::blue() const
{
	return m_blue;
}

void utils::xcolor::setBlue(unsigned char blue)
{
	m_blue = blue;
	m_isValid = true;
}

unsigned char utils::xcolor::green() const
{
	return m_green;
}

void utils::xcolor::setGreen(unsigned char green)
{
	m_green = green;
	m_isValid = true;
}

unsigned char utils::xcolor::alpha() const
{
	return m_alpha;
}

void utils::xcolor::setAlpha(unsigned char alpha)
{
	m_alpha = alpha;
	m_isValid = true;
}

void utils::xcolor::setAll(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
	m_isValid = true;
}


utils::xcolor& utils::xcolor::operator=(const utils::xcolor& color)
{
	if (this != &color)
	{
		m_red = color.m_red;
		m_green = color.m_green;
		m_blue = color.m_blue;
		m_alpha = color.m_alpha;
		m_isValid = color.m_isValid;
	}
	return *this;
}

bool utils::xcolor::operator==(const utils::xcolor& color) const
{
	if (!m_isValid && !color.m_isValid)
	{
		return true;
	}
	return (m_red == color.m_red &&
			m_green == color.m_green &&
			m_blue == color.m_blue &&
			m_alpha == color.m_alpha &&
			m_isValid == color.m_isValid);
}

bool utils::xcolor::operator!=(const utils::xcolor& color) const
{
	return !(*this == color);
}
void utils::xcolor::setRGB(unsigned int colRGB)
{
	// 0x00BBGGRR
	m_red = colRGB & CED_COLOR_MASK_8_BITS;                              // 提取红色分量（最低8位）
	m_green = (colRGB >> CED_COLOR_SHIFT_8_BITS) & CED_COLOR_MASK_8_BITS;// 提取绿色分量（中间8位）
	m_blue = (colRGB >> CED_COLOR_SHIFT_16_BITS) & CED_COLOR_MASK_8_BITS;// 提取蓝色分量（最高8位）
	m_alpha = CED_COLOR_ALPHA_OPAQUE;                                    // 默认alpha为255
	m_isValid = true;
}
unsigned int utils::xcolor::getRGB() const
{
	if (!m_isValid)
	{
		return 0;
	}
	return (m_blue << CED_COLOR_SHIFT_16_BITS) | (m_green << CED_COLOR_SHIFT_8_BITS) | m_red;
}
void utils::xcolor::setRGBA(unsigned int colRGBA)
{
	// 0xAABBGGRR
	m_alpha = (colRGBA >> CED_COLOR_SHIFT_24_BITS) & CED_COLOR_MASK_8_BITS;// 提取alpha分量（最高8位）
	m_red = colRGBA & CED_COLOR_MASK_8_BITS;                               // 提取红色分量（最低8位）
	m_green = (colRGBA >> CED_COLOR_SHIFT_8_BITS) & CED_COLOR_MASK_8_BITS; // 提取绿色分量（第9到第16位）
	m_blue = (colRGBA >> CED_COLOR_SHIFT_16_BITS) & CED_COLOR_MASK_8_BITS; // 提取蓝色分量（第17到第24位）
	m_isValid = true;
}
unsigned int utils::xcolor::getRGBA() const
{
	if (!m_isValid)
	{
		return 0;
	}
	return (m_alpha << CED_COLOR_SHIFT_24_BITS) | (m_blue << CED_COLOR_SHIFT_16_BITS) | (m_green << CED_COLOR_SHIFT_8_BITS) | m_red;
}

std::string utils::xcolor::getAsString() const
{
	if (!m_isValid)
	{
		return std::string{};
	}
	std::ostringstream oss;
	// 使用16进制格式输出每个颜色通道，并确保两位数宽度
	oss << "#"
		<< std::hex << std::uppercase// 设置输出格式为大写的十六进制
		<< std::setw(2) << std::setfill('0') << static_cast<int>(m_alpha)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(m_red)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(m_green)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(m_blue);
	return oss.str();
}

bool utils::xcolor::hexToColor(const std::string& hex)
{
	try
	{
		std::string hexCopy = hex;
		if (hexCopy.empty())
		{
			return false;
		}
		// 去掉#字符
		if (hexCopy[0] == '#')
		{
			hexCopy = hexCopy.substr(1);
		}

		if (hexCopy.length() == 6)
		{
			m_alpha = CED_COLOR_MAX_COMPONENT;
			m_red = S_CAST(unsigned char, (std::stoi(hexCopy.substr(0, 2), nullptr, 16)));
			m_green = S_CAST(unsigned char, (std::stoi(hexCopy.substr(2, 2), nullptr, 16)));
			m_blue = S_CAST(unsigned char, (std::stoi(hexCopy.substr(4, 2), nullptr, 16)));
			m_isValid = true;
			return true;
		}
		else if (hexCopy.length() == 8)
		{
			m_alpha = S_CAST(unsigned char, (std::stoi(hexCopy.substr(0, 2), nullptr, 16)));
			m_red = S_CAST(unsigned char, (std::stoi(hexCopy.substr(2, 2), nullptr, 16)));
			m_green = S_CAST(unsigned char, (std::stoi(hexCopy.substr(4, 2), nullptr, 16)));
			m_blue = S_CAST(unsigned char, (std::stoi(hexCopy.substr(6, 2), nullptr, 16)));
			m_isValid = true;
			return true;
		}
	}
	catch (const std::exception& e)
	{
		return false;
	}
	return false;
}

bool utils::xcolor::decToColor(const std::string& dec)
{
	std::vector<unsigned char> rgba;
	std::istringstream iss(dec);
	std::string segment;

	try
	{
		// 使用逗号分隔字符串
		while (std::getline(iss, segment, ','))
		{

			// 将每个子串转换为unsigned char（0-255）
			rgba.push_back(S_CAST(unsigned char, (std::stoi(segment))));
		}
	}
	catch (const std::invalid_argument& e)
	{
		return false;
	}

	if (rgba.size() == 3)
	{
		// 如果只有RGB值，默认alpha为255（不透明）
		m_red = rgba[0];
		m_green = rgba[1];
		m_blue = rgba[2];
		m_alpha = CED_COLOR_ALPHA_OPAQUE;
		return true;
	}
	else if (rgba.size() == 4)
	{
		// 如果有RGBA值
		m_alpha = rgba[0];
		m_red = rgba[1];
		m_green = rgba[2];
		m_blue = rgba[3];
		return true;
	}
	return false;
}


//double utils::XColor::luminance() const
//{
//	// 将颜色分量从0-255范围归一化到0-1范围
//	double rNorm = m_red / 255.0;
//	double gNorm = m_green / 255.0;
//	double bNorm = m_blue / 255.0;
//
//	// 计算亮度
//	return 0.299 * rNorm + 0.587 * gNorm + 0.114 * bNorm;
//}

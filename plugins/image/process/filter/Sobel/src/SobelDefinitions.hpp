#ifndef _TUTTLE_PLUGIN_SOBEL_DEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_SOBEL_DEFINITIONS_HPP_

#include <tuttle/common/utils/global.hpp>


namespace tuttle {
namespace plugin {
namespace sobel {

const static std::string kParamSize           = "size";

const static std::string kParamNormalizedKernel = "normalizedKernel";

const static std::string kParamBorder = "border";
const static std::string kParamBorderMirror = "Mirror";
const static std::string kParamBorderConstant = "Constant";
const static std::string kParamBorderBlack = "Black";
const static std::string kParamBorderPadded = "Padded";

enum EParamBorder
{
	eParamBorderMirror = 0,
	eParamBorderConstant,
	eParamBorderBlack,
	eParamBorderPadded
};


}
}
}

#endif
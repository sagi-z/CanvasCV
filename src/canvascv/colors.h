#ifndef COLORS_H
#define COLORS_H

#include <opencv2/opencv.hpp>

namespace canvascv
{

/**
 * @brief The Colors class
 * The place for color constants
 */
class Colors
{
public:

    static const cv::Scalar OPAQUE; ///< can be added to other colors
    static const cv::Scalar TRANSPARENT; ///< can be added to other colors

    static const cv::Scalar Maroon; ///< <SPAN style="background: #800000;">&nbsp;&nbsp;</SPAN> RGB: (128,0,0)  HEX: #800000
    static const cv::Scalar DarkRed; ///< <SPAN style="background: #8B0000;">&nbsp;&nbsp;</SPAN> RGB: (139,0,0)  HEX: #8B0000
    static const cv::Scalar Brown; ///< <SPAN style="background: #A52A2A;">&nbsp;&nbsp;</SPAN> RGB: (165,42,42)  HEX: #A52A2A
    static const cv::Scalar Firebrick; ///< <SPAN style="background: #B22222;">&nbsp;&nbsp;</SPAN> RGB: (178,34,34)  HEX: #B22222
    static const cv::Scalar Crimson; ///< <SPAN style="background: #DC143C;">&nbsp;&nbsp;</SPAN> RGB: (220,20,60)  HEX: #DC143C
    static const cv::Scalar Red; ///< <SPAN style="background: #FF0000;">&nbsp;&nbsp;</SPAN> RGB: (255,0,0)  HEX: #FF0000
    static const cv::Scalar Tomato; ///< <SPAN style="background: #FF6347;">&nbsp;&nbsp;</SPAN> RGB: (255,99,71)  HEX: #FF6347
    static const cv::Scalar Coral; ///< <SPAN style="background: #FF7F50;">&nbsp;&nbsp;</SPAN> RGB: (255,127,80)  HEX: #FF7F50
    static const cv::Scalar IndianRed; ///< <SPAN style="background: #CD5C5C;">&nbsp;&nbsp;</SPAN> RGB: (205,92,92)  HEX: #CD5C5C
    static const cv::Scalar LightCoral; ///< <SPAN style="background: #F08080;">&nbsp;&nbsp;</SPAN> RGB: (240,128,128)  HEX: #F08080
    static const cv::Scalar DarkSalmon; ///< <SPAN style="background: #E9967A;">&nbsp;&nbsp;</SPAN> RGB: (233,150,122)  HEX: #E9967A
    static const cv::Scalar Salmon; ///< <SPAN style="background: #FA8072;">&nbsp;&nbsp;</SPAN> RGB: (250,128,114)  HEX: #FA8072
    static const cv::Scalar LightSalmon; ///< <SPAN style="background: #FFA07A;">&nbsp;&nbsp;</SPAN> RGB: (255,160,122)  HEX: #FFA07A
    static const cv::Scalar OrangeRed; ///< <SPAN style="background: #FF4500;">&nbsp;&nbsp;</SPAN> RGB: (255,69,0)  HEX: #FF4500
    static const cv::Scalar DarkOrange; ///< <SPAN style="background: #FF8C00;">&nbsp;&nbsp;</SPAN> RGB: (255,140,0)  HEX: #FF8C00
    static const cv::Scalar Orange; ///< <SPAN style="background: #FFA500;">&nbsp;&nbsp;</SPAN> RGB: (255,165,0)  HEX: #FFA500
    static const cv::Scalar Gold; ///< <SPAN style="background: #FFD700;">&nbsp;&nbsp;</SPAN> RGB: (255,215,0)  HEX: #FFD700
    static const cv::Scalar DarkGoldenRod; ///< <SPAN style="background: #B8860B;">&nbsp;&nbsp;</SPAN> RGB: (184,134,11)  HEX: #B8860B
    static const cv::Scalar GoldenRod; ///< <SPAN style="background: #DAA520;">&nbsp;&nbsp;</SPAN> RGB: (218,165,32)  HEX: #DAA520
    static const cv::Scalar PaleGoldenRod; ///< <SPAN style="background: #EEE8AA;">&nbsp;&nbsp;</SPAN> RGB: (238,232,170)  HEX: #EEE8AA
    static const cv::Scalar DarkKhaki; ///< <SPAN style="background: #BDB76B;">&nbsp;&nbsp;</SPAN> RGB: (189,183,107)  HEX: #BDB76B
    static const cv::Scalar Khaki; ///< <SPAN style="background: #F0E68C;">&nbsp;&nbsp;</SPAN> RGB: (240,230,140)  HEX: #F0E68C
    static const cv::Scalar Olive; ///< <SPAN style="background: #808000;">&nbsp;&nbsp;</SPAN> RGB: (128,128,0)  HEX: #808000
    static const cv::Scalar Yellow; ///< <SPAN style="background: #FFFF00;">&nbsp;&nbsp;</SPAN> RGB: (255,255,0)  HEX: #FFFF00
    static const cv::Scalar YellowGreen; ///< <SPAN style="background: #9ACD32;">&nbsp;&nbsp;</SPAN> RGB: (154,205,50)  HEX: #9ACD32
    static const cv::Scalar DarkOliveGreen; ///< <SPAN style="background: #556B2F;">&nbsp;&nbsp;</SPAN> RGB: (85,107,47)  HEX: #556B2F
    static const cv::Scalar OliveDrab; ///< <SPAN style="background: #6B8E23;">&nbsp;&nbsp;</SPAN> RGB: (107,142,35)  HEX: #6B8E23
    static const cv::Scalar LawnGreen; ///< <SPAN style="background: #7CFC00;">&nbsp;&nbsp;</SPAN> RGB: (124,252,0)  HEX: #7CFC00
    static const cv::Scalar ChartReuse; ///< <SPAN style="background: #7FFF00;">&nbsp;&nbsp;</SPAN> RGB: (127,255,0)  HEX: #7FFF00
    static const cv::Scalar GreenYellow; ///< <SPAN style="background: #ADFF2F;">&nbsp;&nbsp;</SPAN> RGB: (173,255,47)  HEX: #ADFF2F
    static const cv::Scalar DarkGreen; ///< <SPAN style="background: #006400;">&nbsp;&nbsp;</SPAN> RGB: (0,100,0)  HEX: #006400
    static const cv::Scalar Green; ///< <SPAN style="background: #008000;">&nbsp;&nbsp;</SPAN> RGB: (0,128,0)  HEX: #008000
    static const cv::Scalar ForestGreen; ///< <SPAN style="background: #228B22;">&nbsp;&nbsp;</SPAN> RGB: (34,139,34)  HEX: #228B22
    static const cv::Scalar Lime; ///< <SPAN style="background: #00FF00;">&nbsp;&nbsp;</SPAN> RGB: (0,255,0)  HEX: #00FF00
    static const cv::Scalar LimeGreen; ///< <SPAN style="background: #32CD32;">&nbsp;&nbsp;</SPAN> RGB: (50,205,50)  HEX: #32CD32
    static const cv::Scalar LightGreen; ///< <SPAN style="background: #90EE90;">&nbsp;&nbsp;</SPAN> RGB: (144,238,144)  HEX: #90EE90
    static const cv::Scalar PaleGreen; ///< <SPAN style="background: #98FB98;">&nbsp;&nbsp;</SPAN> RGB: (152,251,152)  HEX: #98FB98
    static const cv::Scalar DarkSeaGreen; ///< <SPAN style="background: #8FBC8F;">&nbsp;&nbsp;</SPAN> RGB: (143,188,143)  HEX: #8FBC8F
    static const cv::Scalar MediumSpringGreen; ///< <SPAN style="background: #00FA9A;">&nbsp;&nbsp;</SPAN> RGB: (0,250,154)  HEX: #00FA9A
    static const cv::Scalar SpringGreen; ///< <SPAN style="background: #00FF7F;">&nbsp;&nbsp;</SPAN> RGB: (0,255,127)  HEX: #00FF7F
    static const cv::Scalar SeaGreen; ///< <SPAN style="background: #2E8B57;">&nbsp;&nbsp;</SPAN> RGB: (46,139,87)  HEX: #2E8B57
    static const cv::Scalar MediumAquaMarine; ///< <SPAN style="background: #66CDAA;">&nbsp;&nbsp;</SPAN> RGB: (102,205,170)  HEX: #66CDAA
    static const cv::Scalar MediumSeaGreen; ///< <SPAN style="background: #3CB371;">&nbsp;&nbsp;</SPAN> RGB: (60,179,113)  HEX: #3CB371
    static const cv::Scalar LightSeaGreen; ///< <SPAN style="background: #20B2AA;">&nbsp;&nbsp;</SPAN> RGB: (32,178,170)  HEX: #20B2AA
    static const cv::Scalar DarkSlateGray; ///< <SPAN style="background: #2F4F4F;">&nbsp;&nbsp;</SPAN> RGB: (47,79,79)  HEX: #2F4F4F
    static const cv::Scalar Teal; ///< <SPAN style="background: #008080;">&nbsp;&nbsp;</SPAN> RGB: (0,128,128)  HEX: #008080
    static const cv::Scalar DarkCyan; ///< <SPAN style="background: #008B8B;">&nbsp;&nbsp;</SPAN> RGB: (0,139,139)  HEX: #008B8B
    static const cv::Scalar Aqua; ///< <SPAN style="background: #00FFFF;">&nbsp;&nbsp;</SPAN> RGB: (0,255,255)  HEX: #00FFFF
    static const cv::Scalar Cyan; ///< <SPAN style="background: #00FFFF;">&nbsp;&nbsp;</SPAN> RGB: (0,255,255)  HEX: #00FFFF
    static const cv::Scalar LightCyan; ///< <SPAN style="background: #E0FFFF;">&nbsp;&nbsp;</SPAN> RGB: (224,255,255)  HEX: #E0FFFF
    static const cv::Scalar DarkTurquoise; ///< <SPAN style="background: #00CED1;">&nbsp;&nbsp;</SPAN> RGB: (0,206,209)  HEX: #00CED1
    static const cv::Scalar Turquoise; ///< <SPAN style="background: #40E0D0;">&nbsp;&nbsp;</SPAN> RGB: (64,224,208)  HEX: #40E0D0
    static const cv::Scalar MediumTurquoise; ///< <SPAN style="background: #48D1CC;">&nbsp;&nbsp;</SPAN> RGB: (72,209,204)  HEX: #48D1CC
    static const cv::Scalar PaleTurquoise; ///< <SPAN style="background: #AFEEEE;">&nbsp;&nbsp;</SPAN> RGB: (175,238,238)  HEX: #AFEEEE
    static const cv::Scalar AquaMarine; ///< <SPAN style="background: #7FFFD4;">&nbsp;&nbsp;</SPAN> RGB: (127,255,212)  HEX: #7FFFD4
    static const cv::Scalar PowderBlue; ///< <SPAN style="background: #B0E0E6;">&nbsp;&nbsp;</SPAN> RGB: (176,224,230)  HEX: #B0E0E6
    static const cv::Scalar CadetBlue; ///< <SPAN style="background: #5F9EA0;">&nbsp;&nbsp;</SPAN> RGB: (95,158,160)  HEX: #5F9EA0
    static const cv::Scalar SteelBlue; ///< <SPAN style="background: #4682B4;">&nbsp;&nbsp;</SPAN> RGB: (70,130,180)  HEX: #4682B4
    static const cv::Scalar CornFlowerBlue; ///< <SPAN style="background: #6495ED;">&nbsp;&nbsp;</SPAN> RGB: (100,149,237)  HEX: #6495ED
    static const cv::Scalar DeepSkyBlue; ///< <SPAN style="background: #00BFFF;">&nbsp;&nbsp;</SPAN> RGB: (0,191,255)  HEX: #00BFFF
    static const cv::Scalar DodgerBlue; ///< <SPAN style="background: #1E90FF;">&nbsp;&nbsp;</SPAN> RGB: (30,144,255)  HEX: #1E90FF
    static const cv::Scalar LightBlue; ///< <SPAN style="background: #ADD8E6;">&nbsp;&nbsp;</SPAN> RGB: (173,216,230)  HEX: #ADD8E6
    static const cv::Scalar SkyBlue; ///< <SPAN style="background: #87CEEB;">&nbsp;&nbsp;</SPAN> RGB: (135,206,235)  HEX: #87CEEB
    static const cv::Scalar LightSkyBlue; ///< <SPAN style="background: #87CEFA;">&nbsp;&nbsp;</SPAN> RGB: (135,206,250)  HEX: #87CEFA
    static const cv::Scalar MidnightBlue; ///< <SPAN style="background: #191970;">&nbsp;&nbsp;</SPAN> RGB: (25,25,112)  HEX: #191970
    static const cv::Scalar Navy; ///< <SPAN style="background: #000080;">&nbsp;&nbsp;</SPAN> RGB: (0,0,128)  HEX: #000080
    static const cv::Scalar DarkBlue; ///< <SPAN style="background: #00008B;">&nbsp;&nbsp;</SPAN> RGB: (0,0,139)  HEX: #00008B
    static const cv::Scalar MediumBlue; ///< <SPAN style="background: #0000CD;">&nbsp;&nbsp;</SPAN> RGB: (0,0,205)  HEX: #0000CD
    static const cv::Scalar Blue; ///< <SPAN style="background: #0000FF;">&nbsp;&nbsp;</SPAN> RGB: (0,0,255)  HEX: #0000FF
    static const cv::Scalar RoyalBlue; ///< <SPAN style="background: #4169E1;">&nbsp;&nbsp;</SPAN> RGB: (65,105,225)  HEX: #4169E1
    static const cv::Scalar BlueViolet; ///< <SPAN style="background: #8A2BE2;">&nbsp;&nbsp;</SPAN> RGB: (138,43,226)  HEX: #8A2BE2
    static const cv::Scalar Indigo; ///< <SPAN style="background: #4B0082;">&nbsp;&nbsp;</SPAN> RGB: (75,0,130)  HEX: #4B0082
    static const cv::Scalar DarkSlateBlue; ///< <SPAN style="background: #483D8B;">&nbsp;&nbsp;</SPAN> RGB: (72,61,139)  HEX: #483D8B
    static const cv::Scalar SlateBlue; ///< <SPAN style="background: #6A5ACD;">&nbsp;&nbsp;</SPAN> RGB: (106,90,205)  HEX: #6A5ACD
    static const cv::Scalar MediumSlateBlue; ///< <SPAN style="background: #7B68EE;">&nbsp;&nbsp;</SPAN> RGB: (123,104,238)  HEX: #7B68EE
    static const cv::Scalar MediumPurple; ///< <SPAN style="background: #9370DB;">&nbsp;&nbsp;</SPAN> RGB: (147,112,219)  HEX: #9370DB
    static const cv::Scalar DarkMagenta; ///< <SPAN style="background: #8B008B;">&nbsp;&nbsp;</SPAN> RGB: (139,0,139)  HEX: #8B008B
    static const cv::Scalar DarkViolet; ///< <SPAN style="background: #9400D3;">&nbsp;&nbsp;</SPAN> RGB: (148,0,211)  HEX: #9400D3
    static const cv::Scalar DarkOrchid; ///< <SPAN style="background: #9932CC;">&nbsp;&nbsp;</SPAN> RGB: (153,50,204)  HEX: #9932CC
    static const cv::Scalar MediumOrchid; ///< <SPAN style="background: #BA55D3;">&nbsp;&nbsp;</SPAN> RGB: (186,85,211)  HEX: #BA55D3
    static const cv::Scalar Purple; ///< <SPAN style="background: #800080;">&nbsp;&nbsp;</SPAN> RGB: (128,0,128)  HEX: #800080
    static const cv::Scalar Thistle; ///< <SPAN style="background: #D8BFD8;">&nbsp;&nbsp;</SPAN> RGB: (216,191,216)  HEX: #D8BFD8
    static const cv::Scalar Plum; ///< <SPAN style="background: #DDA0DD;">&nbsp;&nbsp;</SPAN> RGB: (221,160,221)  HEX: #DDA0DD
    static const cv::Scalar Violet; ///< <SPAN style="background: #EE82EE;">&nbsp;&nbsp;</SPAN> RGB: (238,130,238)  HEX: #EE82EE
    static const cv::Scalar Magenta; ///< <SPAN style="background: #FF00FF;">&nbsp;&nbsp;</SPAN> RGB: (255,0,255)  HEX: #FF00FF
    static const cv::Scalar Fuchsia; ///< <SPAN style="background: #FF00FF;">&nbsp;&nbsp;</SPAN> RGB: (255,0,255)  HEX: #FF00FF
    static const cv::Scalar Orchid; ///< <SPAN style="background: #DA70D6;">&nbsp;&nbsp;</SPAN> RGB: (218,112,214)  HEX: #DA70D6
    static const cv::Scalar MediumVioletRed; ///< <SPAN style="background: #C71585;">&nbsp;&nbsp;</SPAN> RGB: (199,21,133)  HEX: #C71585
    static const cv::Scalar PaleVioletRed; ///< <SPAN style="background: #DB7093;">&nbsp;&nbsp;</SPAN> RGB: (219,112,147)  HEX: #DB7093
    static const cv::Scalar DeepPink; ///< <SPAN style="background: #FF1493;">&nbsp;&nbsp;</SPAN> RGB: (255,20,147)  HEX: #FF1493
    static const cv::Scalar HotPink; ///< <SPAN style="background: #FF69B4;">&nbsp;&nbsp;</SPAN> RGB: (255,105,180)  HEX: #FF69B4
    static const cv::Scalar LightPink; ///< <SPAN style="background: #FFB6C1;">&nbsp;&nbsp;</SPAN> RGB: (255,182,193)  HEX: #FFB6C1
    static const cv::Scalar Pink; ///< <SPAN style="background: #FFC0CB;">&nbsp;&nbsp;</SPAN> RGB: (255,192,203)  HEX: #FFC0CB
    static const cv::Scalar AntiqueWhite; ///< <SPAN style="background: #FAEBD7;">&nbsp;&nbsp;</SPAN> RGB: (250,235,215)  HEX: #FAEBD7
    static const cv::Scalar Beige; ///< <SPAN style="background: #F5F5DC;">&nbsp;&nbsp;</SPAN> RGB: (245,245,220)  HEX: #F5F5DC
    static const cv::Scalar Bisque; ///< <SPAN style="background: #FFE4C4;">&nbsp;&nbsp;</SPAN> RGB: (255,228,196)  HEX: #FFE4C4
    static const cv::Scalar BlanchedAlmond; ///< <SPAN style="background: #FFEBCD;">&nbsp;&nbsp;</SPAN> RGB: (255,235,205)  HEX: #FFEBCD
    static const cv::Scalar Wheat; ///< <SPAN style="background: #F5DEB3;">&nbsp;&nbsp;</SPAN> RGB: (245,222,179)  HEX: #F5DEB3
    static const cv::Scalar CornSilk; ///< <SPAN style="background: #FFF8DC;">&nbsp;&nbsp;</SPAN> RGB: (255,248,220)  HEX: #FFF8DC
    static const cv::Scalar LemonChiffon; ///< <SPAN style="background: #FFFACD;">&nbsp;&nbsp;</SPAN> RGB: (255,250,205)  HEX: #FFFACD
    static const cv::Scalar LightGoldenRodYellow; ///< <SPAN style="background: #FAFAD2;">&nbsp;&nbsp;</SPAN> RGB: (250,250,210)  HEX: #FAFAD2
    static const cv::Scalar LightYellow; ///< <SPAN style="background: #FFFFE0;">&nbsp;&nbsp;</SPAN> RGB: (255,255,224)  HEX: #FFFFE0
    static const cv::Scalar SaddleBrown; ///< <SPAN style="background: #8B4513;">&nbsp;&nbsp;</SPAN> RGB: (139,69,19)  HEX: #8B4513
    static const cv::Scalar Sienna; ///< <SPAN style="background: #A0522D;">&nbsp;&nbsp;</SPAN> RGB: (160,82,45)  HEX: #A0522D
    static const cv::Scalar Chocolate; ///< <SPAN style="background: #D2691E;">&nbsp;&nbsp;</SPAN> RGB: (210,105,30)  HEX: #D2691E
    static const cv::Scalar Peru; ///< <SPAN style="background: #CD853F;">&nbsp;&nbsp;</SPAN> RGB: (205,133,63)  HEX: #CD853F
    static const cv::Scalar SandyBrown; ///< <SPAN style="background: #F4A460;">&nbsp;&nbsp;</SPAN> RGB: (244,164,96)  HEX: #F4A460
    static const cv::Scalar BurlyWood; ///< <SPAN style="background: #DEB887;">&nbsp;&nbsp;</SPAN> RGB: (222,184,135)  HEX: #DEB887
    static const cv::Scalar Tan; ///< <SPAN style="background: #D2B48C;">&nbsp;&nbsp;</SPAN> RGB: (210,180,140)  HEX: #D2B48C
    static const cv::Scalar RosyBrown; ///< <SPAN style="background: #BC8F8F;">&nbsp;&nbsp;</SPAN> RGB: (188,143,143)  HEX: #BC8F8F
    static const cv::Scalar Moccasin; ///< <SPAN style="background: #FFE4B5;">&nbsp;&nbsp;</SPAN> RGB: (255,228,181)  HEX: #FFE4B5
    static const cv::Scalar NavajoWhite; ///< <SPAN style="background: #FFDEAD;">&nbsp;&nbsp;</SPAN> RGB: (255,222,173)  HEX: #FFDEAD
    static const cv::Scalar PeachPuff; ///< <SPAN style="background: #FFDAB9;">&nbsp;&nbsp;</SPAN> RGB: (255,218,185)  HEX: #FFDAB9
    static const cv::Scalar MistyRose; ///< <SPAN style="background: #FFE4E1;">&nbsp;&nbsp;</SPAN> RGB: (255,228,225)  HEX: #FFE4E1
    static const cv::Scalar LavenderBlush; ///< <SPAN style="background: #FFF0F5;">&nbsp;&nbsp;</SPAN> RGB: (255,240,245)  HEX: #FFF0F5
    static const cv::Scalar Linen; ///< <SPAN style="background: #FAF0E6;">&nbsp;&nbsp;</SPAN> RGB: (250,240,230)  HEX: #FAF0E6
    static const cv::Scalar OldLace; ///< <SPAN style="background: #FDF5E6;">&nbsp;&nbsp;</SPAN> RGB: (253,245,230)  HEX: #FDF5E6
    static const cv::Scalar PapayaWhip; ///< <SPAN style="background: #FFEFD5;">&nbsp;&nbsp;</SPAN> RGB: (255,239,213)  HEX: #FFEFD5
    static const cv::Scalar SeaShell; ///< <SPAN style="background: #FFF5EE;">&nbsp;&nbsp;</SPAN> RGB: (255,245,238)  HEX: #FFF5EE
    static const cv::Scalar MintCream; ///< <SPAN style="background: #F5FFFA;">&nbsp;&nbsp;</SPAN> RGB: (245,255,250)  HEX: #F5FFFA
    static const cv::Scalar SlateGray; ///< <SPAN style="background: #708090;">&nbsp;&nbsp;</SPAN> RGB: (112,128,144)  HEX: #708090
    static const cv::Scalar LightSlateGray; ///< <SPAN style="background: #778899;">&nbsp;&nbsp;</SPAN> RGB: (119,136,153)  HEX: #778899
    static const cv::Scalar LightSteelBlue; ///< <SPAN style="background: #B0C4DE;">&nbsp;&nbsp;</SPAN> RGB: (176,196,222)  HEX: #B0C4DE
    static const cv::Scalar Lavender; ///< <SPAN style="background: #E6E6FA;">&nbsp;&nbsp;</SPAN> RGB: (230,230,250)  HEX: #E6E6FA
    static const cv::Scalar FloralWhite; ///< <SPAN style="background: #FFFAF0;">&nbsp;&nbsp;</SPAN> RGB: (255,250,240)  HEX: #FFFAF0
    static const cv::Scalar AliceBlue; ///< <SPAN style="background: #F0F8FF;">&nbsp;&nbsp;</SPAN> RGB: (240,248,255)  HEX: #F0F8FF
    static const cv::Scalar GhostWhite; ///< <SPAN style="background: #F8F8FF;">&nbsp;&nbsp;</SPAN> RGB: (248,248,255)  HEX: #F8F8FF
    static const cv::Scalar Honeydew; ///< <SPAN style="background: #F0FFF0;">&nbsp;&nbsp;</SPAN> RGB: (240,255,240)  HEX: #F0FFF0
    static const cv::Scalar Ivory; ///< <SPAN style="background: #FFFFF0;">&nbsp;&nbsp;</SPAN> RGB: (255,255,240)  HEX: #FFFFF0
    static const cv::Scalar Azure; ///< <SPAN style="background: #F0FFFF;">&nbsp;&nbsp;</SPAN> RGB: (240,255,255)  HEX: #F0FFFF
    static const cv::Scalar Snow; ///< <SPAN style="background: #FFFAFA;">&nbsp;&nbsp;</SPAN> RGB: (255,250,250)  HEX: #FFFAFA
    static const cv::Scalar Black; ///< <SPAN style="background: #000000;">&nbsp;&nbsp;</SPAN> RGB: (0,0,0)  HEX: #000000
    static const cv::Scalar DimGray; ///< <SPAN style="background: #696969;">&nbsp;&nbsp;</SPAN> RGB: (105,105,105)  HEX: #696969
    static const cv::Scalar DimGrey; ///< <SPAN style="background: #696969;">&nbsp;&nbsp;</SPAN> RGB: (105,105,105)  HEX: #696969
    static const cv::Scalar Gray; ///< <SPAN style="background: #808080;">&nbsp;&nbsp;</SPAN> RGB: (128,128,128)  HEX: #808080
    static const cv::Scalar Grey; ///< <SPAN style="background: #808080;">&nbsp;&nbsp;</SPAN> RGB: (128,128,128)  HEX: #808080
    static const cv::Scalar DarkGray; ///< <SPAN style="background: #A9A9A9;">&nbsp;&nbsp;</SPAN> RGB: (169,169,169)  HEX: #A9A9A9
    static const cv::Scalar DarkGrey; ///< <SPAN style="background: #A9A9A9;">&nbsp;&nbsp;</SPAN> RGB: (169,169,169)  HEX: #A9A9A9
    static const cv::Scalar Silver; ///< <SPAN style="background: #C0C0C0;">&nbsp;&nbsp;</SPAN> RGB: (192,192,192)  HEX: #C0C0C0
    static const cv::Scalar LightGray; ///< <SPAN style="background: #D3D3D3;">&nbsp;&nbsp;</SPAN> RGB: (211,211,211)  HEX: #D3D3D3
    static const cv::Scalar LightGrey; ///< <SPAN style="background: #D3D3D3;">&nbsp;&nbsp;</SPAN> RGB: (211,211,211)  HEX: #D3D3D3
    static const cv::Scalar Gainsboro; ///< <SPAN style="background: #DCDCDC;">&nbsp;&nbsp;</SPAN> RGB: (220,220,220)  HEX: #DCDCDC
    static const cv::Scalar WhiteSmoke; ///< <SPAN style="background: #F5F5F5;">&nbsp;&nbsp;</SPAN> RGB: (245,245,245)  HEX: #F5F5F5
    static const cv::Scalar White; ///< <SPAN style="background: #FFFFFF;">&nbsp;&nbsp;</SPAN> RGB: (255,255,255)  HEX: #FFFFFF
};

}

#endif // COLORS_H

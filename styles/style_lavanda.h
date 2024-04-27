//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleLavanda();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define LAVANDA_STYLE_PROPS_COUNT  16

// Custom style name: Lavanda
static const GuiStyleProp lavandaStyleProps[LAVANDA_STYLE_PROPS_COUNT] = {
    { 0, 0, 0xab9bd3ff },    // DEFAULT_BORDER_COLOR_NORMAL 
    { 0, 1, 0x3e4350ff },    // DEFAULT_BASE_COLOR_NORMAL 
    { 0, 2, 0xdadaf4ff },    // DEFAULT_TEXT_COLOR_NORMAL 
    { 0, 3, 0xee84a0ff },    // DEFAULT_BORDER_COLOR_FOCUSED 
    { 0, 4, 0xf4b7c7ff },    // DEFAULT_BASE_COLOR_FOCUSED 
    { 0, 5, 0xb7657bff },    // DEFAULT_TEXT_COLOR_FOCUSED 
    { 0, 6, 0xd5c8dbff },    // DEFAULT_BORDER_COLOR_PRESSED 
    { 0, 7, 0x966ec0ff },    // DEFAULT_BASE_COLOR_PRESSED 
    { 0, 8, 0xd7ccf7ff },    // DEFAULT_TEXT_COLOR_PRESSED 
    { 0, 9, 0x8fa2bdff },    // DEFAULT_BORDER_COLOR_DISABLED 
    { 0, 10, 0x6b798dff },    // DEFAULT_BASE_COLOR_DISABLED 
    { 0, 11, 0x8292a9ff },    // DEFAULT_TEXT_COLOR_DISABLED 
    { 0, 16, 0x00000010 },    // currentTextSize
    { 0, 18, 0x84adb7ff },    // DEFAULT_LINE_COLOR 
    { 0, 19, 0x5b5b81ff },    // DEFAULT_BACKGROUND_COLOR 
    { 0, 20, 0x00000018 },    // DEFAULT_TEXT_LINE_SPACING 
};

// WARNING: This style uses a custom font: "Cartridge.ttf" (size: 16, spacing: 1)

#define LAVANDA_STYLE_FONT_ATLAS_COMP_SIZE 2636

// Font atlas image pixels data: DEFLATE compressed
static unsigned char lavandaFontData[LAVANDA_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0xdb, 0xd2, 0xa4, 0xba, 0x0d, 0x06, 0x50, 0xde, 0xff, 0xa5, 0xbf, 0x5c, 0xa4, 0x52, 0x49, 0xaa, 0x66, 0x63, 0x24,
    0x0b, 0x9a, 0xee, 0x59, 0xb3, 0xee, 0x7e, 0xa6, 0x4f, 0x06, 0xf9, 0x04, 0x96, 0x73, 0x00, 0x00, 0x00, 0x00, 0x24, 0x7f,
    0xfc, 0x4b, 0x4e, 0xfe, 0x6f, 0x2e, 0xbf, 0xd3, 0x7f, 0xfe, 0x9e, 0xc5, 0xff, 0xf8, 0xff, 0xff, 0x75, 0xed, 0x7d, 0x73,
    0xf9, 0xf3, 0x3b, 0x47, 0xf2, 0xc7, 0xef, 0x97, 0x8d, 0xd7, 0xff, 0xd3, 0x3b, 0xd4, 0xff, 0xff, 0x7f, 0xff, 0x55, 0xca,
    0xf1, 0xfc, 0x35, 0x47, 0xf1, 0x73, 0xb2, 0xf8, 0x9c, 0xab, 0xbf, 0xb0, 0x56, 0xfa, 0xb5, 0x73, 0x9e, 0xd3, 0x2b, 0xb2,
    0xf3, 0xca, 0xf3, 0xd7, 0x1d, 0xe5, 0xb2, 0x3a, 0x96, 0x25, 0x5c, 0x3d, 0xcb, 0xef, 0x8b, 0xff, 0x5c, 0x78, 0xe5, 0xf9,
    0xf1, 0x6c, 0xd4, 0x09, 0xeb, 0x72, 0xac, 0xd6, 0x18, 0x3b, 0xa5, 0x7a, 0xf6, 0x1d, 0x2b, 0xff, 0xff, 0x5a, 0x6d, 0xbb,
    0x5f, 0x3b, 0xae, 0x4a, 0xbf, 0xfe, 0xad, 0xaf, 0x94, 0x5c, 0x06, 0xfe, 0x7f, 0x5a, 0xdf, 0x7e, 0xe2, 0xfa, 0xcf, 0xf6,
    0xb5, 0x7d, 0xa5, 0xad, 0x9c, 0xb8, 0x46, 0xab, 0xf1, 0x9f, 0x56, 0x8b, 0x90, 0x45, 0x99, 0xde, 0x1d, 0xff, 0x9d, 0x76,
    0x34, 0x37, 0x95, 0x6a, 0xb5, 0xbe, 0xad, 0x5f, 0x37, 0x29, 0xb6, 0xb4, 0xbd, 0xf8, 0x5f, 0x7f, 0x4e, 0xc6, 0xca, 0x6d,
    0xdd, 0x7b, 0xa9, 0x97, 0x76, 0xef, 0xb7, 0x75, 0x6b, 0x95, 0x34, 0x7a, 0x40, 0xfd, 0xfa, 0x7a, 0xaa, 0xc4, 0x6b, 0x51,
    0x7a, 0xa5, 0x75, 0xcb, 0x58, 0xbd, 0x9f, 0x72, 0x6b, 0x95, 0x47, 0xca, 0xf2, 0x68, 0xb7, 0x3a, 0xd5, 0x5f, 0x94, 0x72,
    0x5c, 0x56, 0xfa, 0xe5, 0xfd, 0xab, 0x30, 0xa5, 0xd1, 0x59, 0x6e, 0x6b, 0xff, 0xd7, 0xad, 0x71, 0x9a, 0x57, 0x72, 0xca,
    0x7d, 0x83, 0x94, 0x47, 0x95, 0xeb, 0x73, 0xbc, 0x1e, 0x8b, 0xcc, 0xc7, 0x7f, 0xaf, 0x1f, 0xf3, 0xa7, 0x72, 0xef, 0xf6,
    0x12, 0xe6, 0xeb, 0xc6, 0xb7, 0xc7, 0x7f, 0x1a, 0x7d, 0x8c, 0xea, 0x6b, 0x52, 0x9e, 0x7b, 0x39, 0x6f, 0x23, 0xaf, 0x5f,
    0x99, 0x19, 0xba, 0x26, 0x3b, 0xad, 0x41, 0x1a, 0xe3, 0xc5, 0xdd, 0x96, 0x7c, 0x7a, 0xce, 0xea, 0xc9, 0xf8, 0xcf, 0x85,
    0x19, 0x89, 0xd5, 0x95, 0x97, 0x65, 0x29, 0xe4, 0x86, 0xf6, 0xff, 0x0d, 0x73, 0xaa, 0x4f, 0xc5, 0x7f, 0x96, 0x73, 0x5b,
    0x13, 0xd7, 0x74, 0x46, 0xce, 0x43, 0x3e, 0x70, 0x16, 0xae, 0xc4, 0xff, 0xd1, 0x6c, 0xc9, 0x7f, 0x3b, 0xfe, 0x8f, 0x8b,
    0xbd, 0xf8, 0xa3, 0x31, 0x0b, 0xf8, 0x5b, 0xf1, 0x9f, 0x46, 0xcc, 0x64, 0x6c, 0x64, 0xde, 0xeb, 0x33, 0xcc, 0xc6, 0xff,
    0x7b, 0x6b, 0xe1, 0x55, 0xff, 0x3f, 0x17, 0xef, 0x30, 0xfd, 0x4e, 0xfc, 0x1f, 0xa3, 0xfd, 0xff, 0xf5, 0x15, 0x33, 0x31,
    0x6b, 0xf4, 0x77, 0xc4, 0x7f, 0x9a, 0xf5, 0x70, 0x06, 0x67, 0x6e, 0xbe, 0x39, 0xfe, 0x3b, 0xd1, 0x93, 0x76, 0x5d, 0x79,
    0xde, 0xba, 0xe5, 0xa4, 0xc7, 0x56, 0xad, 0x19, 0xee, 0x9e, 0xff, 0xef, 0xdf, 0xb7, 0xc9, 0xf6, 0xab, 0xf3, 0xf0, 0xf8,
    0xff, 0xb9, 0xf8, 0x3f, 0xca, 0x33, 0xd8, 0xe7, 0xf7, 0x5a, 0xea, 0xf5, 0x4f, 0x1a, 0xf7, 0xbb, 0x8f, 0x91, 0xdf, 0xf2,
    0xb6, 0xf3, 0x70, 0xed, 0x35, 0x29, 0xcf, 0x6d, 0x9c, 0xdf, 0x01, 0xfb, 0xe7, 0xd9, 0x88, 0xce, 0xbc, 0xec, 0xfb, 0xea,
    0xd8, 0xbc, 0xa6, 0x8d, 0xfd, 0x5b, 0x4a, 0x81, 0x67, 0x9e, 0x31, 0x40, 0xfc, 0x8b, 0x7f, 0x78, 0xfb, 0x73, 0xcb, 0xe2,
    0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xdf, 0x5a, 0xa1, 0x57, 0x5b, 0x15, 0xbe,
    0x93, 0xcb, 0xb8, 0x93, 0x9d, 0x6e, 0xbd, 0x42, 0x3d, 0xad, 0x9c, 0x04, 0xbd, 0xd7, 0xe5, 0x34, 0x0b, 0x5a, 0x46, 0xf2,
    0x13, 0x74, 0xf3, 0x4e, 0x55, 0x32, 0x08, 0x54, 0xf7, 0x32, 0xe8, 0xee, 0x1b, 0x50, 0xfb, 0x8c, 0xce, 0x6a, 0xfd, 0x0c,
    0x65, 0x41, 0xe8, 0xe4, 0x16, 0xde, 0x5b, 0x95, 0x5a, 0xcf, 0xd1, 0xb6, 0x93, 0xf5, 0x23, 0xad, 0x55, 0x73, 0x93, 0xf1,
    0x9f, 0x4b, 0xb9, 0x0e, 0x3a, 0x19, 0x6a, 0x3a, 0x59, 0x6a, 0x32, 0x9c, 0x5f, 0xe7, 0x5a, 0x94, 0x5e, 0xcb, 0x39, 0x95,
    0xe2, 0x77, 0xaa, 0xef, 0xca, 0x90, 0x46, 0xa4, 0xd5, 0xca, 0xb0, 0xb3, 0x3e, 0x72, 0xa7, 0x55, 0xd9, 0xbb, 0x7a, 0x53,
    0xca, 0x45, 0x7c, 0x77, 0xde, 0x9d, 0x94, 0xe3, 0xe7, 0xd9, 0xf8, 0x4f, 0xe3, 0x17, 0xe5, 0x52, 0xce, 0xe5, 0x6e, 0x46,
    0xb6, 0x6e, 0x26, 0xa7, 0x5e, 0x16, 0x81, 0x7e, 0xfc, 0x5f, 0x3b, 0x03, 0xbf, 0x1d, 0xff, 0x3b, 0xfb, 0xc7, 0x4c, 0xc4,
    0x7f, 0xb6, 0x3f, 0x39, 0x23, 0xe5, 0x5a, 0x79, 0x97, 0x55, 0xfc, 0x3c, 0x19, 0xff, 0xfd, 0x8c, 0x85, 0x67, 0xbf, 0x38,
    0x5b, 0xf1, 0x3f, 0xf9, 0xba, 0xa3, 0x99, 0x5f, 0x6b, 0x3f, 0xfe, 0xd3, 0xec, 0xb9, 0xce, 0xf5, 0x3d, 0xa7, 0xf2, 0x06,
    0x77, 0xae, 0xcf, 0x5c, 0xc8, 0xd7, 0xbb, 0xdb, 0x33, 0xa8, 0xe5, 0xae, 0xce, 0xb2, 0x35, 0xde, 0x8d, 0xff, 0x3c, 0xd8,
    0xff, 0xaf, 0x8c, 0xc8, 0x32, 0xde, 0xfe, 0x7f, 0x57, 0xfc, 0xdf, 0xd5, 0xff, 0xbf, 0xd2, 0x92, 0x75, 0xf7, 0xf5, 0x7a,
    0x3a, 0xfe, 0xef, 0x98, 0x97, 0xca, 0x48, 0xcb, 0xd5, 0xaf, 0x31, 0xae, 0xf6, 0xff, 0x33, 0xb4, 0x2b, 0x55, 0x6f, 0xdc,
    0x92, 0x9b, 0xdb, 0xff, 0x34, 0x46, 0x4a, 0xbb, 0xf1, 0x7f, 0x5c, 0xca, 0x83, 0x9a, 0xe6, 0xb9, 0xae, 0xd4, 0xe1, 0x3b,
    0x3b, 0xef, 0xed, 0xf4, 0xe0, 0xd3, 0xdc, 0x79, 0x2e, 0x8d, 0x91, 0xea, 0x6f, 0xc4, 0xff, 0xce, 0x2e, 0x5d, 0x4f, 0xf5,
    0xdc, 0x8f, 0x0b, 0x6d, 0xde, 0x1b, 0xe3, 0x7f, 0x7e, 0xfe, 0x6f, 0xfd, 0x1d, 0xf2, 0xf0, 0xf8, 0x3f, 0x8d, 0x28, 0x7e,
    0x5f, 0xfc, 0xd7, 0xdb, 0xa3, 0x99, 0xfd, 0x90, 0xa7, 0x6b, 0x80, 0xce, 0xee, 0x99, 0x9d, 0x9d, 0x53, 0x53, 0xda, 0x89,
    0x6c, 0xae, 0x47, 0x32, 0xd5, 0xff, 0x7f, 0x2e, 0xfe, 0xd3, 0xba, 0x0e, 0x3b, 0xdf, 0xbf, 0xbf, 0x3b, 0x40, 0xb6, 0x5a,
    0x95, 0x8c, 0xc6, 0xff, 0xc4, 0x2c, 0x5d, 0xb7, 0x4e, 0x7a, 0x7e, 0x3f, 0xf4, 0xe7, 0xee, 0x59, 0xa7, 0xb9, 0xab, 0x5b,
    0x46, 0xee, 0x8c, 0x67, 0x64, 0x2e, 0x2d, 0x5b, 0xf1, 0xbf, 0x7f, 0x2e, 0xaa, 0xfb, 0x32, 0xae, 0xfb, 0xc6, 0xf5, 0x79,
    0x91, 0x6e, 0xfc, 0xf7, 0x32, 0xd4, 0x5f, 0xfd, 0x1d, 0xff, 0x3c, 0x57, 0x72, 0x8c, 0xdf, 0x91, 0x4e, 0xab, 0xae, 0xea,
    0xf6, 0x2c, 0xd2, 0xda, 0x35, 0xff, 0x7d, 0xf1, 0xff, 0x44, 0xad, 0x31, 0x33, 0xf3, 0xd9, 0xb9, 0x17, 0x99, 0xf2, 0x35,
    0x3d, 0x3d, 0xc2, 0xfa, 0xfc, 0x19, 0xbd, 0x6f, 0x47, 0xa3, 0x37, 0x65, 0xe3, 0x9d, 0x88, 0xff, 0xa3, 0x71, 0xbf, 0x62,
    0xff, 0x4e, 0xdf, 0x77, 0x66, 0x35, 0xce, 0x8d, 0x4f, 0x25, 0xcc, 0xbd, 0xbf, 0x67, 0x10, 0x3f, 0x17, 0x75, 0x7f, 0x6b,
    0xa9, 0xfd, 0x1d, 0xf1, 0xff, 0x4c, 0x8d, 0x3d, 0x3d, 0x12, 0x81, 0x4f, 0x8e, 0xa9, 0x33, 0x32, 0x02, 0xe7, 0xb7, 0xda,
    0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa9, 0xf5, 0x63, 0x33, 0xf9, 0x89, 0xd6,
    0x6b, 0x9a, 0x6b, 0x2b, 0x21, 0xd2, 0x58, 0xaf, 0xbc, 0x97, 0x47, 0xbf, 0x9a, 0xd5, 0xe5, 0x7c, 0xbd, 0x4c, 0x2e, 0xae,
    0xa1, 0x59, 0xad, 0x10, 0xc9, 0xf6, 0x5a, 0xbc, 0xee, 0xae, 0x06, 0xab, 0x4f, 0xeb, 0x64, 0x65, 0x38, 0xdf, 0x4d, 0xa0,
    0xbb, 0x0f, 0xc0, 0x59, 0x96, 0x83, 0x94, 0x3f, 0xef, 0xec, 0x95, 0x29, 0xe7, 0x0a, 0x4b, 0x61, 0xed, 0x54, 0x5a, 0x57,
    0x68, 0x27, 0xa3, 0xc3, 0xff, 0x7e, 0x5e, 0xe5, 0x6c, 0x65, 0x79, 0xa5, 0x55, 0xb2, 0xf1, 0xd5, 0xf3, 0x9d, 0xac, 0x32,
    0x10, 0xf6, 0xf3, 0xe8, 0x67, 0x70, 0x05, 0x7e, 0x86, 0xfe, 0x5a, 0x59, 0x25, 0x96, 0xad, 0x3a, 0x3e, 0x43, 0x75, 0xed,
    0x6e, 0x79, 0xa5, 0x51, 0x73, 0xa7, 0x5d, 0xe3, 0x1f, 0xad, 0x0c, 0x09, 0x19, 0xc8, 0x08, 0x90, 0x56, 0x09, 0x9d, 0xb5,
    0x42, 0x69, 0xaf, 0x42, 0xac, 0x67, 0x39, 0x4a, 0xf9, 0x3b, 0x1e, 0x8d, 0x3c, 0xa8, 0xb3, 0xd9, 0x97, 0x76, 0xf6, 0xfd,
    0x98, 0xc9, 0x98, 0x39, 0xb5, 0x86, 0xbe, 0x97, 0x9b, 0xa9, 0xfe, 0xd9, 0x9d, 0xab, 0x2a, 0x1b, 0xbd, 0xd0, 0x14, 0x7f,
    0x5d, 0x2e, 0xb7, 0x71, 0x7f, 0x6e, 0xf5, 0xea, 0x71, 0x53, 0xcf, 0xa9, 0x34, 0x95, 0x3b, 0xe9, 0xae, 0xf8, 0x3f, 0xc6,
    0xb3, 0x9c, 0xf5, 0x7a, 0x21, 0xbb, 0x3d, 0xf5, 0xbd, 0xf8, 0x5f, 0xef, 0x52, 0xb4, 0x5f, 0xe3, 0xdf, 0x19, 0xff, 0xbd,
    0x5c, 0x88, 0x59, 0xf6, 0xba, 0xbb, 0x7b, 0xaf, 0xd5, 0xfa, 0xf1, 0x69, 0x67, 0x97, 0xed, 0xe5, 0xd1, 0xc8, 0x22, 0xb7,
    0x6c, 0x6f, 0xdd, 0xfe, 0x7e, 0xff, 0x7f, 0xdd, 0x8f, 0x3f, 0xab, 0x1b, 0xef, 0xc8, 0xff, 0x77, 0x7f, 0xfc, 0xd7, 0xb2,
    0x26, 0xf7, 0xda, 0xf0, 0x6b, 0x79, 0x74, 0x27, 0xb3, 0xdf, 0x7d, 0x2a, 0xfe, 0xeb, 0xe3, 0xea, 0xe9, 0x31, 0x45, 0xe7,
    0x0c, 0xe5, 0x42, 0x2e, 0xc3, 0xb4, 0xf2, 0xa7, 0x9e, 0x8f, 0x7a, 0x67, 0xb3, 0xf0, 0xdc, 0xf9, 0x3e, 0x57, 0xc6, 0x62,
    0xcf, 0xc5, 0x7f, 0x36, 0x46, 0xe0, 0x7b, 0xed, 0x56, 0x2e, 0xb7, 0x22, 0xf7, 0xc7, 0xff, 0xdc, 0x9e, 0x95, 0xfb, 0x79,
    0xfa, 0x7a, 0x7d, 0xfc, 0x6c, 0xcc, 0x82, 0x75, 0xce, 0xe8, 0xb5, 0xb9, 0x8e, 0x3c, 0x32, 0xeb, 0x9d, 0xad, 0xb8, 0xb9,
    0x7f, 0x4e, 0xbe, 0x36, 0x1b, 0x34, 0x1f, 0xff, 0x47, 0x71, 0xfc, 0x7d, 0x5c, 0xdc, 0xab, 0x63, 0x77, 0xdc, 0x9a, 0xad,
    0xfe, 0xf8, 0xec, 0x9c, 0x55, 0xfd, 0xbb, 0x3f, 0xdf, 0xff, 0xdf, 0xcb, 0x1a, 0x9b, 0x56, 0x3f, 0x38, 0x1b, 0x75, 0x69,
    0x27, 0xf3, 0xf2, 0x4e, 0x8c, 0x4d, 0xee, 0x53, 0x36, 0xf1, 0x1d, 0xcf, 0x47, 0x11, 0xd3, 0xf1, 0x5f, 0x2f, 0x85, 0x7c,
    0xb4, 0x1f, 0xd4, 0xbf, 0x5a, 0xfb, 0x11, 0x98, 0xc1, 0xb3, 0xf8, 0xfc, 0xfc, 0x5f, 0xda, 0x75, 0x5f, 0x36, 0x33, 0x9e,
    0x57, 0xdb, 0x8e, 0x67, 0xf3, 0x09, 0xe6, 0xa5, 0xf9, 0x0b, 0xfb, 0xb9, 0xff, 0xf3, 0x48, 0xa4, 0x4d, 0xe7, 0x9c, 0xbe,
    0xbf, 0x1e, 0xd9, 0xbb, 0x77, 0x96, 0xdb, 0xcf, 0x62, 0x6e, 0x7f, 0xf7, 0x2c, 0x9f, 0x33, 0xe8, 0xde, 0xdf, 0xc8, 0xe0,
    0xfd, 0x44, 0x8e, 0xad, 0x79, 0x9a, 0x7c, 0xf8, 0x3b, 0x1e, 0xce, 0xef, 0x43, 0xf1, 0xff, 0xe9, 0x72, 0xef, 0xcf, 0xd5,
    0xc9, 0x7a, 0xfb, 0xfd, 0xf1, 0xcf, 0x3b, 0xe3, 0xff, 0x78, 0x49, 0xfc, 0xf3, 0x7c, 0x0d, 0x2e, 0xfe, 0xdf, 0x77, 0x2e,
    0x77, 0x9f, 0xff, 0x7d, 0xfb, 0xaf, 0xd3, 0x96, 0xdf, 0x55, 0xaa, 0xca, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x58, 0xaf, 0x1c, 0xca, 0x50, 0x46, 0xa0, 0xe3, 0xff, 0x72, 0x97, 0xac, 0x73, 0x4d, 0x76, 0xf2,
    0xc6, 0xf7, 0x57, 0x53, 0xa5, 0x7d, 0x6c, 0x27, 0x17, 0x4b, 0x2e, 0xad, 0x00, 0xc9, 0xc8, 0x7e, 0x09, 0xfd, 0x52, 0x4a,
    0x29, 0x9b, 0x68, 0x25, 0x7f, 0x5b, 0x1a, 0x2b, 0x83, 0x7b, 0x59, 0x9f, 0x3e, 0x75, 0x9e, 0x57, 0x19, 0x57, 0xfa, 0xbf,
    0x26, 0x37, 0xac, 0x1e, 0xbc, 0xfa, 0x3d, 0x7a, 0x59, 0x58, 0x73, 0x69, 0xa5, 0x5b, 0xda, 0xb9, 0x5c, 0xfb, 0xeb, 0x29,
    0xd3, 0x5c, 0x4f, 0x99, 0xed, 0xd2, 0x5e, 0x45, 0x63, 0x2f, 0x5f, 0x6a, 0xb7, 0x8e, 0xdc, 0xcf, 0xf0, 0x9d, 0xed, 0x77,
    0xbe, 0xb2, 0x47, 0x41, 0xbe, 0xe8, 0x3c, 0x77, 0xeb, 0x86, 0xfe, 0xef, 0xcc, 0xed, 0xb1, 0xb3, 0x9b, 0xf9, 0xef, 0xca,
    0x55, 0xdf, 0xdb, 0x87, 0xe6, 0x8d, 0xab, 0x38, 0x77, 0xea, 0xf0, 0x6e, 0x1e, 0xb3, 0x6f, 0x58, 0xdf, 0x9b, 0x8f, 0xfe,
    0xae, 0xe7, 0xce, 0xf3, 0x3a, 0xcf, 0xe1, 0x1b, 0x7e, 0x6b, 0x25, 0xc3, 0x6b, 0xa7, 0xfd, 0xcf, 0xa5, 0xf2, 0xba, 0x92,
    0xdd, 0xb4, 0xb7, 0xeb, 0xd5, 0x67, 0x8e, 0x1e, 0x9b, 0xd9, 0xb6, 0x3a, 0xd9, 0xae, 0xcf, 0x77, 0xac, 0xaa, 0x1f, 0xd9,
    0x6d, 0xff, 0xbb, 0x7f, 0xed, 0xec, 0x43, 0xf4, 0x89, 0xf3, 0xdc, 0xdb, 0x1d, 0x2d, 0x8b, 0x51, 0xf7, 0x95, 0x32, 0xae,
    0x1d, 0xdb, 0xed, 0x25, 0xef, 0x8d, 0xff, 0xab, 0xed, 0xff, 0x51, 0xbe, 0xea, 0x77, 0x7a, 0xda, 0x6f, 0x3b, 0xba, 0x93,
    0x71, 0xf3, 0xa9, 0x23, 0x77, 0xc4, 0x7f, 0x4e, 0xb3, 0xf1, 0xe7, 0xd2, 0x2c, 0xc3, 0x4e, 0x66, 0xe7, 0xfa, 0xd1, 0x5c,
    0x2a, 0x8f, 0xf3, 0x36, 0xf4, 0xfb, 0xe2, 0x7f, 0xef, 0xbd, 0xd7, 0xf1, 0x5d, 0xcf, 0x50, 0x7b, 0xb4, 0xe7, 0xe1, 0xee,
    0x8b, 0xe1, 0xa3, 0xfd, 0xad, 0xf2, 0x9a, 0x28, 0xaf, 0xe5, 0x04, 0xbd, 0xbb, 0xfd, 0xbf, 0x96, 0xdd, 0xfc, 0x3d, 0x47,
    0xaf, 0xb5, 0x49, 0xef, 0x88, 0xff, 0xdc, 0x3a, 0x26, 0x4c, 0x69, 0xa6, 0xf5, 0x28, 0xee, 0x7d, 0x94, 0xd7, 0xb5, 0x0b,
    0xeb, 0x3d, 0x35, 0xae, 0xb4, 0x29, 0xb3, 0x11, 0xdb, 0x3b, 0xf2, 0xa6, 0xfe, 0xff, 0x2f, 0xc5, 0xff, 0x1d, 0x31, 0xfe,
    0x4c, 0xfc, 0xef, 0xf5, 0x00, 0xfa, 0xfb, 0x2d, 0x75, 0x6a, 0x86, 0x9d, 0xbd, 0x6d, 0x76, 0x8f, 0x76, 0x6b, 0x87, 0xfc,
    0xa1, 0x36, 0xec, 0xcd, 0xb4, 0x7e, 0x5b, 0xff, 0xff, 0xfc, 0xfe, 0x4f, 0xda, 0xf3, 0xcb, 0xef, 0x8b, 0xff, 0x7e, 0x0b,
    0x70, 0x4f, 0xfc, 0xe7, 0xc1, 0x79, 0xd2, 0xb4, 0x67, 0x3d, 0xd3, 0xb8, 0x6f, 0xfc, 0xc6, 0x71, 0x61, 0xe5, 0xec, 0x77,
    0xf6, 0xd0, 0xfc, 0xee, 0xf8, 0x5f, 0xcf, 0x2f, 0xfd, 0x4a, 0xff, 0x3f, 0x1b, 0xb3, 0xa0, 0xd3, 0xf1, 0xff, 0x3d, 0xf9,
    0x46, 0x33, 0x36, 0xee, 0xf8, 0xbe, 0x56, 0xa3, 0x1b, 0x57, 0x3b, 0xa3, 0x89, 0xeb, 0x73, 0xae, 0xcf, 0xec, 0x76, 0x92,
    0x57, 0x9d, 0xab, 0x3b, 0xee, 0x58, 0x7e, 0x66, 0xfe, 0xef, 0x5b, 0x22, 0x5f, 0xfc, 0xaf, 0xe6, 0xd0, 0xcf, 0xc7, 0xcf,
    0xb5, 0x23, 0x95, 0xe7, 0x0c, 0x3f, 0x17, 0xff, 0x9f, 0x9b, 0xe7, 0xb9, 0x67, 0x37, 0x93, 0xce, 0x33, 0x36, 0xc7, 0x2d,
    0xcf, 0xce, 0xbd, 0xef, 0x59, 0x8b, 0xe3, 0x0b, 0xe2, 0x7f, 0x66, 0x36, 0xb4, 0x53, 0x12, 0x59, 0xec, 0x8b, 0x9c, 0xe2,
    0x33, 0xd7, 0x77, 0xf5, 0xff, 0x67, 0x5b, 0xc5, 0x4f, 0x3e, 0xe7, 0xf1, 0xfc, 0x95, 0xde, 0x3d, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xf6, 0xda, 0xba, 0x7a, 0xd6, 0xbd, 0x34, 0xd7, 0xef, 0xf6, 0xd7,
    0xa9, 0xd6, 0x77, 0x0d, 0xb8, 0xf2, 0x9e, 0x29, 0xaf, 0xbf, 0xa9, 0x7d, 0x52, 0xe5, 0x5b, 0xdf, 0x93, 0x75, 0xff, 0x58,
    0xe4, 0x18, 0xc8, 0xc8, 0x35, 0x91, 0x72, 0x4e, 0x87, 0xa3, 0x99, 0x95, 0x28, 0xe5, 0xab, 0x2e, 0x8d, 0xbf, 0xa7, 0xbc,
    0x32, 0xf3, 0x68, 0x7c, 0xef, 0x37, 0xaf, 0xb9, 0xef, 0x66, 0xf2, 0xec, 0xef, 0x50, 0x31, 0xbd, 0xea, 0x31, 0xcd, 0x3c,
    0xdc, 0x19, 0x5b, 0xef, 0x9a, 0x8d, 0x15, 0xcd, 0x9d, 0x48, 0xaf, 0xd4, 0xd7, 0x69, 0xc7, 0x46, 0xad, 0x3e, 0xcb, 0xe8,
    0x75, 0x3a, 0xb7, 0x43, 0x4c, 0x46, 0x33, 0x69, 0xbe, 0x79, 0x5d, 0x5e, 0x9a, 0xd9, 0x35, 0xf2, 0x05, 0xeb, 0x90, 0xb3,
    0xb1, 0x5b, 0x4a, 0x1a, 0x3b, 0x9d, 0x7c, 0x66, 0xa5, 0xf3, 0xde, 0x95, 0xf9, 0xce, 0x55, 0xa3, 0xd5, 0x7c, 0x6e, 0x4f,
    0x5d, 0x75, 0xbd, 0x9c, 0x1f, 0x19, 0x7f, 0xc7, 0x67, 0xe2, 0xbf, 0x93, 0x77, 0x37, 0xcb, 0x3e, 0xe5, 0xb3, 0xab, 0x9e,
    0xbb, 0xd9, 0xc6, 0xd2, 0xda, 0x6d, 0x60, 0xff, 0xef, 0x9f, 0xc8, 0xba, 0x73, 0x6f, 0xfb, 0x3f, 0xdf, 0xc7, 0xae, 0x66,
    0x38, 0xe8, 0x5d, 0xdd, 0xdd, 0x9d, 0x2e, 0xf2, 0x33, 0xf1, 0xbf, 0xce, 0xba, 0x98, 0x91, 0xd9, 0x84, 0x4f, 0x1c, 0x4b,
    0x73, 0xd7, 0xa7, 0x0c, 0x8d, 0x17, 0xd6, 0xb9, 0x2e, 0x9f, 0x88, 0xff, 0x9c, 0xf4, 0xce, 0xd3, 0xd8, 0xd9, 0x62, 0xea,
    0xef, 0x93, 0xb3, 0x15, 0xfd, 0x5d, 0x6b, 0xe6, 0xe2, 0x3f, 0xc3, 0x3b, 0xb0, 0xbd, 0x3d, 0xfe, 0xa7, 0x67, 0x00, 0xfa,
    0x31, 0x9e, 0xf6, 0x68, 0x6d, 0x62, 0x94, 0xdb, 0xf9, 0xfb, 0x73, 0x59, 0xf7, 0xcf, 0xf2, 0x2a, 0xa7, 0x75, 0x45, 0x64,
    0x60, 0x1e, 0x25, 0xed, 0x1e, 0xf6, 0xfd, 0xf1, 0xbf, 0xb3, 0xe7, 0x4f, 0xbe, 0x2e, 0xfe, 0x57, 0x35, 0x5a, 0x3d, 0xbf,
    0x6d, 0x5a, 0xb1, 0xda, 0x3b, 0x96, 0x8d, 0xd9, 0x9f, 0x2c, 0xb3, 0xe6, 0xdf, 0xd3, 0xfb, 0xaf, 0x7d, 0xdb, 0x99, 0xf8,
    0x9f, 0x89, 0xce, 0x4f, 0xc6, 0x7f, 0x36, 0xf6, 0x8e, 0x99, 0x6c, 0xdd, 0xd2, 0xcc, 0x14, 0x9a, 0x97, 0xce, 0xff, 0x75,
    0x46, 0x34, 0xf3, 0x7b, 0x75, 0xdc, 0x71, 0x6c, 0x6f, 0xe6, 0x2b, 0x37, 0x8e, 0x7d, 0x8f, 0x0f, 0xb4, 0xff, 0x13, 0x35,
    0xc3, 0x67, 0xdb, 0xff, 0x63, 0xf4, 0x7a, 0x5c, 0xff, 0xfa, 0x3c, 0xd0, 0xa3, 0x7c, 0x4b, 0xfc, 0xe7, 0x4b, 0xe3, 0xff,
    0xb8, 0xb0, 0xa3, 0xdc, 0x5b, 0x6b, 0x80, 0x3c, 0x3a, 0xfe, 0x7f, 0x57, 0xfc, 0xcf, 0xde, 0x01, 0xc9, 0xf2, 0x1e, 0x67,
    0x2d, 0xca, 0xd3, 0xb8, 0xd3, 0x98, 0xe2, 0x6e, 0x6a, 0xef, 0x79, 0xfe, 0x27, 0xcd, 0xd1, 0x49, 0x35, 0x1e, 0xef, 0x39,
    0x76, 0xcf, 0xac, 0x6b, 0xca, 0x73, 0x8a, 0x19, 0xb9, 0xce, 0x9f, 0xcc, 0xba, 0xbb, 0x1a, 0xc5, 0x3d, 0xff, 0xeb, 0x77,
    0xef, 0xa7, 0x66, 0xe8, 0xc8, 0xe4, 0xb3, 0x44, 0x6f, 0x79, 0x3a, 0x60, 0xfa, 0x1e, 0xda, 0x37, 0xe4, 0x21, 0xce, 0xcf,
    0x67, 0x4c, 0xbf, 0xef, 0xa9, 0x01, 0xee, 0xed, 0xe9, 0x7d, 0xcf, 0xb3, 0xc1, 0x88, 0x7e, 0xd6, 0xcf, 0xa8, 0xfc, 0xe6,
    0xef, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xa6, 0xfd, 0xfb, 0x9f, 0x72, 0x00, 0xf1, 0x0f, 0xfc, 0x75, 0xf1, 0xff, 0x2f };

// Font glyphs rectangles data (on atlas)
static const Rectangle lavandaFontRecs[189] = {
    { 4, 4, 5 , 16 },
    { 17, 4, 1 , 9 },
    { 26, 4, 3 , 3 },
    { 37, 4, 7 , 8 },
    { 52, 4, 5 , 11 },
    { 65, 4, 10 , 8 },
    { 83, 4, 7 , 9 },
    { 98, 4, 1 , 3 },
    { 107, 4, 3 , 12 },
    { 118, 4, 3 , 12 },
    { 129, 4, 5 , 4 },
    { 142, 4, 5 , 5 },
    { 155, 4, 2 , 3 },
    { 165, 4, 3 , 1 },
    { 176, 4, 1 , 1 },
    { 185, 4, 4 , 12 },
    { 197, 4, 5 , 9 },
    { 210, 4, 3 , 9 },
    { 221, 4, 5 , 9 },
    { 234, 4, 5 , 9 },
    { 247, 4, 5 , 9 },
    { 260, 4, 5 , 9 },
    { 273, 4, 5 , 9 },
    { 286, 4, 5 , 9 },
    { 299, 4, 5 , 9 },
    { 312, 4, 5 , 9 },
    { 325, 4, 1 , 4 },
    { 334, 4, 2 , 6 },
    { 344, 4, 4 , 5 },
    { 356, 4, 4 , 3 },
    { 368, 4, 4 , 5 },
    { 380, 4, 5 , 9 },
    { 393, 4, 7 , 10 },
    { 408, 4, 7 , 9 },
    { 423, 4, 6 , 9 },
    { 437, 4, 5 , 9 },
    { 450, 4, 6 , 9 },
    { 464, 4, 5 , 9 },
    { 477, 4, 5 , 9 },
    { 490, 4, 6 , 9 },
    { 4, 28, 5 , 10 },
    { 17, 28, 1 , 9 },
    { 26, 28, 6 , 9 },
    { 40, 28, 6 , 9 },
    { 54, 28, 5 , 9 },
    { 67, 28, 8 , 11 },
    { 83, 28, 6 , 10 },
    { 97, 28, 7 , 9 },
    { 112, 28, 5 , 9 },
    { 125, 28, 7 , 11 },
    { 140, 28, 5 , 9 },
    { 153, 28, 6 , 9 },
    { 167, 28, 7 , 9 },
    { 182, 28, 6 , 9 },
    { 196, 28, 6 , 10 },
    { 210, 28, 9 , 10 },
    { 227, 28, 6 , 11 },
    { 241, 28, 5 , 10 },
    { 254, 28, 5 , 9 },
    { 267, 28, 3 , 12 },
    { 278, 28, 4 , 12 },
    { 290, 28, 3 , 12 },
    { 301, 28, 5 , 4 },
    { 314, 28, 6 , 1 },
    { 328, 28, 3 , 3 },
    { 339, 28, 6 , 7 },
    { 353, 28, 5 , 9 },
    { 366, 28, 4 , 7 },
    { 378, 28, 5 , 10 },
    { 391, 28, 4 , 7 },
    { 403, 28, 3 , 10 },
    { 414, 28, 4 , 11 },
    { 426, 28, 5 , 11 },
    { 439, 28, 1 , 9 },
    { 448, 28, 5 , 12 },
    { 461, 28, 5 , 9 },
    { 474, 28, 1 , 9 },
    { 483, 28, 8 , 9 },
    { 4, 52, 5 , 9 },
    { 17, 52, 4 , 7 },
    { 29, 52, 5 , 10 },
    { 42, 52, 5 , 10 },
    { 55, 52, 5 , 7 },
    { 68, 52, 5 , 7 },
    { 81, 52, 3 , 9 },
    { 92, 52, 5 , 7 },
    { 105, 52, 5 , 8 },
    { 118, 52, 9 , 8 },
    { 135, 52, 5 , 9 },
    { 148, 52, 6 , 9 },
    { 162, 52, 4 , 7 },
    { 174, 52, 5 , 12 },
    { 187, 52, 1 , 12 },
    { 196, 52, 5 , 12 },
    { 209, 52, 6 , 2 },
    { 223, 52, 1 , 9 },
    { 232, 52, 5 , 10 },
    { 245, 52, 6 , 9 },
    { 259, 52, 7 , 9 },
    { 274, 52, 5 , 9 },
    { 287, 52, 6 , 12 },
    { 301, 52, 5 , 10 },
    { 314, 52, 5 , 10 },
    { 327, 52, 8 , 8 },
    { 343, 52, 4 , 7 },
    { 355, 52, 6 , 5 },
    { 369, 52, 6 , 3 },
    { 383, 52, 8 , 8 },
    { 399, 52, 5 , 1 },
    { 412, 52, 4 , 4 },
    { 424, 52, 5 , 7 },
    { 437, 52, 3 , 4 },
    { 448, 52, 3 , 4 },
    { 459, 52, 5 , 12 },
    { 472, 52, 5 , 10 },
    { 485, 52, 7 , 11 },
    { 500, 52, 1 , 1 },
    { 4, 76, 4 , 10 },
    { 16, 76, 2 , 4 },
    { 26, 76, 4 , 7 },
    { 38, 76, 6 , 5 },
    { 52, 76, 11 , 9 },
    { 71, 76, 7 , 7 },
    { 86, 76, 5 , 12 },
    { 99, 76, 5 , 9 },
    { 112, 76, 7 , 12 },
    { 127, 76, 7 , 12 },
    { 142, 76, 7 , 12 },
    { 157, 76, 7 , 12 },
    { 172, 76, 7 , 11 },
    { 187, 76, 7 , 12 },
    { 202, 76, 10 , 9 },
    { 220, 76, 5 , 12 },
    { 233, 76, 5 , 12 },
    { 246, 76, 5 , 12 },
    { 259, 76, 5 , 12 },
    { 272, 76, 5 , 11 },
    { 285, 76, 2 , 12 },
    { 295, 76, 2 , 12 },
    { 305, 76, 3 , 12 },
    { 316, 76, 3 , 11 },
    { 327, 76, 7 , 9 },
    { 342, 76, 6 , 12 },
    { 356, 76, 7 , 12 },
    { 371, 76, 7 , 12 },
    { 386, 76, 7 , 12 },
    { 401, 76, 7 , 12 },
    { 416, 76, 7 , 11 },
    { 431, 76, 5 , 5 },
    { 444, 76, 7 , 9 },
    { 459, 76, 6 , 12 },
    { 473, 76, 6 , 12 },
    { 487, 76, 6 , 12 },
    { 4, 100, 6 , 11 },
    { 18, 100, 5 , 13 },
    { 31, 100, 5 , 9 },
    { 44, 100, 5 , 11 },
    { 57, 100, 6 , 10 },
    { 71, 100, 6 , 10 },
    { 85, 100, 6 , 10 },
    { 99, 100, 6 , 10 },
    { 113, 100, 6 , 9 },
    { 127, 100, 6 , 11 },
    { 141, 100, 7 , 7 },
    { 156, 100, 4 , 10 },
    { 168, 100, 4 , 10 },
    { 180, 100, 4 , 10 },
    { 192, 100, 4 , 10 },
    { 204, 100, 4 , 9 },
    { 216, 100, 2 , 10 },
    { 226, 100, 2 , 10 },
    { 236, 100, 3 , 10 },
    { 247, 100, 3 , 9 },
    { 258, 100, 5 , 9 },
    { 271, 100, 5 , 12 },
    { 284, 100, 4 , 10 },
    { 296, 100, 4 , 10 },
    { 308, 100, 4 , 10 },
    { 320, 100, 5 , 10 },
    { 333, 100, 4 , 9 },
    { 345, 100, 5 , 5 },
    { 358, 100, 6 , 7 },
    { 372, 100, 5 , 10 },
    { 385, 100, 5 , 10 },
    { 398, 100, 5 , 10 },
    { 411, 100, 5 , 9 },
    { 424, 100, 6 , 12 },
    { 438, 100, 5 , 12 },
    { 451, 100, 6 , 11 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
static const GlyphInfo lavandaFontGlyphs[189] = {
    { 32, 0, 12, 5, { 0 }},
    { 33, 0, 3, 2, { 0 }},
    { 34, 0, 3, 4, { 0 }},
    { 35, 0, 3, 8, { 0 }},
    { 36, 0, 2, 6, { 0 }},
    { 37, 0, 4, 11, { 0 }},
    { 38, 0, 3, 8, { 0 }},
    { 39, 0, 3, 2, { 0 }},
    { 40, 0, 2, 4, { 0 }},
    { 41, 0, 2, 4, { 0 }},
    { 42, 0, 3, 6, { 0 }},
    { 43, 0, 5, 6, { 0 }},
    { 44, 0, 10, 3, { 0 }},
    { 45, 0, 7, 4, { 0 }},
    { 46, 0, 11, 2, { 0 }},
    { 47, 0, 2, 5, { 0 }},
    { 48, 0, 3, 6, { 0 }},
    { 49, 0, 3, 4, { 0 }},
    { 50, 0, 3, 6, { 0 }},
    { 51, 0, 3, 6, { 0 }},
    { 52, 0, 3, 6, { 0 }},
    { 53, 0, 3, 6, { 0 }},
    { 54, 0, 3, 6, { 0 }},
    { 55, 0, 3, 6, { 0 }},
    { 56, 0, 3, 6, { 0 }},
    { 57, 0, 3, 6, { 0 }},
    { 58, 0, 7, 2, { 0 }},
    { 59, 0, 7, 3, { 0 }},
    { 60, 0, 5, 5, { 0 }},
    { 61, 0, 6, 5, { 0 }},
    { 62, 0, 5, 5, { 0 }},
    { 63, 0, 3, 6, { 0 }},
    { 64, 0, 4, 8, { 0 }},
    { 65, 0, 3, 8, { 0 }},
    { 66, 0, 3, 7, { 0 }},
    { 67, 0, 3, 6, { 0 }},
    { 68, 0, 3, 7, { 0 }},
    { 69, 0, 3, 6, { 0 }},
    { 70, 0, 3, 6, { 0 }},
    { 71, 0, 3, 7, { 0 }},
    { 72, 0, 2, 6, { 0 }},
    { 73, 0, 3, 2, { 0 }},
    { 74, 0, 3, 7, { 0 }},
    { 75, 0, 3, 7, { 0 }},
    { 76, 0, 3, 6, { 0 }},
    { 77, 0, 3, 9, { 0 }},
    { 78, 0, 2, 7, { 0 }},
    { 79, 0, 3, 8, { 0 }},
    { 80, 0, 3, 6, { 0 }},
    { 81, 0, 3, 8, { 0 }},
    { 82, 0, 3, 6, { 0 }},
    { 83, 0, 3, 7, { 0 }},
    { 84, 0, 3, 8, { 0 }},
    { 85, 0, 3, 7, { 0 }},
    { 86, 0, 2, 7, { 0 }},
    { 87, 0, 2, 10, { 0 }},
    { 88, 0, 3, 7, { 0 }},
    { 89, 0, 3, 6, { 0 }},
    { 90, 0, 3, 6, { 0 }},
    { 91, 0, 2, 4, { 0 }},
    { 92, 0, 2, 5, { 0 }},
    { 93, 0, 2, 4, { 0 }},
    { 94, 0, 3, 6, { 0 }},
    { 95, 0, 13, 7, { 0 }},
    { 96, 0, 3, 4, { 0 }},
    { 97, 0, 5, 7, { 0 }},
    { 98, 0, 3, 6, { 0 }},
    { 99, 0, 5, 5, { 0 }},
    { 100, 0, 2, 6, { 0 }},
    { 101, 0, 5, 5, { 0 }},
    { 102, 0, 2, 4, { 0 }},
    { 103, 0, 4, 5, { 0 }},
    { 104, 0, 3, 6, { 0 }},
    { 105, 0, 3, 2, { 0 }},
    { 106, -1, 3, 5, { 0 }},
    { 107, 0, 3, 6, { 0 }},
    { 108, 0, 3, 2, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 6, { 0 }},
    { 111, 0, 5, 5, { 0 }},
    { 112, 0, 5, 6, { 0 }},
    { 113, 0, 5, 6, { 0 }},
    { 114, 0, 5, 6, { 0 }},
    { 115, 0, 5, 6, { 0 }},
    { 116, 0, 3, 4, { 0 }},
    { 117, 0, 5, 6, { 0 }},
    { 118, 0, 4, 6, { 0 }},
    { 119, 0, 4, 10, { 0 }},
    { 120, 0, 5, 6, { 0 }},
    { 121, -1, 5, 6, { 0 }},
    { 122, 0, 5, 5, { 0 }},
    { 123, 0, 2, 6, { 0 }},
    { 124, 0, 2, 2, { 0 }},
    { 125, 0, 2, 6, { 0 }},
    { 126, 0, 7, 7, { 0 }},
    { 161, 0, 5, 2, { 0 }},
    { 162, 0, 4, 6, { 0 }},
    { 163, 0, 3, 7, { 0 }},
    { 8364, 0, 3, 8, { 0 }},
    { 165, 0, 3, 6, { 0 }},
    { 352, 0, 0, 7, { 0 }},
    { 167, 0, 3, 6, { 0 }},
    { 353, 0, 2, 6, { 0 }},
    { 169, 0, 4, 9, { 0 }},
    { 170, 0, 3, 5, { 0 }},
    { 171, 0, 6, 7, { 0 }},
    { 172, 0, 6, 7, { 0 }},
    { 174, 0, 4, 9, { 0 }},
    { 175, 0, 2, 6, { 0 }},
    { 176, 0, 3, 5, { 0 }},
    { 177, 0, 5, 6, { 0 }},
    { 178, 0, 3, 4, { 0 }},
    { 179, 0, 3, 4, { 0 }},
    { 381, 0, 0, 6, { 0 }},
    { 181, 0, 5, 6, { 0 }},
    { 182, 0, 3, 8, { 0 }},
    { 183, 0, 7, 2, { 0 }},
    { 382, 0, 2, 5, { 0 }},
    { 185, 0, 3, 3, { 0 }},
    { 186, 0, 3, 5, { 0 }},
    { 187, 0, 6, 7, { 0 }},
    { 338, 0, 3, 12, { 0 }},
    { 339, 0, 5, 8, { 0 }},
    { 376, 0, 1, 6, { 0 }},
    { 191, 0, 5, 6, { 0 }},
    { 192, 0, 0, 8, { 0 }},
    { 193, 0, 0, 8, { 0 }},
    { 194, 0, 0, 8, { 0 }},
    { 195, 0, 0, 8, { 0 }},
    { 196, 0, 1, 8, { 0 }},
    { 197, 0, 0, 8, { 0 }},
    { 198, 0, 3, 11, { 0 }},
    { 199, 0, 3, 6, { 0 }},
    { 200, 0, 0, 6, { 0 }},
    { 201, 0, 0, 6, { 0 }},
    { 202, 0, 0, 6, { 0 }},
    { 203, 0, 1, 6, { 0 }},
    { 204, -1, 0, 2, { 0 }},
    { 205, 0, 0, 3, { 0 }},
    { 206, -1, 0, 3, { 0 }},
    { 207, -1, 1, 3, { 0 }},
    { 208, -1, 3, 7, { 0 }},
    { 209, 0, 0, 7, { 0 }},
    { 210, 0, 0, 8, { 0 }},
    { 211, 0, 0, 8, { 0 }},
    { 212, 0, 0, 8, { 0 }},
    { 213, 0, 0, 8, { 0 }},
    { 214, 0, 1, 8, { 0 }},
    { 215, 0, 5, 6, { 0 }},
    { 216, 0, 3, 8, { 0 }},
    { 217, 0, 0, 7, { 0 }},
    { 218, 0, 0, 7, { 0 }},
    { 219, 0, 0, 7, { 0 }},
    { 220, 0, 1, 7, { 0 }},
    { 221, 0, 0, 6, { 0 }},
    { 222, 0, 3, 6, { 0 }},
    { 223, 0, 3, 6, { 0 }},
    { 224, 0, 2, 7, { 0 }},
    { 225, 0, 2, 7, { 0 }},
    { 226, 0, 2, 7, { 0 }},
    { 227, 0, 2, 7, { 0 }},
    { 228, 0, 3, 7, { 0 }},
    { 229, 0, 1, 7, { 0 }},
    { 230, 0, 5, 8, { 0 }},
    { 231, 0, 5, 5, { 0 }},
    { 232, 0, 2, 5, { 0 }},
    { 233, 0, 2, 5, { 0 }},
    { 234, 0, 2, 5, { 0 }},
    { 235, 0, 3, 5, { 0 }},
    { 236, -1, 2, 2, { 0 }},
    { 237, 0, 2, 3, { 0 }},
    { 238, -1, 2, 3, { 0 }},
    { 239, -1, 3, 3, { 0 }},
    { 240, 0, 3, 6, { 0 }},
    { 241, 0, 2, 6, { 0 }},
    { 242, 0, 2, 5, { 0 }},
    { 243, 0, 2, 5, { 0 }},
    { 244, 0, 2, 5, { 0 }},
    { 245, 0, 2, 6, { 0 }},
    { 246, 0, 3, 5, { 0 }},
    { 247, 0, 5, 6, { 0 }},
    { 248, -1, 5, 6, { 0 }},
    { 249, 0, 2, 6, { 0 }},
    { 250, 0, 2, 6, { 0 }},
    { 251, 0, 2, 6, { 0 }},
    { 252, 0, 3, 6, { 0 }},
    { 253, -1, 2, 6, { 0 }},
    { 254, 0, 3, 6, { 0 }},
    { 255, -1, 3, 6, { 0 }},
};

// Style loading function: Lavanda
static void GuiLoadStyleLavanda(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < LAVANDA_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(lavandaStyleProps[i].controlId, lavandaStyleProps[i].propertyId, lavandaStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int lavandaFontDataSize = 0;
    unsigned char *data = DecompressData(lavandaFontData, LAVANDA_STYLE_FONT_ATLAS_COMP_SIZE, &lavandaFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, lavandaFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, lavandaFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}

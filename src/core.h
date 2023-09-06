#ifndef CORE_H
#define CORE_H

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;

typedef signed char i8;
typedef signed short int i16;
typedef signed int i32;
typedef signed long int i64;

typedef float f32;
typedef double f64;

#ifndef PI
#define PI 3.14159265358979f
#endif

#ifndef RAYLIB_H
typedef u32 Color;
#endif

// NOTE: reverse rgba
#define COL_BG     *(Color*) (u32[1]) { 0xff282828 }
#define COL_FG     *(Color*) (u32[1]) { 0xffb2dbeb }
#define COL_BLUE   *(Color*) (u32[1]) { 0xff888545 }
#define COL_AQUA   *(Color*) (u32[1]) { 0xff6a9d68 }
#define COL_PURPLE *(Color*) (u32[1]) { 0xff8662b1 }
#define COL_RED    *(Color*) (u32[1]) { 0xff1d24cc }
#define COL_GREEN  *(Color*) (u32[1]) { 0xff1a9798 }
#define COL_YELLOW *(Color*) (u32[1]) { 0xff2199d7 }
#define COL_ORANGE *(Color*) (u32[1]) { 0xff0e5dd6 }

#endif // CORE_H

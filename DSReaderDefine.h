#ifndef __DSReaderDefine_h
#define __DSReaderDefine_h

#include <string>
#include<vector>
#ifdef __cplusplus
#define DLLExport  extern "C" __declspec(dllexport)
#else
#define DLLExport  extern "C"
#endif

typedef void ( *CallBackFunction)(int step, int proc);

#ifdef __cplusplus
	extern "C" {
#endif


#ifndef UNICODE                    // UNICODE 2 need  define this
	typedef  char     DSR_CHAR; 
#else
	typedef  wchar_t  DSR_CHAR;
#endif


//            状态信息
// --------------------------------------------------------------------

//#define DCMLIB_S_OK                    0       // 成功
//#define DCMLIB_S_FAILURE               1       // 失败
//#define DCMLIB_S_WORKING               2       // 执行中
//
//#define DCMLIB_E_UNEXPECTED           0x81     // 异常
//#define DCMLIB_E_INTERNALE            0x82     // 内部错误
//#define DCMLIB_E_OUT_OF_MEM           0x83     // 内存不足
//#define DCMLIB_E_SERVER_NOT_RESPONE   0x84     // 服务器无响应
//#define DCMLIB_E_FAILED_TO_EXEC       0x85     // 执行失败
//#define DCMLIB_E_ACCESS_DENIED        0x86     // 拒绝访问
//#define DCMLIB_E_INPUT_FILENOFOUND    0x87     // 输入文件不存在
//
//
//#define DCMLIB_F_NETWORK              0x90     // 网络连接失败，请确认网络连接正常；或服务端设置正常
//#define DCMLIB_F_INVALID_OBJ          0x91     // 对象指针无效
//#define DCMLIB_F_INVALID_QUERY        0x92     // 无效的查询条件

/***********************************************************/
//定义返回的状态信息
/***********************************************************/
	enum E_GDCM_Info
	{
		E_OK,				//成功
		E_DIR_NULL,			//路径为空
		E_FILE_FAIL,		//文件获取失败
		E_SLICE_FAIL,		//切片获取失败
		E_SLICE_ONLY_ONE,	//切片只有一张
		E_SLICE_TOO_LESS,	//切片数量过少
		E_BUFF_NULL,		//内存为空
		E_DSREADER_NULL		//DSReader为空
	};
/***********************************************************/
//定义数据类型
/***********************************************************/
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef long long int int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long int uint64;

	typedef float real32;
	typedef double real64;


//#define BasicMsgTextSize 23

typedef	struct 
{
	int SeriesID;				//序列的序号
	int SeriesNumber;			//序列号
	std::vector<std::string*> *filenames;//相同序列的文件名
	int16 *pSingleData;			//单张图像数据
	int16 ImgSizeX;				//切片图像的宽度
	int16 ImgSizeY;				//切片图像的高度
	real32 ImgSpacingX;		    //x方向的像素间距
	real32 ImgSpacingY;			//y方向的像素间距
	real32 ImgThickness;		//图像层厚
	int16    windowCenter;		//窗位
	int16    windowWidth;		//窗宽
	real32    rescaleIntercept;	//灰度转HU值的截距
	real32    rescaleSlope;		//灰度转HU值的斜率

	std::string  SeriesDescription;	//序列描述
	std::string  AccessionNumber;	//检查号
	std::string  admissionid;		//病人ID
	std::string  name;				//病人姓名
	std::string  gender;			//病人性别
	std::string  age;				//病人年龄

	std::string  InstitutionName;	//检查医院
	std::string  part;				//检查部位
	std::string  date;				//检查日期
	std::string  AcquisitionTime;	//检查时间
	std::string  doctor;			//医生姓名
	std::string  modality;			//检查的所用模态
	int16  XrayTubeCurrent;			//曝光电流
	int16  KVP;						//曝光电压
	
}DifferentSeries;

typedef struct {
	int16  dataType;			//数据符号类型，检查数据为有符号还是无符号
	int16   xSize;				//切片图像的宽度
	int16   ySize;				//切片图像的高度
	int16   zSize;				//切片图像的张数
	real32   xSpacing;			//x方向的像素间距
	real32   ySpacing;			//y方向的像素间距
	real32   zSpacing;			//z方向的像素间距
	int16    windowCenter;		//窗位
	int16    windowWidth;		//窗宽
	real32    rescaleIntercept;	//灰度转HU值的截距
	real32    rescaleSlope;		//灰度转HU值的斜率
	int16*    ptrData;			//体数据指针
/**************修改后的信息--2017/7/12--戴知宇***************/
	std::string  admissionid;	//病人ID
	std::string  name;			//病人姓名
	std::string  gender;		//病人性别
	std::string  age;			//病人年龄
	std::string  part;			//检查部位
	std::string  date;			//检查日期
	std::string  doctor;		//医生姓名
/**************添加模态信息--2017/7/26--戴知宇***************/
	std::string  modality;		//检查的所用模态
/**************添加单张图像像素信息--2017/7/27--戴知宇***************/
	int16*	pSingleData;		//单张图像数据指针
/**************添加单张图像像素信息--2018/2/1--戴知宇***************/
	std::string  BirthDate;		//病人出生日期
	std::string  Manufacturer;	//扫描设备
	std::string  AccessionNumber;	//检查号
	std::string  InstitutionName;	//检查医院
	std::string  AcquisitionTime;	//检查时间
	int16  PatientSize;				//病人身高
	int16  PatientWeight;			//病人体重
	int16  ExposureTime;			//曝光时间
	int16  XrayTubeCurrent;			//曝光电流
	int16  KVP;						//曝光电压

} DSR_VolumeInfo;

#ifdef __cplusplus
	}
#endif





#endif

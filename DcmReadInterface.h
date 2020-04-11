#ifndef DCMREAD_H
#define DCMREAD_H

#include "GDCM/DSReaderAPI.h"
#include "GDCM/CharSetConverter.h"

typedef DSR_VolumeInfo DICOMPropertyData;

class DcmReadInterface             //DCM文件读取接口
{

public:
	DcmReadInterface();
	~DcmReadInterface();

	static void DcmReader(const char *dir);                   //DCM文件读取器

	static void GetPatientInfo(DSR_VolumeInfo &pVolumeInfo);  //读取病人信息

	static void GetOneImgData(DSR_VolumeInfo &pVolumeInfo);   //读取一张图像

	static void GetVolumeData(DSR_VolumeInfo &pVolumeInfo);   //读取体数据

	static void DeleteDcmReader();                            //删除DCM文件读取器




private:

};

#endif // DCMREAD_H

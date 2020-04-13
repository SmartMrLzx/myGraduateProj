#include "dcmreadinterface.h"

DcmReadInterface::DcmReadInterface()
{


}

void DcmReadInterface::DcmReader(const char* dir)               //创建dcm阅读器
{
	CharSetConverter Converter;                                     //创建char字符串转换器
	DSR_CreateDicomSeriesReaderObj();                               //创建Dicom序列阅读器对象
	DSR_SetDicomSeriesReaderDir(Converter.AnsiToUnicode(dir));      //设置Dicom序列阅读器路径

	DSR_ReadDicomSeries(NULL);          //读取Dicom序列
}

void DcmReadInterface::GetPatientInfo(DSR_VolumeInfo &pVolumeInfo)      //获取病人信息
{
	DSR_GetVolumeInfo(&pVolumeInfo);             //读取体素信息

}

void DcmReadInterface::GetOneImgData(DSR_VolumeInfo &pVolumeInfo)        //获取单张图像数据
{
	if (pVolumeInfo.pSingleData != NULL)            //如果体素信息中的单个数据集不为空
	{
		free(pVolumeInfo.pSingleData);          //？
		pVolumeInfo.pSingleData = NULL;         //单个数据集置空
	}


}

void DcmReadInterface::GetVolumeData(DSR_VolumeInfo &pVolumeInfo)             //获取体素数据
{
	//if (pVolumeInfo.ptrData != NULL)
	//{
	//	free(pVolumeInfo.ptrData);
	//	pVolumeInfo.ptrData = NULL;
	//}

	DSR_GetVolumeData(&pVolumeInfo);            //获取体素数据


}
DcmReadInterface::~DcmReadInterface()
{

}
void DcmReadInterface::DeleteDcmReader()           //删除dcm阅读器
{
	DSR_ReleaseVolumeData();
}


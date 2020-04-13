#ifndef __DSReaderAPI_h
#define __DSReaderAPI_h

#include "DSReaderDefine.h"
#include<vector>
class DicomSeriesReader;

//DicomSeriesReader* DSR_ReaderHdl = NULL;

DLLExport void    DSR_CreateDicomSeriesReaderObj();                   //创建DicomSeriesReader对象

DLLExport void    DSR_ReleseDicomSeriesReaderObj();                   //释放DicomSeriesReader对象

DLLExport E_GDCM_Info  DSR_SetDicomSeriesReaderDir(const DSR_CHAR*  dir );                           //设置DicomSeriesReader路径

DLLExport E_GDCM_Info  DSR_ReadDicomSeries(const CallBackFunction CbfPtr = NULL );                   //读取Dicom序列

DLLExport E_GDCM_Info  DSR_GetSameSeriesInfo(std::vector<DifferentSeries*> *pDifferSeries);          //获取同序列信息

DLLExport E_GDCM_Info  DSR_GetSameSeriesSingleData(std::vector<DifferentSeries*> *pDifferSeries);    //获取同序列的单个数据

DLLExport E_GDCM_Info  DSR_GetVolumeInfo(DSR_VolumeInfo *pVolInfo);             //获取体素信息

DLLExport E_GDCM_Info  DSR_GetVolumeData(DSR_VolumeInfo *pVolInfo);             //获取体素数据

DLLExport void    DSR_ReleaseVolumeData();               //释放体素数据


#endif

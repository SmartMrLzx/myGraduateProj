#ifndef __DSReaderAPI_h
#define __DSReaderAPI_h

#include "DSReaderDefine.h"
#include<vector>
class DicomSeriesReader;

//DicomSeriesReader* DSR_ReaderHdl = NULL;

DLLExport void    DSR_CreateDicomSeriesReaderObj();

DLLExport void    DSR_ReleseDicomSeriesReaderObj();

DLLExport E_GDCM_Info  DSR_SetDicomSeriesReaderDir(const DSR_CHAR*  dir );

DLLExport E_GDCM_Info  DSR_ReadDicomSeries(const CallBackFunction CbfPtr = NULL );

DLLExport E_GDCM_Info  DSR_GetSameSeriesInfo(std::vector<DifferentSeries*> *pDifferSeries);

DLLExport E_GDCM_Info  DSR_GetSameSeriesSingleData(std::vector<DifferentSeries*> *pDifferSeries);

DLLExport E_GDCM_Info  DSR_GetVolumeInfo(DSR_VolumeInfo *pVolInfo);

DLLExport E_GDCM_Info  DSR_GetVolumeData(DSR_VolumeInfo *pVolInfo);

DLLExport void    DSR_ReleaseVolumeData();


#endif

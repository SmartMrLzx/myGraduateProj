#include "dcmreadinterface.h"

DcmReadInterface::DcmReadInterface()
{


}

void DcmReadInterface::DcmReader(const char* dir)
{
	CharSetConverter Converter;
	DSR_CreateDicomSeriesReaderObj();
	DSR_SetDicomSeriesReaderDir(Converter.AnsiToUnicode(dir));

	DSR_ReadDicomSeries(NULL);
}

void DcmReadInterface::GetPatientInfo(DSR_VolumeInfo &pVolumeInfo)
{
	DSR_GetVolumeInfo(&pVolumeInfo);

}

void DcmReadInterface::GetOneImgData(DSR_VolumeInfo &pVolumeInfo)
{
	if (pVolumeInfo.pSingleData != NULL)
	{
		free(pVolumeInfo.pSingleData);
		pVolumeInfo.pSingleData = NULL;
	}


}

void DcmReadInterface::GetVolumeData(DSR_VolumeInfo &pVolumeInfo)
{
	//if (pVolumeInfo.ptrData != NULL)
	//{
	//	free(pVolumeInfo.ptrData);
	//	pVolumeInfo.ptrData = NULL;
	//}

	DSR_GetVolumeData(&pVolumeInfo);


}
DcmReadInterface::~DcmReadInterface()
{

}
void DcmReadInterface::DeleteDcmReader()
{
	DSR_ReleaseVolumeData();
}


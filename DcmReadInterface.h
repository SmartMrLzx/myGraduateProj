#ifndef DCMREAD_H
#define DCMREAD_H

#include "GDCM/DSReaderAPI.h"
#include "GDCM/CharSetConverter.h"

typedef DSR_VolumeInfo DICOMPropertyData;

class DcmReadInterface             //DCM�ļ���ȡ�ӿ�
{

public:
	DcmReadInterface();
	~DcmReadInterface();

	static void DcmReader(const char *dir);                   //DCM�ļ���ȡ��

	static void GetPatientInfo(DSR_VolumeInfo &pVolumeInfo);  //��ȡ������Ϣ

	static void GetOneImgData(DSR_VolumeInfo &pVolumeInfo);   //��ȡһ��ͼ��

	static void GetVolumeData(DSR_VolumeInfo &pVolumeInfo);   //��ȡ������

	static void DeleteDcmReader();                            //ɾ��DCM�ļ���ȡ��




private:

};

#endif // DCMREAD_H


#ifndef __CharSetConverter_h
#define __CharSetConverter_h

#include "WinSock.h"

class CharSetConverter
{
public:
	CharSetConverter()                  //char字符串转换器
	{
		this->m_Ansi = NULL;        
		this->m_Utf8 = NULL;
		this->m_Unicode = NULL;
	}
	virtual ~CharSetConverter()
	{
		this->Release();
	}

	void WcharToChar(const wchar_t *wchr, char *chr)
	{

		// WideCharToMultiByte 返回长度不包括字符串终止符（'\0'）
        int len = WideCharToMultiByte(CP_ACP,0,wchr,(int)wcslen(wchr),NULL,0,NULL,NULL);
		if(0 == len)
		{
			chr[len] = '\0';
			return;
		}
        WideCharToMultiByte(CP_ACP,0,wchr,(int)wcslen(wchr),chr,len,NULL,NULL);
		chr[len] = '\0';
	}

	
	// char* 转换为 wchar_t*
	void CharToWchar(const char *chr,wchar_t *wchr)
	{
		// MultiByteToWideChar 返回长度不包括字符串终止符（'\0'）
        int len = MultiByteToWideChar(CP_ACP,0,chr,(int)strlen(chr),NULL,0);
		if(0 == len)
		{
			wchr[len]='\0';
			return ;
		}
        MultiByteToWideChar(CP_ACP,0,chr,(int)strlen(chr),wchr,len);
		wchr[len]='\0';
	}

	//-------------------------------------------------------------------------
	char *AnsiToUtf8(const char* buf)
	{
		this->Release();
		int len = ::MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
        if (len == 0) return 0;
 
		this->m_Unicode = new wchar_t[len];
		::MultiByteToWideChar(CP_ACP, 0, buf, -1, this->m_Unicode, len);

		len = ::WideCharToMultiByte(CP_UTF8, 0, this->m_Unicode, -1, NULL, 0, NULL, NULL);
        if (len == 0) return 0;
 
		this->m_Utf8 = new char[len+1];
		::WideCharToMultiByte(CP_UTF8, 0, this->m_Unicode, -1, this->m_Utf8, len, NULL, NULL);
        this->m_Utf8[len]='\0';

		return this->m_Utf8;
	}

	wchar_t *AnsiToUnicode(const char* buf)
	{
		this->Release();
		int len = ::MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
        if (len == 0) return 0;
 
		this->m_Unicode = new wchar_t[len+1];
		::MultiByteToWideChar(CP_ACP, 0, buf, -1, this->m_Unicode, len);
		this->m_Unicode[len]='\0';
 
		return this->m_Unicode;
	}


	char *UnicodeToUtf8(const wchar_t *buf)
	{
		this->Release();
		int len = ::WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
        if (len == 0) return 0;
 
		this->m_Utf8 = new char[len+1];
		::WideCharToMultiByte(CP_UTF8, 0, buf, -1, this->m_Utf8, len, NULL, NULL);
		this->m_Utf8[len]='\0';
 
		return this->m_Utf8;

	}

	char *UnicodeToAnsi(const wchar_t* buf)
	{
		this->Release();
		int len = ::WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
        if (len == 0) return 0;
 
		this->m_Ansi = new char[len+1];
		::WideCharToMultiByte(CP_ACP, 0, buf, -1, this->m_Ansi, len, NULL, NULL);
		this->m_Ansi[len]='\0';
 
		return this->m_Ansi;
	}
 
	wchar_t *Utf8ToUnicode(const char* buf)
	{
		this->Release();
		int len = ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
        if (len == 0) return 0;
 
		this->m_Unicode = new wchar_t[len+1];
		::MultiByteToWideChar(CP_UTF8, 0, buf, -1, this->m_Unicode, len);
		this->m_Unicode[len]='\0';
 
		return this->m_Unicode;
	}

	void Utf8ToUnicode(const char* src,wchar_t *des)
	{
		int len = ::MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if (len == 0) return ;
 
		::MultiByteToWideChar(CP_UTF8, 0, src, -1, des, len);
		des[len]='\0';
	}

protected:
	void Release()
	{
		if(this->m_Ansi)
		{
			delete this->m_Ansi;
			this->m_Ansi = NULL;
		}
		if(this->m_Utf8)
		{
			delete this->m_Utf8;
			this->m_Utf8 = NULL;
		}
		if(this->m_Unicode)
		{
			delete this->m_Unicode;
			this->m_Unicode = NULL;
		}
	}

private:
	char *m_Ansi;               //ANSI码
	char *m_Utf8;               //UTF8码
	wchar_t *m_Unicode;         //Unicode码

};

#endif

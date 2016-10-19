#include "tools.h"

using namespace std;

void ANSIToUnicode(const char* str, wchar_t* result, int MaxSize)
{
	int  len = 0;
	len = strlen(str);
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
		0,
		str,
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	if (wcslen(pUnicode) <= MaxSize)
		wcscpy_s(result, wcslen(pUnicode) + 1, pUnicode);
	else
		result = NULL;
	delete  pUnicode;
}

// ����:execmd����ִ�������������洢��result�ַ��������� 
// ����:cmd��ʾҪִ�е�����
// result��ִ�еĽ���洢���ַ�������
// ����ִ�гɹ�����1��ʧ�ܷ���0  
int execmd(char* cmd, char* result, int MaxSize) {
	char buffer[128];                         //���建����   
	int count = 0;
	FILE* pipe = _popen(cmd, "r");            //�򿪹ܵ�����ִ������ 
	if (!pipe)
		return 0;                      //����0��ʾ����ʧ�� 

	while (!feof(pipe)&&count*128<MaxSize) {
		if (fgets(buffer, 128, pipe)) {             //���ܵ������result�� 
			strcat_s(result, strlen(result) + strlen(buffer) + 1, buffer);
		}
	}
	_pclose(pipe);                            //�رչܵ� 
	return 1;                                 //����1��ʾ���гɹ� 
}


void UnicodeToANSI(const wchar_t* str, char* result, int MaxSize)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	if (strlen(pElementText) <= MaxSize)
		strcpy_s(result, MaxSize, pElementText);
	else
		result = NULL;
}

int checkLang(wchar_t *str)
{
	int lang = 0;//English as default
	int count_char[3] = { 0 };//[0]->English, [1]->kana, [2]->character
	for (int i = 0; i < wcslen(str); i++)
	{
		if ((str[i] >= 0x41 && str[i] <= 0x5A) || (str[i] >= 0x61 && str[i] <= 0x7A))//English character
			count_char[0]++;
		else if (str[i] >= 0x3040 && str[i] <= 0x31FE || str[i] >= 0xFF00 && str[i] <= 0xFFEF)//Japanese Kana
			count_char[1]++;
		else if (str[i] >= 0x2E80 && str[i] <= 0xFE4F)//Chinese character
			count_char[2]++;
	}
	if (count_char[0] >= count_char[1] + count_char[2])
		lang = 0;//English
	else if (count_char[1] / ((double)count_char[2]) >= 0.6)
		lang = 1;//Japanese
	else
		lang = 2;//Chinese
	return lang;
}

void trim(char *str, char* result, int MaxSize)
{
	int idx = 0;
	for (int i = 0; i < strlen(str)&& i < MaxSize-1; i++)
		if (str[i] >= '0' && str[i] <= '9' || str[i] >= 'a' && str[i] <= 'f' || str[i] >= 'A' && str[i] <= 'F')
			result[idx++] = str[i];
	result[idx] = '\0';
}

bool unicode2char(wchar_t * str, char* result, int MaxSize)
{
	char buff[6] = { 0 };
	if (wcslen(str) >= MaxSize / 5)
		return false;
	//int idx = 0;
	for (int i = 0; i < wcslen(str); i++)
	{
		//if (str[i] >= 0x2E80 && str[i] <= 0xFE4F)//For non-English Characters, change to unicode format
		//{
			sprintf(buff, "%4x ", str[i]);
			for (int j = 0; j < 6; j++)
				//result[idx++] = buff[j];
				result[i * 5 + j] = buff[j];
		//}
		//else//For ASCII characters, remain them
		//	result[idx++] = str[i];
	}
	printf("charArray: %s\n", result);
	return true;
}

//�ظ�ʱ�������ASCII�ַ���һ��ת��Unicode��
bool char2Unicode(char* str, wchar_t *result, int MaxSize)
{
	char* temp = new char[MaxSize];
	trim(str, temp,  MaxSize);
	for (int i = 0; i < strlen(temp); i += 4)
	{
		result[i / 4] = (temp[i] >= 'a' ? temp[i] - 'a' + 10 : temp[i] - '0') * 4096 +
			(temp[i + 1] >= 'a' ? temp[i + 1] - 'a' + 10 : temp[i + 1] - '0') * 256 +
			(temp[i + 2] >= 'a' ? temp[i + 2] - 'a' + 10 : temp[i + 2] - '0') * 16 +
			(temp[i + 3] >= 'a' ? temp[i + 3] - 'a' + 10 : temp[i + 3] - '0');

	}
	printf("UnicodeChar: %ws\n", result);
	delete[] temp;
	return true;
}

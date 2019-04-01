#pragma once

class CEncode
{
public:
	CEncode(void);
	~CEncode(void);
	CString base64encode(CString src, int srclen);
};
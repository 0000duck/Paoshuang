#ifndef _HAOI_H_
#define _HAOI_H_
#include <Windows.h> 
//extern "C"
//{

BOOL WINAPI GetBusy(LPSTR szResult);
BOOL WINAPI HGetBusyW(LPWSTR szResult);
BOOL WINAPI IsRight(LPSTR szResponse);
BOOL WINAPI IsRightW(LPWSTR szResponse);
BOOL WINAPI GetAnswer(LPSTR szId,LPSTR szResult);
BOOL WINAPI GetAnswerW(LPWSTR szId,LPWSTR szResult);
BOOL WINAPI RegID(
				   LPSTR szId,
				   LPSTR szPass,
				   LPSTR szEmail,
				   LPSTR szQQ,
				   LPSTR szFrom,
				   DWORD szIsSon,
				   LPSTR szResult);
BOOL WINAPI RegIDW(
				   LPWSTR szId,
				   LPWSTR szPass,
				   LPWSTR szEmail,
				   LPWSTR szQQ,
				   LPWSTR szFrom,
				   DWORD szIsSon,
				   LPWSTR szResult);

BOOL WINAPI SendError(
					   LPSTR szId,
					   LPSTR szResult);
BOOL WINAPI SendErrorW(
					   LPWSTR szId,
					   LPWSTR szResult);
BOOL WINAPI UseKey(
					LPSTR szUpId,
					LPSTR szKeys,
					DWORD szti,
					LPSTR szResult);
BOOL WINAPI UseKeyW(
					LPWSTR szUpId,
					LPWSTR szKeys,
					DWORD szti,
					LPWSTR szResult);
BOOL WINAPI KeyEndDate(
						LPSTR szUpId,
						LPSTR szKeys,
						LPSTR szResult);
BOOL WINAPI KeyEndDateW(
						LPWSTR szUpId,
						LPWSTR szKeys,
						LPWSTR szResult);
BOOL WINAPI CheckKey(
					  LPSTR szUpId,
					  LPSTR szKeys,
					  LPSTR szResult);
BOOL WINAPI Pay(
				 LPSTR szId,
				 LPSTR szCard,
				 LPSTR szResult);
BOOL WINAPI Login(
				   LPSTR szId,
				   LPSTR szPass,
				   LPSTR szResult);
BOOL WINAPI LoginW(
				   LPWSTR szId,
				   LPWSTR szPass,
				   LPWSTR szResult);

BOOL WINAPI GetPoint(
					  LPSTR szId,
					  LPSTR szResult);
BOOL WINAPI GetPointW(
					  LPWSTR szId,
					  LPWSTR szResult);

VOID WINAPI SetQuality(DWORD dwQuality);

BOOL WINAPI SetRebate(LPSTR szRebate);

VOID WINAPI SetLogType(LONG Type);

BOOL WINAPI WriteWordtoJPG(LPSTR szFilePath,LPSTR PicWH,LPSTR lpWordPoint,DWORD HWord,LPSTR szColor);

BOOL WINAPI SendByte(
					  LPSTR szMyUserStr,
					  LPSTR szGameID,
					  LPVOID lpFileBuffer,
					  DWORD dwSize,
					  DWORD dwTimeOut,
					  DWORD dwLostPoint,
					  LPSTR szBeiZhu,
					  LPSTR szResult);

BOOL WINAPI SendByteW(
					  LPWSTR szMyUserStr,
					  LPWSTR szGameID,
					  LPVOID lpFileBuffer,
					  DWORD dwSize,
					  DWORD dwTimeOut,
					  DWORD dwLostPoint,
					  LPWSTR szBeiZhu,
					  LPWSTR szResult);

BOOL WINAPI SendFile(
					  LPSTR szMyUserStr,
					  LPSTR szGameID,
					  LPSTR szFilePath,
					  DWORD dwTimeOut,
					  DWORD dwLostPoint,
					  LPSTR szBeiZhu,
					  LPSTR szResult);

BOOL WINAPI SendFileEx(
						LPSTR szMyUserStr,
						LPSTR szGameID,
						LPSTR szFilePath,
						DWORD dwTimeOut,
						DWORD dwLostPoint,
						LPSTR szBeiZhu,
						LPSTR szResult,
						LPSTR szreply);

BOOL WINAPI SendByteEx(
						LPSTR szMyUserStr,
						LPSTR szGameID,
						LPVOID lpFileBuffer,
						DWORD dwSize,
						DWORD dwTimeOut,
						DWORD dwLostPoint,
						LPSTR szBeiZhu,
						LPSTR szResult,
						LPSTR szreply);

BOOL WINAPI SendFileExW(
						LPWSTR szMyUserStr,
						LPWSTR szGameID,
						LPWSTR szFilePath,
						DWORD dwTimeOut,
						DWORD dwLostPoint,
						LPWSTR szBeiZhu,
						LPWSTR szResult,
						LPWSTR szreply);

BOOL WINAPI SendByteExW(
						LPWSTR szMyUserStr,
						LPWSTR szGameID,
						LPVOID lpFileBuffer,
						DWORD dwSize,
						DWORD dwTimeOut,
						DWORD dwLostPoint,
						LPWSTR szBeiZhu,
						LPWSTR szResult,
						LPWSTR szreply);


BOOL WINAPI SendFileW(
					  LPWSTR szMyUserStr,
					  LPWSTR szGameID,
					  LPWSTR szFilePath,
					  DWORD dwTimeOut,
					  DWORD dwLostPoint,
					  LPWSTR szBeiZhu,
					  LPWSTR szResult);
BOOL WINAPI CheckKeyW(
					  LPWSTR szUpId,
					  LPWSTR szKeys,
					  LPWSTR szResult);
BOOL WINAPI PayW(
				 LPWSTR szId,
				 LPWSTR szCard,
				 LPWSTR szResult);



VOID WINAPI SetIsLog(BOOL IS);

VOID WINAPI SetIsSavePic(BOOL IS);

VOID WINAPI GetVer(LPSTR lpVer);

//}

//////////////////////////////////////////////////////////////////////////
#endif
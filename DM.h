#if !defined(AFX_DM_H__17EDBEBD_48E0_4B83_9165_9919A0C54BCE__INCLUDED_)
#define AFX_DM_H__17EDBEBD_48E0_4B83_9165_9919A0C54BCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM.h : header file
//
#include "DMPro_Types.h"


/////////////////////////////////////////////////////////////////////////////
// CDM window

class CDM : public CWnd
{
// Construction
public:
	CDM();

// Attributes
public:
	//DM Decoder
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL						m_DM_Status;
	HINSTANCE					m_DM_dllinstance;
	int							m_DM_maxrow;
	int							m_DM_maxcol;

	int							m_maxrow;
	int							m_maxcol;

	PDM_Decoder					m_DM_pDecoder;
	PDM_Options					m_DM_DecOpt;

	TDM_OptMode					m_DM_opt;
	TDM_ImageInfo*				m_pDM_imageinfo;
	TDM_Info*					m_pDM_info;

	TRow*						m_pDM_bits;
	TRow						m_pDM_membits;  // Image in Memory
	TConnect_DM_Decoder         m_DM_ConnectDecoder;
	TCreate_DM_Options          m_DM_CreateOptions;
	TDecodeDM_Bits              m_DM_DecodeBits;
	TGetDM_ImageInfo            m_DM_GetImageInfo;
	TGetDM_Info                 m_DM_GetInfo;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL InitDM_Decoder(void);
	void ConnectDM_Decoder(int ImageSizeX, int ImageSizeY);

	BOOL ReadDM_Decoder(int nRslt,char *strResult);
	void ExitDM_Decoder(void);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDM();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DM_H__17EDBEBD_48E0_4B83_9165_9919A0C54BCE__INCLUDED_)

// DM.cpp : implementation file
//

#include "stdafx.h"
#include "CSVT.h"

#include "DMPro_Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM

CDM::CDM()
{
	m_DM_Status=FALSE;
	m_DM_maxrow = 5000,
	m_DM_maxcol = 8192;

}

CDM::~CDM()
{
}

BEGIN_MESSAGE_MAP(CDM, CWnd)
	//{{AFX_MSG_MAP(CDM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDM message handlers
BOOL CDM:: InitDM_Decoder(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//DM Decoder
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	m_DM_dllinstance = LoadLibrary("..\\Project\\MSVC_Pro\\DM_EP_32.dll");
	if (m_DM_dllinstance==NULL)
	{
		AfxMessageBox("Error 2D Matrix");
		m_DM_Status=FALSE;
	}
	else
	{
		m_DM_ConnectDecoder		= (TConnect_DM_Decoder    )GetProcAddress(m_DM_dllinstance,"Connect_DM_Decoder");
		m_DM_CreateOptions		= (TCreate_DM_Options     )GetProcAddress(m_DM_dllinstance,"Create_DM_Options");
		m_DM_DecodeBits			= (TDecodeBitsF           )GetProcAddress(m_DM_dllinstance,"DecodeDM_Bits");
		m_DM_GetImageInfo		= (TGetDM_ImageInfo       )GetProcAddress(m_DM_dllinstance,"GetDM_ImageInfo");
		m_DM_GetInfo			= (TGetDM_Info            )GetProcAddress(m_DM_dllinstance,"GetDM_Info");
		
		m_DM_Status=TRUE;
	}
	return m_DM_Status;
}

void CDM::ConnectDM_Decoder(int ImageSizeX, int ImageSizeY)
{
	int	    row;
	CString str;
	m_maxrow	=ImageSizeY;
	m_maxcol	=ImageSizeX;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//str.Format("Image x=%d, Image Y=%d",m_DM_maxcol,m_DM_maxrow);
	//AfxMessageBox(str);

	m_pDM_membits = (TRow ) malloc(m_DM_maxrow*m_DM_maxcol);		//  Image in Memory
	m_pDM_bits    = (TRow*) malloc(m_DM_maxrow*sizeof(TRow));		//  pointers to ScanLines
  
	for (row = 0; row < m_DM_maxrow; row++)
	{
		m_pDM_bits[row] = &m_pDM_membits[m_DM_maxcol*row];
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_DM_ConnectDecoder==NULL)
		AfxMessageBox("Error Connecting DM Decorder");
	else
		m_DM_pDecoder = m_DM_ConnectDecoder(m_maxcol,m_maxrow);
			
	if (m_DM_pDecoder != NULL)
	{
		m_DM_opt.maxDMCount		= 2;
		m_DM_opt.cellColor		= CL_WHITEONBLACK;	//CL_ANY;		// equal to default value
		m_DM_opt.mirrorMode		= MM_NORMAL;		//MM_ANY;
		m_DM_opt.speedMode		= DMSP_ULTIMATEPLUS;	// DMSP_REGULAR;
		m_DM_opt.qualityMask	= DM_QM_ALL;
		m_DM_opt.labelMode		= LM_ST_DOT;
		m_DM_opt.timeOut		= 0;//107;
		m_DM_opt.filterMode		= FM_NON;
		m_DM_opt.qzMode			= DMQZ_NORMAL; 
			
		m_DM_DecOpt				= m_DM_CreateOptions(m_DM_pDecoder,m_DM_opt);
		//AfxMessageBox("Decoder connected OK");
   }
	else
		m_DM_Status=FALSE;
}
BOOL CDM::ReadDM_Decoder(int nRslt,char *strDMRead)
{
	BOOL Accept=TRUE;
	CString str;
	int col,row;//k;

	unsigned char *imgPt=gcmil.GetImagePointer(T_IMAGE_4);
//	unsigned char *imgPt3=gcmil.GetImagePointer(T_IMAGE_3);

//	str.Format("\n Image Max row = %d, Image Max Col=%d",m_maxrow,m_maxcol);
//	AfxMessageBox(str);



	for(row=0; row<m_maxrow; row++)
	{
		for(col=0; col<m_maxcol; col++)
		{
			m_pDM_bits[row][col]					=imgPt[(m_maxcol*row)+col];	
			//imgPt3[(m_maxcol*row)+col]				=imgPt[(m_maxcol*row)+col];	

		}
	
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_DM_DecodeBits(m_DM_DecOpt,m_maxrow, m_maxcol, m_pDM_bits);
	//
		m_pDM_imageinfo = m_DM_GetImageInfo(m_DM_DecOpt);

		int dmCnt	= m_pDM_imageinfo->DMCount;
		int rr		= m_pDM_imageinfo->RejectionReason;


		if (dmCnt>0)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			m_pDM_info = m_DM_GetInfo(m_DM_DecOpt,1);			//0- first decoded info
			//printf("\n RSErr = %4d",pdm_info->RSErr);
			//printf("\n rowcols: ");
			//for (k=0;k<4;k++)
			//{
			//		printf("(%4d,%4d),   ", int(m_pDM_info->rowcols[2*k]) , int(m_pDM_info->rowcols[2*k+1]));
			//}
			if (m_pDM_info->pchlen>0)
			{
				str.Format("%s",m_pDM_info->pch);
				strcpy(strDMRead,str);
				 //strncpy(Symbology_Identifier,(char*)&(gcavt.pDM_info->pch[-3]),3);
				 //Symbology_Identifier[3] = 0;
    			 //printf("\n Symbology_Id = %s,  RSErr = %4d",Symbology_Identifier,gcavt.pDM_info->RSErr);
			 }
			 						 

					 //printf("\n Dimensions: %4d * %4d",m_pDM_info->VDim, m_pDM_info->HDim);			 
		}
		else
			AfxMessageBox("Fail DM");
//	}
//	else
	//	AfxMessageBox("DM not found");

     
	return Accept;
}
void CDM:: ExitDM_Decoder(void)
{
	TDisconnect_DM_Decoder  Disconnect_DM_Decoder;
	TDelete_DM_Options      Delete_DM_Options;

	Disconnect_DM_Decoder = (TDisconnect_DM_Decoder )GetProcAddress(m_DM_dllinstance,"Disconnect_DM_Decoder");
	Delete_DM_Options     = (TDelete_DM_Options     )GetProcAddress(m_DM_dllinstance,"Delete_DM_Options");


	free(m_pDM_bits);
	free(m_pDM_membits);

	Delete_DM_Options(m_DM_DecOpt);
    Disconnect_DM_Decoder(m_DM_pDecoder);
}


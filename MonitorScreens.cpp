// Monitors.cpp : implementation file
// ref: https://www.codeproject.com/Articles/3690/MFC-Classes-for-Multiple-Monitors
//

#include "stdafx.h"
#include "MonitorScreens.h"
#include "Monitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CMonitorScreens

CMonitorScreens::CMonitorScreens()
{
	m_MonitorArray.SetSize( GetMonitorCount() );

	ADDMONITOR addMonitor;
	addMonitor.pMonitors = &m_MonitorArray;
	addMonitor.currentIndex = 0;

	::EnumDisplayMonitors( NULL, NULL, AddMonitorsCallBack, (LPARAM)&addMonitor );
}

CMonitorScreens::~CMonitorScreens()
{
	for ( int i = 0; i < m_MonitorArray.GetSize(); i++ )
		delete m_MonitorArray.GetAt( i );
}


// CMonitorScreens member functions

BOOL CALLBACK CMonitorScreens::AddMonitorsCallBack( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	LPADDMONITOR pAddMonitor = (LPADDMONITOR)dwData;

	CMonitor* pMonitor = new CMonitor;
	pMonitor->Attach( hMonitor );

	pAddMonitor->pMonitors->SetAt( pAddMonitor->currentIndex, pMonitor );
	pAddMonitor->currentIndex++;

	return TRUE;
}
//
// returns the primary monitor
CMonitor CMonitorScreens::GetPrimaryMonitor()
{
	//the primary monitor always has its origin at 0,0
	HMONITOR hMonitor = ::MonitorFromPoint( CPoint( 0,0 ), MONITOR_DEFAULTTOPRIMARY );
	ASSERT( IsMonitor( hMonitor ) );

	CMonitor monitor;
	monitor.Attach( hMonitor );
	ASSERT( monitor.IsPrimaryMonitor() );

	return monitor;
}

//
// is the given handle a valid monitor handle
BOOL CMonitorScreens::IsMonitor( const HMONITOR hMonitor )
{
	if ( hMonitor == NULL )
		return FALSE;

	MATCHMONITOR match;
	match.target = hMonitor;
	match.foundMatch = FALSE;

	::EnumDisplayMonitors( NULL, NULL, FindMatchingMonitorHandle, (LPARAM)&match );

	return match.foundMatch;
}



//this is the callback method that gets called via IsMontior
BOOL CALLBACK CMonitorScreens::FindMatchingMonitorHandle( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	LPMATCHMONITOR pMatch = (LPMATCHMONITOR)dwData;

	if ( hMonitor == pMatch->target )
	{
		//found a monitor with the same handle we are looking for		
		pMatch->foundMatch = TRUE;
		return FALSE; //stop enumerating
	}

	//haven't found a match yet
	pMatch->foundMatch = FALSE;
	return TRUE;	//keep enumerating
}


BOOL CMonitorScreens::AllMonitorsShareDisplayFormat()
{
	return ::GetSystemMetrics( SM_SAMEDISPLAYFORMAT );
}

//
// the number of monitors on the system
int CMonitorScreens::GetMonitorCount()
{ 
	return ::GetSystemMetrics(SM_CMONITORS);
}


CMonitor CMonitorScreens::GetMonitor( const int index ) const
{
#if _MFC_VER >= 0x0700
	ASSERT( index >= 0 && index < m_MonitorArray.GetCount() ); 
#else
	ASSERT( index >= 0 && index < m_MonitorArray.GetSize() );
#endif

	CMonitor* pMonitor = (CMonitor*)m_MonitorArray.GetAt( index );

	return *pMonitor;
}

//
// returns the rectangle that is the union of all active monitors
void CMonitorScreens::GetVirtualDesktopRect( LPRECT lprc )
{
	::SetRect( lprc, 
				::GetSystemMetrics( SM_XVIRTUALSCREEN ),
				::GetSystemMetrics( SM_YVIRTUALSCREEN ),
				::GetSystemMetrics( SM_CXVIRTUALSCREEN ),
				::GetSystemMetrics( SM_CYVIRTUALSCREEN ) );
	
}

//
// these methods determine wheter the given item is
// visible on any monitor
BOOL CMonitorScreens::IsOnScreen( const LPRECT lprc )
{
	return ::MonitorFromRect( lprc, MONITOR_DEFAULTTONULL ) != NULL;
}

BOOL CMonitorScreens::IsOnScreen( const POINT pt )
{
	return ::MonitorFromPoint( pt, MONITOR_DEFAULTTONULL ) != NULL;
}

BOOL CMonitorScreens::IsOnScreen( const CWnd* pWnd )
{
	return ::MonitorFromWindow( pWnd->GetSafeHwnd(), MONITOR_DEFAULTTONULL ) != NULL;
}

CMonitor CMonitorScreens::GetNearestMonitor( const LPRECT lprc )
{
	CMonitor monitor;
	monitor.Attach( ::MonitorFromRect( lprc, MONITOR_DEFAULTTONEAREST ) );

	return monitor;

}

CMonitor CMonitorScreens::GetNearestMonitor( const POINT pt )
{
	CMonitor monitor;
	monitor.Attach( ::MonitorFromPoint( pt, MONITOR_DEFAULTTONEAREST ) );

	return monitor;
}

CMonitor CMonitorScreens::GetNearestMonitor( const CWnd* pWnd )
{
	ASSERT( pWnd );
	ASSERT( ::IsWindow( pWnd->m_hWnd ) );

	CMonitor monitor;
	monitor.Attach( ::MonitorFromWindow( pWnd->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST ) );

	return monitor;
}



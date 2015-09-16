#include "EquationEditorWindow.h"

const wchar_t* const CEquationEditorWindow::className = L"EquationEditorWindow";

CEquationEditorWindow::CEquationEditorWindow(): hwnd(nullptr)
{
    presenter = CEquationPresenter();
}

bool CEquationEditorWindow::RegisterClassW()
{
    WNDCLASSEX wnd;
    ZeroMemory( &wnd, sizeof(wnd) );
    wnd.cbSize = sizeof(wnd);
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.lpfnWndProc = equationEditorWindowProc;
    wnd.hInstance = ::GetModuleHandle(NULL);
    wnd.lpszClassName = className;
    wnd.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 );
    wnd.hCursor = ::LoadCursor( NULL, IDC_ARROW );
    return ::RegisterClassEx(&wnd) != 0;
}

bool CEquationEditorWindow::Create()
{
	return ::CreateWindowEx( 0, className, L"Equation Editor", WS_OVERLAPPEDWINDOW | WS_EX_LAYERED, 0, 0, 500, 400,
		0, 0, ::GetModuleHandle( NULL ), this ) != 0;
}

void CEquationEditorWindow::Show(int cmdShow)
{
    ::ShowWindow(hwnd, cmdShow);
}

void CEquationEditorWindow::OnDestroy()
{
    ::PostQuitMessage(0);
}

void CEquationEditorWindow::OnCreate()
{
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>( ::GetWindowLong(hwnd, GWL_HINSTANCE) );

    HWND hwndEdit = ::CreateWindowEx( 0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        0, 0, 0, 0, hwnd, NULL, hInstance, NULL );

    presenter.AddControlView( new CEditControlView(hwndEdit), NULL );

	originEditControlProc = (WNDPROC) ::SetWindowLong( hwndEdit, GWL_WNDPROC, (DWORD) editControlProc );
}

void CEquationEditorWindow::OnSize( int cxSize, int cySize ) 
{
}

void CEquationEditorWindow::OnChar() 
{
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(::GetWindowLong( hwnd, GWL_HINSTANCE ));

	HWND hwndEdit = ::CreateWindowEx( 0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL );

	::SetWindowLong( hwndEdit, GWL_WNDPROC, (DWORD) editControlProc );

	presenter.AddControlView( new CEditControlView( hwndEdit ), ::GetFocus() );
}

LRESULT CEquationEditorWindow::equationEditorWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    CEquationEditorWindow *wnd = nullptr;

    if (message == WM_NCCREATE) {
        // �������� ��������� �� ��������� ������ ����, ������� �� �������� � ������� CreateWindowEx
        wnd = static_cast<CEquationEditorWindow*>( LPCREATESTRUCT(lParam)->lpCreateParams );
        // � ��������� � ���� GWL_USERDATA
        ::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>( LPCREATESTRUCT(lParam)->lpCreateParams ) );
        // ���������� handle
        wnd->hwnd = handle;
    }
    // ������ �������� ��������� �� ��� ��������� ����, �� ��� �� ���� GWL_USERDATA
    wnd = reinterpret_cast<CEquationEditorWindow*>( ::GetWindowLong(handle, GWL_USERDATA) );

    switch (message) {
    case WM_DESTROY:
        wnd->OnDestroy();
        return 0;

    case WM_CREATE:
        wnd->OnCreate();
        return 0;

	case WM_KEYDOWN:
		wnd->OnChar();
		return 0;

    case WM_SIZE:
        wnd->OnSize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    return ::DefWindowProc(handle, message, wParam, lParam);
}

LRESULT CALLBACK CEquationEditorWindow::editControlProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
	CEquationEditorWindow *wnd = reinterpret_cast<CEquationEditorWindow*>(::GetWindowLong( ::GetParent( hwnd ), GWL_USERDATA ));
	
	switch( message ) {
	case WM_CHAR:
		wnd->OnChar();
	default:
		break;
	}
	return ::CallWindowProc( (WNDPROC) wnd->originEditControlProc, hwnd, message, wParam, lParam );
}
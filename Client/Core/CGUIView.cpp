#include "StdInc.h"

using namespace Gwen;

CGUIView::CGUIView(Renderer::DirectX9* pRenderer)
{
	// Save the renderer
	m_pRenderer = pRenderer;

	// Set the skin to use this renderer
	// TODO: Should use the image-based variation
	skin.SetRender( pRenderer );

	// Create the base canvas
	m_pCanvas = new Controls::Canvas( &skin );
	m_pCanvas->SetSkin( &skin );

	// Create the input handler
	m_pInput = new Input::Windows();
	m_pInput->Initialize( m_pCanvas );

	/*
	// Add an example window
	Controls::WindowControl* pWindow = new Controls::WindowControl( m_pCanvas );
	pWindow->SetTitle( ::String( "Window (%p)", this ).C_String() );
	pWindow->SetSize(200, 50);
	pWindow->SetPos(10, 400);
	pWindow->SetDeleteOnClose(true);
	
	Controls::Label* pLabel = new Controls::Label( pWindow );
	pLabel->SetText( "This is example." );
	pLabel->SizeToContents();
	*/
}


CGUIView::~CGUIView()
{
	SAFE_DELETE(m_pInput);
	SAFE_DELETE(m_pCanvas);
}

void CGUIView::Render()
{
	m_pCanvas->RenderCanvas();
}

bool CGUIView::ProcessInput(MSG msg)
{
	bool b = m_pInput->ProcessMessage(msg);
	if(b)
		CLogFile::Printf("Parsing message %d", msg.message);
	return b;
}

void CGUIView::SetScreenSize(int iWidth, int iHeight)
{
	m_pCanvas->SetSize(iWidth, iHeight);
}

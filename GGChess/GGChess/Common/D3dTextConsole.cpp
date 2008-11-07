#include "D3dTextConsole.h"

D3dTextConsole* D3dTextConsole::object = NULL;

D3dTextConsole* D3dTextConsole::getTextOutputObject() {
	if(object == NULL) {
		object = new D3dTextConsole();
	}
	return object;
}

D3dTextConsole::D3dTextConsole() {
	m_device = NULL;
	m_font = NULL;

	listenning_to_char = false;
	listenning_to_any  = false;
}

D3dTextConsole::~D3dTextConsole() {
	m_font->InvalidateDeviceObjects();
	m_font->DeleteDeviceObjects();
	delete m_font;
}

void D3dTextConsole::init(IDirect3DDevice9* device) {
	m_device = device;

	hMutex = CreateMutex(NULL, FALSE, NULL);

	m_font = new CD3DFont((const TCHAR*)"Times New Roman", 24, 0);
	setFont("Times New Roman", 24);
	setColor(0xFFF00FFF);
}

void D3dTextConsole::setFont(std::string fontName, int size) {
	if (m_font)  {
		m_font->InvalidateDeviceObjects();
		m_font->DeleteDeviceObjects();
		delete m_font;
	}

	m_font = new CD3DFont((const TCHAR*)fontName.c_str(), size, 0);
	m_font->InitDeviceObjects( m_device );
	m_font->RestoreDeviceObjects();	
}

void D3dTextConsole::offTheRecord(std::string str) {
	USES_CONVERSION;
	m_font->DrawTextW(20, 20, m_color, (const TCHAR*)A2W(str.c_str()));	
}
void D3dTextConsole::writeText(std::string text) {
	if (WaitForSingleObject(hMutex, 1000) == WAIT_TIMEOUT) {
		MessageBoxA(0, "The wait timeout excedded. Unbelievable.", 
			"D3dTextConsole::flushFromStream()", MB_OK);
	}

	m_stream << text;
	flushFromStream();

	ReleaseMutex(hMutex);
}

std::string D3dTextConsole::readLine() {

	flushFromStream();
	listenning_to_char = true;
	timer = 0.0f;

	// wait for an enter...
	while (listenning_to_char) {}

	std::stringstream ans(m_stream.str());
	flushFromStream();
	return ans.str();
}

DWORD D3dTextConsole::blockUntillKey() {
	listenning_to_any = true;

	// wait for a key...
	while (listenning_to_any == true) {}

	return currentKey;
}

void D3dTextConsole::RegisterEvent(UINT msg, MsgCallback callbackFunc, void* parameter) {
	registeredMsgs.push_back( msg );
	registeredFuncs.push_back( callbackFunc );
	funcsParams.push_back(parameter);
}

void D3dTextConsole::draw(float timeDelta) {

	USES_CONVERSION;
	std::string everything = m_text + m_stream.str();

	// make the cursor blink.
	if (listenning_to_char) {
		timer += timeDelta;
		if (timer > 1.0f) timer = 0.0f;

		if (timer > 0.5f) {
			everything += "_";
		}
	}

	D3DVIEWPORT9 vp;
	m_device->GetViewport(&vp);
	SIZE size; 
	const char* stam = everything.c_str();

	m_font->GetTextExtent((TCHAR*)A2W(stam), (SIZE*)&size);

	if ((unsigned int)size.cy >= vp.Height) {
		m_text = "";
		m_stream.str("");
	}
/*	if ((unsigned int)size.cx >= vp.Width) {
		flushFromStream();
		m_text += "\n";
	}*/


	// draw
	m_font->DrawTextW(20, 20, m_color, (const TCHAR*)A2W(everything .c_str()));	
	
}

void D3dTextConsole::ProccessWinMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	
	if (WaitForSingleObject(hMutex, 1000) == WAIT_TIMEOUT) {
		MessageBoxA(0, "The wait timeout exceeded. Unbelievable.", 
			"D3dTextConsole::flushFromStream()", MB_OK);
	}

	if (msg == WM_KEYDOWN && listenning_to_any) {
		currentKey = (DWORD)wParam;
		listenning_to_any = false;
	}

	if (msg == WM_CHAR && listenning_to_char) {
		if (wParam == VK_BACK) {
			std::string text = m_stream.str();
			if (text.size() > 0) {
				text.erase(text.size()-1, 1);
				m_stream.str("");
				m_stream << text;
			}
		} else if (wParam == VK_RETURN) {
			// flush the stream into the text.
			m_stream << std::endl;	
			listenning_to_char = false;
		} else {
			m_stream << char(wParam);
		}
	}

	for (UINT i=0;i<registeredMsgs.size();i++) {
		if(msg == registeredMsgs[i]) {
			registeredFuncs[i](wParam, lParam, funcsParams[i]);
		}
	}

	ReleaseMutex(hMutex);
}

void D3dTextConsole::flushFromStream() {
	if (WaitForSingleObject(hMutex, 1000) == WAIT_TIMEOUT) {
		MessageBoxA(0, "The wait timeout exceeded. Unbelievable.", 
			"D3dTextConsole::flushFromStream()", MB_OK);
	}

	m_text += m_stream.str();
	m_stream.str("");

	ReleaseMutex(hMutex);
}

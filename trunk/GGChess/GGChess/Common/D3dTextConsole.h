#ifndef __WRITE_TEXT__
#define __WRITE_TEXT__

#include <d3d9.h>
#include <sstream>
#include <ATLBase.h>
#include <string>
#include <vector>
#include <string.h>
#include "d3dfont.h"

typedef void (*MsgCallback)(WPARAM , LPARAM, void*);

class D3dTextConsole {
public:
	~D3dTextConsole();
	static D3dTextConsole* getTextOutputObject();

	std::stringstream& stream() {return m_stream;}

	void init(IDirect3DDevice9* device);
	void draw(float timeDelta);

	void writeText(std::string text);
	std::string readLine();		// blocking

	void setFont(std::string fontName, int size);
	void setColor(DWORD color) { m_color = color; }

	void ProccessWinMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	void offTheRecord(std::string str);

	/************************************************************************/
	/* SYNCHRONUS I/O                                                       */
	/************************************************************************/
	DWORD blockUntillKey();
	void RegisterEvent(UINT msg, MsgCallback callbackFunc, void* parameter);

private:

	void flushFromStream();

	// font stuff
	CD3DFont* m_font;

	IDirect3DDevice9* m_device;

	volatile bool listenning_to_char;
	volatile bool listenning_to_any;

	DWORD currentKey;

	std::stringstream m_stream;
	std::string m_text;
	DWORD m_color;


	// Registered events
	std::vector<UINT> registeredMsgs;
	std::vector<MsgCallback> registeredFuncs;
	std::vector<void*> funcsParams;

	// sinchronization stuff
	HANDLE hMutex;

	// timer 
	float timer;

	// singletone stuff
	D3dTextConsole();
	static D3dTextConsole* object;
};

#endif // __WRITE_TEXT__
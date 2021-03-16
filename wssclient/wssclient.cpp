// wssclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <easywsclient\easywsclient.hpp>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#include <Windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>


using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;


void handle_message(const gsstl::string &message, void* userdata){
	printf("<<<< %s\n", message.c_str());
	printf(">>>>>>>>>>>>>>>>>>>> %s \n", (char *)userdata);

	if (message == "world") { ws->close(); }

}


void handle_error(const easywsclient::WSError &err, void* userdata){

}

int main()
{
#ifdef _WIN32
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
		return 1;
	}
#endif

	ws = WebSocket::from_url("wss://echo.websocket.org");
	assert(ws);
	
	ws->send("goodbye");
	ws->send("hello");

	while (ws->getReadyState() != WebSocket::CLOSED) {

		ws->poll(0, handle_error, 0);
		ws->dispatch(handle_message, handle_error, 0);

	}

	delete ws;
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

#pragma once

#include "Header.h"


#ifndef VERTEX_H
#define VERTEX_H

/* Window class */
#define VERTEX_WC  _T("CustomControl")

#endif

class Vertex {
private:
	static LRESULT CALLBACK VertexWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	UINT						id;
	FLOAT						weight;
	POINT						pt;
	POINT						center;
	HWND						hWnd;
	vector<UINT>			connections;
	string						name;
	BOOL						isSelected;
	BOOL						isValid;
public:
									Vertex();
									Vertex(UINT _id, HWND _hWnd, POINT _pt);
									~Vertex();
	BOOL						IsValid();
	static UINT				GenerateID();
	static UINT				GetLastAvailableNumAsVertexName();
	UINT						GetID();
	UINT						SetID(UINT _id);
	FLOAT						GetWeight();
	FLOAT						SetWeight(FLOAT _weight);
	POINT						GetCenter();
	POINT						GetPT();
	POINT						SetPT(POINT _pt);
	HWND&					GetWindow();
	HWND&					SetWindow(HWND& _hWnd);
	RECT						GetRect();
	vector<UINT>			GetConnections();
	vector<UINT>			SetConnections(vector<UINT> cons);
	UINT						Connect(UINT _id);
	UINT						Disconnect(UINT _id);
	string						GetName();
	string						SetName(string _name);
	BOOL						IsNear(const POINT _pt);
	BOOL						IsSelected();
	void							Select();
	void							Deselect();
	void							DrawVertex(HDC _mDC);
	void							RedrawVertex();
	static int					GetVertexIdx(UINT __id);
	static Vertex*			GetVertex(UINT __id);
	static Vertex*			GetVertex(HWND __hWnd);
	static Vertex*			GetSelected();
	static void				DeselectAll();
	static void				DeleteVertex(UINT _id);
	static void				DeleteSelected();
	static void				UpdateInfoPanels();
	static vector<pair<UINT, vector<UINT>>> GetUniqueConnectionsTable();

	static void				VertexRegister(void);
	static void				VertexUnregister(void);

	//	static void inline		OnLeftMouseMove(void);
	//	static void inline		OnRightMouseMove(void);
};

BOOL operator == (Vertex& _Left, Vertex& _Right);
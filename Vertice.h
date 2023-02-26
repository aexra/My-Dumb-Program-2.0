#pragma once

#include "Header.h"


#ifndef VERTICE_H
#define VERTICE_H

/* Window class */
#define VERTICE_WC  _T("CustomControl")

#endif

class Vertice {
private:
	static LRESULT CALLBACK VerticeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	UINT						id;
	FLOAT						weight;
	POINT						pt;
	HWND						hWnd;
	vector<UINT>			connections;
	string						name;
	BOOL						isSelected;
	BOOL isValid;
public:
									Vertice();
									Vertice(UINT _id, HWND _hWnd, POINT _pt);
	BOOL						IsValid();
	static UINT				GenerateID();
	UINT						GetID();
	UINT						SetID(UINT _id);
	FLOAT						GetWeight();
	FLOAT						SetWeight(FLOAT _weight);
	POINT						GetPT();
	POINT						SetPT(POINT _pt);
	HWND&					GetWindow();
	HWND&					SetWindow(HWND& _hWnd);
	vector<UINT>			GetConnections();
	vector<UINT>			SetConnections(vector<UINT> cons);
	UINT						Connect(UINT _id);
	UINT						Disconnect(UINT _id);
	string						GetName();
	string						SetName(string _name);
	BOOL						IsSelected();
	void							Select();
	void							Deselect();
	static int					GetVerticeIdx(UINT __id);
	static Vertice*		GetVertice(UINT __id);
	static void				DeselectAll();
	static void				DeleteSelected();
	static void				UpdateInfoPanels();

	static void				VerticeRegister(void);
	static void				VerticeUnregister(void);
};
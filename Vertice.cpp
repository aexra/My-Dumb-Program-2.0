#include "Vertice.h"

extern vector<Vertice> vertices;

Vertice::Vertice(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(_id);
	isSelected = false;
}

UINT Vertice::GetID() {
	return id;
}
UINT Vertice::SetID(UINT _id) {
	id = _id;
	return id;
}

FLOAT	 Vertice::GetWeight() {
	return weight;
}
FLOAT	 Vertice::SetWeight(FLOAT _weight) {
	weight = _weight;
	return weight;
}

POINT	 Vertice::GetPT() {
	return pt;
}
POINT	 Vertice::SetPT(POINT _pt) {
	pt = _pt;
	return pt;
}

HWND&	 Vertice::GetWindow() {
	return hWnd;
}
HWND&	 Vertice::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
	return hWnd;
}

// TODO
vector<UINT> Vertice::GetConnections() {
	return connections;
}
vector<UINT> Vertice::SetConnections(vector<UINT> cons) {
	return connections;
}

// TODO
UINT Vertice::Connect(UINT _id) {
	return _id;
}
UINT Vertice::Disconnect(UINT _id) {
	return _id;
}

string Vertice::GetName() {
	return name;
}
string Vertice::SetName(string _name) {
	name = _name;
	return name;
}

void Vertice::Select() {

}
void Vertice::Deselect() {

}
void Vertice::DeselectAll() {

}

void Vertice::DeleteSelected() {

}

void Vertice::UpdateInfoPanels() {

}


void Vertice::VerticeRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Vertice::VerticeWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = VERTICE_WC;
	RegisterClass(&wc);
}
void Vertice::VerticeUnregister(void)
{
	UnregisterClass(VERTICE_WC, NULL);
}

LRESULT CALLBACK VerticeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

}
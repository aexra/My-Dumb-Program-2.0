#include "Edge.h"

extern HWND FieldWnd;

vector<Edge*> Edge::edges = { };
ULONG Edge::lastID = 800;

Edge::Edge(Vertex* _Begin, Vertex* _End)
{
	begin = _Begin;
	end = _End;
	BUTTONPARAMS btp;
	btp.alignv = aligns::center;
	btn = new BUTTON(FieldWnd, "X", V3(crossPos.x, crossPos.y, 0),
		lastID++, V3(30, 30, 0));
	RecalcPosition();
	//btn->Hide();
	//MessageBoxA(NULL, (to_string(crossPos.x) + "--" + to_string(crossPos.y)).c_str(), "a", MB_OK);
}
POINT Edge::RecalcPosition()
{
	crossPos.x = abs(begin->GetCenter().x + end->GetCenter().x) / 2 - btn->transform.size.x;
	crossPos.y = abs(begin->GetCenter().y + end->GetCenter().y) / 2 - btn->transform.size.y;
	btn->MoveControl(V3(crossPos.x, crossPos.y, 0));
	return crossPos;
}
POINT Edge::getpt(BOOL toRecalc)
{
	return (toRecalc? RecalcPosition() : crossPos);
}
BUTTON* Edge::getbtn()
{
	return btn;
}
void Edge::DeleteEdge(Edge* e)
{
	BUTTON::DeleteButton(e->btn);
	delete e;
}

//bool operator==(const Edge& a, const Edge& b)
//{
//	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
//		return 1;
//	return 0;
//}
//bool operator!=(const Edge& a, const Edge& b)
//{
//	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
//		return 0;
//	return 1;
//}
//bool operator==(const Edge& a, const pair<Vertex*, Vertex*>& b)
//{
//	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
//		return 1;
//	return 0;
//}
//bool operator!=(const Edge& a, const pair<Vertex*, Vertex*>& b)
//{
//	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
//		return 0;
//	return 1;
//}

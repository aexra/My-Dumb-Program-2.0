#include "Edge.h"

extern HWND FieldWnd;

vector<Edge*> Edge::edges = { };
ULONG Edge::lastID = 800;

Edge::Edge(Vertex* _Begin, Vertex* _End)
{
	begin = _Begin;
	end = _End;
	RecalcPosition();
	btn = new BUTTON(FieldWnd, "x", V3(crossPos.x, crossPos.y, 0),
		lastID++);
	//btn->Hide();
}
POINT Edge::RecalcPosition()
{
	crossPos.x = abs(begin->GetCenter().x - end->GetCenter().x) / 2;
	crossPos.y = abs(begin->GetCenter().y - end->GetCenter().y) / 2;
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

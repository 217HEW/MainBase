#include <iostream>
#include <vector>

using std::vector;
using std::ostream;
using std::cout;
using std::endl;

template<typename T, size_t N>
inline size_t num_elms(const T(&)[N])
{
	return N;
}

typedef struct {
	double x;
	double y;
} POINT;

/*
* Polygon の各点を ostream に出力する
*/
ostream &printPolygon(ostream &os, const vector<POINT> &pv)
{
	vector<POINT > ::const_iterator cit;
	os << "Polygon ";
	for (cit = pv.begin(); cit != pv.end(); ++cit)
	{
		os << "(" << cit->x << ", " << cit->y << ") ";
	}
	os << endl;
	return os;
}

/*
* pt が pv 内部の点か判定する処理
*/
bool PtInPolygon(const POINT &pt, const vector<POINT> &pv)
{
	size_t ui, uj; // ui: 始点 index, uj : 終点 index
	int cnt = 0; // pt 点の右側の辺の数
	for (uj = 0; uj < pv.size(); uj++)
	{
		ui = (uj > 0) ? uj - 1 : pv.size() - 1;
		// pt 点の右側を通過する辺を数える
		if ((pv[ui].y - pt.y) * (pv[uj].y - pt.y) < 0)
		{
			double offset = 0;
			if (pv[ui].y != pv[uj].y)
			{
				offset = (pv[uj].x - pv[ui].x) * 
					(pt.y - pv[ui].y) / (pv[uj].y - pv[ui].y);
			}
			if (pv[ui].x + offset > pt.x)
			{
				++cnt;
			}
		}
	}
	return cnt % 2 ? true : false;
}

int main(void)
{
	POINT Polygon[] = { {1, 1}, {2, 1}, {4, 3}, {1, 3} };
	vector<POINT > pv(Polygon, Polygon + num_elms(Polygon)); // 多角形 vector
	POINT pt = { 0 }; // 指定点

	// 多角形
	printPolygon(cout, pv);
	// 指定点が中にある場合
	pt.x = 2.5;
	pt.y = 2.0;
	cout << "pt=(" << pt.x << ", " << pt.y << ") は";
	cout << (PtInPolygon(pt, pv) ? "内点です" : "外点です") << endl;
	// 指定点が外にある場合
	pt.x = 3.5;
	pt.y = 2.0;
	cout << "pt=(" << pt.x << ", " << pt.y << ") は";
	cout << (PtInPolygon(pt, pv) ? "内点です" : "外点です") << endl;

	return 0;
}
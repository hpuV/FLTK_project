#include "GUI.h"
#include <sstream>

using namespace Graph_lib;


void Button::attach(Graph_lib::Window& win)
	{
		pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}

int In_box::get_int()
{
	Fl_Input& pi = reference_to<Fl_Input>(pw);
//	return atoi(pi.value());
	const char* p = pi.value();
	if (!isdigit(p[0])) return -999999;
	return atoi(p);
}

string In_box::get_string()
{
	Fl_Input& pi = reference_to<Fl_Input>(pw);
	return string(pi.value());
}

void In_box::attach(Graph_lib::Window& win)
{
	pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}

//------------------------------------------------------------------------------
//額外定義
void In_box::put(int i)
{
	Fl_Output& po = reference_to<Fl_Output>(pw);
	std::stringstream ss;
	ss << i;
	po.value(ss.str().c_str());
}

void In_box::put(const string& s)
{
	reference_to<Fl_Output>(pw).value(s.c_str());
}


//------------------------------------------------------------------------------

void Out_box::put(int i)
{
	Fl_Output& po = reference_to<Fl_Output>(pw);
	std::stringstream ss;
	ss << i;
	po.value(ss.str().c_str());
}

void Out_box::put(const string& s)
{
	reference_to<Fl_Output>(pw).value(s.c_str());
}

void Out_box::attach(Graph_lib::Window& win)
{
	pw = new Fl_Output(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}


void MultiLineOut_Box::setValue(const string& s) {
	buff->text(s.c_str());
	}
void MultiLineOut_Box::append(const string& s) {
		pw->insert(s.c_str());
		int lins = pw->insert_position();
		pw->scroll(lins,0);
}
void MultiLineOut_Box::attach(Graph_lib::Window &window) {
	pw = new Fl_Text_Display(loc.x, loc.y, width, height, label.c_str());
	buff = new Fl_Text_Buffer();
	pw->buffer(buff);
	own = &window;
}
void MultiLineOut_Box::setAlign(Fl_Align a) {
	pw->align(a);
	}
void MultiLineOut_Box::saveFile(const string& filename) {
	buff->savefile(filename.c_str());
	}

Menu::Menu(Point xy, int w, int h, Kind kk, const string& s)
:Widget(xy,w,h,s,0), k(kk), offset(0)
{
}

int Menu::attach(Button& b)
{
	b.width = width;
	b.height = height;

	switch(k) {
	case horizontal:
		b.loc = Point(loc.x+offset,loc.y);
		offset+=b.width;
		break;
	case vertical:
		b.loc = Point(loc.x,loc.y+offset);
		offset+=b.height;
		break;
	}
	selection.push_back(&b);
	return int(selection.size()-1);
}

int Menu::attach(Button* p)
{
//	owned.push_back(p);
	return attach(*p);
}

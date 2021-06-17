#include <fstream>

#include "Graph.h"
#include "GUI.h"
#include "Window.h"

using namespace std;
using namespace Graph_lib;

struct IO_box : Widget{
     IO_box(Point xy, int w, int h, const string& title)
     :Widget(xy,w,h,title,0) {}

     string get_string();
     void put(const string&);
     void attach(Graph_lib::Window& win);
    };

string IO_box::get_string() {
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    return string(pi.value());
}
void IO_box::put(const string& s) {
        reference_to<Fl_Output>(pw).value(s.c_str());
}
void IO_box::attach(Graph_lib::Window& win) {
    pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
    own = &win;
}

// ------------------------------------

class Gui : Graph_lib::Window {
public:	
	Gui(Point xy,int x,int y,const string& title);
	
private:
	// declare various items
	Button quit;
	Button connect;
	Button menuBtn;
	IO_box ip;
	IO_box port;
	Out_box msg;
	Menu recent;
	// declare ip list for recent ip menu
	string recent_ip[4];
	// declare various function
	void readFile();
	// declare various callback function
	static void cb_quit(Address, Address);
	static void cb_menuBtn(Address,Address);
	static void cb_menu(Address,Address);
	static void cb_connect(Address,Address);
};

Gui::Gui(Point xy,int x,int y,const string& title):
Window(xy,x,y,title),
quit(Point(x_max()-70,0),70,25,"Quit",cb_quit),
connect(Point(100,200), 70,25, "Connect", cb_connect),
ip(Point(100,120),120,20,"IP Address"),
port(Point(100,160),50,20," Port: "),
msg(Point(100,240), 300, 80, "Message:"),
menuBtn(Point(15,10),180,30,"Recent", cb_menuBtn),
recent(Point(15,10),180,30,Menu::Kind::vertical,"recent")
{
	readFile();
	// add buttons to menu
	for (int i=0;i<3;++i) {
		recent.attach(new Button(Point(50,50),70,20,recent_ip[i],cb_menu));
	}
	// add items to window
	attach(recent);
	attach(quit);
	attach(connect);
	attach(menuBtn);
	attach(ip);
	attach(port);
	attach(msg);

	recent.hide();

}

// ------------------------------------------

void Gui::readFile() {
	fstream file;
	string line;
	file.open("./recent.txt", ios::in);
	for (int i=0;i<3;++i) {
		getline(file,line);
		recent_ip[i] = line;
	}
}
void Gui::cb_quit(Address, Address win) {
	reference_to<Gui>(win).hide();
}
void Gui::cb_menuBtn(Address, Address _win) {
	Gui& win = reference_to<Gui>(_win);
	win.menuBtn.hide();
	win.recent.show();
}
void Gui::cb_menu(Address _btn, Address _win) {
	Fl_Button* btn = (Fl_Button*)_btn;
	Gui& win = reference_to<Gui>(_win);
	string temp = btn->label();
	for (char& c : temp) {
    	if (c == ':') c = ' ';
    }
    stringstream ss{temp};
	string ip,port,no;
    ss >> no >> ip >> port;
	win.ip.put(ip);
	win.port.put(port);
	win.msg.put("You clicked recent connections #" + no);
	win.menuBtn.show();
	win.recent.hide();
}
void Gui::cb_connect(Address,Address _win) {
	Gui& win = reference_to<Gui>(_win);
	string ip = win.ip.get_string();
    string port = win.port.get_string();
	try {
		if (ip == "" && port =="") throw 2;
		else if (ip == "") throw 1;
		else if (port == "") throw 0;
		else win.msg.put("Trying to connect to " + ip + ":" + port + ".");
	} catch (int erro) {
		switch (erro) {
			case 0:
				win.msg.put("Please enter port.");
				return;
			case 1:
				win.msg.put("Please enter IP address.");
				return;
			case 2:
				win.msg.put("Please enter both IP address and port.");
				return;
		}
	}
}

// ------------------------------------------

int main () {
	Gui win(Point(100,100),600,400,"HomeWork");
	return gui_main();
}

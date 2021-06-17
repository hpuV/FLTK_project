#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "GUI.h"
#include "Window.h"

using namespace Graph_lib;
using namespace std;

//----------------------------------------------------------------------------
//define IO_box

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

//------------------------------------------------------------------------------

struct net_window : Graph_lib::Window {
    net_window(Point xy, int w, int h, const string& title );

private:
    Button connect_button;        //add connect & quit button
    Button quit_button;
 
    IO_box ipaddress;            
    IO_box portnum;
   
   Out_box mes_out;           //add out box for message
    
    Menu rec_menu;             //add menu & menu button
    Button menu_button;
    
    
    string ipname[4]={"","","",""};

    void fileread();
    void filewrite();

    bool isVisible = false;
    
    void hide_menu(){rec_menu.hide();}
    void menu_pressed(){rec_menu.show();}
    
    
    static void cb_menu(Address, Address);
    void menu();   

    static void cb_menubtn(Address, Address);
    void menubtn();   
    static void cb_connect(Address, Address);     
    void connect();
    static void cb_quit(Address, Address);
    void quit();
};

//------------------------------------------------------------------------------

net_window::net_window(Point xy, int w, int h, const string& title)
:Window(xy,w,h,title),
    connect_button(Point(100,220), 70,25, "Connect", cb_connect),
    quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
    ipaddress(Point(100,140), 110, 20, "IP Address:"),
    portnum(Point(100,180), 45, 20, "Port:"),
    mes_out(Point(100,260), 300, 80, "Message:"),
    rec_menu(Point(0,25),180,30,Menu::Kind::vertical,"rec"),
    menu_button(Point(0,0), 180, 25, "Recent Connections", cb_menubtn)
{
    
    //add button to menu
    fileread();

    attach(connect_button);
    attach(quit_button);
    attach(ipaddress);
    attach(portnum);
    attach(mes_out);
    attach(rec_menu);
    
    attach(menu_button);
    rec_menu.hide();	
}

void net_window::fileread(){
    fstream file;
    file.open("recent.txt",std::ios::in);
    for(int i=0; i < 3;++i){
	    std::getline(file,ipname[i]);
        for (char& c : ipname[i]){
            if(c == ' ') c = ':';
        }
        stringstream ss;
        ss << i + 1 << ". " << ipname[i];
        rec_menu.attach(new Button(Point(50,50),70,20,ss.str(),cb_menu));  
	}
    file.close();
    }

void net_window::filewrite(){
    
    stringstream ss;
    
    mes_out.put(ss.str());
    }

//------------------------------------------------------------------------------

void net_window::cb_quit(Address, Address pw)
{  
       reference_to<net_window>(pw).quit();
} 

//------------------------------------------------------------------------------

void net_window::quit()
{
    hide();        
}

//------------------------------------------------------------------------------

void net_window::cb_connect(Address, Address _pw)
{  
    net_window& pw = reference_to<net_window>(_pw);
    
    string ip = pw.ipaddress.get_string();
    
    string port = pw.portnum.get_string();

    try{
	if(ip == "" & port == "") throw 2;
	else if(ip == "") throw 1;
	else if(port == "") throw 0;
	else pw.mes_out.put("Trying to connect to " + ip + ":" + port + ".");
	
	}catch(int ero){
	    switch(ero){
		case 0:
		       pw.mes_out.put("Please enter Port.");
		       return;
		case 1:
		       pw.mes_out.put("Please enter IP Address.");
		       return;
		case 2:
		       pw.mes_out.put("Please enter both IP Address and Port.");
		       return;	
		}
	    }

} 
//-------------------------------------------------------------------------------

void net_window::cb_menubtn(Address, Address pw)
 {  
    reference_to<net_window>(pw).menu();                                  
     }
void net_window::menu(){
    if(isVisible)
	hide_menu();
    else
	menu_pressed();    

    isVisible = !isVisible;
}


void net_window::cb_menu(Address _btn, Address _pw)
 {  
     Fl_Button* btn = (Fl_Button*)_btn;
     net_window& pw = reference_to<net_window>(_pw);                                  

     string temp = btn->label();

    for (char& c : temp){
	if (c == ':') c = ' '; //' ' replace to ':'
    }
    stringstream ss{temp};
    string num, ip, port;
    ss >> num >> ip >> port;
    pw.ipaddress.put(ip);
    pw.portnum.put(port);
    pw.mes_out.put("You clicked recent connections #" + num);
    pw.rec_menu.hide();
    pw.isVisible = false;
}


//------------------------------------------------------------------------------

int main()
try {
    net_window win(Point(100,100),600,400,"mud_lab_01");
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Some exception\n";
    return 2;
}

//------------------------------------------------------------------------------

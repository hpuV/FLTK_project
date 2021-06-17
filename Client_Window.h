#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include "Socket.h"
#include "Window.h"
#include "GUI.h"
#include <fstream>

using namespace Graph_lib;


//window2
class client_window : public Graph_lib::Window {
    public:
        client_window(int,int,string,Socket*);
        ~client_window();
    private:
        //window2
        Button discon_button;
        Button send_button;
        In_box snmsg;

        Menu fl_menu;
        Button flmenu_button;
        
        Socket* socket;
        MultiLineOut_Box msg_out;

        string btname[2]={"",""};

        bool isVisible = false;
    
        void hide_menu(){fl_menu.hide();}
        void menu_pressed(){fl_menu.show();}

        static void cb_flmenu(Address, Address);
        void menu();   
        static void cb_flmenubtn(Address, Address); 
        static void cb_send(Address, Address);     
        static void cb_savebt(Address,Address);
        static void cb_SaveAsbt(Address,Address);
        static void cb_discon(Address, Address);
        void discon();
        static void cb_msgon(FL_SOCKET,Address);
};

client_window::client_window(int w,int h,string t,Socket* s):
    Window(w,h,t),
    send_button(Point(x_max()-100,y_max()-30),100,30,"Send", cb_send),
    discon_button(Point(x_max()-70,0), 70, 20, "disconnect", cb_discon),
    snmsg(Point(0,y_max()-30),x_max()-100,30,""),
    msg_out(Point(0,30),w,h-60,""),
    fl_menu(Point(0,30),100,30,Menu::Kind::vertical,""),
    flmenu_button(Point(0,0),100,30,"File", cb_flmenubtn)
{

    socket = s;

    fl_menu.attach(new Button(Point(0,0),0,0,"save",cb_savebt));
    fl_menu.attach(new Button(Point(0,0),0,0,"save as",cb_SaveAsbt));

    attach(send_button);
    attach(discon_button);
    attach(snmsg);
    attach(fl_menu);
    attach(flmenu_button);

    fl_menu.hide();	

    Fl::add_fd(socket->getFd(),cb_msgon,this);
}
//------------------------------------------------------------------------------
client_window::~client_window() {
    delete socket;
}

void client_window::cb_discon(Address, Address pw)
{  
       reference_to<client_window>(pw).discon();
}
void client_window::discon()
{
    hide();        
}

void client_window::cb_flmenubtn(Address, Address pw)
 {  
    client_window& _pw = reference_to<client_window>(pw);
    if(_pw.isVisible){
	_pw.hide_menu();
    }
    else{
	_pw.menu_pressed();
    }  
    _pw.isVisible = !_pw.isVisible;
}

void client_window::cb_msgon(FL_SOCKET fd,Address pw) {
    client_window& _pw = reference_to<client_window>(pw);
    _pw.msg_out.append(_pw.socket->getMessage());
}

void client_window::cb_send(Address,Address pw) {
    client_window& _pw = reference_to<client_window>(pw);
    string input_string = _pw.snmsg.get_string() + "\n";
    _pw.snmsg.put("");
    _pw.msg_out.append(input_string);
    _pw.socket->sendMessage(input_string);
}

void client_window::cb_savebt(Address,Address _win) {
    client_window& win = reference_to<client_window>(_win);
    win.msg_out.saveFile("log.txt");
    win.fl_menu.hide();
    win.isVisible = false;
}

void client_window::cb_SaveAsbt(Address,Address) {

}

#endif
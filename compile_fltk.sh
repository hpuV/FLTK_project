
subname=${1##*.}
prename=${1%.*}

g++ -I/usr/include/cairo \
-I/usr/include/glib-2.0 \
-I/usr/lib/i386-linux-gnu/glib-2.0/include \
-I/usr/include/pixman-1 \
-I/usr/include/freetype2 \
-I/usr/include/libpng12 \
-I/usr/include/freetype2 \
-I/usr/include/cairo \
-I/usr/include/glib-2.0 \
-I/usr/lib/i386-linux-gnu/glib-2.0/include \
-I/usr/include/pixman-1 \
-I/usr/include/freetype2 \
-I/usr/include/libpng12 \
-g -O2 \
-D_LARGEFILE_SOURCE \
-D_LARGEFILE64_SOURCE \
-D_FILE_OFFSET_BITS=64 \
-D_THREAD_SAFE \
-D_REENTRANT \
Graph.cpp Window.cpp GUI.cpp \
-lfltk_images \
-lfltk \
-lX11 \
-o $prename $1 \
-std=c++11


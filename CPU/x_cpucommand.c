/*
gcc xlib_hello.c -o xlib_hello -I/opt/X11/include -L/opt/X11/lib/ -lX11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xlocale.h>
#include <X11/Xresource.h>



void RedrawText(Display, Window, XTextItem, XFontStruct*);

int HandleKeyRelease()
{

}


int main(int argc, char *argv[])
{

	Display *dpy;
	int screen;
	Window win;
	XEvent event;

	int x, y;
	unsigned int width, height;
	Window root_win;			
	Window childwin;
	Window textinput;
	Colormap colormap;

	//colors
	unsigned long black, white;

	//button stuff
	XColor button_color;
	XColor lightgray_color, darkgray_color;
	XGCValues gcv_lightgray, gcv_darkgray;
	GC gc_lightgray, gc_darkgray;
	unsigned int border_width;
	unsigned int depth;

	// text stuff
	XFontStruct *font;
	XTextItem ti[1];

	// input stuff

	XIM im;
	XIC ic;
	char *failed_arg;
	XIMStyles *styles;
	XIMStyle xim_requested_style;
	XTextItem in[1];

	//prep input stuff

	if(setlocale(LC_ALL, "") == NULL)
	{
		return 9;
	}

	if (!XSupportsLocale())
	{
		return 10;
	}

	if (XSetLocaleModifiers("@im=none") == NULL)
	{
		return 11;
	}



	dpy = XOpenDisplay(NULL);

	if(dpy == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		return 7;
	}

	screen = DefaultScreen(dpy);

	// parent window

	win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
		200, 200, 500, 300,
		1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));

	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);




	// 3 shades of gray for the button

	colormap = DefaultColormap(dpy, screen);
	XParseColor(dpy, colormap, "rgb:cc/cc/cc", &button_color);
	XAllocColor(dpy, colormap, &button_color);

	XParseColor(dpy, colormap, "rgb:ee/ee/ee", &lightgray_color);
	XAllocColor(dpy, colormap, &lightgray_color);
	gcv_lightgray.foreground = lightgray_color.pixel;
	gcv_lightgray.background = button_color.pixel;
	gc_lightgray = XCreateGC(dpy, RootWindow(dpy, screen),
		GCForeground | GCBackground, &gcv_lightgray);

	XParseColor(dpy, colormap, "rgb:88/88/88", &darkgray_color);
	XAllocColor(dpy, colormap, &darkgray_color);
	gcv_darkgray.foreground = darkgray_color.pixel;
	gcv_darkgray.background = button_color.pixel;
	gc_darkgray = XCreateGC(dpy, RootWindow(dpy, screen),
		GCForeground | GCBackground, &gcv_darkgray);

	// color macros
  black = BlackPixel(dpy, screen);
  white = WhitePixel(dpy, screen);




	childwin = XCreateSimpleWindow(dpy, win,
		150, 150, 200, 100,
		1, BlackPixel(dpy, screen), button_color.pixel);

	XSelectInput(dpy, childwin, 
		ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask);
	XMapWindow(dpy, childwin);

	XGetGeometry(dpy, childwin, &root_win, &x, &y, &width, &height,
		&border_width, &depth);

	// int xtext, ytext, widthtext, heighttext;
	// unsigned int text_border_width, text_depth;
	// XGetGeometry(dpy, textinput, &root_win, &xtext, &ytext, &widthtext, &heighttext,
	// 	&text_border_width, &text_depth);



	// input window
	font = XLoadQueryFont(dpy, "7x14");

	textinput = XCreateSimpleWindow(dpy, win,
		50, 50, 100, font->ascent+4, 1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));
	XSelectInput(dpy, textinput, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);
	XMapWindow(dpy, textinput);

	//init input stuff
	im = XOpenIM(dpy, NULL, NULL, NULL);
	if (im == NULL)
	{
		fputs("Could not open input method\n", stdout);
		return 2;
	}

	failed_arg = XGetIMValues(im, XNQueryInputStyle, &styles, NULL);

	if (failed_arg != NULL)
	{
		fputs("XIM can't get styles\n", stdout);
		return 3;
	}

	int i;
	for (i = 0; i < styles->count_styles; i++)
	{
		printf("style %lu\n", styles->supported_styles[i]);
	}
	ic = XCreateIC(im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, textinput, NULL);
	if (ic == NULL)
	{
		printf("Could not open IC\n");
		return 4;
	}

	char * inputbuffer;
	inputbuffer = (char*) malloc(10*(sizeof(char)));
	if (inputbuffer == NULL) exit(1);

	int nbchars_input = 0;

	XSetICFocus(ic);

	while(1)
	{
		XNextEvent(dpy, &event);
		// if (event.xany.window != textinput)
		// {
		// 	XUnsetICFocus(ic);
		// }
		if (event.xany.window == textinput)
		{
			// if(event.type == ButtonPress && event.xbutton.button == 1)
			// {
			// 	XSetICFocus(ic);
			// }
			switch(event.type){
	        case MappingNotify:
	            XRefreshKeyboardMapping(&event.xmapping);
	            break;
	        case KeyPress:
	            {
	                int count = 0;
	                KeySym keysym = 0;
	                //char buf[20];
	                Status status = 0;

	                count = Xutf8LookupString(ic, (XKeyPressedEvent*)&event, &inputbuffer[nbchars_input], 10*(sizeof(char)), &keysym, &status);

	                printf("count: %d\n", count);
	                if (status==XBufferOverflow)
	                    printf("BufferOverflow\n");

									if(keysym == 65288)
	                {
	                	if(nbchars_input == 1)
	                	{
	                		inputbuffer[0] = '\0';
	                		nbchars_input--;
	                		break;
	                	}
	                	printf("delete pressed\n");
	                	nbchars_input -= 2;
	                	inputbuffer[nbchars_input] = '\0';
	                }
	                nbchars_input++;
	                if (count) {
	                	in[0].chars = inputbuffer;
	                	in[0].nchars = nbchars_input;
	                	in[0].delta = 0;
	                	in[0].font = font->fid;

	                    printf("buffer: %.*s\n", nbchars_input, inputbuffer);
	                }

	                if (status == XLookupKeySym || status == XLookupBoth) {
	                    printf("status: %d\n", status);
	                }
	                printf("pressed KEY: %lu\n", keysym);
	            }
	            break;
	        case KeyRelease:
	            {

	                int count = 0;
	                KeySym keysym = 0;
	                //char buf[20];
	                Status status = 0;
	                count = Xutf8LookupString(ic, (XKeyEvent*)&event, &inputbuffer[nbchars_input], 10*(sizeof(char)), &keysym, &status);
	                

	                if(keysym == 65288)
	                {
	                	if(nbchars_input == 1)
	                	{
	                		inputbuffer[0] = '\0';
	                		nbchars_input--;
	                		break;
	                	}
	                	printf("delete pressed\n");
	                	nbchars_input -= 2;
	                	inputbuffer[nbchars_input] = '\0';
	                }
	                nbchars_input++;
	                if (count) {
	                	in[0].chars = inputbuffer;
	                	in[0].nchars = nbchars_input;
	                	in[0].delta = 0;
	                	in[0].font = font->fid;

	                    printf("buffer: %.*s\n", nbchars_input, inputbuffer);
	                }
	                RedrawText(dpy, textinput, in, font);
	              }
	            break;
	        case ConfigureNotify:
	            if (width != event.xconfigure.width
	                    || height != event.xconfigure.height) {
	                width = event.xconfigure.width;
	                height = event.xconfigure.height;
	                printf("Size changed to: %d by %d", width, height);
	            }
	            break;
	          }
			}
		if(event.xany.window == childwin)
		{
			if(event.type == Expose)
			{
				XDrawLine(dpy, childwin, gc_lightgray,
					0, 0, width-1, 0);
				XDrawLine(dpy, childwin, gc_lightgray,
					0, 0, 0, height-1);
				XDrawLine(dpy, childwin, gc_darkgray,
					width-1, 0, width-1, height-1);
				XDrawLine(dpy, childwin, gc_darkgray,
					0, height-1, width-1, height-1);
				font = XLoadQueryFont(dpy, "7x14");
				ti[0].chars = "Start";
				ti[0].nchars = 5;
				ti[0].delta = 0;
				ti[0].font = font->fid;
				XDrawText(dpy, childwin, DefaultGC(dpy, screen),
					(width-XTextWidth(font, ti[0].chars, ti[0].nchars))/2,
					(height-(font->ascent+font->descent))/2+font->ascent,
					ti, 1);
				//XUnloadFont(dpy, font->fid);
			}
			if(event.type == ButtonPress && event.xbutton.button == 1)
			{
				XDrawLine(dpy, childwin, gc_darkgray,
					0, 0, width-1, 0);
				XDrawLine(dpy, childwin, gc_darkgray,
					0, 0, 0, height-1);
				XDrawLine(dpy, childwin, gc_lightgray,
					width-1, 0, width-1, height-1);
				XDrawLine(dpy, childwin, gc_lightgray,
					0, height-1, width-1, height-1);
			}
			if(event.type == ButtonRelease && event.xbutton.button == 1)
			{
				XDrawLine(dpy, childwin, gc_lightgray,
					0, 0, width-1, 0);
				XDrawLine(dpy, childwin, gc_lightgray,
					0, 0, 0, height-1);
				XDrawLine(dpy, childwin, gc_darkgray,
					width-1, 0, width-1, height-1);
				XDrawLine(dpy, childwin, gc_darkgray,
					0, height-1, width-1, height-1);
			}
		}
	}

	return 0;
}

//clear and refresh text for textinput windows
void RedrawText(Display dpy, Window win, XTextItem txt, XFontStruct *font)
{
	XClearArea(dpy, win, 0, 0, 0, 0, 1);
	int screen = DefaultScreen(dpy);
	XDrawText(dpy, win, DefaultGC(dpy, screen),
		2, (font->ascent+2),
		txt, 1);
	return;
}
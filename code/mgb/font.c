#include <stdlib.h>
#include <stdarg.h>
#include "mgb.h"

#define FDOWN (145*2)
unsigned char *fontready;
int fwidth;
int fheight;
int fxpos,fypos;
#define FMAXX 128
#define FMAXY 128
unsigned char *textstate,*textbak;

unsigned char tainted=0;

#define SCROLLHISTORYSIZE 8192
int scrollhistoryin;
unsigned char *scrollhistory;

unsigned char fontdata[96][13]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x00,0x00},
{0x00,0x0a,0x0a,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x0a,0x0a,0x1f,0x0a,0x1f,0x0a,0x0a,0x00,0x00,0x00,0x00},
{0x00,0x04,0x1e,0x05,0x05,0x0e,0x14,0x14,0x0f,0x04,0x00,0x00,0x00},
{0x00,0x12,0x15,0x0a,0x08,0x04,0x02,0x0a,0x15,0x09,0x00,0x00,0x00},
{0x00,0x02,0x05,0x05,0x02,0x05,0x19,0x09,0x16,0x00,0x00,0x00,0x00},
{0x00,0x0c,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x08,0x04,0x04,0x02,0x02,0x02,0x04,0x04,0x08,0x00,0x00,0x00},
{0x00,0x02,0x04,0x04,0x08,0x08,0x08,0x04,0x04,0x02,0x00,0x00,0x00},
{0x00,0x00,0x04,0x15,0x1f,0x0e,0x1f,0x15,0x04,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x04,0x04,0x1f,0x04,0x04,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x04,0x02,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x0e,0x04,0x00,0x00},
{0x00,0x10,0x10,0x08,0x08,0x04,0x02,0x02,0x01,0x01,0x00,0x00,0x00},
{0x00,0x04,0x0a,0x11,0x11,0x11,0x11,0x11,0x0a,0x04,0x00,0x00,0x00},
{0x00,0x04,0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x1f,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x10,0x08,0x04,0x02,0x01,0x1f,0x00,0x00,0x00},
{0x00,0x1f,0x10,0x08,0x04,0x0e,0x10,0x10,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x08,0x08,0x0c,0x0a,0x0a,0x09,0x1f,0x08,0x08,0x00,0x00,0x00},
{0x00,0x1f,0x01,0x01,0x0d,0x13,0x10,0x10,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x01,0x01,0x0f,0x11,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x1f,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x02,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x11,0x0e,0x11,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x11,0x1e,0x10,0x10,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x00,0x00,0x04,0x0e,0x04,0x00,0x00,0x04,0x0e,0x04,0x00,0x00},
{0x00,0x00,0x00,0x04,0x0e,0x04,0x00,0x00,0x0c,0x04,0x02,0x00,0x00},
{0x00,0x10,0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x10,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x1f,0x00,0x00,0x00,0x00,0x00},
{0x00,0x01,0x02,0x04,0x08,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x10,0x08,0x04,0x04,0x00,0x04,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x19,0x15,0x15,0x0d,0x01,0x1e,0x00,0x00,0x00},
{0x00,0x04,0x0a,0x11,0x11,0x11,0x1f,0x11,0x11,0x11,0x00,0x00,0x00},
{0x00,0x0f,0x12,0x12,0x12,0x0e,0x12,0x12,0x12,0x0f,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x01,0x01,0x01,0x01,0x01,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x0f,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x0f,0x00,0x00,0x00},
{0x00,0x1f,0x01,0x01,0x01,0x0f,0x01,0x01,0x01,0x1f,0x00,0x00,0x00},
{0x00,0x1f,0x01,0x01,0x01,0x0f,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x01,0x01,0x01,0x19,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x11,0x11,0x11,0x11,0x1f,0x11,0x11,0x11,0x11,0x00,0x00,0x00},
{0x00,0x0e,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0e,0x00,0x00,0x00},
{0x00,0x1c,0x08,0x08,0x08,0x08,0x08,0x08,0x09,0x06,0x00,0x00,0x00},
{0x00,0x11,0x11,0x09,0x05,0x03,0x05,0x09,0x11,0x11,0x00,0x00,0x00},
{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x1f,0x00,0x00,0x00},
{0x00,0x11,0x11,0x1b,0x15,0x15,0x11,0x11,0x11,0x11,0x00,0x00,0x00},
{0x00,0x11,0x13,0x13,0x15,0x15,0x19,0x19,0x11,0x11,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x0f,0x11,0x11,0x11,0x0f,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x15,0x0e,0x10,0x00,0x00},
{0x00,0x0f,0x11,0x11,0x11,0x0f,0x05,0x09,0x11,0x11,0x00,0x00,0x00},
{0x00,0x0e,0x11,0x01,0x01,0x0e,0x10,0x10,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x1f,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00},
{0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x11,0x11,0x11,0x11,0x0a,0x0a,0x0a,0x04,0x04,0x00,0x00,0x00},
{0x00,0x11,0x11,0x11,0x11,0x15,0x15,0x15,0x1b,0x11,0x00,0x00,0x00},
{0x00,0x11,0x11,0x0a,0x0a,0x04,0x0a,0x0a,0x11,0x11,0x00,0x00,0x00},
{0x00,0x11,0x11,0x0a,0x0a,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00},
{0x00,0x1f,0x10,0x08,0x08,0x04,0x02,0x02,0x01,0x1f,0x00,0x00,0x00},
{0x00,0x0e,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x0e,0x00,0x00,0x00},
{0x00,0x01,0x01,0x02,0x02,0x04,0x08,0x08,0x10,0x10,0x00,0x00,0x00},
{0x00,0x0e,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0e,0x00,0x00,0x00},
{0x00,0x04,0x0a,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00},
{0x00,0x0c,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x10,0x1e,0x11,0x11,0x1e,0x00,0x00,0x00},
{0x00,0x01,0x01,0x01,0x0f,0x11,0x11,0x11,0x11,0x0f,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x11,0x01,0x01,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x10,0x10,0x10,0x1e,0x11,0x11,0x11,0x11,0x1e,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x11,0x1f,0x01,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x0c,0x12,0x02,0x02,0x0f,0x02,0x02,0x02,0x02,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x11,0x11,0x11,0x1e,0x10,0x11,0x0e,0x00},
{0x00,0x01,0x01,0x01,0x0d,0x13,0x11,0x11,0x11,0x11,0x00,0x00,0x00},
{0x00,0x00,0x04,0x00,0x06,0x04,0x04,0x04,0x04,0x0e,0x00,0x00,0x00},
{0x00,0x00,0x08,0x00,0x0c,0x08,0x08,0x08,0x08,0x09,0x09,0x06,0x00},
{0x00,0x01,0x01,0x01,0x09,0x05,0x03,0x05,0x09,0x11,0x00,0x00,0x00},
{0x00,0x06,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0e,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0b,0x15,0x15,0x15,0x15,0x11,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0d,0x13,0x11,0x11,0x11,0x11,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x11,0x11,0x11,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0f,0x11,0x11,0x11,0x0f,0x01,0x01,0x01,0x00},
{0x00,0x00,0x00,0x00,0x1e,0x11,0x11,0x11,0x1e,0x10,0x10,0x10,0x00},
{0x00,0x00,0x00,0x00,0x0d,0x13,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0e,0x11,0x06,0x08,0x11,0x0e,0x00,0x00,0x00},
{0x00,0x00,0x02,0x02,0x0f,0x02,0x02,0x02,0x12,0x0c,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x19,0x16,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x0a,0x0a,0x04,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x11,0x11,0x15,0x15,0x15,0x0a,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x11,0x0a,0x04,0x04,0x0a,0x11,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x19,0x16,0x10,0x11,0x0e,0x00},
{0x00,0x00,0x00,0x00,0x1f,0x08,0x04,0x02,0x01,0x1f,0x00,0x00,0x00},
{0x00,0x18,0x04,0x04,0x04,0x03,0x04,0x04,0x04,0x18,0x00,0x00,0x00},
{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00},
{0x00,0x03,0x04,0x04,0x04,0x18,0x04,0x04,0x04,0x03,0x00,0x00,0x00},
{0x00,0x12,0x15,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f}};

unsigned char *fonty[64],*fonts[256];
int fontx[128];

void initfont(void)
{
unsigned short white,black,red;
int i,j,k;
unsigned char *p;
unsigned short *ps,*ps2;

	scrollhistoryin=0;
	scrollhistory=malloc(FMAXX*SCROLLHISTORYSIZE);
	if(!scrollhistory) nomem(30);
	textstate=malloc(FMAXX*FMAXY);
	if(!textstate) nomem(21);
	textbak=malloc(FMAXX*FMAXY);
	if(!textbak) nomem(31);
	memset(textstate,' ',FMAXX*FMAXY);
	white=maprgb(255,255,255);
	black=maprgb(0,0,0);
	red=maprgb(255,0,0);
	fontready=malloc(2*96*6*2*13);
	if(!fontready) return;
	p=&fontdata[0][0];
	ps=(void *)fontready;
	ps2=(void *)(fontready+96*6*2*13);
	for(i=0;i<96*13;++i)
	{
		k=*p++;
		for(j=0;j<6;++j)
		{
			if(k&0x01)
			{
				*ps++=white;
				*ps2++=black;
			}
			else
			{
				*ps++=black;
				*ps2++=red;
			}
			k>>=1;
		}
	}
	p=videomem+FDOWN*stride;
	for(i=0;i<64;++i)
	{
		fonty[i]=p;
		p+=13*stride;
	}
	for(i=0;i<128;++i)
		fontx[i]=i*12;
	for(i=0;i<256;++i)
	{
		j=i&0x7f;
		if(j<0x20) j=0;
		else j-=0x20;
		if(i&0x80) j+=96;
		fonts[i]=fontready+j*6*13*2;
	}
	fwidth=IXSIZE/6;
	fheight=(IYSIZE-FDOWN)/13;
	fxpos=fypos=0;
}
void drawcharxy(unsigned int x,unsigned int y,unsigned char c)
{
unsigned char *ps,*pd;
int i;

	tainted=1;
	if(x>=fwidth || y>=fheight) return;
	if(x<FMAXX && y<FMAXY)
		textstate[y*FMAXX+x]=c;
	pd=fonty[y]+fontx[x];
	ps=fonts[c];

	for(i=0;i<13;++i)
	{
		memcpy(pd,ps,12);
		pd+=stride;
		ps+=12;
	}
}
void drawstringxy(unsigned int x,unsigned int y,char *s)
{
char ch;
	 while((ch=*s++))
		drawcharxy(x++,y,ch);
}

void con(void)
{
	drawcharxy(fxpos,fypos,0x80 ^ textstate[fypos*FMAXX+fxpos]);
}
void coff(void)
{
	drawcharxy(fxpos,fypos,0x80 ^ textstate[fypos*FMAXX+fxpos]);
}

void scrollup(void)
{
int i,j,k;
unsigned char *p1,*p2;

	j=13*(fheight-1);
	k=fontx[fwidth];
	p1=fonty[0];
	p2=fonty[1];
	for(i=0;i<j;++i)
	{
		memcpy(p1,p2,k);
		p1+=stride;
		p2+=stride;
	}
	for(j=0;j<13;++i,++j)
	{
		memset(p1,0,k);
		p1+=stride;
	}
	p1=textstate;
	p2=textstate+FMAXX;
	memcpy(scrollhistory+(scrollhistoryin++ & (SCROLLHISTORYSIZE-1))*FMAXX,p1,FMAXX);
	for(i=0;i<FMAXY-1;++i)
	{
		memcpy(p1,p2,FMAXX);
		p1+=FMAXX;
		p2+=FMAXX;
	}
	memset(p1,' ',FMAXX);
}

void ddprints(char *s)
{
char *p,ch;
static char escape=0;
int i;
static int escapedata[20],ecount;

	coff();
	p=s;
	ecount=escape=0;
	while((ch=*p++))
	{
		if(escape)
		{
			if(ch>='0' && ch<='9')
			{
				escapedata[ecount]*=10;
				escapedata[ecount]+=ch-'0';
			} else if(ch==';')
			{
				++ecount;
				escapedata[ecount]=0;
			}
			else
			{
				escape=0;
				switch(ch)
				{
				case 'k':
					i=fxpos;
					while(i<fwidth)
						drawcharxy(i++,fypos,' ');
					break;
				case 'x':
					fxpos=escapedata[0];
					if(fxpos<=0) fxpos=0;
					if(fxpos>=fwidth) fxpos=fwidth-1;
					break;
				}
			}
			continue;
		}
		switch(ch)
		{
		case '\r':
			fxpos=0;
			break;
		case '\n':
			fxpos=0;
			if(fypos<fheight-1)
				++fypos;
			else
				scrollup();
			break;
		case '\t':
			while(fxpos&7)
				drawcharxy(fxpos++,fypos,' ');
			break;
		case 8:
			if(fxpos)
				drawcharxy(--fxpos,fypos,' ');
			break;
		case 0x1b:
			ecount=0;
			escapedata[ecount]=0;
			escape=1;
			break;
		default:
			drawcharxy(fxpos++,fypos,ch);
			break;
		}
	}
	con();
	updatef();
}
void ddprintf(char *s,...)
{
char tbuff[256];
va_list ap;
	va_start(ap, s);
	vsprintf(tbuff,s,ap);
	va_end(ap);
	ddprints(tbuff);
}
void dco(char c)
{
	if(fxpos>=fwidth)
		ddprints("\n");
	ddprintf("%c",c);
}
void ddprintfxy(unsigned int x,unsigned int y,char *s,...)
{
char tbuff[256];
va_list ap;
	va_start(ap, s);
	vsprintf(tbuff,s,ap);
	va_end(ap);
	drawstringxy(x,y,tbuff);
}

void repaint(void)
{
int i,j;
unsigned char *p;

	for(j=0;j<fheight;++j)
	{
		p=textstate+j*FMAXX;
		for(i=0;i<fwidth;++i)
			drawcharxy(i,j,*p++);
	}
}


int showhistory(int back)
{
unsigned char *p;
int i,j,k;

	if(back>scrollhistoryin) back=scrollhistoryin;
	memcpy(textbak,textstate,FMAXX*FMAXY);
	for(j=0;j<fheight;++j)
	{
		k=scrollhistoryin-back+j;
		if(k<scrollhistoryin)
			p=scrollhistory+FMAXX*(k&(SCROLLHISTORYSIZE-1));
		else
			p=textbak+FMAXX*(k-scrollhistoryin);
		for(i=0;i<fwidth;++i)
			drawcharxy(i,j,*p++);
	}

	updatef();
	memcpy(textstate,textbak,FMAXX*FMAXY);
	return back;
}

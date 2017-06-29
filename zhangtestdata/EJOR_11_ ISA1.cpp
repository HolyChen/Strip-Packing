//this is a good version of program
#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include<algorithm>



//using namespace std;

#define Max 80315
     //Maximum packing number

int L,W,n,g;//Actual length and width
int h,besth,waste,bestwaste;
int minweigh,minlong;
//int maxweigh,maxlong;
int typen;
int kkk;
//int lh;
//           L
//  (cx,minh)---------
//     |              |
//  W  |              |
//     |              |
//     ---------------
typedef struct
{
	int x;
	int y;
	int h1;
    int h2;
	int w;
}RP;

typedef struct
{
	int length;
	int width;
	int area,carea;
	int ntype;
}Rect;
Rect r[Max];
int bestrr[Max];
int y[Max];

RP R[Max];
typedef struct
{
	int length;
	int width;
	int flag;
}Rect1;
Rect1 r1[Max];
double a;

void sort(int rr[])
{
	int i,j;
	for(i=0;i<typen;i++)
		rr[i]=i;
	for(i=1;i<typen;i++)
	{
	      		for(j=0;j<i&&r[rr[i]].length<r[rr[j]].length;j++)  // sorting by area or perimeter from big to small
//	for(j=0;j<i&&r[rr[i]].width<r[rr[j]].width;j++)  // sorting by area or perimeter from big to small
//		for(j=0;j<i&&r[rr[i]].carea<r[rr[j]].carea;j++)  // sorting by area or perimeter from big to small
				;
		for(int k=i;k>j;k--)
			rr[k]=rr[k-1];
		rr[j]=i;
	}
	for(i=0;i<typen-1;i++)
	{
//		for(j=i+1;j<typen&&r[rr[j]].carea==r[rr[i]].carea;j++)
//	for(j=i+1;j<typen&&r[rr[j]].width==r[rr[i]].width;j++)
	for(j=i+1;j<typen&&r[rr[j]].length==r[rr[i]].length;j++)
		{
//	 if(r[rr[j]].area>r[rr[i]].area)
//if(r[rr[j]].length>r[rr[i]].length)
if(r[rr[j]].width>r[rr[i]].width)
			{
				int temp=rr[i];
				rr[i]=rr[j];
				rr[j]=temp;
			}
		}
	}
/*	
	for(i=0;i<typen;i++)
	{
				printf("rri=%d l=%d w=%d tn=%d\n",rr[i],r[rr[i]].length,r[rr[i]].width,r[rr[i]].ntype);
	}
	getch();
*/	
}

void sort1(int rr[])
{
	int i,j;
	for(i=0;i<typen;i++)
		rr[i]=i;
	for(i=1;i<typen;i++)
	{
//	      		for(j=0;j<i&&r[rr[i]].length<r[rr[j]].length;j++)  // sorting by area or perimeter from big to small
//	for(j=0;j<i&&r[rr[i]].width<r[rr[j]].width;j++)  // sorting by area or perimeter from big to small
		for(j=0;j<i&&r[rr[i]].carea<r[rr[j]].carea;j++)  // sorting by area or perimeter from big to small
				;
		for(int k=i;k>j;k--)
			rr[k]=rr[k-1];
		rr[j]=i;
	}
	for(i=0;i<typen-1;i++)
	{
		for(j=i+1;j<typen&&r[rr[j]].carea==r[rr[i]].carea;j++)
//	for(j=i+1;j<typen&&r[rr[j]].width==r[rr[i]].width;j++)
//	for(j=i+1;j<typen&&r[rr[j]].length==r[rr[i]].length;j++)
		{
//	 if(r[rr[j]].area>r[rr[i]].area)
//if(r[rr[j]].length>r[rr[i]].length)
if(r[rr[j]].width>r[rr[i]].width)
			{
				int temp=rr[i];
				rr[i]=rr[j];
				rr[j]=temp;
			}
		}
	}
/*
	for(i=0;i<typen;i++)
	{
				printf("rri=%d l=%d w=%d tn=%d\n",rr[i],r[rr[i]].length,r[rr[i]].width,r[rr[i]].ntype);
	}
	getch();
*/	
}
void sort2(int rr[])
{
	int i,j;
	for(i=0;i<typen;i++)
		rr[i]=i;
	for(i=1;i<typen;i++)
	{
//	      		for(j=0;j<i&&r[rr[i]].length<r[rr[j]].length;j++)  // sorting by area or perimeter from big to small
	for(j=0;j<i&&r[rr[i]].width<r[rr[j]].width;j++)  // sorting by area or perimeter from big to small
//		for(j=0;j<i&&r[rr[i]].area<r[rr[j]].area;j++)  // sorting by area or perimeter from big to small
				;
		for(int k=i;k>j;k--)
			rr[k]=rr[k-1];
		rr[j]=i;
	}
	for(i=0;i<typen-1;i++)
	{
//		for(j=i+1;j<typen&&r[rr[j]].area==r[rr[i]].area;j++)
	for(j=i+1;j<typen&&r[rr[j]].width==r[rr[i]].width;j++)
//	for(j=i+1;j<typen&&r[rr[j]].length==r[rr[i]].length;j++)
		{
//	 if(r[rr[j]].area>r[rr[i]].area)
if(r[rr[j]].length>r[rr[i]].length)
//if(r[rr[j]].width>r[rr[i]].width)
			{
				int temp=rr[i];
				rr[i]=rr[j];
				rr[j]=temp;
			}
		}
	}
/*	
	for(i=0;i<typen;i++)
	{
				printf("rri=%d l=%d w=%d tn=%d\n",rr[i],r[rr[i]].length,r[rr[i]].width,r[rr[i]].ntype);
	}
	getch();
*/	
}

void sort3(int rr[])
{
	int i,j;
	for(i=0;i<typen;i++)
		rr[i]=i;
	for(i=1;i<typen;i++)
	{
//	      		for(j=0;j<i&&r[rr[i]].length<r[rr[j]].length;j++)  // sorting by area or perimeter from big to small
//	for(j=0;j<i&&r[rr[i]].width<r[rr[j]].width;j++)  // sorting by area or perimeter from big to small
		for(j=0;j<i&&r[rr[i]].area<r[rr[j]].area;j++)  // sorting by area or perimeter from big to small
				;
		for(int k=i;k>j;k--)
			rr[k]=rr[k-1];
		rr[j]=i;
	}
	for(i=0;i<typen-1;i++)
	{
		for(j=i+1;j<typen&&r[rr[j]].area==r[rr[i]].area;j++)
//	for(j=i+1;j<typen&&r[rr[j]].width==r[rr[i]].width;j++)
//	for(j=i+1;j<typen&&r[rr[j]].length==r[rr[i]].length;j++)
		{
//	 if(r[rr[j]].area>r[rr[i]].area)
//if(r[rr[j]].length>r[rr[i]].length)
if(r[rr[j]].width>r[rr[i]].width)
			{
				int temp=rr[i];
				rr[i]=rr[j];
				rr[j]=temp;
			}
		}
	}
/*	
	for(i=0;i<typen;i++)
	{
				printf("rri=%d l=%d w=%d tn=%d\n",rr[i],r[rr[i]].length,r[rr[i]].width,r[rr[i]].ntype);
	}
	getch();
*/	
}


int calgnval(int obj,int h1,int h2, int w)
{
    int sign;	
	if(h1>=h2)
	{
		sign=1;	
		if(w==r[obj].width&&(h1<r[obj].length))//||hy==r[obj].length))
		{
			waste=1;
		}
        else if((w==r[obj].width)&&(h1==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=0;
		}      
		else if((w==r[obj].width&&h1>r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=2;
 		}
	 /*   else if((w==r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=2;
 		}
	  */
		else if((w>r[obj].width)&&(h1==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=3;
 		}
 		else if(w>r[obj].width)//&&(h1>r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=4;
 		}
		else
		{
			waste=20000;
		}
	}
	else
	{
		sign=0;
		if(w==r[obj].width&&(h2<r[obj].length))//||hy==r[obj].length))
		{
			waste=1;
		}
        else if((w==r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=0;
		}      
		else if((w==r[obj].width&&h2>r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=2;
 		}
 	  /*  else if((w==r[obj].width&&h1==r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=2;
 		}
	   */
		else if((w>r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=3;
 		}
		else if(w>r[obj].width)//&&(h2>r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=4;
 		}
		else
		{
			waste=20000;
		}
	}	
	return sign;
}


int packing1(int rr[])
{
	int k,i,rr1[Max],sign1,ntypr[Max];
	int cx,tw=0,rorder,minwaste,minweight,minlength;
  	h=L;
	int nk=0;
//	int wastearea=0;
	int pin=0;
	g=0;
	minweight=minweigh;
	minlength=minlong;
    for(i=g;i<typen;i++)
	{
		rr1[i]=rr[i];
		ntypr[rr1[i]]=r[rr[i]].ntype;
	}
	int minh;
//	int h1,h2;//maxh;
	int nW=1;
	R[0].x=0;
	R[0].y=0;
	R[0].w=W;
	R[0].h1=2*L;
	R[0].h2=2*L;
    while(pin<n)
	{											    	   
			    cx=0;
                minh=R[0].y;
                for(k=1;k<nW;k++)     //cx,cy
				{
					if(R[k].y<minh)
					{
						cx=k;
						minh=R[k].y;
					}
				}
				if(R[cx].w>=minweight)
				{
				    minwaste=20000;
					int sign;
					for(i=g;i<typen;i++)
					{
								sign=calgnval(rr1[i],R[cx].h1,R[cx].h2,R[cx].w);
								if(waste==0)
								{
									minwaste=waste;
									rorder=i;
									sign1=sign;
									break;
								}
								else
								{
									if(waste<minwaste)
									{
										minwaste=waste;
									    rorder=i;
										sign1=sign;
									}
								}
					}
int rorder1=rr1[rorder];
					int maxww=r[rorder1].width;
					int maxll=r[rorder1].carea;
					if(sign1==1)
					{
					if(minwaste==3)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
								
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].length==R[cx].h1)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						}
					}
				
					else if(minwaste==4)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
							    
								for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].carea>=maxll)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						
						}
					}
					
					}
					else
					{
					if(minwaste==3)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
								
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].length==R[cx].h2)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						}
					}
				
					else if(minwaste==4)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].carea>=maxll)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						
						}
					}

					}

					rorder1=rr1[rorder];
				    if(minwaste<20000)
					{
						ntypr[rorder1]=ntypr[rorder1]-1;
					//	rorder1=rr1[rorder];
						pin++;
						if(ntypr[rorder1]==0)
						{
						//	int temi;
						//	temi=rr1[rorder];
							for(k=rorder;k>g;k--)
							{
								rr1[k]=rr1[k-1];
							}
							rr1[g]=rorder1;
							g=g+1;
						    if(g<typen)
							{
								if(r[rorder1].width==minweight)
								{
									minweight=r[rr1[g]].width;
									for(i=g+1;i<typen;i++)
										if(r[rr1[i]].width<minweight)
											minweight=r[rr1[i]].width;
								}
								if(r[rorder1].length==minlength)
								{
									minlength=r[rr1[g]].length;
									for(i=g+1;i<typen;i++)
										if(r[rr1[i]].length<minlength)
											minlength=r[rr1[i]].length;
								}
							}
						}
						if(sign1==1)
						{	   
							if(R[cx].y+r[rorder1].length>h)
									h=R[cx].y+r[rorder1].length;
	                        if(h>besth) return h;
							switch (minwaste)
							{
								case 1:
								{
                                    int th=R[cx].h2;
									int my=R[cx].y;
									k=cx-1;
									while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
									{
										if(R[k].y>=my)
										{
											R[k].w=R[cx].x-R[k].x;
											R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
											my=R[k].y;
										}
										k--;
									}
	                                R[cx].h1=0;
									R[cx].y=R[cx].y+r[rorder1].length;
									if(cx+1==nW)
									{
										R[cx].h2=th-r[rorder1].length;
									}
									else
									{
										k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
											k++;
								
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
									
									    R[cx+1].h1=r[rorder1].length-th;
									}
									break;
								}
								case 0:
								{
										if(R[cx].h1==R[cx].h2)
										{
											if(cx+1==nW)
											{
												nW--;	
											}
											else
											{
											
												for(k=cx;k<nW-2;k++)
												{
													R[k]=R[k+2];
												}
												nW=nW-2;
											}
										}
										else
										{
											if(cx+1==nW)
											{
												nW--;
											}
											else
											{
												R[cx+1].h1=r[rorder1].length-R[cx].h2;
												for(k=cx;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
											}
										}
									    break;
								}
								case 2:
								{		
									int th=R[cx].h2;
									int ty=R[cx].y;
									if(R[cx].h2<r[rorder1].length)
									{	
										R[cx].h1=R[cx].h1-r[rorder1].length;
										R[cx].y=R[cx].y+r[rorder1].length;
										
										k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
											k++;
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
										
                                        R[cx+1].h1=r[rorder1].length-th;
									}
									else
									{
										if(R[cx].h2==r[rorder1].length)
										{
											if(cx+1==nW)
											{
												R[cx].h1=R[cx].h1-r[rorder1].length;
												R[cx].y=R[cx].y+r[rorder1].length;	
												R[cx].h2=R[cx].h2-r[rorder1].length;
											}
											else
											{
												R[cx].w=R[cx+1].w+R[cx].w;
                                                R[cx].y=R[cx].y+r[rorder1].length;
												R[cx].h1=R[cx].h1-r[rorder1].length;
												R[cx].h2=R[cx+1].h2;
												for(k=cx+1;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
											}
										}
										else
										{
											R[cx].h1=R[cx].h1-r[rorder1].length;
											R[cx].y=R[cx].y+r[rorder1].length;	
											R[cx].h2=th-r[rorder1].length;
										}
									}
									break;
								}
								case 3:
								{		
									R[cx].x=R[cx].x+r[rorder1].width;  
									R[cx].w=R[cx].w-r[rorder1].width;
									break;
								}
								case 4:
								{
									if(R[cx].h1<r[rorder1].length)
									{
										int ty=R[cx].y;
										int th=R[cx].h2;
										int tw=R[cx].w;
										int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
										R[cx].y=R[cx].y+r[rorder1].length;  
	                                    R[cx].h1=0;
										if(cx+1==nW)
										{
											R[cx].h2=th-r[rorder1].length;
											nW++;
										}
										else
										{
											k=cx+1;
											while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
												k++;
									
											R[cx].w=R[k].x+R[k].w-R[cx].x;
											R[cx].h2=R[k].y+R[k].h2-R[cx].y;
										
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
										}
	                                    R[cx+1].h1=r[rorder1].length;
										R[cx+1].h2=th;
										R[cx+1].x=R[cx].x+r[rorder1].width;  
										R[cx+1].y=ty;
										R[cx+1].w=tw-r[rorder1].width;
									}
									else
									{
										int tw=R[cx].w;
										int th=R[cx].h2;
										int ty=R[cx].y;
										if(R[cx].h2<=r[rorder1].length)
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
											if(cx+1==nW)
											{
												R[cx].h2=th-r[rorder1].length;
												nW++;
											}
											else
											{
												k=cx+1;
												while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
													k++;
											
												R[cx].w=R[k].x+R[k].w-R[cx].x;
												R[cx].h2=R[k].h2+R[k].y-R[cx].y;
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
											}
										    R[cx+1].x=R[cx].x+r[rorder1].width;
										    R[cx+1].y=ty;
											R[cx+1].w=tw-r[rorder1].width;
											R[cx+1].h1=r[rorder1].length;
											R[cx+1].h2=th;		
										}
										else
										{				
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
										    R[cx+1].x=R[cx].x+r[rorder1].width;
										    R[cx+1].y=ty;
											R[cx+1].w=tw-r[rorder1].width;
											R[cx+1].h1=r[rorder1].length;
											R[cx+1].h2=th;
										}
									}
									break;
								}
							}
						}
						else
						{		
							if(R[cx].y+r[rorder1].length>h)
									h=R[cx].y+r[rorder1].length;
	                        if(h>besth) return h;
							switch (minwaste)
							{
								case 1:
								{
									int th=R[cx].h2;
									int my=R[cx].y;
									k=cx-1;
									while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
									{
										if(R[k].y>=my)
										{
											R[k].w=R[cx].x-R[k].x;
											R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
											my=R[k].y;
										}
										k--;
									}
	                                R[cx].y=R[cx].y+r[rorder1].length;
                                    R[cx].h1=0;	
									if(cx+1==nW)
									{
										R[cx].h2=th-r[rorder1].length;
									}
									else
									{	
									    k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
												k++;
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
								
								        R[cx+1].h1=R[cx].y-R[cx+1].y;
									}
									break;
								}
								case 0:
								{
										int th=R[cx].h2;
									    int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}	
										if(cx+1==nW)
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=0;	
											R[cx].h2=th-r[rorder1].length;
										}
										else
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=0;	
											R[cx].h2=R[cx+1].h2;
											R[cx].w=R[cx+1].w+R[cx].w;
											for(k=cx+1;k<nW-1;k++)
											{
												R[k]=R[k+1];
											}
											nW--;
										}
										break;
								}
								case 2:
								{
									if(R[cx].h1<r[rorder1].length)
									{
											int my=R[cx].y;
											k=cx-1;
											while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
											{
												if(R[k].y>=my)
												{
													R[k].w=R[cx].x-R[k].x;
													R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
													my=R[k].y;
												}
												k--;
											}
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=0;			
									}
									else
									{
										if(R[cx].h1==r[rorder1].length)
										{
												for(k=cx;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
										}
										else
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
										}
									}
									break;
								}
								case 3:
								{		
										R[cx].w=R[cx].w-r[rorder1].width;
	                                    int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x+R[cx].w-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
									
										R[cx+1].x=R[cx].x+R[cx].w;
										R[cx+1].w=R[cx+1].w+r[rorder1].width;
										R[cx+1].h1=0;
										break;
								}
								case 4:
								{
									if(R[cx].h2<r[rorder1].length)
									{
										int th=R[cx].h2;
										int tw=R[cx].w;
										R[cx].w=R[cx].w-r[rorder1].width;
										R[cx].h2=r[rorder1].length;
                                        int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x+R[cx].w-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
											R[cx+1].x=R[cx].x+R[cx].w;
											R[cx+1].y=R[cx].y+r[rorder1].length;
											R[cx+1].h1=0;
											k=cx+2;
										    while(R[k].y+R[k].h2<=R[cx+1].y&&k<nW)
												k++;
										 	R[cx+1].w=R[k].x+R[k].w-R[cx+1].x;
											R[cx+1].h2=R[k].y+R[k].h2-R[cx+1].y;
											R[cx+2].h1=R[cx+1].y-R[cx+2].y;
									}
									else
									{
										int th=R[cx].h2;
										if(R[cx].h1<=r[rorder1].length)
										{
										    
											R[cx].w=R[cx].w-r[rorder1].width;
											R[cx].h2=r[rorder1].length;
											int my=R[cx].y;
											k=cx-1;
											while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
											{
												if(R[k].y>=my)
												{
													R[k].w=R[cx].x+R[cx].w-R[k].x;
													R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
													my=R[k].y;
												}
												k--;
											}	
											if(cx+1==nW)
											{
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].w=r[rorder1].width;
												R[cx+1].h2=th-r[rorder1].length;
											}
											else
											{
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
												R[cx+1].w=r[rorder1].width;
											}
										}
										else
										{
											int th=R[cx].h2;
											R[cx].w=R[cx].w-r[rorder1].width;
											R[cx].h2=r[rorder1].length;
											if(cx+1==nW)
											{
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
												R[cx+1].w=r[rorder1].width;
											}
											else
											{
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
											    R[cx+1].w=r[rorder1].width;
											}
										}
									}
									break;
								}
							}
						}
					}
				}	
				else
				{
					if(cx==0)
					{
                        R[cx].y=R[cx+1].y;
	                    R[cx].h1=R[cx].h1-R[cx].h2; 
						R[cx].w=R[cx+1].w+R[cx].w;
						R[cx].h2=R[cx+1].h2;
						for(k=cx+1;k<nW-1;k++)
						{
							R[k]=R[k+1];
						}
						nW--;	
					}
					else 
					{
						if(cx+1==nW)
						{
						    nW--;
						}
						else 
						{
							if(R[cx].h1==R[cx].h2)
							{
								if(cx+1==nW)
								{
									nW--;
								}
								else
								{
									for(k=cx;k<nW-2;k++)
									{
										R[k]=R[k+2];
									}
									nW=nW-2;
								}
							}
							else
							{
								if(R[cx].h1<R[cx].h2)
								{
								    for(k=cx;k<nW-1;k++)
									{
										R[k]=R[k+1];
									}
									nW--;
								}
								else
								{
								
									R[cx].y=R[cx+1].y;
								    R[cx].h1=R[cx].h1-R[cx].h2; 

									R[cx].w=R[cx].w+R[cx+1].w;
									R[cx].h2=R[cx+1].h2;

									for(k=cx+1;k<nW-1;k++)
									{
										R[k]=R[k+1];
									}
									nW--;
								}
							}
						}
					}
					
				}
	}
//	printf("\n n=%d h=%d\n",n,h);
	return h;
}

int calgnval1(int obj,int h1,int h2, int w)
{
    int sign;	
	if(h1>=h2)
	{
		sign=1;	
		if(w==r[obj].width&&(h1<r[obj].length))//||hy==r[obj].length))
		{
			waste=2;
		}
        else if((w==r[obj].width)&&(h1==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=0;
		}      
		else if((w==r[obj].width&&h1>r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=1;
 		}
	 /*   else if((w==r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=2;
 		}
	  */
		else if((w>r[obj].width)&&(h1==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=3;
 		}
 		else if(w>r[obj].width)//&&(h1>r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=4;
 		}
		else
		{
			waste=20000;
		}
	}
	else
	{
		sign=0;
		if(w==r[obj].width&&(h2<r[obj].length))//||hy==r[obj].length))
		{
			waste=2;
		}
        else if((w==r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=0;
		}      
		else if((w==r[obj].width&&h2>r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=1;
 		}
 	  /*  else if((w==r[obj].width&&h1==r[obj].length))//(w==r[obj].width&&(h-R[posx].y)>=r[obj].length)
		{
			waste=2;
 		}
	   */
		else if((w>r[obj].width)&&(h2==r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=3;
 		}
		else if(w>r[obj].width)//&&(h2>r[obj].length))//((h-R[posx].y)>=r[obj].length))
		{
			waste=4;
 		}
		else
		{
			waste=20000;
		}
	}	
	return sign;
}


int packing11(int rr[])
{
	int k,i,rr1[Max],sign1,ntypr[Max];
	int cx,tw=0,rorder,minwaste,minweight,minlength;
  	h=L;
	int nk=0;
//	int wastearea=0;
	int pin=0;
	g=0;
	minweight=minweigh;
	minlength=minlong;
    for(i=g;i<typen;i++)
	{
		rr1[i]=rr[i];
		ntypr[rr1[i]]=r[rr[i]].ntype;
	}
	int minh;
//	int h1,h2;//maxh;
	int nW=1;
	R[0].x=0;
	R[0].y=0;
	R[0].w=W;
	R[0].h1=2*L;
	R[0].h2=2*L;
    while(pin<n)
	{											    	   
			    cx=0;
                minh=R[0].y;
                for(k=1;k<nW;k++)     //cx,cy
				{
					if(R[k].y<minh)
					{
						cx=k;
						minh=R[k].y;
					}
				}
				if(R[cx].w>=minweight)
				{
				    minwaste=20000;
					int sign;
					for(i=g;i<typen;i++)
					{
								sign=calgnval1(rr1[i],R[cx].h1,R[cx].h2,R[cx].w);
								if(waste==0)
								{
									minwaste=waste;
									rorder=i;
									sign1=sign;
									break;
								}
								else
								{
									if(waste<minwaste)
									{
										minwaste=waste;
									    rorder=i;
										sign1=sign;
									}
								}
					}
int rorder1=rr1[rorder];
					int maxww=r[rorder1].width;
					int maxll=r[rorder1].carea;
					if(sign1==1)
					{
					if(minwaste==3)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
								
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].length==R[cx].h1)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						}
					}
				
					else if(minwaste==4)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
							    
								for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].carea>=maxll)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						
						}
					}
					
					}
					else
					{
					if(minwaste==3)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
								
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].length==R[cx].h2)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						}
					}
				
					else if(minwaste==4)
					{
						if(R[cx].w-r[rorder1].width<minweight)
						{
							    for(i=g;i<typen;i++)
								{
									if(r[rr1[i]].width<R[cx].w)
									{
									
									if(r[rr1[i]].width>maxww&&r[rr1[i]].carea>=maxll)
									{
										rorder=i;
										maxww=r[rr1[i]].width;
									}
									}
								}
						
						}
					}

					}

					rorder1=rr1[rorder];
				    if(minwaste<20000)
					{
						ntypr[rorder1]=ntypr[rorder1]-1;
					//	rorder1=rr1[rorder];
						pin++;
						if(ntypr[rorder1]==0)
						{
						//	int temi;
						//	temi=rr1[rorder];
							for(k=rorder;k>g;k--)
							{
								rr1[k]=rr1[k-1];
							}
							rr1[g]=rorder1;
							g=g+1;
						    if(g<typen)
							{
								if(r[rorder1].width==minweight)
								{
									minweight=r[rr1[g]].width;
									for(i=g+1;i<typen;i++)
										if(r[rr1[i]].width<minweight)
											minweight=r[rr1[i]].width;
								}
								if(r[rorder1].length==minlength)
								{
									minlength=r[rr1[g]].length;
									for(i=g+1;i<typen;i++)
										if(r[rr1[i]].length<minlength)
											minlength=r[rr1[i]].length;
								}
							}
						}
						if(sign1==1)
						{	   
							if(R[cx].y+r[rorder1].length>h)
									h=R[cx].y+r[rorder1].length;
	                        if(h>besth) return h;
							switch (minwaste)
							{
							
								case 0:
								{
										if(R[cx].h1==R[cx].h2)
										{
											if(cx+1==nW)
											{
												nW--;	
											}
											else
											{
											
												for(k=cx;k<nW-2;k++)
												{
													R[k]=R[k+2];
												}
												nW=nW-2;
											}
										}
										else
										{
											if(cx+1==nW)
											{
												nW--;
											}
											else
											{
												R[cx+1].h1=r[rorder1].length-R[cx].h2;
												for(k=cx;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
											}
										}
									    break;
								}
								case 1:
								{		
									int th=R[cx].h2;
									int ty=R[cx].y;
									if(R[cx].h2<r[rorder1].length)
									{	
										R[cx].h1=R[cx].h1-r[rorder1].length;
										R[cx].y=R[cx].y+r[rorder1].length;
										
										k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
											k++;
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
										
                                        R[cx+1].h1=r[rorder1].length-th;
									}
									else
									{
										if(R[cx].h2==r[rorder1].length)
										{
											if(cx+1==nW)
											{
												R[cx].h1=R[cx].h1-r[rorder1].length;
												R[cx].y=R[cx].y+r[rorder1].length;	
												R[cx].h2=R[cx].h2-r[rorder1].length;
											}
											else
											{
												R[cx].w=R[cx+1].w+R[cx].w;
                                                R[cx].y=R[cx].y+r[rorder1].length;
												R[cx].h1=R[cx].h1-r[rorder1].length;
												R[cx].h2=R[cx+1].h2;
												for(k=cx+1;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
											}
										}
										else
										{
											R[cx].h1=R[cx].h1-r[rorder1].length;
											R[cx].y=R[cx].y+r[rorder1].length;	
											R[cx].h2=th-r[rorder1].length;
										}
									}
									break;
								}
	case 2:
								{
                                    int th=R[cx].h2;
									int my=R[cx].y;
									k=cx-1;
									while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
									{
										if(R[k].y>=my)
										{
											R[k].w=R[cx].x-R[k].x;
											R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
											my=R[k].y;
										}
										k--;
									}
	                                R[cx].h1=0;
									R[cx].y=R[cx].y+r[rorder1].length;
									if(cx+1==nW)
									{
										R[cx].h2=th-r[rorder1].length;
									}
									else
									{
										k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
											k++;
								
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
									
									    R[cx+1].h1=r[rorder1].length-th;
									}
									break;
								}
								case 3:
								{		
									R[cx].x=R[cx].x+r[rorder1].width;  
									R[cx].w=R[cx].w-r[rorder1].width;
									break;
								}
								case 4:
								{
									if(R[cx].h1<r[rorder1].length)
									{
										int ty=R[cx].y;
										int th=R[cx].h2;
										int tw=R[cx].w;
										int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
										R[cx].y=R[cx].y+r[rorder1].length;  
	                                    R[cx].h1=0;
										if(cx+1==nW)
										{
											R[cx].h2=th-r[rorder1].length;
											nW++;
										}
										else
										{
											k=cx+1;
											while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
												k++;
									
											R[cx].w=R[k].x+R[k].w-R[cx].x;
											R[cx].h2=R[k].y+R[k].h2-R[cx].y;
										
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
										}
	                                    R[cx+1].h1=r[rorder1].length;
										R[cx+1].h2=th;
										R[cx+1].x=R[cx].x+r[rorder1].width;  
										R[cx+1].y=ty;
										R[cx+1].w=tw-r[rorder1].width;
									}
									else
									{
										int tw=R[cx].w;
										int th=R[cx].h2;
										int ty=R[cx].y;
										if(R[cx].h2<=r[rorder1].length)
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
											if(cx+1==nW)
											{
												R[cx].h2=th-r[rorder1].length;
												nW++;
											}
											else
											{
												k=cx+1;
												while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
													k++;
											
												R[cx].w=R[k].x+R[k].w-R[cx].x;
												R[cx].h2=R[k].h2+R[k].y-R[cx].y;
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
											}
										    R[cx+1].x=R[cx].x+r[rorder1].width;
										    R[cx+1].y=ty;
											R[cx+1].w=tw-r[rorder1].width;
											R[cx+1].h1=r[rorder1].length;
											R[cx+1].h2=th;		
										}
										else
										{				
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
										    R[cx+1].x=R[cx].x+r[rorder1].width;
										    R[cx+1].y=ty;
											R[cx+1].w=tw-r[rorder1].width;
											R[cx+1].h1=r[rorder1].length;
											R[cx+1].h2=th;
										}
									}
									break;
								}
							}
						}
						else
						{		
							if(R[cx].y+r[rorder1].length>h)
									h=R[cx].y+r[rorder1].length;
	                        if(h>besth) return h;
							switch (minwaste)
							{
							
								case 0:
								{
										int th=R[cx].h2;
									    int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}	
										if(cx+1==nW)
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=0;	
											R[cx].h2=th-r[rorder1].length;
										}
										else
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h1=0;	
											R[cx].h2=R[cx+1].h2;
											R[cx].w=R[cx+1].w+R[cx].w;
											for(k=cx+1;k<nW-1;k++)
											{
												R[k]=R[k+1];
											}
											nW--;
										}
										break;
								}
								case 1:
								{
									if(R[cx].h1<r[rorder1].length)
									{
											int my=R[cx].y;
											k=cx-1;
											while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
											{
												if(R[k].y>=my)
												{
													R[k].w=R[cx].x-R[k].x;
													R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
													my=R[k].y;
												}
												k--;
											}
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=0;			
									}
									else
									{
										if(R[cx].h1==r[rorder1].length)
										{
												for(k=cx;k<nW-1;k++)
												{
													R[k]=R[k+1];
												}
												nW--;
										}
										else
										{
											R[cx].y=R[cx].y+r[rorder1].length;
											R[cx].h2=R[cx].h2-r[rorder1].length;
											R[cx].h1=R[cx].h1-r[rorder1].length;
										}
									}
									break;
								}
	case 2:
								{
									int th=R[cx].h2;
									int my=R[cx].y;
									k=cx-1;
									while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
									{
										if(R[k].y>=my)
										{
											R[k].w=R[cx].x-R[k].x;
											R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
											my=R[k].y;
										}
										k--;
									}
	                                R[cx].y=R[cx].y+r[rorder1].length;
                                    R[cx].h1=0;	
									if(cx+1==nW)
									{
										R[cx].h2=th-r[rorder1].length;
									}
									else
									{	
									    k=cx+1;
										while(R[k].y+R[k].h2<=R[cx].y&&k<nW)
												k++;
										R[cx].w=R[k].x+R[k].w-R[cx].x;
										R[cx].h2=R[k].y+R[k].h2-R[cx].y;
								
								        R[cx+1].h1=R[cx].y-R[cx+1].y;
									}
									break;
								}
								case 3:
								{		
										R[cx].w=R[cx].w-r[rorder1].width;
	                                    int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x+R[cx].w-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
									
										R[cx+1].x=R[cx].x+R[cx].w;
										R[cx+1].w=R[cx+1].w+r[rorder1].width;
										R[cx+1].h1=0;
										break;
								}
								case 4:
								{
									if(R[cx].h2<r[rorder1].length)
									{
										int th=R[cx].h2;
										int tw=R[cx].w;
										R[cx].w=R[cx].w-r[rorder1].width;
										R[cx].h2=r[rorder1].length;
                                        int my=R[cx].y;
										k=cx-1;
										while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
										{
											if(R[k].y>=my)
											{
												R[k].w=R[cx].x+R[cx].w-R[k].x;
												R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
												my=R[k].y;
											}
											k--;
										}
											for(k=nW;k>cx+1;k--)
											{
												R[k]=R[k-1];
											}
											nW++;
											R[cx+1].x=R[cx].x+R[cx].w;
											R[cx+1].y=R[cx].y+r[rorder1].length;
											R[cx+1].h1=0;
											k=cx+2;
										    while(R[k].y+R[k].h2<=R[cx+1].y&&k<nW)
												k++;
										 	R[cx+1].w=R[k].x+R[k].w-R[cx+1].x;
											R[cx+1].h2=R[k].y+R[k].h2-R[cx+1].y;
											R[cx+2].h1=R[cx+1].y-R[cx+2].y;
									}
									else
									{
										int th=R[cx].h2;
										if(R[cx].h1<=r[rorder1].length)
										{
										    
											R[cx].w=R[cx].w-r[rorder1].width;
											R[cx].h2=r[rorder1].length;
											int my=R[cx].y;
											k=cx-1;
											while(k>=0&&R[cx].y+r[rorder1].length>R[k].y)
											{
												if(R[k].y>=my)
												{
													R[k].w=R[cx].x+R[cx].w-R[k].x;
													R[k].h2=R[cx].y+r[rorder1].length-R[k].y;
													my=R[k].y;
												}
												k--;
											}	
											if(cx+1==nW)
											{
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].w=r[rorder1].width;
												R[cx+1].h2=th-r[rorder1].length;
											}
											else
											{
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
												R[cx+1].w=r[rorder1].width;
											}
										}
										else
										{
											int th=R[cx].h2;
											R[cx].w=R[cx].w-r[rorder1].width;
											R[cx].h2=r[rorder1].length;
											if(cx+1==nW)
											{
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
												R[cx+1].w=r[rorder1].width;
											}
											else
											{
												for(k=nW;k>cx+1;k--)
												{
													R[k]=R[k-1];
												}
												nW++;
												R[cx+1].x=R[cx].x+R[cx].w;
												R[cx+1].y=R[cx].y+r[rorder1].length;
												R[cx+1].h1=0;
												R[cx+1].h2=th-r[rorder1].length;
											    R[cx+1].w=r[rorder1].width;
											}
										}
									}
									break;
								}
							}
						}
					}
				}	
				else
				{
					if(cx==0)
					{
                        R[cx].y=R[cx+1].y;
	                    R[cx].h1=R[cx].h1-R[cx].h2; 
						R[cx].w=R[cx+1].w+R[cx].w;
						R[cx].h2=R[cx+1].h2;
						for(k=cx+1;k<nW-1;k++)
						{
							R[k]=R[k+1];
						}
						nW--;	
					}
					else 
					{
						if(cx+1==nW)
						{
						    nW--;
						}
						else 
						{
							if(R[cx].h1==R[cx].h2)
							{
								if(cx+1==nW)
								{
									nW--;
								}
								else
								{
									for(k=cx;k<nW-2;k++)
									{
										R[k]=R[k+2];
									}
									nW=nW-2;
								}
							}
							else
							{
								if(R[cx].h1<R[cx].h2)
								{
								    for(k=cx;k<nW-1;k++)
									{
										R[k]=R[k+1];
									}
									nW--;
								}
								else
								{
								
									R[cx].y=R[cx+1].y;
								    R[cx].h1=R[cx].h1-R[cx].h2; 

									R[cx].w=R[cx].w+R[cx+1].w;
									R[cx].h2=R[cx+1].h2;

									for(k=cx+1;k<nW-1;k++)
									{
										R[k]=R[k+1];
									}
									nW--;
								}
							}
						}
					}
					
				}
	}
//	printf("\n n=%d h=%d\n",n,h);
	return h;
}



int findbestpacking(int rr[])
{
		srand(time(NULL));
	int i,k,rp,brp;
    int g1=0;
 	besth=4*L;
    int flag1,temp;
rp=rand()%2;
brp=rp;	
	if(rp==0)
	flag1=packing1(rr);
	else
flag1=packing11(rr);
//	flag1=packing1(rr);     
	if(flag1==L) return L;
//	printf("%d\n",flag1);
    besth=flag1;  
	k=g1;
    clock_t start, end;
	start=clock();
	while(k<typen-1)
	{
		   for(i=k+1;i<typen;i++)      //determine l in bestn
		   {
				  temp=rr[i];
				  rr[i]=rr[k];
				  rr[k]=temp;
rp=rand()%2;
brp=rp;	
	if(rp==0)
	flag1=packing1(rr);
	else
flag1=packing11(rr);
			//	  flag1=packing1(rr);
                  end = clock();
	              double runtime=(float)(end - start) / CLK_TCK;
			//	  printf("%d\n",flag1);
			//	  getch();
				//  if (flag1==L) return L;

                  if(flag1<besth)
				  {
					besth=flag1;
					if(besth==L) 	return besth;
				  }
				  else
				  {		
					temp=rr[i];
					rr[i]=rr[k];
					rr[k]=temp;	
				  }   
				  	  if(runtime>60.0)
					  return besth;
		   }
		   k++;		   
	}	
//	 printf("\n***besth=%d \n",besth);

   

 //   double tem=0.5;
//	double a=0.93;
	int k1,k2;
	int best=besth;
	//int rrr=0;
	while(1)
	{
		for(int i=0;i<typen;i++)
		{		
		    k1=rand()%(typen-1);
		//	k2=k1+rand()%((int)(typen/2-1));
			do
			{
				k2=rand()%typen;
			}while(k2==k1);
			
                  temp=rr[k1];
				  rr[k1]=rr[k2];
				  rr[k2]=temp;
				//  flag1=packing1(rr);
rp=rand()%2;
brp=rp;	
	if(rp==0)
	flag1=packing1(rr);
	else
flag1=packing11(rr);
                  end = clock();
	              double runtime=(float)(end - start) / CLK_TCK;

			//	  if (flag1==L) return L;

                 if(flag1<besth)
				  {
					besth=flag1;
				    if (flag1==L) return L;
				  }
				  else
				  {		
			/*		if(exp((float)(besth-flag1)/tem)<((float)(rand()%20)/20))
					{	
					  temp=rr[k1];
					  rr[k1]=rr[k2];
					  rr[k2]=temp;	
					}
					*/
					 int suml=besth+flag1;
					  double p1=(1.0*flag1/suml);
				//	  p1=1/(k+1);
					  double p2=(float)(rand()%20)/20;
					  if(p1>p2)
					  {
					  
					 temp=rr[k1];
					  rr[k1]=rr[k2];
					  rr[k2]=temp;
					  }
				 }

				  	  if(runtime>60.0)
					  return besth;
		}
	//	tem=a*tem;
		
        int rrr=rand()%3;
		if(rrr==0)
			sort3(rr);
		else if(rrr==1)
			sort2(rr);
		else if(rrr==2)
			sort1(rr);
		else
			sort(rr);
			
	}

    return besth;  
}

void computetype()
{
	int i,j;
    for(i=0;i<n;i++)
	{
		r1[i].flag=0;
	}
    int m=0;
	for(i=0;i<n;i++)
	{
		if(r1[i].flag==0)
		{
			r[m].length=r1[i].length;
			r[m].width=r1[i].width;
		    r[m].ntype=1;
			for(j=i+1;j<n;j++)
			{
				if(r1[j].flag==0)
				{
					if(r1[j].length==r[m].length&&r1[j].width==r[m].width)
					{
						r[m].ntype++;
						r1[j].flag=1;
					}
				}
			}
			m++;
		}
	}
	typen=m;
	FILE *fp1;
    fp1= fopen("www.txt", "w+");	
	for(i=0;i<typen;i++)
	{			
			r[i].area=r[i].length*r[i].width;
			r[i].carea=r[i].length+r[i].width;
			fprintf(fp1,"%d %d %d\n",r[i].length,r[i].width,r[i].ntype);			
	}
	fclose(fp1);  	

	minweigh=r[0].width;
	minlong=r[0].length;

	for(i=1;i<typen;i++)
	{
				if(r[i].width<minweigh)
					minweigh=r[i].width;
				if(r[i].length<minlong)
					minlong=r[i].length;
	}
	//		L=(int)(totalarea/W+1);    
}


void inputdata(int k,Rect1 *r)
{
	char *fl;
	FILE *fp;
    switch (k)
    {
	    case 4:
		{	L=1300;W=600;n=300;fl="mm4.txt"; break;}

	    case 29:
		{	L=20;W=20;n=16;fl="lw161.txt"; break;}
		case 30:
		{	L=20;W=20;n=17;fl="lw172.txt"; break;}
		case 31:
		{	L=20;W=20;n=16;fl="lw163.txt"; break;}
	     case 32:
		{	L=15;W=40;n=25;fl="lw251.txt"; break;}
		case 33:
		{	L=15;W=40;n=25;fl="lw252.txt"; break;}
		case 34:
		{	L=15;W=40;n=25;fl="lw253.txt"; break;}
		case 35:
		{	L=30;W=60;n=28;fl="lw281.txt"; break;}
		case 36:
		{	L=30;W=60;n=29;fl="lw292.txt"; break;}
		case 37:
		{	L=30;W=60;n=28;fl="lw283.txt"; break;}
		case 38:
		{	L=60;W=60;n=49;fl="lw491.txt"; break;}
		case 39:
		{	L=60;W=60;n=49;fl="lw492.txt"; break;}
		case 40:
		{	L=60;W=60;n=49;fl="lw493.txt"; break;}
	    case 41:
		{	L=90;W=60;n=73;fl="lw731.txt"; break;}
		case 42:
		{	L=90;W=60;n=73;fl="lw732.txt"; break;}
		case 43:
		{	L=90;W=60;n=73;fl="lw733.txt"; break;}
		case 44:
		{	L=120;W=80;n=97;fl="lw971.txt"; break;}
		case 45:
		{	L=120;W=80;n=97;fl="lw972.txt"; break;}
		case 46:
		{	L=120;W=80;n=97;fl="lw973.txt"; break;}
		case 47:
		{	L=240;W=160;n=196;fl="lw1961.txt"; break;}
		case 48:
		{	L=240;W=160;n=197;fl="lw1972.txt"; break;}
		case 49:
		{	L=240;W=160;n=196;fl="lw1963.txt"; break;}

  
		case 50:
		{	L=1000;W=1000;n=25;fl="nice1.txt"; break;}
		case 51:
		{	L=1000;W=1000;n=50;fl="nice2.txt"; break;}
		case 52:
		{	L=1000;W=1000;n=100;fl="nice3.txt"; break;}
	    case 53:
		{	L=1000;W=1000;n=200;fl="nice4.txt"; break;}
		case 54:
		{	L=1000;W=1000;n=500;fl="nice5.txt"; break;}
		case 55:
		{	L=1000;W=1000;n=1000;fl="nice6.txt"; break;}
		
		case 56:
		{	L=1000;W=1000;n=25;fl="path1.txt"; break;}
		case 57:
		{	L=1000;W=1000;n=50;fl="path2.txt"; break;}
		case 58:
		{	L=1000;W=1000;n=100;fl="path3.txt"; break;}
		case 59:
		{	L=1000;W=1000;n=200;fl="path4.txt"; break;}
		case 60:
		{	L=1000;W=1000;n=500;fl="path5.txt"; break;}
		case 61:
		{	L=1000;W=1000;n=1000;fl="path6.txt"; break;}
    
		case 62:
		{	L=375;W=1000;n=50;fl="babu.txt"; break;}

       
       
		case 63:
		{	L=40;W=40;n=10;fl="n1.txt"; break;}
		case 64:
		{	L=50;W=30;n=20;fl="n2.txt"; break;}
		case 65:
		{	L=50;W=30;n=30;fl="n3.txt"; break;}
	    case 66:
		{	L=80;W=80;n=40;fl="n4.txt"; break;}
		case 67:
		{	L=100;W=100;n=50;fl="n5.txt"; break;}
		case 68:
		{	L=100;W=50;n=60;fl="n6.txt"; break;}
		case 69:
		{	L=100;W=80;n=70;fl="n7.txt"; break;}
		case 70:
		{	L=80;W=100;n=80;fl="n8.txt"; break;}
		case 71:
		{	L=150;W=50;n=100;fl="n9.txt"; break;}
		case 72:
		{	L=150;W=70;n=200;fl="n10.txt"; break;}
		case 73:
		{	L=150;W=70;n=300;fl="n11.txt"; break;}
		case 74:
		{	L=300;W=100;n=500;fl="n122.txt"; break;}
    	case 75:
		{	L=960;W=640;n=3152;fl="n13.txt"; break;}
  	  
		case 76:
		{	L=600;W=400;n=50;fl="50.txt"; break;}
		case 77:
		{	L=600;W=400;n=100;fl="100.txt"; break;}
		case 78:
		{	L=600;W=400;n=500;fl="500.txt"; break;}
		case 79:
		{	L=600;W=400;n=1000;fl="1000.txt"; break;}
		case 80:
		{	L=600;W=400;n=5000;fl="5000.txt"; break;}
    	case 81:
		{	L=600;W=400;n=10000;fl="10000.txt"; break;}
        case 82:
		{	L=600;W=400;n=15000;fl="15000.txt"; break;}
 
		case 83:
		{	L=330;W=100;n=580;fl="zdf1.txt"; break;}
		case 84:
		{	L=357;W=100;n=660;fl="zdf2.txt"; break;}
		case 85:
		{	L=384;W=100;n=740;fl="zdf3.txt"; break;}
		case 86:
		{	L=407;W=100;n=820;fl="zdf4.txt"; break;}

	    case 87:
		{	L=434;W=100;n=900;fl="zdf5.txt"; break;}
		case 88:
		{	L=4872;W=3000;n=1532;fl="zdf6.txt"; break;}
		case 89:
		{	L=4852;W=3000;n=2432;fl="zdf7.txt"; break;}
		case 90:
		{	L=5172;W=3000;n=2532;fl="zdf8.txt"; break;}
		case 91:
		{	L=5172;W=3000;n=5032;fl="zdf9.txt"; break;}
		case 92:
		{	L=5172;W=6000;n=5064;fl="zdf10.txt"; break;}
        case 93:
		{	L=5172;W=6000;n=7564;fl="zdf11.txt"; break;}
        case 94:
		{	L=5172;W=6000;n=10064;fl="zdf12.txt"; break;}
		case 95:
		{	L=5172;W=9000;n=15096;fl="zdf13.txt"; break;}
		case 96:
		{	L=5172;W=3000;n=25032;fl="zdf14.txt"; break;}
        case 97:
		{	L=5172;W=3000;n=50032;fl="zdf15.txt"; break;}
    	case 98:
		{	L=5172;W=3000;n=75032;fl="zdf16.txt"; break;}

   
    	case 100:
		{	L=30;W=60;n=49;fl="beng01.txt"; break;}
		case 101:
		{	L=57;W=60;n=49;fl="beng02.txt"; break;}
		case 102:
		{	L=84;W=90;n=73;fl="beng03.txt"; break;}
		case 103:
		{	L=107;W=90;n=73;fl="beng04.txt"; break;}
		case 104:
		{	L=134;W=90;n=73;fl="beng05.txt"; break;}
		case 105:
		{	L=36;W=120;n=97;fl="beng06.txt"; break;}
		case 106:
		{	L=67;W=120;n=97;fl="beng07.txt"; break;}
		case 107:
		{	L=101;W=120;n=97;fl="beng08.txt"; break;}
		case 108:
		{	L=126;W=240;n=196;fl="beng09.txt"; break;}
		case 109:
		{	L=156;W=240;n=197;fl="beng10.txt"; break;}
        	
	
		case 110:
		{	L=23;W=240;n=196;fl="ngcut01.txt"; break;}
		case 111:
		{	L=30;W=60;n=49;fl="ngcut02.txt"; break;}
		case 112:
		{	L=28;W=90;n=73;fl="ngcut03.txt"; break;}
		case 113:
		{	L=20;W=90;n=73;fl="ngcut04.txt"; break;}
		case 114:
		{	L=36;W=90;n=73;fl="ngcut05.txt"; break;}
		case 115:
		{	L=29;W=120;n=97;fl="ngcut06.txt"; break;}
		case 116:
		{	L=20;W=120;n=97;fl="ngcut07.txt"; break;}
		case 117:
		{	L=32;W=120;n=97;fl="ngcut08.txt"; break;}
		case 118:
		{	L=49;W=240;n=196;fl="ngcut09.txt"; break;}
		case 119:
		{	L=80;W=240;n=197;fl="ngcut10.txt"; break;}
		case 120:
		{	L=50;W=240;n=196;fl="ngcut11.txt"; break;}
		case 121:
		{	L=87;W=240;n=196;fl="ngcut12.txt"; break;}
	    case 122:
		{	L=23;W=240;n=196;fl="cgcut01.txt"; break;}
		case 123:
		{	L=63;W=60;n=49;fl="cgcut02.txt"; break;}
		case 124:
		{	L=636;W=90;n=73;fl="cgcut03.txt"; break;}


		case 125:
		{	L=1016;W=240;n=196;fl="gcut01.txt"; break;}
		case 126:
		{	L=1133;W=60;n=49;fl="gcut02.txt"; break;}
		case 127:
		{	L=1803;W=90;n=73;fl="gcut03.txt"; break;}
		case 128:
		{	L=2934;W=90;n=73;fl="gcut04.txt"; break;}
		case 129:
		{	L=1172;W=90;n=73;fl="gcut05.txt"; break;}
		case 130:
		{	L=2514;W=120;n=97;fl="gcut06.txt"; break;}
		case 131:
		{	L=4641;W=120;n=97;fl="gcut07.txt"; break;}
		case 132:
		{	L=5703;W=120;n=97;fl="gcut08.txt"; break;}
		case 133:
		{	L=2022;W=240;n=196;fl="gcut09.txt"; break;}
		case 134:
		{	L=5356;W=240;n=197;fl="gcut10.txt"; break;}
		case 135:
		{	L=6537;W=120;n=97;fl="gcut11.txt"; break;}
		case 136:
		{	L=12522;W=240;n=196;fl="gcut12.txt"; break;}
		case 137:
		{	L=4772;W=240;n=197;fl="gcut13.txt"; break;}
	
    
	    case 150:
		{	L=1000;W=1000;n=1000;fl="nice11t.txt"; break;}
		case 151:
		{	L=1000;W=1000;n=1000;fl="nice21t.txt"; break;}
		case 152:
		{	L=1000;W=1000;n=1000;fl="nice31t.txt"; break;}
	    case 153:
		{	L=1000;W=1000;n=1000;fl="nice41t.txt"; break;}
		case 154:
		{	L=1000;W=1000;n=1000;fl="nice51t.txt"; break;}
		case 155:
		{	L=1000;W=1000;n=1000;fl="nice61t.txt"; break;}		
		case 156:
		{	L=1000;W=1000;n=1000;fl="nice71t.txt"; break;}
		case 157:
		{	L=1000;W=1000;n=1000;fl="nice81t.txt"; break;}
		case 158:
		{	L=1000;W=1000;n=1000;fl="nice91t.txt"; break;}
		case 159:
		{	L=1000;W=1000;n=1000;fl="nice101t.txt"; break;}
	  
		case 160:
		{	L=1000;W=1000;n=2000;fl="nice12t.txt"; break;}
		case 161:
		{	L=1000;W=1000;n=2000;fl="nice22t.txt"; break;}
		case 162:
		{	L=1000;W=1000;n=2000;fl="nice32t.txt"; break;}
	    case 163:
		{	L=1000;W=1000;n=2000;fl="nice42t.txt"; break;}
		case 164:
		{	L=1000;W=1000;n=2000;fl="nice52t.txt"; break;}
		case 165:
		{	L=1000;W=1000;n=2000;fl="nice62t.txt"; break;}		
		case 166:
		{	L=1000;W=1000;n=2000;fl="nice72t.txt"; break;}
		case 167:
		{	L=1000;W=1000;n=2000;fl="nice82t.txt"; break;}
		case 168:
		{	L=1000;W=1000;n=2000;fl="nice92t.txt"; break;}
		case 169:
		{	L=1000;W=1000;n=2000;fl="nice102t.txt"; break;}
	    
		case 170:
		{	L=1000;W=1000;n=5000;fl="nice15t.txt"; break;}
		case 171:
		{	L=1000;W=1000;n=5000;fl="nice25t.txt"; break;}
		case 172:
		{	L=1000;W=1000;n=5000;fl="nice35t.txt"; break;}
	    case 173:
		{	L=1000;W=1000;n=5000;fl="nice45t.txt"; break;}
		case 174:
		{	L=1000;W=1000;n=5000;fl="nice55t.txt"; break;}
		case 175:
		{	L=1000;W=1000;n=5000;fl="nice65t.txt"; break;}		
		case 176:
		{	L=1000;W=1000;n=5000;fl="nice75t.txt"; break;}
		case 177:
		{	L=1000;W=1000;n=5000;fl="nice85t.txt"; break;}
		case 178:
		{	L=1000;W=1000;n=5000;fl="nice95t.txt"; break;}
		case 179:
		{	L=1000;W=1000;n=5000;fl="nice105t.txt"; break;}

		case 180:
		{	L=1000;W=1000;n=1000;fl="path11t.txt"; break;}
		case 181:
		{	L=1000;W=1000;n=1000;fl="path21t.txt"; break;}
		case 182:
		{	L=1000;W=1000;n=1000;fl="path31t.txt"; break;}
	    case 183:
		{	L=1000;W=1000;n=1000;fl="path41t.txt"; break;}
		case 184:
		{	L=1000;W=1000;n=1000;fl="path51t.txt"; break;}
		case 185:
		{	L=1000;W=1000;n=1000;fl="path61t.txt"; break;}		
		case 186:
		{	L=1000;W=1000;n=1000;fl="path71t.txt"; break;}
		case 187:
		{	L=1000;W=1000;n=1000;fl="path81t.txt"; break;}
		case 188:
		{	L=1000;W=1000;n=1000;fl="path91t.txt"; break;}
		case 189:
		{	L=1000;W=1000;n=1000;fl="path101t.txt"; break;}
	  
		case 190:
		{	L=1000;W=1000;n=2000;fl="path12t.txt"; break;}
		case 191:
		{	L=1000;W=1000;n=2000;fl="path22t.txt"; break;}
		case 192:
		{	L=1000;W=1000;n=2000;fl="path32t.txt"; break;}
	    case 193:
		{	L=1000;W=1000;n=2000;fl="path42t.txt"; break;}
		case 194:
		{	L=1000;W=1000;n=2000;fl="path52t.txt"; break;}
		case 195:
		{	L=1000;W=1000;n=2000;fl="path62t.txt"; break;}		
		case 196:
		{	L=1000;W=1000;n=2000;fl="path72t.txt"; break;}
		case 197:
		{	L=1000;W=1000;n=2000;fl="path82t.txt"; break;}
		case 198:
		{	L=1000;W=1000;n=2000;fl="path92t.txt"; break;}
		case 199:
		{	L=1000;W=1000;n=2000;fl="path102t.txt"; break;}
	    
		case 200:
		{	L=1000;W=1000;n=5000;fl="path15t.txt"; break;}
		case 201:
		{	L=1000;W=1000;n=5000;fl="path25t.txt"; break;}
		case 202:
		{	L=1000;W=1000;n=5000;fl="path35t.txt"; break;}
	    case 203:
		{	L=1000;W=1000;n=5000;fl="path45t.txt"; break;}
		case 204:
		{	L=1000;W=1000;n=5000;fl="path55t.txt"; break;}
		case 205:
		{	L=1000;W=1000;n=5000;fl="path65t.txt"; break;}		
		case 206:
		{	L=1000;W=1000;n=5000;fl="path75t.txt"; break;}
		case 207:
		{	L=1000;W=1000;n=5000;fl="path85t.txt"; break;}
		case 208:
		{	L=1000;W=1000;n=5000;fl="path95t.txt"; break;}
		case 209:
		{	L=1000;W=1000;n=5000;fl="path105t.txt"; break;}
	
    }
    if((fp=fopen(fl,"r"))==NULL) 
	{
		printf("Cannot open input file:%s",fl);
		return;
	}
	else
	{
            //from 29 to 99:c,n,nice,path,zdf,cx
		    for(int i=0;i<n;i++)
			{
				fscanf(fp,"%d",&r1[i].width);
                fscanf(fp,"%d",&r1[i].length);
			}
            //non-zero-waste from 100 to 124:beng,ngcut,cgcut
   /* 	fscanf(fp,"%d",&n);
	        fscanf(fp,"%d",&W);
			for(int i=0;i<n;i++)
			{
				fscanf(fp,"%d",&r1[i].width);
				fscanf(fp,"%d",&r1[i].width);
				fscanf(fp,"%d",&r1[i].length);
			}
		
 //non-zero-waste from 130 to 143:gcut
	        int te;
      	    fscanf(fp,"%d",&n);
	        fscanf(fp,"%d",&te);
            fscanf(fp,"%d",&W);
			for(int i=0;i<n;i++)
			{
				fscanf(fp,"%d",&r1[i].length);
	            fscanf(fp,"%d",&r1[i].width);
	            fscanf(fp,"%d",&te);
			}
	
		int te;//from 150 to 210: nicet,patht
			fscanf(fp,"%d",&W);
            fscanf(fp,"%d",&L);	
			fscanf(fp,"%d",&te);
            fscanf(fp,"%d",&n);	//for data 150-210	  
		    for(int i=0;i<n;i++)
			{
				fscanf(fp,"%d",&r1[i].width);
                fscanf(fp,"%d",&r1[i].length);
			}
	*/		
			computetype();				
	}	
	fclose(fp);
}

void main()
{
	int rr[Max];
 
	FILE *fp1;
	fp1= fopen("cn694.xls", "w+");
	fprintf(fp1,"%s%c%s%c%s%c%s\n","L",'	',"W",'	',"n",'	',"pack");
	a=0.93;
	for(int ii=0;ii<1;ii++)
	{
	  //  a=a+0.01;
		int sumh=0;
	clock_t start, end;
	for(int jj=29;jj<99;jj++)
	{
		inputdata(jj,r1);
		sort(rr);
		start = clock();
		int th=findbestpacking(rr);
		besth=th;
        printf("besth=%d \n",th);
     	float k=float(besth-L)/float(L);
		end = clock();
	    float runtime=(float)(end - start) / CLK_TCK;
	    sumh=sumh+besth;
	    fprintf(fp1,"%d%c%d%c%d%c%d%c%d%c%.2f%c%.2f\n",L,'	',W,'	',n,'	',g,'	',besth,'	',k,'	',runtime);
	}
	printf("sumh=%d \n",sumh);
	}
	fclose(fp1);
}


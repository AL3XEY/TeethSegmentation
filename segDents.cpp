/* 	HMIN318
	2015-2016
	Gérard Subsol
	 g++  -o segDents segDents.cpp -O2 -lm  -lpthread -lX11


	
#include "CImg.h"
using namespace cimg_library;

/* Main program */
	
#include "CImg.h"
using namespace cimg_library;
int main(int argc,char **argv)
{
    if(argc<2)
    {printf("svp args : nom image \n");exit(1);}



	/* Create and load the 3D image */
	/* See http://cimg.eu/reference/group__cimg__storage.html */
	/* See "Construct image from reading an image file" at http://cimg.eu/reference/structcimg__library_1_1CImg.html */
	CImg<unsigned short> img;
	float voxelsize[3];
	/* Load in Analyze format and get the voxel size in an array */
	img.load_analyze(argv[1],voxelsize);
	
	//resampling
	img = img.resize(img.width(), img.height(), 230);
		
	/* Get the image dimensions */
    unsigned int dim[]={img.width(),img.height(),img.depth()}; 
	printf("Reading %s. Dimensions=%d %d %d\n",argv[1],dim[0],dim[1],dim[2]);
	printf("Voxel size=%f %f %f\n",voxelsize[0],voxelsize[1],voxelsize[2]);
		
	/* Create the display window */
	/* See http://cimg.eu/reference/structcimg__library_1_1CImgDisplay.html */
	/* Looak at normalization at http://cimg.eu/reference/group__cimg__displays.html */
   
	CImgDisplay disp(img.width(),img.height(),"");    
	
	/* The 3 displayed slices of the MPR visualisation */
	int displayedSlice[]={dim[2]}; 
		
	/* Slice corresponding to mouse position: */
	unsigned int coord[]={0,0,0};
	
	/* in MPR [2=original slice size=x y       0 size=z y
	1= size=x z                                */
	/* -1 corresponds to the 4th quarter where there is no MPR plane */
	int plane=2;
	
	/* For a first drawing, activate the redrawing */
	bool redraw=true;
	

	/* Manage the display windows: ESC, or closed -> close the main window*/
	while (!disp.is_closed() && !disp.is_keyESC()) // main loop
	{
		/* Wait for any event on the display windows during 1 ms.
		If nothing, at the beginning draw for the first time the window */
		disp.wait(1);
		
		/* List of events */
		
		/* - Resizing */
		if (disp.is_resized()) 
		{
			disp.resize();
		}
		/* Movement of the mouse */
		
		/* If the mouse is inside the window, find its position w.r.t. to the 3 planes */
			
		unsigned int mX = disp.mouse_x();
		unsigned int mY = disp.mouse_y();
			
		coord[0] = mX; 
		coord[1] = mY;   
		coord[2] = displayedSlice[2]; 
					
		redraw = true;
		
        /* - Click Right button */
		if (disp.button())  
		{
            const int y = disp.mouse_y();const int x = disp.mouse_x();
            const int z = displayedSlice[2];
            unsigned short *clic= img.data(x,y,z);
            printf("%d\n",*clic);
            // on a recupere le clic il faut faire le traitement
            CImg<unsigned short> nimg;
            unsigned char col[]={254};

                /* pour determiner teta end
                for(int i=1;i<40;i++){
                img.draw_fill(x,y,z,col,1,nimg,i);

                int cpt=0;
                cimg_for(nimg,val,unsigned short)
                {
                
                    if(*val!=0)
	                cpt++;
                }
                printf("%d %d\n",i,cpt);
                }*/
            //j'ai calculé 32 sur le graphe
            img.draw_fill(x,y,z,col,1,nimg,atoi(argv[2]));
            img=nimg;
int cpt=0;
 cimg_for(nimg,val,unsigned short)
                {
                
                    if(*val!=0)
	                cpt++;
                }
                printf("%d \n",cpt);
			redraw = true;
		}

		/* - Wheel interaction */
		if (disp.wheel()) 
		{
			displayedSlice[plane]=displayedSlice[plane]+disp.wheel();
			
			if (displayedSlice[plane]<0) 
			{
				displayedSlice[plane] = 0;
			}
			else 
			{
				if (displayedSlice[plane]>=(int)dim[plane])
				{
					displayedSlice[plane] = (int)dim[plane]-1;
				}
		}
			
		/* Flush all mouse wheel events in order to not repeat the wheel event */
		disp.set_wheel();
			
		redraw = true;
		}
		
		/* If any change, we need to redraw the main windows */
		if (redraw)
		{
            CImg<> mpr_img2=img.get_slice(displayedSlice[2]);
						
			/* The MPR image has a given size. It needs to be resized in order to fit at best in the display window */
			mpr_img2.resize(img.width(),img.height()); 
		
			//pixel values are between 0 and 4095
			
			//2D Mean filter
			int meanMaskDimension = 3;
			CImg<unsigned char> meanMask(meanMaskDimension,meanMaskDimension,1,1,0);
			meanMask.fill(1);
			mpr_img2.convolve(meanMask);
			
			
			
			/* Display the MPR image in the display windows */
			disp.display(mpr_img2);
			
			/* To avoid repetitive continuous redrawing */
			redraw=false;
		}
	}
    img.save_analyze("result.img",voxelsize);
	return 0;
}

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
	
	
	//  1ere etape : application du filtre median
	img=img.get_blur_median(1);
	
	//3D Mean filter
	/*int meanMaskDimension = 3;
	CImg<unsigned char> meanMask(meanMaskDimension,meanMaskDimension,meanMaskDimension,1,0);
	meanMask.fill(1);
	img.convolve(meanMask);*/
	 	//tentative de separer l'image en deux en utilisant une ligne calculée a la main
 	CImg<unsigned short> maxilla(img.width(),img.height(), img.depth());
 	CImg<unsigned short> mandible(img.width(),img.height(), img.depth());
 	int x,y,z;
	
	/*for(x = 0 ; x < img.width() ; x++)
 	{
 	      for( z = 0 ; z < img.depth() ; z++)
          {       
                if( z == (int)(0.13 * x + 71.8) ){
                     	for (y = 0 ; y < img.height() ; y++)
 	                    {
 	                         
 	                        img(x,y,z) = 255 ;
 	                    }
                
                }
          }
 	}*/
	
	//ici on cree notre MIP  selon Y
	unsigned short currentMax=0;
	
	CImg<unsigned short> MIPy(img.height(),img.depth());
	for(int i=0 ; i < img.height() ; i++){
	    
	    for(int j=0 ; j < img.depth(); j++){
	        currentMax=0;
	         for(int k=0 ; k < img.width(); k++){
	            if( currentMax < img(k,i,j) )
                    currentMax=img(k,i,j)	    ;
	        
	        }
	        MIPy(i,j)=currentMax;
	    }
	}
	
	MIPy.save_analyze("MIPy.img",voxelsize);
	
	for(int i=0 ; i < img.height() ; i++){
	    
	    for(int j=0 ; j < img.depth(); j++){
            if( MIPy(i,j) < 2048 )
                MIPy(i,j) = 0;
                
	    }
	}
 	MIPy.save_analyze("MIPy2.img",voxelsize);
 	
 	//CImg<unsigned short> MIPy(img.height(),img.depth());
 	
 	


 	//MIPy.save_analyze("MIPy3.img",voxelsize);
 	
 	//CImg<unsigned short> maxilla(img.width(),img.height(), img.depth());
 	//CImg<unsigned short> mandible(img.width(),img.height(), img.depth());
 	
    for(x = 0 ; x < img.width() ; x++)
 	{
 	      for( z = 0 ; z < img.depth() ; z++)
          {       
          
                
                for (y = 0 ; y < img.height() ; y++)
 	            {
 	                if( z > (int)(0.08 * x + 98.2))
 	                {
 	                   maxilla(x,y,z) = 0;
 	                   mandible(x,y,z) = img(x,y,z);;   
 	                } 
 	                else if ( (z) < (int)(0.08 * x + 98.2)){
 	                    maxilla(x,y,z) = img(x,y,z);
 	                    mandible(x,y,z) =0 ;
 	                    
 	               }
 	               else{
 	                maxilla(x,y,z) =0;
 	                mandible(x,y,z) = 0;
 	               
 	               }
 	            }
                
                
          }
 	}
	maxilla.save_analyze("maxilla.img",voxelsize);
	mandible.save_analyze("mandible.img",voxelsize);
	
	/*	
	for(int i=0 ; i < img.height() ; i++){
	    
	    for(int j=0 ; j < img.depth(); j++){
	        currentMax=0;
	         for(int k=0 ; k < img.width(); k++){
	            if( currentMax < maxilla(k,i,j) )
                    currentMax=maxilla(k,i,j)	    ;
	        
	        }
	        MIPy(i,j)=currentMax;
	    }
	}
	*/
	
	/*
	MIPy.save_analyze("MIPy.img",voxelsize);
		for(int i=0 ; i < img.height() ; i++){
	    
	    for(int j=0 ; j < img.depth(); j++){
	        currentMax=0;
	         for(int k=0 ; k < img.width(); k++){
	            if( currentMax < mandible(k,i,j) )
                    currentMax=mandible(k,i,j)	    ;
	        
	        }
	        MIPy(i,j)=currentMax;
	    }
	}
	
	MIPy.save_analyze("MIPy2.img",voxelsize);*/
	
	//MIP in Z
	currentMax=0;
	CImg<unsigned short> MIPzMaxilla(maxilla.width(),maxilla.height());
	for(int i=0 ; i < maxilla.width() ; i++){
	    
	    for(int j=0 ; j < maxilla.height(); j++){
	        currentMax=0;
	         for(int k=0 ; k < maxilla.depth(); k++){
	            if( currentMax < maxilla(i,j,k) )
                    currentMax=maxilla(i,j,k)	    ;
	        
	        }
	        MIPzMaxilla(i,j)=currentMax;
	    }
	}
	MIPzMaxilla.save_analyze("MIPzMaxilla.img",voxelsize);
	
	currentMax=0;
	CImg<unsigned short> MIPzMandible(mandible.width(),mandible.height());
	for(int i=0 ; i < mandible.width() ; i++){
	    
	    for(int j=0 ; j < mandible.height(); j++){
	        currentMax=0;
	         for(int k=0 ; k < mandible.depth(); k++){
	            if( currentMax < mandible(i,j,k) )
                    currentMax=mandible(i,j,k)	    ;
	        
	        }
	        MIPzMandible(i,j)=currentMax;
	    }
	}
	MIPzMandible.save_analyze("MIPzMandible.img",voxelsize);
	
	//Threshold
	
	MIPzMaxilla.threshold(3600); //TODO test
	MIPzMandible.threshold(4094);
	
	MIPzMaxilla.save_analyze("MIPzMaxillaThresholded.img",voxelsize);
	MIPzMandible.save_analyze("MIPzMandibleThresholded.img",voxelsize);
	
	//Find min and max position values, then crop
	
	int xmin=MIPzMaxilla.width(), xmax=0, ymin=MIPzMaxilla.height(), ymax=0;
	for(int x=0 ; x < MIPzMaxilla.width() ; x++){
	    for(int y=0 ; y < MIPzMaxilla.height(); y++){
	        if( MIPzMaxilla(x,y) > 0 ){
	        	if(x < xmin)
	        		xmin = x;
	        	if(x > xmax)
	        		xmax = x;
	        	if(y < ymin)
	        		ymin = y;
	        	if(y > ymax)
	        		ymax = y;
	        }
	    }
	}
	maxilla.crop(xmin, ymin, 0, 0, xmax, ymax, maxilla.depth(), 0);
	
	xmin=MIPzMandible.width(), xmax=0, ymin=MIPzMandible.height(), ymax=0;
	for(int x=0 ; x < MIPzMandible.width() ; x++){
	    for(int y=0 ; y < MIPzMandible.height(); y++){
	        if( MIPzMandible(x,y) > 0 ){
	        	if(x < xmin)
	        		xmin = x;
	        	if(x > xmax)
	        		xmax = x;
	        	if(y < ymin)
	        		ymin = y;
	        	if(y > ymax)
	        		ymax = y;
	        }
	    }
	}
	mandible.crop(xmin, ymin, 0, 0, xmax, ymax, mandible.depth(), 0);
	
	maxilla.save_analyze("MaxillaCropped.img",voxelsize);
	mandible.save_analyze("MandibleCropped.img",voxelsize);
	
	//Actual teeth segmentation
	
	int maxillaTh1, maxillaTh2, maxillaTh3,maxillaTh4, mandibleTh1, mandibleTh2, mandibleTh3, mandibleTh4;

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
/*		if (disp.button())  
		{
            const int y = disp.mouse_y();const int x = disp.mouse_x();
            const int z = displayedSlice[2];
            unsigned short *clic= img.data(x,y,z);
            printf("%d\n",*clic);
            // on a recupere le clic il faut faire le traitement
            CImg<unsigned short> nimg;
            unsigned char col[]={254};
*/
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
/*            img.draw_fill(x,y,z,col,1,nimg,atoi(argv[2]));
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
*/
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
			
			/* Display the MPR image in the display windows */
			disp.display(mpr_img2);
			
			/* To avoid repetitive continuous redrawing */
			redraw=false;
		}
	}
    //img.save_analyze("result.img",voxelsize);
	return 0;
}

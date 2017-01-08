# TeethSegmentation
Teeth segmentation program using Analyze 7.5 image format

## Progress
premier Mip implementé (commenté dans le code)

##Context

Student project developed during a medical imaging course at the University of Montpellier.

##Method

From "Rapid Automatic Segmentation and Visualization of Teeth in CT-Scan Data", Akhoondali et al. :

* CT scan data of head
* 2D mean filter
* Separation of mandible and maxilla : 
	* Maximum intensity projection (Y direction)
	* Find boundary between maxilla and mandible
	* Separate mandible and maxilla
	* For each jaw p...(?) the following
* Separate dental region :
	* MIP (?) in Z direction
	* Threshold filter
	* Find bounding rectangle of mask
	* Increase size of bounding rectangle
	* Crop images using bounding rectangle
* Use thresholds corresponding to mandible or maxilla
* Segment teeth :
	* Select (mad?) points
	* Use thresholds and rules for major segmentation
	* Add boundary pixels to teeth region
	* Final segmented image
* Optimal (mental?) artifact reduction :
	* Obtain weighted images in Z direction
	* Output images with increasing thresholds
	* Select appropriate threshold image
	* Dilate selected image to create mask
	* Use mask to remove artifacts from all (?)
* Visualize teeth :
	* Ino-surface extraction of images
	* Implementation color and opacity (?) (?)
	* Final visualization
	
##Resources

* The [CImg](http://cimg.eu/) image processing library (version 1.7.9 included in current repo)
* The INCISIX  CT-Scan images dataset (DICOM format) taken from the [OsiriX viewer website](http://www.osirix-viewer.com/)

Thanks to them!

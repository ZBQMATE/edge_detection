#include "msk.h"
#include "msk.cpp"
#include "cvs.h"
#include "cvs.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

using namespace std;

int detect(int model) {
	
	//model 1 for roberts, 2 for prewitt, 3 for sobel,  4 for marr_hildreth
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./h.bmp", "rb");
	FILE *picout = fopen("./result.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = width * height;;
	
	
	
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
		img[i] = 155;
	}
	
	
	//read image
	
	unsigned char input_image[height*width*3];
	fread(&input_image, height*width*3, 1, pic);
	
	//transfer to gray scale using blue values
	
	unsigned char blue_img[height*width];
	
	for (int i = 0; i < height*width; i++) {
		blue_img[i] = input_image[i*3];
	}
	
	delete []input_image;
	//define msk
	
	msk roberts;
	int size_roberts = 2;
	double map_roberts[] = {
		-1.0,0.0,
		0.0,1.0
	};
	
	roberts.size = size_roberts;
	for (int i = 0; i < size_roberts * size_roberts; i++) {
		roberts.map[i] = map_roberts[i];
	}
	
	msk prewitt;
	int size_prewitt = 3;
	double map_prewitt[] = {
		-1.0,-1.0,-1.0,
		0.0,0.0,0.0,
		1.0,1.0,1.0
	};
	
	prewitt.size = size_prewitt;
	for (int i = 0; i < size_prewitt * size_prewitt; i++) {
		prewitt.map[i] = map_prewitt[i];
	}
	
	msk sobel;
	int size_sobel = 3;
	double map_sobel[] = {
		-1.0,-2.0,-1.0,
		0.0,0.0,0.0,
		1.0,2.0,1.0
	};
	
	sobel.size = size_sobel;
	for (int i = 0; i < size_sobel * size_sobel; i++) {
		sobel.map[i] = map_sobel[i];
	}
	
	msk marr_hildreth;
	int size_marr_hildreth = 5;
	double map_marr_hildreth[] = {
		0.0,0.0,-1.0,0.0,0.0,
		0.0,-1.0,-2.0,-1.0,0.0,
		-1.0,-2.0,16,-2.0,-1.0,
		0.0,-1.0,-2.0,-1.0,0.0,
		0.0,0.0,-1.0,0.0,0.0,
	};
	
	marr_hildreth.size = size_marr_hildreth;
	for (int i = 0; i < size_marr_hildreth * size_marr_hildreth; i++) {
		marr_hildreth.map[i] = map_marr_hildreth[i];
	}
	
	//define cvs
	
	cvs bitmap;
	bitmap.height = height;
	bitmap.width = width;
	
	//for(int i = 0; i < height*width; i++) {
	//	bitmap.ima[i] = blue_img[i];
	//}
	
	
	//detect edge
	//unsigned char img[];
	double *cur_idx;
	double tpp;
	
	unsigned char imga[(height-roberts.size+1)*(width-roberts.size+1)*4];
	
	unsigned char imgb[(height-prewitt.size+1)*(width-prewitt.size+1)*4];
	
	unsigned char imgc[(height-sobel.size+1)*(width-sobel.size+1)*4];
	
	unsigned char imgd[(height-marr_hildreth.size+1)*(width-marr_hildreth.size+1)*4];
	
	
	
	switch(model) {
		
		//roberts
		case 1 :
			
			for (int i = 0; i < (height-roberts.size+1)*(width-roberts.size+1)*4; i++) {
				imga[i] = 0;
			}
			
			infoheader.biHeight = height - roberts.size + 1;
			infoheader.biWidth = width - roberts.size + 1;
			
			picsize = (height-roberts.size+1)*(width-roberts.size+1);
			
			
			cur_idx = bitmap.plot(blue_img, roberts);
			
			for (int i = 0; i < picsize; i++) {
				tpp = *cur_idx;
				imga[i*4] = (int) tpp;
				cur_idx++;
			}
			
		break;
		
		
		//prewitt
		case 2 :
			
			for (int i = 0; i < (height-prewitt.size+1)*(width-prewitt.size+1)*4; i++) {
				imgb[i] = 0;
			}
			
			infoheader.biHeight = height - prewitt.size + 1;
			infoheader.biWidth = width - prewitt.size + 1;
			
			picsize = (height-prewitt.size+1)*(width-prewitt.size+1);
			
			
			cur_idx = bitmap.plot(blue_img, prewitt);
			
			for (int i = 0; i < picsize; i++) {
				
				tpp = *cur_idx;
				imgb[i*4] = tpp;
				cur_idx++;
				
			}
			
		break;
		
		
		//sobel
		case 3 :
			
			for (int i = 0; i < (height-sobel.size+1)*(width-sobel.size+1)*4; i++) {
				imgc[i] = 0;
			}
			
			infoheader.biHeight = height - sobel.size + 1;
			infoheader.biWidth = width - sobel.size + 1;
			
			picsize = (height-sobel.size+1)*(width-sobel.size+1);
			
			
			cur_idx = bitmap.plot(blue_img, sobel);
			
			for (int i = 0; i < picsize; i++) {
				tpp = *cur_idx;
				imgc[i] = tpp;
				cur_idx++;
			}
			
		break;
		
		
		//marr_hildreth
		case 4 :
			
			for (int i = 0; i < (height-marr_hildreth.size+1)*(width-marr_hildreth.size+1)*4; i++) {
				imgd[i] = 0;
			}
			
			infoheader.biHeight = height - marr_hildreth.size + 1;
			infoheader.biWidth = width - marr_hildreth.size + 1;
			
			picsize = (height-marr_hildreth.size+1)*(width-marr_hildreth.size+1);
			
			
			cur_idx = bitmap.plot(blue_img, marr_hildreth);
			
			for (int i = 0; i < picsize; i++) {
				tpp = *cur_idx;
				imgd[i] = tpp;
				cur_idx++;
			}
			
		break;
		
		
	}
	
	
	
	//save
	
	infoheader.biBitCount = 32;
	
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, picout);
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, picout);
	
	fwrite(img, picsize, 4, picout);
	switch(model) {
		
		case 1 :
			fwrite(imga, picsize, 4, picout);
		break;
		
		case 2 :
			fwrite(imgb, picsize, 4, picout);
		break;
		
		case 3 :
			fwrite(imgc, picsize, 4, picout);
		break;
		
		case 4 :
			fwrite(imgd, picsize, 4, picout);
		break;
		
	}
	
	
	fclose(pic);
	fclose(picout);
	
	return 0;
}

int main() {
	detect(2);
}
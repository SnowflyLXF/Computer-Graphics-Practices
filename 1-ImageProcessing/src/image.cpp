#include <iostream>
#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

using namespace std;
/**
 * Image
 **/
Image::Image (int width_, int height_){

    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    data.raw = new uint8_t[num_pixels*4];
	int b = 0; //which byte to write to
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
		}
	}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){
	
	width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    data.raw = new uint8_t[num_pixels*4];
    
    //memcpy(data.raw, src.data.raw, num_pixels);
    *data.raw = *src.data.raw;
}

Image::Image (char* fname){

	int numComponents; //(e.g., Y, YA, RGB, or RGBA)
	data.raw = stbi_load(fname, &width, &height, &numComponents, 4);
	
	if (data.raw == NULL){
		printf("Error loading image: %s", fname);
		exit(-1);
	}
	

	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
	
}

Image::~Image (){
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){
	
	int lastc = strlen(fname);

	switch (fname[lastc-1]){
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}

void Image::AddNoise (double factor)
{
	/* WORK HERE */
	srand((unsigned)time(0));
	int x, y;
	double rnoise;
	for (x = 0; x < Width(); x++)
	{
		for (y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			rnoise = ((rand() % 256) / 255.0 * 2 - 1.0) * factor;     // generate noise
			if (p.r * (1 + rnoise) > 255)
				rnoise = 255 / p.r - 1.0;
			if (p.g * (1 + rnoise) > 255)
				rnoise = 255 / p.g - 1.0;
			if (p.b * (1 + rnoise) > 255)
				rnoise = 255 / p.b - 1.0;

			p = p * (1 + rnoise);
			p.SetClamp(p.r, p.g, p.b, 255);
			GetPixel(x,y) = p;
		}
	}
}

void Image::Brighten (double factor)
{
	int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p = p*factor;
			GetPixel(x,y) = scaled_p;
		}
	}
}

double ValidPixel(double value){
	// make sure rgb value of pixel is in [0, 255], avoid strange looking errors in the output.
	if (value>255) return 255.0;
	else {
		if (value<=0) return .0;
		else return value;
	}
}

void Image::ChangeContrast (double factor)
{
	/* WORK HERE */
	int x, y;
	float l = .0, r = .0, g = .0, b = .0;
	// calculate average luminance of the whole image
	for (x = 0; x < Width(); x++)
	{
		for (y = 0; y < Height(); y++)
		{
			l += GetPixel(x, y).Luminance();
		}
	}
	l /= (Width() * Height());

	for (x = 0; x < Width(); x++)
	{
		for (y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			r = ValidPixel((p.r-l)*factor + l);
			g = ValidPixel((p.g-l)*factor + l);
			b = ValidPixel((p.b-l)*factor + l);
			p.SetClamp(r, g, b, 255);
			GetPixel(x, y) = p;
		}
	}
}


void Image::ChangeSaturation(double factor)
{
	/* WORK HERE */
	int x, y;
	float l = .0, r = .0, g = .0, b = .0;
	for (x = 0; x < Width(); x++)
	{
		for (y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			// a gray level version of the image.
			l = p.Luminance();
			r = ValidPixel((p.r-l)*factor + l);
			g = ValidPixel((p.g-l)*factor + l);
			b = ValidPixel((p.b-l)*factor + l);;
			p.SetClamp(r, g, b, 255);
			GetPixel(x, y) = p;
		}
	}
}


Image* Image::Crop(int x, int y, int w, int h)
{
	/* WORK HERE */
	Image *new_img = new Image(w, h);
	for (int _x = 0; _x < w; _x++)
		for (int _y = 0; _y < h; _y++)
			new_img->GetPixel(_x, _y) = GetPixel(_x + x, _y + y);
	return new_img;
}


void Image::ExtractChannel(int channel)
{
	/* WORK HERE */
	int x, y;
	if (channel == 0)   // red channel
	{
		for (x = 0; x < Width(); x++)
		{
			for (y = 0; y < Height(); y++)
			{
				Pixel p = GetPixel(x, y);
				GetPixel(x, y) = Pixel(p.r, 0, 0);
			}
		}
	}
	if (channel == 1)   // green channel
	{
		for (x = 0; x < Width(); x++)
		{
			for (y = 0; y < Height(); y++)
			{
				Pixel p = GetPixel(x, y);
				GetPixel(x, y) = Pixel(0, p.g, 0);
			}
		}
	}
	if (channel == 2)   // blue channel
	{
		for (x = 0; x < Width(); x++)
		{
			for (y = 0; y < Height(); y++)
			{
				Pixel p = GetPixel(x, y);
				GetPixel(x, y) = Pixel(0, 0, p.r);
			}
		}
	}
}

void Image::Quantize (int nbits)
{
	/* WORK HERE */
	int step = 256 / pow(2, nbits);
	for (int x = 0; x < Width(); x++)
	{
		for (int y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			p.r = ValidPixel((int)(p.r / step + 0.5) * step);
			p.g = ValidPixel((int)(p.g / step + 0.5) * step);
			p.b = ValidPixel((int)(p.b / step + 0.5) * step);
			GetPixel(x, y) = p;
		}
	}
}

void Image::RandomDither (int nbits)
{
	/* WORK HERE */
	int step = 256 / pow(2, nbits);
	double noise;
	for (int x = 0; x < Width(); x++)
	{
		for (int y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);

			noise = (rand() % step - step/2) * 0.01;
			p.r = ValidPixel((int)(p.r / step + noise + 0.5) * step);
			p.g = ValidPixel((int)(p.g / step + noise + 0.5) * step);
			p.b = ValidPixel((int)(p.b / step + noise + 0.5) * step);
			GetPixel(x, y) = p;
		}
	}
}

static int Bayer4[4][4] =
{
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};

void Image::OrderedDither(int nbits)
{
	/* WORK HERE */
	int n = pow(2, 8-nbits);
	int step = 256 / pow(2, nbits);
	int bayer[128][128];
	int tmp[128][128];
	if (n == 2) {
		bayer[0][0] = 3; bayer[0][1] = 1; bayer[1][0] = 0; bayer[1][1] = 2;
	}
	else {
		if (n == 4)
			for (int i=0; i<4; i++)
				for (int j=0; j<4; j++)
					bayer[i][j] = Bayer4[i][j];
		else{
			for (int i=0; i<4; i++)
				for (int j=0; j<4; j++)
					tmp[i][j] = Bayer4[i][j];
			
			for (int k=4; k<n; k*=2){
				for(int i=0; i<k; i++)
					for(int j=0; j<k; j++){
						bayer[i][j] = tmp[i][j]*7;
						bayer[i][j+k] = tmp[i][j]*5;
						bayer[i+k][j] = tmp[i][j]*4;
						bayer[i+k][j+k] = tmp[i][j]*2;
					}
				for(int i=0; i<2*k; i++)
					for(int j=0; j<2*k; j++)
						tmp[i][j] = bayer[i][j];
			}
		}
	}

	for (int x = 0; x < Width(); x++)
	{
		for (int y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			if ((p.r - (int)(p.r / step) * step) > bayer[x % n][y % n])
				p.r = ValidPixel((int)(p.r / step + 1) * step);
			else
				p.r = ValidPixel((int)(p.r / step) * step);
			if ((p.g - (int)(p.g / step) * step) > bayer[x % n][y % n])
				p.g = ValidPixel((int)(p.g / step + 1) * step);
			else
				p.g = ValidPixel((int)(p.g / step) * step);
			if ((p.b - (int)(p.b / step) * step) > bayer[x % n][y % n])
				p.b = ValidPixel((int)(p.b / step + 1) * step);
			else
				p.b = ValidPixel((int)(p.b / step) * step);
			GetPixel(x, y) = p;
		}
	}
}

/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
	/* WORK HERE */
	Image old_img(Width()+2, Height()+2);
	for (int x=1; x<Width()+1; x++)
		for (int y=1; y<Height()+1; y++)
			old_img.GetPixel(x, y) = GetPixel(x-1, y-1);
	// copy edge so that the filter can be applied to the pixels on the edge
	for (int y=0; y<Height(); y++){
		old_img.GetPixel(0, y) = GetPixel(0, y);
		old_img.GetPixel(Width(), y) = GetPixel(Width()-1, y);
	}
	for (int x=0; x<Width(); x++){
		old_img.GetPixel(x, 0) = GetPixel(x, 0);
		old_img.GetPixel(x, Height()) = GetPixel(x, Height()-1);
	}
	old_img.GetPixel(0, 0) = GetPixel(0, 0);
	old_img.GetPixel(Width(), 0) = GetPixel(Width()-1, 0);
	old_img.GetPixel(0, Height()) = GetPixel(0, Height()-1);
	old_img.GetPixel(Width(), Height()) = GetPixel(Width()-1, Height()-1);

	Image new_img(Width()+2, Height()+2);
	int step = 256 / pow(2, nbits);
	double noise;
	for (int x = 0; x < Width()+2; x++)
	{
		for (int y = 0; y < Height()+2; y++)
		{
			Pixel p = old_img.GetPixel(x, y);

			noise = (rand() % step - step/2) * 0.01;
			p.r = ValidPixel((int)(p.r / step + noise + 0.5) * step);
			p.g = ValidPixel((int)(p.g / step + noise + 0.5) * step);
			p.b = ValidPixel((int)(p.b / step + noise + 0.5) * step);
			new_img.GetPixel(x, y) = p;
		}
	}

	// int step = 256 / pow(2, nbits);
	double rerr, gerr, berr;
	for (int x = 0; x < Width(); x++)
	{
		for (int y = 0; y < Height(); y++)
		{	
			// GetPixel(x, y) = new_img.GetPixel(x+1, y+1);
			rerr = old_img.GetPixel(x+1, y+1).r - new_img.GetPixel(x+1, y+1).r;
			gerr = old_img.GetPixel(x+1, y+1).g - new_img.GetPixel(x+1, y+1).g;
			berr = old_img.GetPixel(x+1, y+1).b - new_img.GetPixel(x+1, y+1).b;
			if (ValidCoord(x, y + 1)){
				GetPixel(x, y + 1).r = ValidPixel(new_img.GetPixel(x, y + 1).r + ALPHA * rerr);
				GetPixel(x, y + 1).g = ValidPixel(new_img.GetPixel(x, y + 1).g + ALPHA * gerr);
				GetPixel(x, y + 1).b = ValidPixel(new_img.GetPixel(x, y + 1).b + ALPHA * berr);
			}
			if (ValidCoord(x + 1, y - 1)){
				GetPixel(x + 1, y - 1).r = ValidPixel(new_img.GetPixel(x + 1, y - 1).r + BETA * rerr);
				GetPixel(x + 1, y - 1).g = ValidPixel(new_img.GetPixel(x + 1, y - 1).g + BETA * gerr);
				GetPixel(x + 1, y - 1).b = ValidPixel(new_img.GetPixel(x + 1, y - 1).b + BETA * berr);
			}
			if (ValidCoord(x + 1, y)){
				GetPixel(x + 1, y).r = ValidPixel(new_img.GetPixel(x + 1, y).r + GAMMA * rerr);
				GetPixel(x + 1, y).g = ValidPixel(new_img.GetPixel(x + 1, y).g + GAMMA * gerr);
				GetPixel(x + 1, y).b = ValidPixel(new_img.GetPixel(x + 1, y).b + GAMMA * berr);
			}
			if (ValidCoord(x + 1, y + 1)){
				GetPixel(x + 1, y + 1).r = ValidPixel(new_img.GetPixel(x + 1, y + 1).r + DELTA * rerr);
				GetPixel(x + 1, y + 1).g = ValidPixel(new_img.GetPixel(x + 1, y + 1).g + DELTA * gerr);
				GetPixel(x + 1, y + 1).b = ValidPixel(new_img.GetPixel(x + 1, y + 1).b + DELTA * berr);
			}
		}
	}
}

double GaussianFilter(double x, double y)
{
	double sigma = 1.0;
	return exp(-(x * x + y * y) / 2 * sigma * sigma) / sqrt(2 * M_PI * sigma * sigma);
}

void Image::Blur(int n)
{
	/* WORK HERE */
	Image old_img(Width(), Height());
	for (int x = 0; x < Width(); x++){
		for (int y = 0; y < Height(); y++)
			old_img.GetPixel(x, y) = GetPixel(x, y);
	}

	float r = 0.0, g = 0.0, b = 0.0, kernel[n][n], sum = 0.0, norm;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			kernel[i][j] = GaussianFilter(i - (int)(n / 2), j - (int)(n / 2));
			sum += kernel[i][j];
		}
		
	// calculate Gaussian filter kernel
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++)
		{
			kernel[i][j] /= sum;
		}
	}

	for (int x = 0; x < Width(); x++)
		for (int y = 0; y < Height(); y++)
		{
			norm = sum;
			float r = 0.0, g = 0.0, b = 0.0;
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{

					if (!ValidCoord(x + i - floor(n / 2), y + j - floor(n / 2)))
					{
						if(ValidCoord(x - i + floor(n / 2), y + j - floor(n / 2)))
						{
							r += old_img.GetPixel(x - i + floor(n / 2), y + j - floor(n / 2)).r * kernel[i][j];
							g += old_img.GetPixel(x - i + floor(n / 2), y + j - floor(n / 2)).g * kernel[i][j];
							b += old_img.GetPixel(x - i + floor(n / 2), y + j - floor(n / 2)).b * kernel[i][j];
						}
						else if(ValidCoord(x + i - floor(n / 2), y - j + floor(n / 2)))
						{
							r += old_img.GetPixel(x + i - floor(n / 2), y - j + floor(n / 2)).r * kernel[i][j];
							g += old_img.GetPixel(x + i - floor(n / 2), y - j + floor(n / 2)).g * kernel[i][j];
							b += old_img.GetPixel(x + i - floor(n / 2), y - j + floor(n / 2)).b * kernel[i][j];
						}
						else
						{
							r += old_img.GetPixel(x - i + floor(n / 2), y - j + floor(n / 2)).r * kernel[i][j];
							g += old_img.GetPixel(x - i + floor(n / 2), y - j + floor(n / 2)).g * kernel[i][j];
							b += old_img.GetPixel(x - i + floor(n / 2), y - j + floor(n / 2)).b * kernel[i][j];
						}
					}
					else
					{
						r += old_img.GetPixel(x + i - floor(n / 2), y + j - floor(n / 2)).r * kernel[i][j];
						g += old_img.GetPixel(x + i - floor(n / 2), y + j - floor(n / 2)).g * kernel[i][j];
						b += old_img.GetPixel(x + i - floor(n / 2), y + j - floor(n / 2)).b * kernel[i][j];
					
					}
				}
			}
			GetPixel(x, y).r = r;
			GetPixel(x, y).g = g;
			GetPixel(x, y).b = b;
		}
}

void Image::Sharpen(int n)
{
	/* WORK HERE */
	double red, green, blue;
	Image old_img(Width(), Height());
	for (int x=0; x<Width(); x++)
		for (int y=0; y<Height(); y++)
			old_img.GetPixel(x, y) = GetPixel(x, y);

	Image noise_img(old_img);
	noise_img.Blur(n);

	for (int x=0; x<Width(); x++)
		for (int y=0; y<Height(); y++){
			red = 0; green  = 0; blue = 0;
			red = 2*old_img.GetPixel(x, y).r - noise_img.GetPixel(x, y).r;
			green = 2*old_img.GetPixel(x, y).g - noise_img.GetPixel(x, y).g;
			blue = 2*old_img.GetPixel(x, y).b - noise_img.GetPixel(x, y).b;;
			GetPixel(x, y).r = ValidPixel(red);
			GetPixel(x, y).g = ValidPixel(green);
			GetPixel(x, y).b = ValidPixel(blue);
		}
}
void Image::EdgeDetect()
{
	/* WORK HERE */  
	Image old_img(Width()+2, Height()+2);
	for (int x=1; x<Width()+1; x++)
		for (int y=1; y<Height()+1; y++)
			old_img.GetPixel(x, y) = GetPixel(x-1, y-1);
	// copy edge so that the filter can be applied to the pixels on the edge
	for (int y=0; y<Height(); y++){
		old_img.GetPixel(0, y) = GetPixel(0, y);
		old_img.GetPixel(Width(), y) = GetPixel(Width()-1, y);
	}
	for (int x=0; x<Width(); x++){
		old_img.GetPixel(x, 0) = GetPixel(x, 0);
		old_img.GetPixel(x, Height()) = GetPixel(x, Height()-1);
	}
	old_img.GetPixel(0, 0) = GetPixel(0, 0);
	old_img.GetPixel(Width(), 0) = GetPixel(Width()-1, 0);
	old_img.GetPixel(0, Height()) = GetPixel(0, Height()-1);
	old_img.GetPixel(Width(), Height()) = GetPixel(Width()-1, Height()-1);

	// apply filter [[-1, -1, -1] [-1, 8, -1] [-1, -1, -1]]
	double r, g, b;
	for (int x=1; x<Width()+1; x++)
		for (int y=1; y<Height()+1; y++){
			r = 0; g = 0; b = 0;
			r = 8*old_img.GetPixel(x, y).r-old_img.GetPixel(x-1, y+1).r-old_img.GetPixel(x-1, y).r-old_img.GetPixel(x-1, y-1).r-old_img.GetPixel(x, y+1).r
				-old_img.GetPixel(x, y-1).r-old_img.GetPixel(x+1, y+1).r-old_img.GetPixel(x+1, y).r-old_img.GetPixel(x+1, y-1).r;
			g = 8*old_img.GetPixel(x, y).g-old_img.GetPixel(x-1, y+1).g-old_img.GetPixel(x-1, y).g-old_img.GetPixel(x-1, y-1).g-old_img.GetPixel(x, y+1).g
				-old_img.GetPixel(x, y-1).g-old_img.GetPixel(x+1, y+1).g-old_img.GetPixel(x+1, y).g-old_img.GetPixel(x+1, y-1).g;
			b = 8*old_img.GetPixel(x, y).b-old_img.GetPixel(x-1, y+1).b-old_img.GetPixel(x-1, y).b-old_img.GetPixel(x-1, y-1).b-old_img.GetPixel(x, y+1).b
				-old_img.GetPixel(x, y-1).b-old_img.GetPixel(x+1, y+1).b-old_img.GetPixel(x+1, y).b-old_img.GetPixel(x+1, y-1).b;
			GetPixel(x-1, y-1).r = r;
			GetPixel(x-1, y-1).g = g;
			GetPixel(x-1, y-1).b = b;
		}
}

double SamplingWeight(double u, double v, int tp){
	double weight;  //unnormalized
	if (tp==0) weight = 1.0;    // point sampling
	if (tp==1) weight = fabs(u*v);	// bilinear sampling
	if (tp==2) weight = GaussianFilter(u, v);  // Gaussian sampling
	return weight;
}

Image* Image::Scale(double sx, double sy)
{
	/* WORK HERE */
	Image *new_img = new Image((int)(Width()*sx+.5), (int)(Height()*sy+.5));
	int rightend, upperend;
	Pixel p=GetPixel(0, 0);
	double r = max(1/sx, 1/sy), weight, u, v, red, green, blue, sum;
	// sampling method, 0-point sampling; 1-bilinear sampling; 2-gaussian smpling
	// SetSamplingMethod(2);

	for (int x=0; x<(int)(Width()*sx+.5); x++){
		for (int y=0; y<(int)(Height()*sy+.5); y++){
			red = .0; green = .0; blue = .0, sum=.0;
			u = x/sx;
			v = y/sy;
			
			//resample
			for (int i=0; i<2*r+1; i++){
 				for (int j=0; j<2*r+1; j++){
					if (ValidCoord(ceil(u-r)+i, ceil(v-r)+j)){
						weight = SamplingWeight((int)(u+r)-i-u, (int)(v+r)-j-v, sampling_method);
						// // Gaussian sampling.
						// weight = GaussianFilter(rightend-u, upperend-v);
						// // point sampling
						// weight = 1; 
						// // bilinear sampling
						// weight = (rightend-u-ceil(u-r))*(upperend-v-ceil(v-r));  
						sum += weight;
						red += weight * GetPixel(ceil(u-r)+i, ceil(v-r)+j).r;
						green += weight * GetPixel(ceil(u-r)+i, ceil(v-r)+j).g;
						blue += weight * GetPixel(ceil(u-r)+i, ceil(v-r)+j).b;
					}
				}
			}
			p.SetClamp(red/sum, green/sum, blue/sum);
			new_img->GetPixel(x, y) = p;
		}
	}
	return new_img;
}

Image* Image::Rotate(double angle)
{
	/* WORK HERE */
	int rightend, upperend, r=1, xmax = 0, xmin= 0, ymax=0, ymin=0, contain;
	Pixel p=GetPixel(0, 0);
	double weight, u, v, red, green, blue, sum;

	xmax = sqrt(Width() * Width() + Height() * Height());
	ymax = sqrt(Width() * Width() + Height() * Height());

	Image *new_img = new Image(xmax, ymax);
	for (int x=0; x<xmax; x++){
		for (int y=0; y<ymax; y++){

			// move the center of rotation to the center of the image
			u = x * cos(angle) + y * sin(angle) + Width() / 2 - xmax / 2 * cos(angle) - ymax/2 * sin(angle);
   			v = -x * sin(angle) + y * cos(angle) + Height() / 2 + xmax / 2 * sin(angle) - ymax / 2 * cos(angle);

			for (int i=0; i<2*r+1; i++){
 				for (int j=0; j<2*r+1; j++){
					if (ValidCoord(ceil(u-r)+i, ceil(v-r)+j)){
						weight = SamplingWeight((int)(u+r)-i-u, (int)(v+r)-j-v, sampling_method);
						sum += weight;
						red += weight * GetPixel((u-r)+i, ceil(v-r)+j).r;
						green += weight * GetPixel((u-r)+i, ceil(v-r)+j).g;
						blue += weight * GetPixel((u-r)+i, ceil(v-r)+j).b;
					}
				}
			}

			if (ValidCoord(u, v)){
				red = GetPixel(u, v).r;
				green = GetPixel(u, v).g;
				blue = GetPixel(u, v).b;
				p.SetClamp(red, green, blue);
				new_img->GetPixel(x, y) = p;
			}
			else{
				p.SetClamp(255, 255, 255);
				new_img->GetPixel(x, y) = p;
			}
		}
	}
	return new_img;
}


void Image::Fun()
{
	/* WORK HERE */
	Image old_img(Width(), Height());
	for (int x = 0; x < Width(); x++)
		for (int y = 0; y < Height(); y++)
			old_img.GetPixel(x, y) = GetPixel(x, y);
	
	SetSamplingMethod(2);

	double xo, yo, ix, iy, weight, sum, red, green, blue;
	int r = 1; // sampling radius
	Pixel p=GetPixel(0, 0);
	for(int x = 0 ; x<Width(); x++)
		for(int y= 0; y<Height(); y++)
		{
			xo = (16.0 * sin(2.0 * M_PI * x / 240.0));
			yo = (16.0 * sin(2.0 * M_PI * y / 240.0));
			ix = min(Width() - 1.0, max(.0, x + xo));
			iy = min(Height() - 1.0, max(.0, y + yo));
			sum = .0;
			red = .0, green = .0; blue = .0;
			for (int i=0; i<2*r+1; i++){
				for (int j=0; j<2*r+1; j++){
					if(old_img.ValidCoord(ceil(ix-r), ceil(iy-r))){
						weight = SamplingWeight((int)(ix+r)-i-ix, (int)(iy+r)-j-iy, sampling_method);
						sum += weight;
						red += weight*old_img.GetPixel(ceil(ix-r), ceil(iy-r)).r;
						green += weight*old_img.GetPixel(ceil(ix-r), ceil(iy-r)).g;
						blue += weight*old_img.GetPixel(ceil(ix-r), ceil(iy-r)).b;
					}
				}
			}
			p.SetClamp(red/sum, green/sum, blue/sum);
			GetPixel(x, y) = p;
			// GetPixel(x, y) = old_img.GetPixel(ix, iy);

		}
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}


Pixel Image::Sample (double u, double v){
    /* WORK HERE */
	//implemented in function(scale, etc)
	return Pixel();
}

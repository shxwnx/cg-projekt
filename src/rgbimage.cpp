#include "rgbimage.h"
#include "color.h"
#include "assert.h"

RGBImage::RGBImage( unsigned int Width, unsigned int Height)
{
	m_Image = new Color[Width*Height];
	m_Height = Height;
	m_Width = Width;
}

RGBImage::~RGBImage()
{
	delete m_Image;
}

void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
	if (0 <= x && x < m_Width && 0 <= y && y < m_Height) {
		m_Image[y*m_Width + x] = c;
	}
}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
	if (x >= m_Width || y >= m_Height || x < 0 || y < 0) {
		return *new Color();
	}
	else {
		return m_Image[y*m_Width + x];
	}
}

unsigned int RGBImage::width() const
{
	return m_Width;
}
unsigned int RGBImage::height() const
{
	return m_Height;
}

unsigned char RGBImage::convertColorChannel( float v)
{
	if (v >= 0.0f && v <= 1.0f) {
		unsigned char tmp = v * 255;
		return tmp;
	}
	else {
		return '0';
	}
}


bool RGBImage::saveToDisk( const char* Filename)
{
	FILE* mybmp;
	
	unsigned char fileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char infoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	int filesize = 54 + 3 * m_Width*m_Height;


	fileheader[2] = (unsigned char)(filesize);
	fileheader[3] = (unsigned char)(filesize >> 8);
	fileheader[4] = (unsigned char)(filesize >> 16);
	fileheader[5] = (unsigned char)(filesize >> 24);

	infoheader[4] = (unsigned char)(m_Width);
	infoheader[5] = (unsigned char)(m_Width >> 8);
	infoheader[6] = (unsigned char)(m_Width >> 16);
	infoheader[7] = (unsigned char)(m_Width >> 24);
	infoheader[8] = (unsigned char)(m_Height);
	infoheader[9] = (unsigned char)(m_Height >> 8);
	infoheader[10] = (unsigned char)(m_Height >> 16);
	infoheader[11] = (unsigned char)(m_Height >> 24);

	mybmp = fopen(Filename, "wb");
	fwrite(fileheader, 1, 14, mybmp);
	fwrite(infoheader, 1, 40, mybmp);

	//unsigned char bitmap[m_Width * m_Height * 3];
	for (int y = this->m_Height - 1; y >= 0; y--) {
		for (int x = 0; x < m_Width; x++) {

			Color pixel = this->getPixelColor(x, y);
			unsigned char bitmap[3];
			bitmap[0] = this->convertColorChannel(pixel.B);
			bitmap[1] = this->convertColorChannel(pixel.G);
			bitmap[2] = this->convertColorChannel(pixel.R);

			fwrite(bitmap, 1, 3, mybmp);
		}
	}
	fclose(mybmp);
	return true;
}

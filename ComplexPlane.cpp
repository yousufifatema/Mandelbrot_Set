#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size.x = pixelWidth;
    m_pixel_size.y = pixelHeight;

    m_aspectRatio = static_cast<double>(pixelHeight)/pixelWidth;
    m_plane_center.x = 0.0;
    m_plane_center.y = 0.0;
    m_plane_size.x = BASE_WIDTH;
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio;
    m_zoomCount = 0;
    m_state = State::CALCULATING; 
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelHeight*pixelWidth);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount); 
    m_plane_size = Vector2f(x,y);
    m_state = State::CALCULATING; 
}
void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount); 
    m_plane_size = Vector2f(x,y);
    m_state = State::CALCULATING; 
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text)
{
   ostringstream strm;

   strm << "Mandelbrot Set\nCenter: (" << m_plane_center.x << "," << m_plane_center.y << ")\nCursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\nLeft-click to Zoom in\nRight click to Zoom out";

   text.setString(strm.str());
}
void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        Uint8 r, g, b;

        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i * m_pixel_size.x].position.x = static_cast<float>(j);
                m_vArray[j + i * m_pixel_size.x].position.y = static_cast<float>(i);

                Vector2i currPixel(j, i);
                iterationsToRGB(countIterations(mapPixelToCoords(currPixel)), r, g, b);

                m_vArray[j + i * m_pixel_size.x].color.r = r;
                m_vArray[j + i * m_pixel_size.x].color.g = g;
                m_vArray[j + i * m_pixel_size.x].color.b = b;                 
            }
        }
    }

    m_state = State::DISPLAYING;
}
int ComplexPlane::countIterations(Vector2f coord)
{
    complex<float> c(coord.x, coord.y);
    complex<float> z = c;

    int i = 0;
    while (abs(z) < 2.0 && i < MAX_ITER)
    {
        z = z * z + c;
        i++;
    }

    return i; 
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float min_x = m_plane_center.x - m_plane_size.x / 2.0;
    float min_y = m_plane_center.y - m_plane_size.y / 2.0;

    float coord_x = static_cast<float>(mousePixel.x) / static_cast<float>(m_pixel_size.x) * m_plane_size.x + min_x;
    float coord_y = static_cast<float>(mousePixel.y) / static_cast<float>(m_pixel_size.y) * m_plane_size.y + min_y;

    Vector2f coordPixel(coord_x, coord_y);
    return coordPixel; 
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else if (count >  static_cast<int>(MAX_ITER) * 4 / 5) // Red
	{
		r = 255;
		g = 0;
		b = 0;
	}
	else if (count > static_cast<int>(MAX_ITER) * 3 / 5) // Yellow
	{
		r = 255;
		g = 255;
		b = 0;
	}
	else if (count > static_cast<int>(MAX_ITER) * 2 / 5) // Green
	{
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count > static_cast<int>(MAX_ITER) * 1 / 5) // Turquoise
	{
		r = 64;
		g = 224;
		b = 208;
	}
	else // Blue -> Purple
	{
		r = 128 * (1 - (count / 12));
		g = 0;
		b = 128;
	}
}    

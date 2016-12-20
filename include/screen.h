
#ifndef DOTRIX_SCREEN_H
#define DOTRIX_SCREEN_H

#include <QWidget>
#include <QImage>
#include <QPainter>

#include <gameboycore/gpu.h>

/**
	\class Screen
	\brief Widget used to draw the GameboyCore screen
*/
class Screen : public QWidget
{
	Q_OBJECT


public:
	Screen() :
		image_(160, 144, QImage::Format_ARGB32)
	{
	}

	~Screen()
	{
	}

public slots:

	void update(gb::GPU::Scanline scanline, int line)
	{
		auto x = 0;
		for (const auto& pixel : scanline)
		{
			QColor color(pixel.r, pixel.g, pixel.b);
			image_.setPixelColor(x++, line, color);
		}
	}

protected:
	void paintEvent(QPaintEvent* event)
	{
		QPainter painter(this);

		// create target rect
		auto size = this->size();
		QRectF target(0, 0, size.rwidth(), size.rheight());

		// source rect
		QRectF source(0, 0, 160, 144);

		// draw screen
		painter.drawImage(target, image_, source);
	}

private:
	QImage image_;
};

#endif

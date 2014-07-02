/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS ShapesDemo is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

#include "eprosimashapesdemo/qt/DrawArea.h"

#include "eprosimashapesdemo/shapesdemo/ShapesDemo.h"
#include "eprosimashapesdemo/shapesdemo/Shape.h"

#include <QPainter>

DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent),
      m_isInitialized(false),
      firstA(10),
      lastA(240)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    m_brush.setStyle(Qt::SolidPattern);



}

DrawArea::~DrawArea()
{

}

QSize DrawArea::sizeHint() const
{
    return QSize(500, 500);
}

QSize DrawArea::minimumSizeHint() const
{
    return QSize(300, 300);
}


void DrawArea::paintEvent(QPaintEvent * /* event */)
{

    QPainter painter(this);
    drawShapes(&painter);
}

uint8_t DrawArea::getAlpha(int pos,size_t total)
{
    float inc = (lastA-firstA)/total;
    return (uint8_t)(firstA+inc*(pos+1));
}

void DrawArea::drawShapes(QPainter* painter)
{
    if(m_isInitialized)
    {
        QMutexLocker locker(mp_SD->getMutex());
        m_shapes.clear();
        if(mp_SD->getShapes(&m_shapes))
        {
            for(std::vector<Shape*>::iterator it = m_shapes.begin();
                it!=m_shapes.end();++it)
            {
                size_t total = (*it)->m_history.size();
                int index = 0;
                for(std::list<ShapeType>::reverse_iterator sit = (*it)->m_history.rbegin();
                    sit!=(*it)->m_history.rend();++sit)
                {
                    paintShape(painter,(*it)->m_type,*sit,getAlpha(index,total),true);
                    ++index;
                }
                paintShape(painter,(*it)->m_type,(*it)->m_mainShape,255);

            }
        }

        else
            cout << "GETSHAPESFALSE"<<endl;
    }
}



void DrawArea::paintShape(QPainter* painter,TYPESHAPE type,ShapeType& shape,uint8_t alpha,bool isHistory)
{
    painter->save();
    m_pen.setColor(SD_QT_BLACK);
    if(isHistory)
        m_pen.setStyle(Qt::DotLine);
    else
        m_pen.setStyle(Qt::SolidLine);
    painter->setPen(m_pen);
    QColor auxc = getColorFromShapeType(shape);
    auxc.setAlpha(alpha);
    m_brush.setColor(auxc);
    painter->setBrush(m_brush);
    switch(type)
    {
    case SQUARE:
    {
        QRect rect(shape.m_x-shape.m_size/2,
                   shape.m_y-shape.m_size/2,
                   shape.m_size,
                   shape.m_size);
        painter->drawRect(rect);
        break;
    }
    case TRIANGLE:
    {
        uint32_t x,y,s;
        x = shape.m_x;
        y = shape.m_y;
        s = shape.m_size;
        double h = 0.5*sqrt(3*pow((double)s,2));
        QPoint points[3] = {
            QPoint(x-s/2, y+h/2),
            QPoint(x+s/2, y+h/2),
            QPoint(x, y-h/2)
        };
        painter->drawPolygon(points,3);
        break;
    }
    case CIRCLE:
    {
        QRect rect(shape.m_x-shape.m_size/2,
                   shape.m_y-shape.m_size/2,
                   shape.m_size,
                   shape.m_size);
        painter->drawEllipse(rect);
        break;
    }
    }
    painter->restore();

}


QColor DrawArea::getColorFromShapeType(ShapeType& st)
{
    switch(st.getColor())
    {
    case SD_PURPLE: return SD_QT_PURPLE;
    case SD_BLUE: return SD_QT_BLUE;
    case SD_RED: return SD_QT_RED;
    case SD_GREEN: return SD_QT_GREEN;
    case SD_YELLOW: return SD_QT_YELLOW;
    case SD_CYAN: return SD_QT_CYAN;
    case SD_MAGENTA: return SD_QT_MAGENTA;
    case SD_ORANGE: return SD_QT_ORANGE;
    }
    return SD_QT_BLUE;
}


void DrawArea::setShapesDemo(ShapesDemo*SD)
{
    if(SD!=NULL)
    {
        mp_SD = SD;
        m_isInitialized = true;
    }
}
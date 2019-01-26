// -*- mode: c++; indent-tabs-mode: nil; tab-width: 2; c-basic-offset: 2; -*-
/*
 * Copyright (c) 2018
 * @author Rafael Cisneros
 *
 * AIST
 *
 * All rights reserved.
 *
 */

#include "plot.h"
#include <qwt_color_map.h>

Plot::Plot(QWidget *parent)
  : QwtPlot(parent) {
  
  spectrogram = new QwtPlotSpectrogram();

  QwtLinearColorMap *colorMap = new QwtLinearColorMap(Qt::darkCyan, Qt::red);
  
  spectrogram->setColorMap(colorMap);
  //spectrogram->setData(new SpectrogramData());
  spectrogram->setData(new RasterData);

  spectrogram->attach(this);

  replot();
}

/*
SpectrogramData::SpectrogramData() {

  setInterval(Qt::XAxis, QwtInterval(-1.5,  1.5));
  setInterval(Qt::YAxis, QwtInterval(-1.5,  1.5));
  setInterval(Qt::ZAxis, QwtInterval( 0.0, 10.0));
}

double SpectrogramData::value(double x, double y) const {
  
  const double c = 0.842;
  const double v1 = x*x + (y - c) * (y + c);
  const double v2 = x * (y + c) + x * (y + c);

  return 1.0 / (v1 * v1 + v2 * v2);
}
*/

RasterData::RasterData() {

  const double matrix[] =
    {
      1, 2, 4, 1,
      6, 3, 5, 2,
      4, 2, 1, 5,
      5, 4, 2, 3
    };

  QVector<double> values;
  for (uint i = 0; i < sizeof(matrix) / sizeof(double); i++)
    values += matrix[i];

  const int numColumns = 4;
  setValueMatrix(values, numColumns);

  setInterval(Qt::XAxis, QwtInterval(-0.5, 3.5, QwtInterval::ExcludeMaximum));
  setInterval(Qt::YAxis, QwtInterval(-0.5, 3.5, QwtInterval::ExcludeMaximum));
  setInterval(Qt::ZAxis, QwtInterval( 1.0, 6.0));

  setResampleMode(BilinearInterpolation);
}

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
#include <iostream>

Plot::Plot(QWidget *parent)
  : QwtPlot(parent) {
  
  spectrogram = new QwtPlotSpectrogram();

  QwtLinearColorMap *colorMap = new QwtLinearColorMap(Qt::blue, Qt::red);

  colorMap->addColorStop(0.10, Qt::cyan);
  colorMap->addColorStop(0.60, Qt::green);
  colorMap->addColorStop(0.95, Qt::yellow);
  
  spectrogram->setColorMap(colorMap);

  rasterData = new QwtMatrixRasterData();

  QVector<double> values;
  for (uint i = 0; i < 160; i++)
    values += 0;

  const int numColumns = 16;
  rasterData->setValueMatrix(values, numColumns);
  
  rasterData->setInterval(Qt::XAxis, QwtInterval(-4.0, 4.0, QwtInterval::ExcludeMaximum));
  rasterData->setInterval(Qt::YAxis, QwtInterval(-2.5, 2.5, QwtInterval::ExcludeMaximum));
  rasterData->setInterval(Qt::ZAxis, QwtInterval( 1.0, 100.0));

  rasterData->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
  
  spectrogram->setData(rasterData);

  spectrogram->attach(this);
}

void Plot::setMatrixData(const double matrix[], const int numRows, const int numColumns) {

  QVector<double> values;

  for (uint i = 0; i < numColumns * numRows; i++)
    values += matrix[i];

  rasterData->setValueMatrix(values, numColumns);

  replot();
}

/*
RasterData::RasterData() {

  const double matrix[] =
    {
      0, 0,  0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 0,  0,  0,  3,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 0,  0, 29, 72,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 0,  5, 95, 89, 39, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 1, 22, 68, 95, 26, 1, 2, 10,  3, 13,  6,  0,  0, 0, 0,
      0, 9, 38, 28, 30,  8, 0, 1, 21, 28, 65, 36,  1,  0, 0, 0,
      0, 1,  1,  8,  3, 10, 0, 8, 22, 48, 60, 71, 21,  2, 0, 0,
      0, 0,  0,  0,  0,  0, 0, 8, 29, 33, 64, 55, 54, 22, 0, 0,
      0, 0,  0,  0,  0,  0, 0, 0,  8, 22, 45, 76, 55, 28, 0, 0,
      0, 0,  0,  0,  0,  0, 0, 0,  6,  4, 19, 36, 70, 19, 0, 0
    };

  QVector<double> values;
  for (uint i = 0; i < sizeof(matrix) / sizeof(double); i++)
    values += matrix[i];

  const int numColumns = 16;
  setValueMatrix(values, numColumns);

  setInterval(Qt::XAxis, QwtInterval(-0.5, 3.5, QwtInterval::ExcludeMaximum));
  setInterval(Qt::YAxis, QwtInterval(-0.5, 3.5, QwtInterval::ExcludeMaximum));
  setInterval(Qt::ZAxis, QwtInterval( 1.0, 100.0));

  setResampleMode(BilinearInterpolation);
}
*/

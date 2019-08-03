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

#ifndef __PLOT_H__
#define __PLOT_H__

#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>

class Plot : public QwtPlot {

  Q_OBJECT

 public:

  Plot(QWidget *parent);

  void setMatrixData(const double *matrix, const int numRows, const int numColumns);

 private:

  QwtPlotSpectrogram *spectrogram;
  QwtMatrixRasterData *rasterData;
};

/*
class RasterData : public QwtMatrixRasterData {

public:

  RasterData();
};
*/

#endif  // __PLOT_H__

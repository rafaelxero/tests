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

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "plot.h"
#include <QWidget>

class Widget : public QWidget {

  Q_OBJECT

 public:

  Widget(QWidget *parent = 0);
  ~Widget();

 private:

  Plot *plot;

  void init_plot();
};

#endif  // __WIDGET_H__

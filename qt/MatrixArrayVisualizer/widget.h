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

#include "qextserialport.h"
#include "plot.h"
#include <QWidget>

class Widget : public QWidget {

  Q_OBJECT

 public:

  Widget(QWidget *parent = 0);
  ~Widget();
           
 public slots:
  void receive();
  
 private:

  QextSerialPort *port;
  Plot *plot;

  int pos_count;
  bool got_zero;

  void init_port();
};

#endif  // __WIDGET_H__

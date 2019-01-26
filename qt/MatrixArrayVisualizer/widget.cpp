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

#include "widget.h"
#include <QHBoxLayout>

Widget::Widget(QWidget *parent)
  : QWidget(parent) {

  plot = new Plot(this);
  
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(plot);
  
}

Widget::~Widget() {
}

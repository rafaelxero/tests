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

#include <QtWidgets/QApplication>
#include "widget.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  Widget w;
  w.show();
  
  return a.exec();
}

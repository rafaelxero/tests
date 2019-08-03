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
#include <QMessageBox>
#include <iostream>

const int COLS = 16;
const int ROWS = 10;
const int END_MARKER = 0xFF;

Widget::Widget(QWidget *parent)
  : QWidget(parent),
    pos_count(0),
    got_zero(false) {

  init_port();
  
  plot = new Plot(this);
  
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(plot);

  connect(port, SIGNAL(readyRead()), this, SLOT(receive()));
}

Widget::~Widget() {
  port->close();
}

void Widget::init_port() {

  QString portname = "/dev/ttyACM0";
  port = new QextSerialPort(portname, QextSerialPort::EventDriven);

  port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
  
  if (!port->isOpen())
    QMessageBox::warning(this, "port error", "Impossible to open the port " + portname + "!");

  port->setBaudRate(BAUD115200);
  port->setFlowControl(FLOW_OFF);
  port->setParity(PAR_NONE);
  port->setDataBits(DATA_8);
  port->setStopBits(STOP_1);
}

void Widget::receive() {

  double matrix[COLS * ROWS];
  
  char data[255];
  int bytesRead = port->read(data, 255);
  data[bytesRead] = '\0';

  //std::cout << "bytesRead: " << bytesRead << std::endl;

  for (int i = 0; i < bytesRead; i++) {

    unsigned char got_byte = (unsigned char) data[i];

    int value = got_byte & 0xFF;

    //std::cout << "value = " << value << std::endl;
    
    if (value == END_MARKER) {

      pos_count = 0;
      //std::cout << std::endl;
      //std::cout << "END_MARKER" << std::endl;

      plot->setMatrixData(matrix, ROWS, COLS);
      plot->replot();
    }

    else if (got_zero) {

      for (int j = 0; j < value; j++) {
        
        matrix[pos_count] = 0;
        //std::cout << matrix[pos_count] << " ";
        pos_count++;

        if (pos_count >= COLS * ROWS) {

          pos_count = 0;
          
          plot->setMatrixData(matrix, ROWS, COLS);
          plot->replot();
        }
      }

      got_zero = false;
    }
    
    else if (got_byte == 0)

      got_zero = true;

    else {
      
      matrix[pos_count] = value;
      //std::cout << matrix[pos_count] << " ";
      pos_count++;

      if (pos_count >= COLS * ROWS) {

        pos_count = 0;
        
        plot->setMatrixData(matrix, ROWS, COLS);
        plot->replot();
      }
    }

    //std::cout << "pos_count = " << pos_count << std::endl;
  }
}

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->setWindowIcon(QIcon(":/snake.ico"));
}

Widget::~Widget()
{
    delete ui;
}


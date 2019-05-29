#include "MainWindow.h"

#include <QFileDialog>

#include <QPainter>

#include <QtWidgets>

#include <iomanip>
#include <iostream>
#include <sstream>

#include <math.h>

#define PI 3.14159265

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), m_Volume(0), m_VectorField(0)
{
	m_Ui = new Ui_MainWindow();
	m_Ui->setupUi(this);

	scene = new MyScene();
	scene->setParent(this);
	m_Ui->graphicsView->setScene(scene);
	

	connect(m_Ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileAction()));
	connect(m_Ui->actionClose, SIGNAL(triggered()), this, SLOT(closeAction()));
	connect(scene, SIGNAL(sceneChanged(int, int)), this, SLOT(changeDim(int, int)));
	connect(scene, SIGNAL(sceneAxisChanged(int)), this, SLOT(changeAxis(int)));


	connect(m_Ui->actionShow_Parallel_Coordinates, SIGNAL(triggered()), this, SLOT(drawMultiSet()));
	connect(m_Ui->actionShow_Star_Plots, SIGNAL(triggered()), this, SLOT(drawStar()));
	connect(m_Ui->actionShow_Chernoff_Faces, SIGNAL(triggered()), this, SLOT(drawChernoff()));

	WIDTH = 980;
	HEIGHT = 600;

	//std::cout << WIDTH << ", " << HEIGHT << std::endl;

	scene->setSceneRect(0, 0, WIDTH, HEIGHT);
	font = QFont("Arial", 10);
}

MainWindow::~MainWindow()
{
	delete m_Volume;
	delete m_VectorField;
	delete m_MultiSet;
}

void MainWindow::drawStar()
{
	if (m_MultiSet == nullptr || m_MultiSet->size() == 0)
	{
		std::cout << "No data found." << std::endl;
		return;
	}

	std::cout << "Drawing Star Plot." << std::endl;

	scene->clear();
	scene->setSceneRect(0, 0, WIDTH, HEIGHT);
	scene->sceneRect();
	scene->setSceneRect(QRectF());
	scene->sceneRect();
	//scene->sceneChanged();
	//scene->updateScene();

	float deg = float(2 * PI) / m_MultiSet->dimensions();

	int DISPLACEMENT = 800;

	m_Ui->progressBar->setEnabled(true);
	m_Ui->progressBar->setRange(0, m_MultiSet->size());
	for (int i = 0; i < m_MultiSet->size(); i++)
	{
		m_Ui->progressBar->setValue(i+1);

		auto data = m_MultiSet->element(i);

		for (int j = 0; j < m_MultiSet->dimensions(); j++)
		{
			// draw name
			auto text = scene->addText(QString(data.name.c_str()));
			text->setPos(i * DISPLACEMENT, 10);

			// draw lines from inner to outer
			scene->addLine(i * DISPLACEMENT, 300, i * DISPLACEMENT + sin(j * deg) * 250, 300 - cos(j * deg) * 250);

			// add text
			text = scene->addText(QString(this->m_MultiSet->variate(j).name.c_str()), QFont("Arial", 10));

			if ((deg * j) > PI)
				text->setPos(i * DISPLACEMENT + sin(j * deg) * 350, 300 - cos(j * deg) * 250);
			else
				text->setPos(i * DISPLACEMENT + sin(j * deg) * 250, 300 - cos(j * deg) * 250);

			// draw spiderlines
			scene->addLine(i * DISPLACEMENT + sin(j * deg) * 50, 300 - cos(j * deg) * 50, i * DISPLACEMENT + sin((j + 1) * deg) * 50, 300 - cos((j + 1) * deg) * 50);
			scene->addLine(i * DISPLACEMENT + sin(j * deg) * 100, 300 - cos(j * deg) * 100, i * DISPLACEMENT + sin((j + 1) * deg) * 100, 300 - cos((j + 1) * deg) * 100);
			scene->addLine(i * DISPLACEMENT + sin(j * deg) * 150, 300 - cos(j * deg) * 150, i * DISPLACEMENT + sin((j + 1) * deg) * 150, 300 - cos((j + 1) * deg) * 150);
			scene->addLine(i * DISPLACEMENT + sin(j * deg) * 200, 300 - cos(j * deg) * 200, i * DISPLACEMENT + sin((j + 1) * deg) * 200, 300 - cos((j + 1) * deg) * 200);

			// draw datalines
			auto value1 = data.values[j];
			int ix;
			if ((j + 1) >= m_MultiSet->dimensions())
				ix = 0;
			else
				ix = j + 1;
			auto value2 = data.values[ix];

			//normalize
			value1 = (value1 - m_MultiSet->variate(j).min) / (m_MultiSet->variate(j).max - m_MultiSet->variate(j).min);
			value2 = (value2 - m_MultiSet->variate(ix).min) / (m_MultiSet->variate(ix).max - m_MultiSet->variate(ix).min);

			scene->addLine(i * DISPLACEMENT + sin(j * deg) * 200 * value1, 300 - cos(j * deg) * 200 * value1, i * DISPLACEMENT + sin((j + 1) * deg) * 200 * value2, 300 - cos((j + 1) * deg) * 200 * value2, QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
		}
	}

	m_Ui->progressBar->setValue(0);
	m_Ui->progressBar->setEnabled(false);
}

void MainWindow::drawChernoff()
{
	if (m_MultiSet == nullptr || m_MultiSet->size() == 0)
	{
		std::cout << "No data found." << std::endl;
		return;
	}

	std::cout << "Drawing Chernoff Faces." << std::endl;

	scene->clear();
	scene->setSceneRect(0, 0, WIDTH, HEIGHT);
	scene->sceneRect();
	scene->setSceneRect(QRectF());
	scene->sceneRect();
	//scene->sceneChanged();
	//scene->updateScene();

	//scene->addRect(0, 0, WIDTH, HEIGHT, QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap));

	QPen bluePen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap);
	QBrush cyanBrush(Qt::cyan);
	QBrush redBrush(Qt::red);
	m_Ui->progressBar->setEnabled(true);
	m_Ui->progressBar->setRange(0, m_MultiSet->size());
	for (int i = 0; i < m_MultiSet->size(); i++)
	{
		m_Ui->progressBar->setValue(i+1);

		auto data = m_MultiSet->element(i);

		auto text = scene->addText(QString(data.name.c_str()));
		text->setPos(i * 300 + 100, 500);

		// draw head, 0. dimension = width, 1. dimension = height
		float height, width;

		if (m_MultiSet->dimensions() > 1)
		{
			auto value = data.values[1];
			auto value2 = data.values[0];
			//normalize
			value = (value - m_MultiSet->variate(1).min) / (m_MultiSet->variate(1).max - m_MultiSet->variate(1).min);
			value2 = (value2 - m_MultiSet->variate(0).min) / (m_MultiSet->variate(0).max - m_MultiSet->variate(0).min);

			height = value * float(200) + float(200);
			width = value2 * float(80) + float(200);
			scene->addEllipse(QRect(i * 300, 100, width, height), bluePen);
		}

		// draw left eye
		if (m_MultiSet->dimensions() > 2)
		{
			auto value = data.values[2];
			//normalize
			value = (value - m_MultiSet->variate(2).min) / (m_MultiSet->variate(2).max - m_MultiSet->variate(2).min);

			auto e = new QGraphicsEllipseItem(QRect(i * 300 + 50, 150, value * float(50) + float(10), value * float(50) + float(10)));
			e->setBrush(cyanBrush);
			e->setPen(bluePen);
			scene->addItem(e);
		}

		// draw right eye
		if (m_MultiSet->dimensions() > 3)
		{
			auto value = data.values[3];
			//normalize
			value = (value - m_MultiSet->variate(3).min) / (m_MultiSet->variate(3).max - m_MultiSet->variate(3).min);

			auto e = new QGraphicsEllipseItem(QRect(i * 300 + 150, 150, value * float(50) + float(10), value * float(50) + float(10)));
			e->setBrush(cyanBrush);
			e->setPen(bluePen);
			scene->addItem(e);
		}

		// draw mouth
		if (m_MultiSet->dimensions() > 4)
		{
			auto value = data.values[4];
			//normalize
			value = (value - m_MultiSet->variate(4).min) / (m_MultiSet->variate(4).max - m_MultiSet->variate(4).min);

			auto e = new QGraphicsRectItem(QRect(i * 300 + 100, 150 + height / 2, value * float(80) + float(20), value * float(40) + float(10)));
			e->setBrush(redBrush);
			e->setPen(bluePen);
			scene->addItem(e);
		}

		// draw nose
		if (m_MultiSet->dimensions() > 5)
		{
			auto value = data.values[5];
			//normalize
			value = (value - m_MultiSet->variate(5).min) / (m_MultiSet->variate(5).max - m_MultiSet->variate(5).min);

			scene->addEllipse(QRect(i * 300 + 20 + width / 2, 80 + height / 2, value * float(30) + float(10), value * float(40) + float(20)), bluePen);
		}

		// draw left ear
		if (m_MultiSet->dimensions() > 6)
		{
			auto value = data.values[6];
			//normalize
			value = (value - m_MultiSet->variate(6).min) / (m_MultiSet->variate(6).max - m_MultiSet->variate(6).min);

			scene->addEllipse(QRect(i * 300, 80 + height / 2, value * float(20) + float(5), value * float(40) + float(5)), bluePen);
		}

		// draw right ear
		if (m_MultiSet->dimensions() > 7)
		{
			auto value = data.values[7];
			//normalize
			value = (value - m_MultiSet->variate(7).min) / (m_MultiSet->variate(7).max - m_MultiSet->variate(7).min);

			scene->addEllipse(QRect(i * 300 + width, 80 + height / 2, value * float(20) + float(5), value * float(40) + float(5)), bluePen);
		}

		// draw hair 1
		if (m_MultiSet->dimensions() > 8)
		{
			auto value = data.values[8];
			//normalize
			value = (value - m_MultiSet->variate(8).min) / (m_MultiSet->variate(8).max - m_MultiSet->variate(8).min);

			scene->addLine(i * 300 + width / 2, 100, i * 300 + width / 2 - value * float(100), value * float(70), bluePen);
		}

		// draw hair 2
		if (m_MultiSet->dimensions() > 9)
		{
			auto value = data.values[9];
			//normalize
			value = (value - m_MultiSet->variate(9).min) / (m_MultiSet->variate(9).max - m_MultiSet->variate(9).min);

			scene->addLine(i * 300 + width / 2, 100, i * 300 + width / 2 + value * float(100), value * float(70), bluePen);
		}

		// draw hair 3 or left eyebrow
		if (m_MultiSet->dimensions() > 10)
		{
			auto value = data.values[10];
			//normalize
			value = (value - m_MultiSet->variate(10).min) / (m_MultiSet->variate(10).max - m_MultiSet->variate(10).min);

			scene->addLine(i * 300 + width / 2 - 40, 140, i * 300 + width / 2 - 40 - value * float(80), value * float(80) + 50, bluePen);
		}

		// draw hair 4 or right eyebrow
		if (m_MultiSet->dimensions() > 11)
		{
			auto value = data.values[11];
			//normalize
			value = (value - m_MultiSet->variate(11).min) / (m_MultiSet->variate(11).max - m_MultiSet->variate(11).min);

			scene->addLine(i * 300 + width / 2 + 40, 140, i * 300 + width / 2 + 40 + value * float(80), value * float(80) + 50, bluePen);
		}

		// draw hair 5 or right eyebrow 2
		if (m_MultiSet->dimensions() > 12)
		{
			auto value = data.values[12];
			//normalize
			value = (value - m_MultiSet->variate(12).min) / (m_MultiSet->variate(12).max - m_MultiSet->variate(12).min);

			scene->addLine(i * 300 + width / 2 + 40, 140, i * 300 + width / 2 + 40 + value * float(80), value * float(80) + 50, bluePen);
		}

		// draw hair 6 or left eyebrow 2
		if (m_MultiSet->dimensions() > 13)
		{
			auto value = data.values[13];
			//normalize
			value = (value - m_MultiSet->variate(13).min) / (m_MultiSet->variate(13).max - m_MultiSet->variate(13).min);

			scene->addLine(i * 300 + width / 2 - 40, 140, i * 300 + width / 2 - 40 - value * float(80), value * float(80) + 50, bluePen);
		}


		// draw hair 7
		if (m_MultiSet->dimensions() > 14)
		{
			auto value = data.values[14];
			//normalize
			value = (value - m_MultiSet->variate(14).min) / (m_MultiSet->variate(14).max - m_MultiSet->variate(14).min);

			scene->addLine(i * 300 + width / 2 - 20, 102, i * 300 + width / 2 - 20 - value * float(100), value * float(70), bluePen);
		}

		// draw hair 8
		if (m_MultiSet->dimensions() > 15)
		{
			auto value = data.values[15];
			//normalize
			value = (value - m_MultiSet->variate(15).min) / (m_MultiSet->variate(15).max - m_MultiSet->variate(15).min);

			scene->addLine(i * 300 + width / 2 + 20, 102, i * 300 + width / 2 + 20 + value * float(100), value * float(70), bluePen);
		}
	}

	m_Ui->progressBar->setEnabled(false);
	m_Ui->progressBar->setValue(0);
}

void MainWindow::drawMultiSet()
{
	if (m_MultiSet == nullptr || m_MultiSet->size() == 0)
	{
		std::cout << "No data found." << std::endl;
		return;
	}

	scene->clear();
	scene->setSceneRect(0, 0, WIDTH, HEIGHT);
	//scene->sceneChanged();
	//scene->updateScene();

	int PRECISION = 2;

	std::cout << "Drawing MultiSet." << std::endl;
		

	//scene->addRect(0, 0, WIDTH, HEIGHT, QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap));

	QPen blackPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap);

	
	int steps = m_MultiSet->dimensions() / 7 + 1;

	int y_axis_start = 25 + 10 * steps;
	int y_axis_height = (HEIGHT - 20) - y_axis_start;

	// Coordinates of the dimensions
	if (dimOrder == NULL) {
		axis_x_coords = new int[m_MultiSet->dimensions()];
		dimOrder = new int[this->m_MultiSet->dimensions()];
		dimInvers = new bool[this->m_MultiSet->dimensions()];
		for (int i = 0; i < this->m_MultiSet->dimensions(); i++)
		{
			axis_x_coords[i] = (int)((((float)WIDTH) / ((float)this->m_MultiSet->dimensions() + 1))*((float)i + 1));
			dimOrder[i] = i;
			dimInvers[i] = false;
		}
	}
	QFontMetrics fm(QFont("Arial", 10));
	
	std::cout << "Dimensions: " <<this->m_MultiSet->dimensions() << "\n";
	// Drawing
	MyLine **firstlines = new MyLine*[m_MultiSet->size()];
	MyLine **previouslines = new MyLine*[m_MultiSet->size()];
	m_Ui->progressBar->setEnabled(true);
	m_Ui->progressBar->setRange(0, m_MultiSet->dimensions());
	for (int k = 0; k < this->m_MultiSet->dimensions(); k++)
	{
		m_Ui->progressBar->setValue(k+1);
		int id1 = dimOrder[k]; // id1 is the dimension in the right order. must be used everywhere except axis_x_coords, use k here.

		// add dimension-lines
		scene->addLine(axis_x_coords[id1], y_axis_start, axis_x_coords[id1], HEIGHT - 20);

		// add dimension-Label
		MyLabel *label = new MyLabel(m_MultiSet, scene, id1);
		label->setText(QString(this->m_MultiSet->variate(id1).name.c_str()));
		label->setPos(axis_x_coords[k] - fm.width(label->text()) / 2, (5 + (k%steps) * 16));
		label->setFlag(QGraphicsItem::ItemIsMovable);
		label->setFlag(QGraphicsItem::ItemIsSelectable);
		label->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
		label->setAcceptDrops(true);
		scene->addItem(label);

		/*auto text = scene->addText(QString(this->m_MultiSet->variate(i).name.c_str()), QFont("Arial", 10));

		text->setPos(axis_x_coords[i] - axis_x_coords[0] / 2, (10 + (i%steps) * 10));
		text->setFlag(QGraphicsItem::ItemIsSelectable);*/

		/*
		std::stringstream temp;
		temp << std::fixed << std::setprecision(PRECISION) << this->m_MultiSet->variate(id1).max;
		QGraphicsSimpleTextItem *txtItem = scene->addSimpleText(QString(temp.str().c_str()), font);
		txtItem->setPos(axis_x_coords[k] - axis_x_coords[0] / 2, 20 + steps * 10);

		temp.str("");
		temp << std::fixed << std::setprecision(PRECISION) << this->m_MultiSet->variate(id1).min;
		txtItem = scene->addSimpleText(QString(temp.str().c_str()), font);
		txtItem->setPos(axis_x_coords[k] - axis_x_coords[0] / 2, HEIGHT - 15);
*/


		// draw lines between axises if not the last axis
		

		if (k + 1 < this->m_MultiSet->dimensions())
		{
			// id2 is the next dimension after id1
			int id2 = dimOrder[k + 1];

			for (int j = 0; j < m_MultiSet->size(); j++)
			{
				auto these = m_MultiSet->variate(id1);
				auto those = m_MultiSet->variate(id2);

				float here = m_MultiSet->element(j).values[id1];
				float there = m_MultiSet->element(j).values[id2];

				// normalize
				here = (here - these.min) / (these.max - these.min);
				there = (there - those.min) / (those.max - those.min);

				// calculate y axis values
				if (dimInvers[id1] == true)
					here = 1 - here;

				if (dimInvers[id2] == true)
					there = 1 - there;

				// calculate y axis values
				here = (HEIGHT - 20) - here * y_axis_height;
				there = (HEIGHT - 20) - there * y_axis_height;

				auto line = new MyLine(axis_x_coords[k], here, axis_x_coords[k + 1], there, QString(m_MultiSet->element(j).name.c_str()));
				//line->setFlag(QGraphicsItem::ItemIsFocusable);
				//line->acceptHoverEvents();
				scene->addItem(line);

				if (k == 0)
					firstlines[j] = line;
				else
					previouslines[j]->next = line;
				
				if (k >= m_MultiSet->dimensions() - 2)
					line->next = firstlines[j];

				previouslines[j] = line;
				//auto line = scene->addLine(axis_x_coords[i], here, axis_x_coords[i + 1], there);
				//line->setFlag(QGraphicsItem::ItemIsSelectable);
			}
		}
	}
	m_Ui->progressBar->setEnabled(false);
	m_Ui->progressBar->setValue(0);
}


/*
void MainWindow::drawMultiSet()
{
int PRECISION = 2;

int WIDTH, HEIGHT;

std::cout << "Drawing MultiSet." << std::endl;

QLabel *drawingArea = m_Ui->labelBottom;
WIDTH = drawingArea->width();
HEIGHT = drawingArea->height();

drawingArea->setStyleSheet("background-color:white;");
QPicture pi;
QPainter p(&pi);

p.setRenderHint(QPainter::Antialiasing);

p.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap));
p.drawRect(0, 0, WIDTH, HEIGHT);

p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

int *axis_x_coords = new int[m_MultiSet->dimensions()];
int steps = m_MultiSet->dimensions() / 7 + 1;

int y_axis_start = 25 + 10 * steps;
int y_axis_height = (HEIGHT - 20) - y_axis_start;

for (int i = 0; i < this->m_MultiSet->dimensions(); i++)
{
axis_x_coords[i] = (int)((((float)WIDTH) / ((float)this->m_MultiSet->dimensions() + 1))*((float)i + 1));
}

for (int i = 0; i < this->m_MultiSet->dimensions(); i++)
{
//std::cout << this->m_MultiSet->variate(i).name << ", min: " << this->m_MultiSet->variate(i).min << " max: " << this->m_MultiSet->variate(i).max << std::endl;

p.drawLine(axis_x_coords[i], y_axis_start, axis_x_coords[i], HEIGHT - 20);

//p.drawText(axis_x_coords[i] - axis_x_coords[0] / 2, (10 + (i%steps) * 10), axis_x_coords[0], 10, Qt::AlignCenter, QString(this->m_MultiSet->variate(i).name.c_str()));

p.drawText(axis_x_coords[i] - axis_x_coords[0] / 2, (10 + (i%steps)*10), QString(this->m_MultiSet->variate(i).name.c_str()));

std::stringstream temp;
temp << std::fixed << std::setprecision(PRECISION) << this->m_MultiSet->variate(i).max;
p.drawText(axis_x_coords[i] - axis_x_coords[0] / 2, 20 + steps * 10, QString(temp.str().c_str()));

temp.str("");
temp << std::fixed << std::setprecision(PRECISION) << this->m_MultiSet->variate(i).min;
p.drawText(axis_x_coords[i] - axis_x_coords[0] / 2, HEIGHT - 5, QString(temp.str().c_str()));

// draw lines between axises if not the last axis
if (i + 1 < this->m_MultiSet->dimensions())
{
for (int j = 0; j < m_MultiSet->size(); j++)
{
auto these = m_MultiSet->variate(i);
auto those = m_MultiSet->variate(i + 1);

float here = m_MultiSet->element(j).values[i];
float there = m_MultiSet->element(j).values[i + 1];

// normalize
here = (here - these.min) / (these.max - these.min);
there = (there - those.min) / (those.max - those.min);

// calculate y axis values
here = (HEIGHT - 20) - here * y_axis_height;
there = (HEIGHT - 20) - there * y_axis_height;

p.drawLine(axis_x_coords[i], here, axis_x_coords[i+1], there);
}
}
}



p.end();

drawingArea->setPicture(pi);
}*/

//-------------------------------------------------------------------------------------------------
// Slots
//-------------------------------------------------------------------------------------------------

void MainWindow::openFileAction()
{
	QString filename = QFileDialog::getOpenFileName(this, "Data File", 0, tr("Data Files (*.dat *.gri *.csv)"));

	if (!filename.isEmpty())
	{
		// store filename
		m_FileType.filename = filename;
		std::string fn = filename.toStdString();
		bool success = false;

		// progress bar and top label
		m_Ui->progressBar->setEnabled(true);
		m_Ui->labelTop->setText("Loading data ...");

		// load data according to file extension
		if (fn.substr(fn.find_last_of(".") + 1) == "dat")		// LOAD VOLUME
		{
			// create VOLUME
			m_FileType.type = VOLUME;
			m_Volume = new Volume();

			// load file
			success = m_Volume->loadFromFile(filename, m_Ui->progressBar);
		}
		else if (fn.substr(fn.find_last_of(".") + 1) == "gri")		// LOAD VECTORFIELD
		{
			// create VECTORFIELD
			m_FileType.type = VECTORFIELD;
			m_VectorField = new VectorField();

			// load file
			success = m_VectorField->loadFromFile(filename, m_Ui->progressBar);
		}
		else if (fn.substr(fn.find_last_of(".") + 1) == "csv")		// LOAD MULTIVARIATE DATA
		{
			// create MULTIVARIATE
			m_FileType.type = MULTIVARIATE;
			m_MultiSet = new MultiSet();

			// load file
			success = m_MultiSet->loadFromFile(filename, m_Ui->progressBar);
		}

		m_Ui->progressBar->setEnabled(false);

		// status message
		if (success)
		{
			QString type;
			if (m_FileType.type == VOLUME) type = "VOLUME";
			else if (m_FileType.type == VECTORFIELD) type = "VECTORFIELD";
			else if (m_FileType.type == MULTIVARIATE) type = "MULTIVARIATE";
			m_Ui->labelTop->setText("File LOADED [" + filename + "] - Type [" + type + "]");

			if (m_FileType.type == MULTIVARIATE)
			{
				delete[] dimOrder;
				dimOrder = NULL;
				this->drawMultiSet();
			}
		}
		else
		{
			m_Ui->labelTop->setText("ERROR loading file " + filename + "!");
			m_Ui->progressBar->setValue(0);
		}
	}
}

void MainWindow::closeAction()
{
	close();
}

void MainWindow::changeDim(int id1, int id2)
{
	int pos1, pos2;
	std::cout << "redraw()\n";
	for (int i = 0; i < this->m_MultiSet->dimensions(); i++) {
		if (dimOrder[i] == id1)
			pos1 = i;
		if (dimOrder[i] == id2)
			pos2 = i;
	}
	dimOrder[pos1] = id2;
	dimOrder[pos2] = id1;
	drawMultiSet();
}

void MainWindow::changeAxis(int id)
{
	std::cout << "inversing axis\n";
	int pos;
	for (int i = 0; i < this->m_MultiSet->dimensions(); i++) {
		if (dimOrder[i] == id)
			pos = i;
	}

	dimInvers[pos] = !dimInvers[pos];
	drawMultiSet();
}

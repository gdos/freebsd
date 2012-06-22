#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

#include <pcap.h>

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_histogram.h"
#include "qwt_symbol.h"

#include "libradarpkt/pkt.h"
#include "PktLogData.h"
#include "PktLogDataRadiotap.h"

#include "libradarpkt/ar5416_radar.h"
#include "libradarpkt/ar9280_radar.h"

/*
 * XXX eww, using pointers rather than references.
 */
void
plotSet(QwtPlot *p, PktLogData *pl)
{
	QwtPlotCurve *c = new QwtPlotCurve("curve");
	QwtSymbol *s = new QwtSymbol();
	std::vector<double> dur;
	std::vector<double> rssi;

	// The default is a single 1 pixel dot.
	// This makes it very difficult to see.
	s->setStyle(QwtSymbol::Cross);
	s->setSize(2, 2);

	p->setTitle("Example");

	//p->setAutoLegend(true);
	//p->setLegendPos(Qwt::Bottom);

	// Curve Plot - dots, == scatterplot
	c->setStyle(QwtPlotCurve::Dots);
	// And set the symbol type, a default dot is not really
	// all that helpful.
	c->setSymbol(s);

	/* Load in values */
	dur = pl->GetDuration();
	rssi = pl->GetRssi();
//	for (int i = 0; i < dur.size(); i++)
//		printf("%d: dur=%f, rssi=%f\n", i, dur[i], rssi[i]);

	printf("dur size=%d, rssi size=%d\n", dur.size(), rssi.size());

	// Plot them
	c->setSamples(&dur[0], &rssi[0], dur.size());
	c->attach(p);

	/* Plot */
	p->replot();
	p->show();
}

static void
usage()
{
	printf("usage: <ar5416|ar9280> <pcap>\n");
	exit(127);
}

int
main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PktLogDataRadiotap pr;
	int type = 0;

	if (argc < 3)
		usage();

	if (strcmp(argv[1], "ar5416")== 0)
		type = CHIP_AR5416;
	else if (strcmp(argv[1], "ar9280")== 0)
		type = CHIP_AR9280;
	else
		usage();

	pr.LoadPcapOffline(argv[2], type);

	QwtPlot plot(QwtText("example"));

	// Default size
	plot.setGeometry(0, 0, 640, 400);

	// Scale
	plot.setAxisScale(QwtPlot::xBottom, 0.0, 256.0);

	plotSet(&plot, &pr);

	return a.exec();
}

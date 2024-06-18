#include <QApplication>
#include <QIcon>
#include <QPrinter>
#include <QTextDocument>
#include <QDir>
#include <QPalette>

void printQIcons()
{
    qInfo() << "Preparing QIcons...";

    QString html = QString("<html><body><h1>QIcons for Qt %1 on %2</h1><br/><table>").arg(qVersion(), QSysInfo().prettyProductName());

    int errors = 0;
    int columnCount = 7;

    for (int i = 0; QIcon::ThemeIcon(i) != QIcon::ThemeIcon::NThemeIcons; i++)
    {
        QString filename = QDir().tempPath() + "/QIconToPdf-" + QString::number(i) + ".png";
        QImage image = QIcon::fromTheme(QIcon::ThemeIcon(i)).pixmap(32).toImage();

        image.invertPixels();

        if (!image.save(filename)) errors++;
        html += QString("<td style='vertical-align: middle; margin-right: 15px;'><img src='%3' width='32'>&nbsp;&nbsp;%1 | %2</td>").arg(QString::number(i), QIcon::fromTheme(QIcon::ThemeIcon(i)).name(), filename);

        if (!((i + 1) % columnCount)) html += "</tr><tr>";
    }

    html += QString("</tr></table></body><br/><p><i>%1 Icons failed.</i></p></html>").arg(errors);

    QString filename = QCoreApplication::applicationDirPath() + QString("/QIcons_%1_%2_%3.pdf").arg(QString(qVersion()).replace(".", "_"), QSysInfo().productType(), QSysInfo().productVersion());

    if (QFile(filename).exists()) QFile(filename).remove();

    qInfo() << "Start printing...";
    QTextDocument document;
    document.setHtml(html);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A3);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(filename);
    document.print(&printer);

    qInfo().noquote() << "Printing finished. Output:" << filename;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setPalette(QPalette(QPalette::Light));

    printQIcons();

    return a.exec();
}

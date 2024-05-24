#include <QApplication>
#include <QIcon>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QTemporaryFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString html = "<html><body><table>";

    int tableIndex = 0;

    QStringList errors;

    int columnCount = 6;

    for (int i = 0; QIcon::ThemeIcon(i) != QIcon::ThemeIcon::NThemeIcons; i++)
    {

        QString filename = QDir().tempPath() + "/QIconToPdf-" + QString::number(i) + ".png";
        QImage image = QIcon::fromTheme(QIcon::ThemeIcon(i)).pixmap(32).toImage();

        image.invertPixels();

        if (image.save(filename))
        {
            tableIndex++;
            html += QString("<td><img src=\"%3\" width='32'> (%1) %2</td>").arg(QString::number(i), QIcon::fromTheme(QIcon::ThemeIcon(i)).name(), filename);

            if (!(tableIndex % columnCount)) html += "</tr><tr>";
        }
        else errors << QString::number(i);
    }

    html += QString("</tr></table></body><p><i>Can't print the follwing icons: %1</i></p></html>").arg(errors.join(", "));

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A3);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(QCoreApplication::applicationDirPath() + "/output_" + QDateTime().currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf");

    document.print(&printer);

    return a.exec();
}

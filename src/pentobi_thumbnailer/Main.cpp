//-----------------------------------------------------------------------------
/** @file pentobi_thumbnailer/Main.cpp
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <QCoreApplication>
#include <QImage>
#include <QImageWriter>
#include <QString>
#include "libboardgame_util/Exception.h"
#include "libboardgame_util/Options.h"
#include "libpentobi_thumbnail/CreateThumbnail.h"

using namespace std;
using libboardgame_util::Exception;
using libboardgame_util::Options;

//-----------------------------------------------------------------------------

namespace {

void mainFunction(int argc, char* argv[])
{
    vector<string> specs = { "size|s:" };
    Options opt(argc, argv, specs);
    auto size = opt.get<int>("size", 128);
    auto& files = opt.get_args();
    if (size <= 0)
        throw Exception("Invalid image size");
    if (files.size() > 2)
        throw Exception("Too many file arguments");
    if (files.size() < 2)
        throw Exception("Need input and output file argument");
    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    if (! createThumbnail(QString::fromLocal8Bit(files[0].c_str()), size, size,
                          image))
        throw Exception("Thumbnail generation failed");
    QImageWriter writer(QString::fromLocal8Bit(files[1].c_str()), "png");
    if (! writer.write(image))
        throw Exception(writer.errorString().toLocal8Bit().constData());
}

} //namespace

//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    try
    {
        mainFunction(argc, argv);
    }
    catch (const exception& e)
    {
        cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

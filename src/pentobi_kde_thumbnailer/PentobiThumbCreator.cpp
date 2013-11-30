//-----------------------------------------------------------------------------
/** @file pentobi_kde_thumbnailer/PentobiThumbCreator.cpp
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#include "PentobiThumbCreator.h"

#include <QImage>
#include "libpentobi_thumbnail/CreateThumbnail.h"

using namespace std;

//-----------------------------------------------------------------------------

extern "C"
{
    KDE_EXPORT ThumbCreator* new_creator()
    {
        return new PentobiThumbCreator;
    }
}

//-----------------------------------------------------------------------------

PentobiThumbCreator::~PentobiThumbCreator()
{
}

bool PentobiThumbCreator::create(const QString& path, int width, int height,
                                 QImage& image)
{
    image = QImage(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    return createThumbnail(path, width, height, image);
}

//-----------------------------------------------------------------------------

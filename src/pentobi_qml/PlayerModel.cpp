//-----------------------------------------------------------------------------
/** @file pentobi_qml/PlayerModel.cpp
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#include "PlayerModel.h"

#include <QElapsedTimer>
#include <QFile>
#include <QtConcurrentRun>

using namespace std;
using libboardgame_util::set_abort;

//-----------------------------------------------------------------------------

PlayerModel::PlayerModel(QObject* parent)
    : QObject(parent),
      m_isGenMoveRunning(false),
      m_level(1),
      m_genMoveId(0),
      m_player(BoardModel::getInitialGameVariant(), "")
{
    connect(&m_genMoveWatcher, SIGNAL(finished()), SLOT(genMoveFinished()));
}

PlayerModel::~PlayerModel()
{
    cancelGenMove();
}

PlayerModel::GenMoveResult PlayerModel::asyncGenMove(BoardModel* bm,
                                                     unsigned genMoveId)
{
    QElapsedTimer timer;
    timer.start();
    auto& bd = bm->getBoard();
    GenMoveResult result;
    result.genMoveId = genMoveId;
    result.boardModel = bm;
    result.move = m_player.genmove(bd, bd.get_effective_to_play());
    auto elapsed = timer.elapsed();
    // Enforce minimum thinking time of 0.8 sec
    if (elapsed < 800)
        QThread::msleep(800 - elapsed);
    return result;
}

void PlayerModel::cancelGenMove()
{
    if (! m_isGenMoveRunning)
        return;
    // After waitForFinished() returns, we can be sure that the move generation
    // is no longer running, but we will still receive the finished event.
    // Increasing m_genMoveId will make genMoveFinished() ignore the event.
    ++m_genMoveId;
    set_abort();
    m_genMoveWatcher.waitForFinished();
    setIsGenMoveRunning(false);
}

void PlayerModel::genMoveFinished()
{
    auto result = m_genMoveWatcher.future().result();
    if (result.genMoveId != m_genMoveId)
        // Callback from a canceled move generation
        return;
    setIsGenMoveRunning(false);
    auto& bd = result.boardModel->getBoard();
    auto mv = result.move;
    if (mv.is_pass())
    {
        qWarning("PlayerModel: no more moves");
        return;
    }
    if (mv.is_null())
    {
        qWarning("PlayerModel: failed to generate move");
        return;
    }
    Color c = bd.get_effective_to_play();
    if (! bd.is_legal(c, mv))
    {
        qWarning("PlayerModel: player generated illegal move");
        return;
    }
    emit moveGenerated(mv.to_int());
}

int PlayerModel::isGenMoveRunning() const
{
    return m_isGenMoveRunning;
}

int PlayerModel::level() const
{
    return m_level;
}

void PlayerModel::loadBook(Variant variant)
{
    QFile file(QString(":/pentobi_books/book_%1.blksgf")
               .arg(to_string_id(variant)));
    if (! file.open(QIODevice::ReadOnly))
    {
        qWarning() << "PlayerModel: could not open " << file.fileName();
        return;
    }
    QTextStream stream(&file);
    QString text = stream.readAll();
    istringstream in(text.toLocal8Bit().constData());
    m_player.load_book(in);
}

void PlayerModel::setIsGenMoveRunning(bool isGenMoveRunning)
{
    if (m_isGenMoveRunning == isGenMoveRunning)
        return;
    m_isGenMoveRunning = isGenMoveRunning;
    emit isGenMoveRunningChanged(isGenMoveRunning);
}

void PlayerModel::startGenMove(BoardModel* boardModel)
{
    startGenMoveAtLevel(boardModel, m_level);
}

void PlayerModel::startGenMoveAtLevel(BoardModel* boardModel, int level)
{
    cancelGenMove();
    m_player.set_level(level);
    auto variant = boardModel->getBoard().get_variant();
    if (! m_player.is_book_loaded(variant))
        loadBook(variant);
    ++m_genMoveId;
    QFuture<GenMoveResult> future =
            QtConcurrent::run(this, &PlayerModel::asyncGenMove, boardModel,
                              m_genMoveId);
    m_genMoveWatcher.setFuture(future);
    setIsGenMoveRunning(true);
}

void PlayerModel::setLevel(int level)
{
    if (m_level == level)
        return;
    m_level = level;
    levelChanged(m_level);
}

//-----------------------------------------------------------------------------
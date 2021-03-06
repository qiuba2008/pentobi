//-----------------------------------------------------------------------------
/** @file pentobi/RatingHistory.h
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifndef PENTOBI_RATING_HISTORY_H
#define PENTOBI_RATING_HISTORY_H

#include <vector>
#include <QString>
#include "libboardgame_base/Rating.h"
#include "libpentobi_base/Color.h"
#include "libpentobi_base/PentobiTree.h"
#include "libpentobi_base/Variant.h"

using namespace std;
using libboardgame_base::Rating;
using libpentobi_base::Color;
using libpentobi_base::PentobiTree;
using libpentobi_base::Variant;

//-----------------------------------------------------------------------------

/** History of rated games in a certain game variant. */
class RatingHistory
{
public:
    /** Maximum number of games to remember in the history. */
    static const unsigned maxGames = 100;

    struct GameInfo
    {
        /** Game number.
            The first game played has number 0. */
        unsigned number;

        /** Color played by the human.
            In game variants with multiple colors per player, the human played
            all colors played by the player of this color. */
        Color color;

        /** Game result.
            0=Loss, 0.5=tie, 1=win from the viewpoint of the human. */
        float result;

        /** Date of the game in "YYYY-MM-DD" format. */
        QString date;

        /** The playing level of the computer opponent. */
        int level;

        /** The rating of the human after the game. */
        Rating rating;
    };


    explicit RatingHistory(Variant variant);

    /** Initialize rating to a given a-priori value. */
    void init(Rating rating);

    /** Get level and user color for next rated games.
        @param maxLevel The maximum playing level.
        @param random A random number to determine the color for the human.
        @param[out] level The playing level for the next game.
        @param[out] color The color for the human in the next game. */
    void getNextRatedGameSettings(int maxLevel, unsigned random, int& level,
                                  Color& color);

    /** Append a new game. */
    void addGame(float score, Rating opponentRating, unsigned nuOpponents,
                 Color color, float result, const QString& date, int level,
                 const PentobiTree& tree);

    /** Get file name of the n'th rated game. */
    QString getFile(unsigned n) const;

    void load(Variant variant);

    /** Saves the history. */
    void save() const;

    const vector<GameInfo>& getGameInfos() const;

    Variant getVariant() const;

    const Rating& getRating() const;

    const Rating& getBestRating() const;

    unsigned getNuGames() const;

    void clear();

private:
    Variant m_variant;

    Rating m_rating;

    unsigned m_nuGames;

    Rating m_bestRating;

    QString m_dir;

    QString m_file;

    vector<GameInfo> m_games;
};

inline const vector<RatingHistory::GameInfo>& RatingHistory::getGameInfos()
    const
{
    return m_games;
}

inline unsigned RatingHistory::getNuGames() const
{
    return m_nuGames;
}

inline const Rating& RatingHistory::getBestRating() const
{
    return m_bestRating;
}

inline const Rating& RatingHistory::getRating() const
{
    return m_rating;
}

inline Variant RatingHistory::getVariant() const
{
    return m_variant;
}

//-----------------------------------------------------------------------------

#endif // PENTOBI_RATING_HISTORY_H

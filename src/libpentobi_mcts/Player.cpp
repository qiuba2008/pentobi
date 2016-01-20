//-----------------------------------------------------------------------------
/** @file libpentobi_mcts/Player.cpp
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Player.h"

#include <fstream>
#include <iomanip>
#include "libboardgame_util/CpuTimeSource.h"
#include "libboardgame_util/WallTimeSource.h"
#include "libboardgame_sys/Memory.h"

namespace libpentobi_mcts {

using libboardgame_util::CpuTimeSource;
using libboardgame_util::WallTimeSource;
using libpentobi_base::BoardType;

//-----------------------------------------------------------------------------

namespace {

// Rationale for choosing the number of simulations:
// * The number at level 1 is very small in Classic/Duo to avoid that level 1
//   is too strong for absolute beginners. (Searches with such a small number
//   of simulations still produce reasonable moves because of the prior
//   initialization of node values.) In Trigon, it starts with a higher number
//   because the playing strength is weaker there.
// * The number at the highest level is chosen such that the average time per
//   game and player is 120s in Duo/Callisto2, 240s in Classic, 300s in Trigon,
//   330s in Nexos on an Intel i3-4130.
// * The numbers most other levels are chosen such that they roughly correspond
//   to equal Elo differences in self-play experiments.
// * However, the counts for level 7 are based on time measurments again
//   because it is the maximum level supported on Android and therefore should
//   be as strong as possible without taking too much time on typical mobile
//   hardware. Its target is to take 2% of the time of level 9. Level 8 is
//   adjusted to be about in the middle between level 7 and 9 (on a log scale).

static const float counts_classic[Player::max_supported_level] =
    { 3, 24, 87, 213, 667, 1989, 36000, 250000, 1829033 };

static const float counts_duo[Player::max_supported_level] =
    { 3, 17, 44, 123, 426, 1672, 120000, 840000, 6929504 };

static const float counts_trigon[Player::max_supported_level] =
    { 228, 433, 727, 1501, 2912, 7395, 10000, 70000, 489723 };

static const float counts_nexos[Player::max_supported_level] =
    { 100, 470, 800, 1650, 3300, 6000, 11000, 77000, 579663 };

static const float counts_callisto_2[Player::max_supported_level] =
    { 3, 24, 87, 213, 800, 1989, 48000, 336000, 2409112 };

} // namespace

//-----------------------------------------------------------------------------

Player::Player(Variant initial_variant, unsigned max_level, string  books_dir,
               unsigned nu_threads)
    : m_is_book_loaded(false),
      m_use_book(true),
      m_resign(false),
      m_books_dir(move(books_dir)),
      m_max_level(max_level),
      m_level(4),
      m_fixed_simulations(0),
      m_resign_threshold(0.09f),
      m_resign_min_simulations(500),
      m_search(initial_variant, nu_threads, get_memory()),
      m_book(initial_variant),
      m_time_source(new WallTimeSource)
{
    for (unsigned i = 0; i < Board::max_player_moves; ++i)
    {
        // Hand-tuned such that time per move is more evenly spread among all
        // moves than with a fixed number of simulations (because the
        // simulations per second increase rapidly with the move number) but
        // the average time per game is roughly the same.
        m_weight_max_count_duo[i] = 0.7f * exp(0.1f * static_cast<float>(i));
        m_weight_max_count_classic[i] = m_weight_max_count_duo[i];
        m_weight_max_count_trigon[i] = m_weight_max_count_duo[i];
        m_weight_max_count_callisto[i] = m_weight_max_count_duo[i];
        m_weight_max_count_callisto_2[i] = m_weight_max_count_duo[i];
        // Less weight for the first move(s) because number of legal moves
        // is lower and the search applies some pruning rules to reduce the
        // branching factor in early moves
        if (i == 0)
        {
            m_weight_max_count_classic[i] *= 0.2f;
            m_weight_max_count_trigon[i] *= 0.2f;
            m_weight_max_count_duo[i] *= 0.6f;
            m_weight_max_count_callisto[i] *= 0.2f;
            m_weight_max_count_callisto_2[i] *= 0.2f;
        }
        else if (i == 1)
        {
            m_weight_max_count_classic[i] *= 0.2f;
            m_weight_max_count_trigon[i] *= 0.5f;
            m_weight_max_count_callisto[i] *= 0.6f;
            m_weight_max_count_callisto_2[i] *= 0.2f;
        }
        else if (i == 2)
        {
            m_weight_max_count_classic[i] *= 0.3f;
            m_weight_max_count_trigon[i] *= 0.6f;
        }
        else if (i == 3)
        {
            m_weight_max_count_trigon[i] *= 0.8f;
        }
    }
}

Player::~Player() = default;

Move Player::genmove(const Board& bd, Color c)
{
    m_resign = false;
    if (! bd.has_moves(c))
        return Move::null();
    Move mv;
    auto variant = bd.get_variant();
    auto board_type = bd.get_board_type();
    auto level = min(max(m_level, 1u), m_max_level);
    // Don't use more than 2 moves per color from opening book in lower levels
    if (m_use_book
        && (level >= 4 || bd.get_nu_moves() < 2u * bd.get_nu_colors()))
    {
        if (! is_book_loaded(variant))
            load_book(m_books_dir
                      + "/book_" + to_string_id(variant) + ".blksgf");
        if (m_is_book_loaded)
        {
            mv = m_book.genmove(bd, c);
            if (! mv.is_null())
                return mv;
        }
    }
    Float max_count = 0;
    double max_time = 0;
    if (m_fixed_simulations > 0)
        max_count = m_fixed_simulations;
    else if (m_fixed_time > 0)
        max_time = m_fixed_time;
    else
    {
        switch (board_type)
        {
        case BoardType::classic:
            max_count = counts_classic[level - 1];
            break;
        case BoardType::duo:
            max_count = counts_duo[level - 1];
            break;
        case BoardType::trigon:
        case BoardType::trigon_3:
        case BoardType::callisto:
        case BoardType::callisto_3:
            max_count = counts_trigon[level - 1];
            break;
        case BoardType::nexos:
            max_count = counts_nexos[level - 1];
            break;
        case BoardType::callisto_2:
            max_count = counts_callisto_2[level - 1];
            break;
        }
        // Don't weight max_count in low levels, otherwise it is still too
        // strong for beginners (later in the game, the weight becomes much
        // greater than 1 because the simulations become very fast)
        bool weight_max_count = (level >= 4);
        if (weight_max_count)
        {
            auto player_move = bd.get_nu_onboard_pieces(c);
            float weight = 1; // Init to avoid compiler warning
            switch (board_type)
            {
            case BoardType::classic:
                weight = m_weight_max_count_classic[player_move];
                break;
            case BoardType::duo:
                weight = m_weight_max_count_duo[player_move];
                break;
            case BoardType::callisto:
            case BoardType::callisto_3:
                weight = m_weight_max_count_callisto[player_move];
                break;
            case BoardType::callisto_2:
                weight = m_weight_max_count_callisto_2[player_move];
                break;
            case BoardType::trigon:
            case BoardType::trigon_3:
            case BoardType::nexos:
                weight = m_weight_max_count_trigon[player_move];
                break;
            }
            max_count = ceil(max_count * weight);
        }
    }
    if (max_count != 0)
        LIBBOARDGAME_LOG("MaxCnt ", fixed, setprecision(0), max_count);
    else
        LIBBOARDGAME_LOG("MaxTime ", max_time);
    if (! m_search.search(mv, bd, c, max_count, 0, max_time, *m_time_source))
        return Move::null();
    // Resign only in two-player game variants
    if (get_nu_players(variant) == 2)
        if (m_search.get_root_visit_count() > m_resign_min_simulations
                && m_search.get_root_val().get_mean() < m_resign_threshold)
            m_resign = true;
    return mv;
}

/** Suggest how much memory to use for the trees depending on the maximum
    level used. */
size_t Player::get_memory()
{
    size_t available = libboardgame_sys::get_memory();
    if (available == 0)
    {
        LIBBOARDGAME_LOG("WARNING: could not determine system memory"
                         " (assuming 512MB)");
        available = 512000000;
    }
    // Don't use all of the available memory
#if PENTOBI_LOW_RESOURCES
    size_t reasonable = available / 4;
#else
    size_t reasonable = available / 3;
#endif
    size_t wanted = 2000000000;
    if (m_max_level < max_supported_level)
    {
        // Trigon has the lowest relative number of simulations on lower levels
        // compared to the highest level. We approximate the memory used in a
        // search as being proportional to the number of simulations.
        float factor =
                counts_trigon[max_supported_level - 1]
                / counts_trigon[m_max_level - 1];
        wanted /= static_cast<size_t>(factor);
    }
    size_t memory = min(wanted, reasonable);
    LIBBOARDGAME_LOG("Using ", memory / 1000000, " MB of ",
                     available / 1000000, " MB");
    return memory;
}

Rating Player::get_rating(Variant variant, unsigned level)
{
    // The ratings are based on experiments that measured the winning rates in
    // games between subsequent playing levels of Pentobi. The number of
    // simulations per level is already chosen such that the levels have
    // roughly equal Elo differences, see Player::genmove(). A scale factor
    // less than 1 was applied to the Elo differences to take into account that
    // self-play experiments usually overestimate the performance against
    // humans. The scale factor and lower anchor for the Elo ratings were
    // chosen such that the Elo range is roughly between 1000 (beginner level)
    // and 2000 (lower expert level) taking into account that Pentobi is
    // relatively strong against humans in Duo and relatively weak in Trigon.
    // Only the most popular game variants were tested (Classic 2, Duo,
    // Trigon 2) and these ratings were used for other game variants on the
    // same board type. The ratings are not always measured again for new
    // versions of Pentobi and not really comparable between different versions
    // of Pentobi.
    auto max_supported_level = Player::max_supported_level;
    level = min(max(level, 1u), max_supported_level);
    Rating result;
    switch (get_board_type(variant))
    {
    case BoardType::classic:
    case BoardType::callisto_2: // Not yet measured
        {
            // Anchor 1000, scale 0.63
            static float elo[Player::max_supported_level] =
                { 1000, 1134, 1267, 1400, 1534, 1668, 1801, 1935, 2068 };
            result = Rating(elo[level - 1]);
        }
        break;
    case BoardType::duo:
        {
            // Anchor 1100, scale 0.80
            static float elo[Player::max_supported_level] =
                { 1100, 1229, 1359, 1489, 1618, 1748, 1878, 2008, 2137 };
            result = Rating(elo[level - 1]);
        }
        break;
    case BoardType::trigon:
    case BoardType::trigon_3:
    case BoardType::callisto: // Not yet measured
    case BoardType::callisto_3: // Not yet measured
    case BoardType::nexos: // Not yet measured
        {
            // Anchor 1000, scale 0.60
            static float elo[Player::max_supported_level] =
                { 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800 };
            result = Rating(elo[level - 1]);
        }
        break;
    }
    return result;
}

bool Player::is_book_loaded(Variant variant) const
{
    return m_is_book_loaded && m_book.get_tree().get_variant() == variant;
}

void Player::load_book(istream& in)
{
    m_book.load(in);
    m_is_book_loaded = true;
}

bool Player::load_book(const string& filepath)
{
    ifstream in(filepath);
    if (! in)
    {
        LIBBOARDGAME_LOG("Could not load book ", filepath);
        return false;
    }
    m_book.load(in);
    m_is_book_loaded = true;
    LIBBOARDGAME_LOG("Loaded book ", filepath);
    return true;
}

bool Player::resign() const
{
    return m_resign;
}

void Player::use_cpu_time(bool enable)
{
    if (enable)
        m_time_source.reset(new CpuTimeSource);
    else
        m_time_source.reset(new WallTimeSource);
}

//-----------------------------------------------------------------------------

} // namespace libpentobi_mcts

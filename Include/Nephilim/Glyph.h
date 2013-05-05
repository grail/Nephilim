////////////////////////////////////////////////////////////
#ifndef NEPHILIM_GLYPH_H
#define NEPHILIM_GLYPH_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "Platform.h"
#include "Rect.h"


NEPHILIM_NS_BEGIN

////////////////////////////////////////////////////////////
/// \brief Structure describing a glyph
///
////////////////////////////////////////////////////////////
class NEPHILIM_API Glyph
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Glyph() : advance(0) {}

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int     advance;     ///< Offset to move horizontically to the next character
    IntRect bounds;      ///< Bounding rectangle of the glyph, in coordinates relative to the baseline
    IntRect textureRect; ///< Texture coordinates of the glyph inside the font's texture
};

NEPHILIM_NS_END


#endif // SFML_GLYPH_HPP


////////////////////////////////////////////////////////////
/// \class sf::Glyph
/// \ingroup graphics
///
/// A glyph is the visual representation of a character.
///
/// The sf::Glyph structure provides the information needed
/// to handle the glyph:
/// \li its coordinates in the font's texture
/// \li its bounding rectangle
/// \li the offset to apply to get the starting position of the next glyph
///
/// \see sf::Font
///
////////////////////////////////////////////////////////////
